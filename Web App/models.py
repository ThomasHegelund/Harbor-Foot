# from __future__ import annotations
from sqlalchemy import Boolean, Column, Integer, String
from sqlalchemy.orm import Mapped, mapped_column

from database import Base

class Berth(Base):
    __tablename__ = "berths"

    id: Mapped[int] = mapped_column(primary_key=True, index=True)
    name: Mapped[str]
    occupied: Mapped[bool]
    default_boat_id: Mapped[int] = mapped_column(nullable=True)

class RentedBerth(Berth):
    boat_length: Mapped[float] = mapped_column(nullable=True)
    is_paid: Mapped[bool] = mapped_column(nullable=True)

class FreeBerth(Berth):
    return_date: Mapped[str] = mapped_column(nullable=True)

class Boat(Base):
    __tablename__ = "boats"

    id = Column(Integer, primary_key=True, index=True)
    berth_id = Column(Integer)
    captain_id = Column(Integer)
    in_harbor = Column(Boolean, default=True)


class Captain(Base):
    __tablename__ = "captains"
    id = Column(Integer, primary_key=True, index=True)
    name = Column(String)
    email = Column(String)


# schemas?