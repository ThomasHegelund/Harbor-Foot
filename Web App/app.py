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
    uuid: uuid.UUID
    berth_uuid: uuid.UUID
    captain_id: uuid.UUID
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
    new_occupation_status: bool

def convert_json_class_object_to_dict(json_class_object):
    return dict([*json_class_object])


@app.get("/")
def home(request: Request, db: Session = Depends(get_db)):
    unpaid_berths = db.query(models.RentedBerth)\
        .filter(models.RentedBerth.is_paid == False)\
        .filter(models.RentedBerth.occupied == True)\
        .all()
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
    new_berth = models.Berth(name = name, occupied = occupied, default_boat_id = default_boat_uuid)
    db.add(new_berth)
    db.commit()

    # url = app.url_path_for("home")
    # return RedirectResponse(url=url, status_code=status.HTTP_303_SEE_OTHER)

@app.post("/add_boat")
def add_boat(request: Request, boat_json: BoatJson, db: Session = Depends(get_db)):
    new_boat = models.Boat(**convert_json_class_object_to_dict(boat_json))
    db.add(new_boat)
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
    print(boat_in_harbor)
    boats = db.query(models.Boat).filter(models.Boat.uuid == boat_in_harbor.boat_uuid).first()
    print(boats.uuid)

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
    print(message.as_string())
    s = smtplib.SMTP('localhost')
    s.send_message("info@localhost", captain.email, message.as_string())
    s.quit()


@app.post("/update_berth_payment_status")
def update_boat_in_harbor(payment_status: PaymentStatus, db: Session = Depends(get_db)):
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