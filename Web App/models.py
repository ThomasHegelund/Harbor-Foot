from sqlalchemy import Boolean, Column, Integer, String

from database import Base

class Berth(Base):
    __tablename__ = "berths"

    id = Column(Integer, primary_key=True, index=True)
    name = Column(String)
    occupied = Column(Boolean, default=False)
    default_boat_id = Column(Integer, nullable=True)


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
    boat_id = Column(Integer)


# schemas?