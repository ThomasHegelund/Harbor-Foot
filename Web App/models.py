# from __future__ import annotations
from sqlalchemy import Boolean, Column, Integer, String
from sqlalchemy.orm import Mapped, mapped_column, relationship
from fastapi_utils.guid_type import GUID, GUID_DEFAULT_SQLITE

from database import Base


class Berth(Base):
    __tablename__ = "berths"
    uuid: Mapped[GUID] = mapped_column(GUID, primary_key=True, default=GUID_DEFAULT_SQLITE)
    name: Mapped[str]
    occupied: Mapped[bool]
    default_boat_uuid: Mapped[GUID] = mapped_column(GUID, nullable=True)

class RentedBerth(Berth):
    boat_length: Mapped[float] = mapped_column(nullable=True)
    is_paid: Mapped[bool] = mapped_column(default=False)

class FreeBerth(Berth):
    return_date: Mapped[str] = mapped_column(nullable=True)

class Boat(Base):
    __tablename__ = "boats"

    uuid = Column(GUID, default=GUID_DEFAULT_SQLITE, primary_key=True, index=True)
    berth_uuid = Column(GUID)
    captain_uuid = Column(GUID)
    in_harbor = Column(Boolean, default=True)


class Captain(Base):
    __tablename__ = "captains"
    uuid = Column(GUID, default=GUID_DEFAULT_SQLITE, primary_key=True, index=True)
    name = Column(String)
    email = Column(String)

# schemas?