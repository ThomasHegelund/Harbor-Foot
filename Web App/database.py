from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker

# Create the database

SQLALCHEMY_DATABASE_URL = "sqlite:///harbor.sqlite"

engine = create_engine(
    SQLALCHEMY_DATABASE_URL, connect_args={"check_same_thread": False}, echo=False
)
# connection = engine.connect()
Session= sessionmaker(bind=engine)

Base = declarative_base()
