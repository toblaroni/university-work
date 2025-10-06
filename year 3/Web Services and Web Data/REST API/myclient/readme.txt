1. Client Program Instructions
1.1 Setup
First create a Python virtual environment and activate:

"python -m venv <virutal-env-name>"
"source ./venv/bin/activate"        # For UNIX

Next install dependencies:

"pip install -r ./myclient/requirements.txt"

1.2 Usage
To run the program, activate the virtual environment and run:

"python myclient/client.py"

You can then run the following commands:

'register' -  Prompts you for a username, email and password. The client then attempts to register a new user with these credentials.

'login <URL>' - Prompts you for username and password and attempts to log you in. <URL> is the URL of the API. 

'logout' - Attempts to log you out of the web app. User must be logged-in to logout.

'list' - Returns a list of all module instances and the professors teaching them.

'view' - Returns a list of all professors and their overall rating across all modules.

'average <professor_id> <module_code>' - Returns the average rating of the professor (<professor_id>) for the specified module (<module_code>). 

'rate <professor_id> <module_code> <year> <semester> <rating>' - Creates a rating for a specific professor (<professor_id>) for the specified module instance. The user must be logged-in in order to rate a module instance.

'help' - Returns a list of valid commands.

'exit' - Exit the client program.

2. Info for Assessors
The Pythonanywhere domain is 'https://sc21t2hh.pythonanywhere.com/'.

Use the following credentials to login to the admin account (https://sc21t2hh.pythonanywhere.com/admin/): 
username - admin
password - comp3011

