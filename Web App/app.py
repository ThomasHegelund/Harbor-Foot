from typing import Optional
from fastapi import FastAPI, Depends, Request, Form, status

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


@app.get("/")
def home(request: Request, db: Session = Depends(get_db)):
    paid_berths = db.query(models.RentedBerth).filter(models.RentedBerth.is_paid == True).all()
    unpaid_berths = db.query(models.RentedBerth).filter(models.RentedBerth.is_paid == False).all()

    # unoccupied_berths = db.query(models.Berth).filter(models.Berth.occupied == False).all()

    # unpaid_berths = db.query(models.RentedBerth).filter(models.RentedBerth.occupied == False).all()

    return templates.TemplateResponse("index.html",
                                      {"request": request, "paid_berths": paid_berths, "unpaid_berths": unpaid_berths})

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

# @app.get("/update/{todo_id}")
# def update(request: Request, todo_id: int, db: Session = Depends(get_db)):
#     todo = db.query(models.Todo).filter(models.Todo.id == todo_id).first()
#     todo.complete = not todo.complete
#     db.commit()

#     url = app.url_path_for("home")
#     return RedirectResponse(url=url, status_code=status.HTTP_302_FOUND)


# @app.get("/delete/{todo_id}")
# def delete(request: Request, todo_id: int, db: Session = Depends(get_db)):
#     todo = db.query(models.Todo).filter(models.Todo.id == todo_id).first()
#     db.delete(todo)
#     db.commit()

#     url = app.url_path_for("home")
#     return RedirectResponse(url=url, status_code=status.HTTP_302_FOUND)
