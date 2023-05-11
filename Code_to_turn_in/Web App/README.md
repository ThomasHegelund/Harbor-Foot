# Dependencies
This web server requires [Python 3.11.3](https://www.python.org/downloads/release/python-3113/)

It is recommended that you create an environment before installing the dependencies. This can either be done using [Conda](https://conda.io/projects/conda/en/latest/user-guide/tasks/manage-environments.html) or [venv](https://docs.python.org/3/library/venv.html). 

*Note: Remember to activate the environment*

To install the dependencies simply run:
```
pip install -r env.txt
```

# Running the web server
```
uvicorn app:app --reload --host 0.0.0.0 --port 8000
```
