from typing import Optional
from fastapi import FastAPI, Depends, Request, Form, status
from datetime import date

from starlette.responses import RedirectResponse
from starlette.templating import Jinja2Templates

from sqlalchemy.orm import Session

import models
from database import engine, Session

from pydantic import BaseModel

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
    berth_id: int
    new_berth_payment_status: bool

class ReturnDate(BaseModel):
    berth_id: int
    return_date: date


@app.get("/")
def home(request: Request, db: Session = Depends(get_db)):
    unpaid_berths = db.query(models.RentedBerth)\
        .filter(models.RentedBerth.is_paid == False)\
        .filter(models.RentedBerth.occupied == True)\
        .all()

    return templates.TemplateResponse("index.html",
                                      {"request": request, "unpaid_berths": unpaid_berths})

@app.get("/overview")
def overview(request: Request, db: Session = Depends(get_db)):
    free_berths = db.query(models.FreeBerth)\
        .filter(models.FreeBerth.occupied == False)\
        .filter(models.FreeBerth.return_date != None)\
        .filter(models.FreeBerth.return_date > date.today())\
        .all()
    free_berths.sort(key=lambda x: x.return_date, reverse=True)
    return templates.TemplateResponse("overview.html",
                                      {"request": request, "free_berths": free_berths})


@app.post("/add")
def add(request: Request, name: str = Form(...), occupied: bool = Form(False), default_boat_id: int | None = Form(None), db: Session = Depends(get_db)):
    new_berth = models.Berth(name = name, occupied = occupied, default_boat_id = default_boat_id)
    db.add(new_berth)
    db.commit()

    url = app.url_path_for("home")
    return RedirectResponse(url=url, status_code=status.HTTP_303_SEE_OTHER)

@app.post("/update_berth_occupation_status")
def update_berth_occupation_status(request: Request, berth_id: int = Form(...), new_occupation_status: bool = Form(...), db: Session = Depends(get_db)):
    db.query(models.Berth)\
        .filter(models.Berth.id == berth_id)\
        .update({'occupied': new_occupation_status})
    db.commit()

@app.post("/update_boat_in_harbor")
def update_boat_in_harbor(request: Request, boat_id: int = Form(...), new_in_harbor: bool = Form(...), db: Session = Depends(get_db)):
    db.query(models.Boat)\
        .filter(models.Boat.id == boat_id)\
        .update({'in_harbor': new_in_harbor})
    db.commit()

@app.post("/update_berth_payment_status")
def update_boat_in_harbor(payment_status: PaymentStatus, db: Session = Depends(get_db)):
    db.query(models.RentedBerth)\
        .filter(models.RentedBerth.id == payment_status.berth_id)\
        .update({'is_paid': payment_status.new_berth_payment_status})
    db.commit()

@app.post("/update_berth_return_date")
def update_boat_in_harbor(return_date: ReturnDate, db: Session = Depends(get_db)):
    db.query(models.FreeBerth)\
        .filter(models.FreeBerth.id == return_date.berth_id)\
        .update({'return_date': return_date.return_date})
    db.commit()