import requests
from getpass import getpass
import json
from urllib.parse import urljoin

# For formatting outputs
# https://rich.readthedocs.io/en/latest/
from rich.console import Console
from rich.table import Table

class MyClient:
    def __init__(self, url):
        self.baseUrl = url
        self.console = Console()
        self._getCsrfToken()
        self.token = ""
        self.loggedIn = False

        self.commands = {
            "register": self._handleRegister,
            "login": self._handleLogin,
            "logout": self._handleLogout,
            "list": self._handleList,
            "view": self._handleView,
            "average": self._handleAverage,
            "rate": self._handleRate,
        }


    def run(self):
        while(True):
            cmd = input(" $ ").split()

            if len(cmd) == 0:
                continue

            if cmd[0] in self.commands.keys():
                fn = self.commands[cmd[0]]
                fn(*cmd[1:])
                continue
            elif cmd[0] == "help":
                print(f"Valid commands: [ {', '.join(self.commands.keys()) + ', help, exit'} ]")
                continue
            elif cmd[0] == "exit":
                if self.loggedIn:
                    self._handleLogout()
                break

            print(f"'{cmd[0]}' is not a recognised command.\nType 'help' to see a list of valid commands.")


    def _getCsrfToken(self):
        try:
            response = requests.get(self.baseUrl + "/api/csrf-token")  # Make a GET request to the base URL
            response.raise_for_status()
            self.csrfToken = response.cookies.get('csrftoken')
        except requests.exceptions.RequestException as e:
            print(f"Error retrieving CSRF token: {e}")
            exit(-1)


    def _handleRegister(self, *args):
        if len(args) != 0:
            print("'register' expects no arguments.")
            return

        username = input("Enter your username: ")
        email = input("Enter your email address: ")

        password = getpass("Enter your password: ")
        password_check = getpass("Re-enter your password: ")

        while password != password_check:
            print("Error: Passwords didn't match. Try Again.")
            password = getpass("Enter your password: ")
            password_check = getpass("Re-enter your password: ")

        data = {
            "username": username,
            "email": email,
            "password": password
        }

        headers={
            "X-CSRFToken": self.csrfToken,
            "Content-Type": "application/json"
        }

        res = self._clientRequest(
            method="POST",
            url=self.baseUrl + "/api/register/",
            data=data,
            headers=headers
        )

        if res == None:
            return

        try:
            print(f"{res['detail']}")
        except KeyError as e:
            print("Error occurred while interpreting server response.")
            print(f"Missing key: {e}")


    def _handleLogin(self, *args):
        if len(args) != 1:
            print('Usage: login <url>')
            return

        url = urljoin(args[0], "/api/login/")
        username = input("Enter your username: ")
        password = getpass("Enter your password: ")

        data = {
            "username": username,
            "password": password
        }

        headers = {
            "X-CSRFToken": self.csrfToken,
            "Content-Type": "application/json"
        }

        res = self._clientRequest(
            method='POST',
            url=url,
            data=data,
            headers=headers
        )

        if res == None:
            return
        
        try:
            self.token = f"Token {res['token']}"    # Store the token
            print(f"{res['detail']}")
            self.loggedIn = True
        except KeyError as e:
            print("Error occurred while interpreting server response.")
            print(f"Missing key: {e}")


    def _handleLogout(self, *args):
        if len(args) != 0:
            print("'logout' expects no arguments")
            return

        headers = {
            "X-CSRFToken": self.csrfToken,
            "Authorization": self.token
        }

        res = self._clientRequest(
            method="POST",
            url=self.baseUrl + "/api/logout/",
            headers=headers
        )

        if res != None:
            print(f"{res['detail']}")


    def _handleList(self, *args):
        if len(args) != 0:
            print("'logout' expects no arguments")
            return

        headers = { "X-CSRFToken": self.csrfToken }

        res = self._clientRequest(
            method="GET",
            url=self.baseUrl + "/api/list/",
            headers=headers
        )

        if res == None:
            return 

        # === Display the data ===
        # https://rich.readthedocs.io/en/latest/tables.html
        try:
            table = Table(title="Module Instances", show_lines=True)

            table.add_column("Code", justify="center")
            table.add_column("Name", justify="center")
            table.add_column("Year", justify="center")
            table.add_column("Semester", justify="center")
            table.add_column("Taught By", justify="center")

            for instance in res["module_instances"]:
                moduleCode = instance['module']['code']
                moduleName = instance['module']['name']
                year = str(instance['year'])
                semester = str(instance['semester'])
                professors = "".join([f"{p['name']} ({p['professor_id']}), " for p in instance['professors']])

                table.add_row(moduleCode, moduleName, year, semester, professors)

            self.console.print(table)

        except KeyError as e:
            print("Server Error. Missing data in server response.")
            print(f"Expected: {e}")


    def _handleView(self, *args):
        if len(args) != 0:
            print("'view' expects no arguments")
            return
        
        headers = { "X-CSRFToken": self.csrfToken }

        res = self._clientRequest(
            method="GET",
            url=self.baseUrl + "/api/view/",
            headers=headers
        )

        if res == None:
            return
        
        try:
            table = Table(title="Overall Ratings", show_lines=True)

            table.add_column("Professor", justify="center")
            table.add_column("Rating", justify="center")

            for rating in res["professor_ratings"]:
                professor = f"{rating['professor_name']} ({rating['professor_id']})"
                rating = str(rating['rating']) + '/5' if rating['rating'] else "No ratings yet"
                table.add_row(professor, rating)

            self.console.print(table)

        except KeyError as e:
            print("Server Error. Missing data in server response.")
            print(f"Expected: {e}")


    def _handleAverage(self, *args):
        if len(args) != 2:
            print('Usage: average <professor_id> <module_code>')
            return
        
        professor_id, module_code = args

        data = {
            "professor_id": professor_id,
            "module_code": module_code
        }

        headers = { 
            "X-CSRFToken": self.csrfToken,
            "Content-Type": "application/json"
        }

        res = self._clientRequest(
            method="GET",
            url=self.baseUrl + "/api/average/",
            headers=headers,
            data=data
        )

        if res == None:
            return

        try:
            data = res['average_rating']
            table = Table(title="Average Rating for Module", show_lines=True)

            table.add_column("Professor", justify="center")
            table.add_column("Module", justify="center")
            table.add_column("Average Rating", justify="center")

            professor = f"{data['professor_name']} ({data['professor_id']})"
            module = f"{data['module_name']} ({data['module_code']})"
            rating = str(data['average_rating']) + '/5' if data['average_rating'] else "No ratings yet."

            table.add_row(professor, module, rating)
            self.console.print(table)

        except KeyError as e:
            print("Server Error. Missing data in server response.")
            print(f"Expected: {e}")
        

    def _handleRate(self, *args):
        if len(args) != 5:
            print('Usage: rate <professor_id> <module_code> <year> <semester> <rating>')
            return

        professor_id, module_code, year, semester, rating = args

        data = {
            "professor_id": professor_id,
            "module_code": module_code,
            "year": year,
            "semester": semester,
            "rating": rating
        }

        headers = { 
            "X-CSRFToken": self.csrfToken,
            "Content-Type": "application/json",
            "Authorization": self.token
        }

        res = self._clientRequest(
            method="POST",
            url=self.baseUrl + "/api/rate/",
            headers=headers,
            data=data
        )

        if res == None:
            return

        try:
            print(f"{res['detail']}")
        except KeyError as e:
            print("Error occurred while interpreting server response.")
            print(f"Missing key: {e}")


    def _clientRequest(self, method, url, headers, data={}):
        # Sends our requests and handles errors
        try:
            res = requests.request(
                method=method,
                url=url,
                json=data,
                headers=headers,
                cookies={ "csrftoken": self.csrfToken }
            )
            res.raise_for_status()  # Raises an exception based off the status
            return res.json()

        except requests.exceptions.HTTPError as e:
            try:
                error_data = e.response.json()
                print(f"An Error(s) occurred:\n{error_data['detail']}")
            except json.JSONDecodeError:
                print(f"Server Error. Could not decode error response.")
            except (KeyError, TypeError):
                print(f"Format of response was incorrect: {e}")
        except requests.exceptions.RequestException as e:
            print(f"An Error Occurred: {e}")
        except (json.JSONDecodeError, KeyError):
            print("Server Error: Invalid JSON response from server.")

        return None


def main():
    # url = r"http://127.0.0.1:8000"
    url = "https://sc21t2hh.pythonanywhere.com"
    MyClient(url).run()

if __name__=="__main__":
    main()
