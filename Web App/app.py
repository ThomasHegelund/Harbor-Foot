from typing import Optional
from fastapi import FastAPI, Depends, Request, Form, status
from datetime import date

from starlette.responses import RedirectResponse
from starlette.templating import Jinja2Templates

from sqlalchemy.orm import Session
from fastapi_utils.guid_type import GUID
import models
from database import engine, Session

from pydantic import BaseModel, EmailStr
import uuid

from datetime import datetime, timedelta
import smtplib
from email.message import EmailMessage

from enum import Enum

models.Base.metadata.create_all(bind=engine)

templates = Jinja2Templates(directory="templates")

app = FastAPI()


# Dependency
def get_db():
    db = Session()
    try:
        yield db
    finally:
        db.close()


class PaymentStatus(BaseModel):
    berth_uuid: uuid.UUID
    new_berth_payment_status: bool

class ReturnDate(BaseModel):
    berth_uuid: uuid.UUID 
    return_date: date

class BoatJson(BaseModel):
    berth_uuid: uuid.UUID
    captain_uuid: uuid.UUID
    in_harbor: Optional[bool]

class CaptainJson(BaseModel):
    uuid: uuid.UUID
    name: str
    email: EmailStr

class BoatInHarbor(BaseModel):
    boat_uuid: uuid.UUID
    in_harbor: bool

class OccupationStatusJson(BaseModel):
    berth_uuid: uuid.UUID
    occupation_status: bool

class BerthStatus(BaseModel):
    berth_uuid: uuid.UUID
    occupied: bool
    main_boat_in_harbor: bool

class ReturnCodeInformation(BaseModel):
    occupied: bool
    main_boat_in_harbor: bool
    return_date: str|None

class LedMode(Enum):
    GREEN = '0'
    RED = '1'
    REDBLINK = '2'

class DisplayMode(Enum):
    OFF = '0'
    ON = '1'

DISPLAY_NO_DATE = '0000'

class ReturnCode:
    def __init__(self) -> None:
        self.return_code = ''
        self.occupied = None
        self.main_boat_in_harbor = None
        self.return_date = None
        self.led_mode = None
        self.display_mode = None
        self.display_code = None

    def set_occupation_status(self, occupied: bool) -> None:
        self.occupied = occupied
    
    def set_main_boat_in_harbor(self, main_boat_in_harbor: bool) -> None:
        self.main_boat_in_harbor = main_boat_in_harbor
    
    def set_return_date(self, return_date: str | None) -> None:
        self.return_date = return_date
    
    def get(self) -> str:
        self.__update_return_code()
        return self.return_code

    def __update_return_code(self) -> None:
        if not self.__all_parameters_are_set():
            raise ValueError('All parameters must be set')
        
        self.__update_led_status_code()
        self.__update_display_code()
        self.return_code = self.led_mode.value + self.display_mode.value + self.display_code

    def __update_led_status_code(self) -> None:
        if self.main_boat_in_harbor is True:
            self.led_mode = LedMode.RED
        
        elif self.return_code is not None:
            self.led_mode = LedMode.GREEN
        
        elif self.occupied is True:
            self.led_mode = LedMode.REDBLINK
        
        elif self.occupied is False:
            self.led_mode = LedMode.RED
        
    def __update_display_code(self) -> None:
        if self.main_boat_in_harbor is True:
            self.display_mode = DisplayMode.OFF
            self.display_code = DISPLAY_NO_DATE

        elif self.return_date is None:
            self.display_mode = DisplayMode.OFF
            self.display_code = DISPLAY_NO_DATE
        
        else:
            self.display_mode = DisplayMode.ON
            self.display_code = format_date(self.return_date)

    def __all_parameters_are_set(self) -> bool:
        return None not in [self.occupied, self.main_boat_in_harbor]

def convert_json_class_object_to_dict(json_class_object):
    return dict([*json_class_object])


@app.get("/")
def home(request: Request, db: Session = Depends(get_db)):
    unpaid_berths = db.query(models.RentedBerth)\
        .filter(models.RentedBerth.is_paid == False)\
        .filter(models.RentedBerth.occupied == True)\
        .all()
    boats = [get_boat(unpaid_berth.default_boat_uuid, db) if unpaid_berth.default_boat_uuid is not None else None for unpaid_berth in unpaid_berths]
    unpaid_berths = [unpaid_berth for unpaid_berth, boat in zip(unpaid_berths, boats) if boat is None or boat.in_harbor is False]

    return templates.TemplateResponse("index/index.html",
                                      {"request": request, "unpaid_berths": unpaid_berths})

@app.get("/overview")
def overview(request: Request, db: Session = Depends(get_db)):
    free_berths = db.query(models.FreeBerth)\
        .filter(models.FreeBerth.occupied == False)\
        .filter(models.FreeBerth.return_date != None)\
        .filter(models.FreeBerth.return_date > date.today())\
        .all()
    free_berths.sort(key=lambda x: x.return_date, reverse=True)

    return templates.TemplateResponse("overview/index.html",
                                      {"request": request, "free_berths": free_berths})

@app.get("/berth_info/{berth_uuid}")
def berth_info(request: Request, berth_uuid: uuid.UUID = None,  db: Session = Depends(get_db)):
    minimum_return_date = datetime.now()
    minimum_return_date = minimum_return_date.strftime('%Y-%m-%d')
    berth = db.query(models.FreeBerth)\
    .filter(models.FreeBerth.uuid == berth_uuid)\
    .first()
    return templates.TemplateResponse("berth_info/index.html",
                                    {"request": request, "berth": berth, "minimum_return_date": minimum_return_date})

@app.get("/return_date/{berth_uuid}")
def return_date(berth_uuid: uuid.UUID, db: Session = Depends(get_db)):
    berth = db.query(models.FreeBerth)\
        .filter(models.FreeBerth.uuid == berth_uuid)\
        .first()
    return format_date(berth.return_date)

def format_date(date: str):
    return "".join(date.split('-')[1:])

@app.post("/add_berth")
def add_berth(request: Request, name: str = Form(...), occupied: bool = Form(False), default_boat_uuid: uuid.UUID | None = Form(None), db: Session = Depends(get_db)):
    new_berth = models.Berth(name = name, occupied = occupied, default_boat_uuid = default_boat_uuid)
    db.add(new_berth)
    db.commit()
    # url = app.url_path_for("home")
    # return RedirectResponse(url=url, status_code=status.HTTP_303_SEE_OTHER)

@app.post("/add_boat")
def add_boat(request: Request, boat_json: BoatJson, db: Session = Depends(get_db)):
    new_boat = models.Boat(**convert_json_class_object_to_dict(boat_json))
    db.add(new_boat)
    db.commit()

    db.query(models.Berth)\
        .filter(models.Berth.uuid == boat_json.berth_uuid)\
        .update({'default_boat_uuid': new_boat.uuid})
    db.commit()

@app.post("/add_captain")
def add_captain(request: Request, captain_json: CaptainJson, db: Session = Depends(get_db)):
    new_captain = models.Captain(**convert_json_class_object_to_dict(captain_json))
    db.add(new_captain)
    db.commit()

@app.post("/update_berth_occupation_status")
def update_berth_occupation_status(request: Request, occupation_status_json: OccupationStatusJson, db: Session = Depends(get_db)):
    db.query(models.Berth)\
        .filter(models.Berth.uuid == occupation_status_json.berth_uuid)\
        .update({'occupied': occupation_status_json.new_occupation_status})
    db.commit()

@app.post("/update_boat_in_harbor")
def update_boat_in_harbor(request: Request, boat_in_harbor: BoatInHarbor, db: Session = Depends(get_db)):
    db.query(models.Boat)\
        .filter(models.Boat.uuid == boat_in_harbor.boat_uuid)\
        .update({'in_harbor': boat_in_harbor.in_harbor})
    db.commit()

    if boat_in_harbor.in_harbor == True:
        return

    boat = db.query(models.Boat)\
        .filter(models.Boat.uuid == boat_in_harbor.boat_uuid)\
        .first()
    
    
    captain = db.query(models.Captain)\
        .filter(models.Captain.uuid == boat.captain_uuid)\
        .first()

    url = "/".join(str(request.url).split('/')[:3])

    send_reminder_to_update_return_date(captain, boat, url)

def send_reminder_to_update_return_date(captain: models.Captain, boat: models.Boat, url: str):
    message = EmailMessage()
    message.set_content(
        f"""
        Dear Captain {captain.name}
        We can see that you have left the harbor. If you plan on staying away for one or multiple nights please update your return date on the link below:
        {url}/berth_info/{boat.berth_uuid}
        """
    )
    
    message['Subject'] = f'Boat return date reminder'
    message['From'] = "info@localhost"
    message['To'] = captain.email
    s = smtplib.SMTP('localhost')
    s.send_message("info@localhost", captain.email, message.as_string())
    s.quit()


@app.post("/update_berth_payment_status")
def update_berth_payment_status(payment_status: PaymentStatus, db: Session = Depends(get_db)):
    db.query(models.RentedBerth)\
        .filter(models.RentedBerth.uuid == payment_status.berth_uuid)\
        .update({'is_paid': payment_status.new_berth_payment_status})
    db.commit()

@app.post("/update_berth_return_date")
def update_berth_return_date(return_date: ReturnDate, db: Session = Depends(get_db)):
    db.query(models.FreeBerth)\
        .filter(models.FreeBerth.uuid == return_date.berth_uuid)\
        .update({'return_date': return_date.return_date})
    db.commit()

@app.post("/update_berth_status")
def update_berth_status(request: Request, berth_status: BerthStatus, db: Session = Depends(get_db)):
    free_berth = get_free_berth(berth_status.berth_uuid, db)
    
    if free_berth.default_boat_uuid is None:
        boat = None
        captain = None
    else:
        boat = get_boat(free_berth.default_boat_uuid, db)
        captain = get_boat(boat.captain_uuid, db)
    update_berth_status_in_database(berth_status, db)
    update_main_boat_in_berth_status_in_database(berth_status, db)
    
    return_code = get_return_code(berth_status, free_berth, db)
    send_reminder_if_needed(request, free_berth, boat, captain, berth_status.main_boat_in_harbor)

    return return_code.get()
    

def update_berth_status_in_database(berth_status: BerthStatus, db: Session):
    db.query(models.Berth)\
        .filter(models.Berth.uuid == berth_status.berth_uuid)\
        .update({'occupied': berth_status.occupied})
    db.commit()

def update_main_boat_in_berth_status_in_database(berth_status: BerthStatus, db: Session):
    berth = db.query(models.Berth)\
        .filter(models.Berth.uuid == berth_status.berth_uuid)\
        .first()
    db.query(models.Boat)\
        .filter(models.Boat.uuid == berth.default_boat_uuid)\
        .update({'in_harbor': berth_status.main_boat_in_harbor})
    db.commit()

def get_return_code(berth_status: BerthStatus, free_berth: models.FreeBerth, db: Session) -> ReturnCode:
    return_code = ReturnCode()
    return_code.set_occupation_status(berth_status.occupied)
    return_code.set_main_boat_in_harbor(berth_status.main_boat_in_harbor)
    return_code.set_return_date(free_berth.return_date)

    return return_code


def send_reminder_if_needed(request: Session, free_berth: models.FreeBerth, boat: models.Boat, captain: models.Captain, new_in_harbor: bool):
    if boat is None:
        return
    
    if boat.in_harbor is False:
        return
    
    if new_in_harbor is True:
        return

    if free_berth.return_date is not None:
        return

    url = "/".join(str(request.url).split('/')[:3])
    send_reminder_to_update_return_date(captain, boat, url)


def calculate_return_code(return_date_information: ReturnCodeInformation, db: Session):
    if return_date_information.occupied is True:
        if return_date_information.main_boat_in_harbor is True:
            return '100000'
        if return_date_information.return_date is None:
            return '200000'
        return '000000'
    
    if return_date_information.return_date is not None:
        return '01' + format_date(return_date_information.return_date)

    return '100000'


def get_free_berth(berth_uuid: uuid.UUID, db: Session) -> models.FreeBerth:
    if berth_uuid is None:
        raise ValueError('berth_uuid cannot be None')
    
    free_berth = db.query(models.FreeBerth)\
        .filter(models.FreeBerth.uuid == berth_uuid)\
        .first()
    
    return free_berth

def get_boat(boat_uuid: uuid.UUID, db: Session) -> models.Boat:
    if boat_uuid is None:
        raise ValueError('boat_uuid cannot be None')
    
    boat = db.query(models.Boat)\
            .filter(models.Boat.uuid == boat_uuid)\
            .first()
    return boat

def get_captain(captain_uuid: uuid.UUID, db: Session) -> models.Captain:
    if captain_uuid is None:
        raise ValueError('captain_uuid cannot be None')
    
    captain = db.query(models.Captain)\
                .filter(models.Captain.uuid == captain_uuid)\
                .first()
    
    return captain