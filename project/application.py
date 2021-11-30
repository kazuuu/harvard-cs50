import os
import logging
import sqlite3
import json
import time
import re
from enum import Enum

# https://www.sqlitetutorial.net/sqlite-date/
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Before run you have to set IEX Cloud Token
# $ export API_KEY=pk_c1c995c3b8324b2f8fefe4120d11e9bc
# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///bizmoney.db")

errorMessage = ""

class User:
    def __init__(self, id, username, hash):
        self.id = id
        self.username = username
        self.hash = hash
        self.fullname = fullname


class Department:
    def __init__(self, id, name, owner_user_id, balance):
        self.id = id
        self.name = name
        self.owner_user_id = owner_user_id
        self.balance = balance


class Transfer:
    def __init__(self, id, date, value, from_department_id, to_department_id, description, from_department_name, to_department_name):
        self.id = id
        self.date = date
        self.value = value
        self.from_department_id = from_department_id
        self.to_department_id = to_department_id
        self.description = description
        # Virtual Attributes
        self.from_department_name = from_department_name
        self.to_department_name = to_department_name


class Charge:
    def __init__(self, id, from_department_id, to_department_id, value, date, description, status, from_department_name, to_department_name):
        self.id = id
        self.from_department_id = from_department_id
        self.to_department_id = to_department_id
        self.value = value
        self.date = date
        self.description = description
        self.status = status
        # Virtual Attributes
        self.from_department_name = from_department_name
        self.to_department_name = to_department_name


class Transaction:
    def __init__(self, id, date, department_id, value, description, type, externalId, department_name):
        self.id = id
        self.date = date
        self.department_id = department_id
        self.value = value
        self.description = description
        self.type = type
        self.externalId = externalId
        # Virtual Attributes
        self.department_name = department_name



class TransactionType(Enum):
        REVENUE = 1
        EXPENSE = 2
        TRANSFER = 3
        CHARGE = 4


class ReportDepartment:
    def __init__(self, department_id, department_name, total_result, total_revenues, total_expenses):
        self.department_id = department_id
        self.department_name = department_name
        self.total_result = total_result
        self.total_revenues = total_revenues
        self.total_expenses = total_expenses


@app.route("/")
@login_required
def index():
    """Show balance"""

    if not session.get("department_id"):
        return redirect("/select-department")

    return render_template("index.html", departmentBalance=usd(session["department_balance"]))


@app.route("/login", methods=["GET", "POST"])
def httpLogin():
    """Log user in"""

    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        session["username"] = rows[0]["username"]
        session["department_id"] = None

        ret = db.execute("SELECT id, name FROM departments")
        session["departments"] = ret

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/register", methods=["GET", "POST"])
def httpRegister():
    """Register new user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure password Confirm was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide password confirmation", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Check if username already exists
        if len(rows) > 0:
            return apology("Username already exists, try another", 400)

        # Check if the password is equal to the password confirmation
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("Your password and confirmation does not match", 400)

        # Query database for username
        userId = db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get(
            "username"), generate_password_hash(request.form.get("password")))

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE id = ?", userId)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        session["username"] = rows[0]["username"]
        session["department_id"] = None
        # session["department_name"] = None
        # session["departments"] = []
        # session["my_departments"] = []
        # session["my_departments_total"] = 0;

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")
#   return apology("TODO")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)


@app.route("/departments", methods=["GET", "POST"])
@login_required
def httpDepartments():
    """Add or Remove Departments"""

    if request.method == "POST":
        # Ensure Symbol was submitted
        if not request.form.get("name"):
            return apology("must provide department name", 400)

        # Insert the buy in the table
        ret = db.execute("INSERT INTO departments (name, owner_user_id) VALUES (?, ?)", request.form.get(
            "name").upper(), session["user_id"])

        loadDepartments()

        return redirect("/select-department")

    else:
        ret = db.execute("SELECT * FROM departments WHERE owner_user_id = ?",
                         session["user_id"])

        departments = []
        userTotal = 0

        for item in ret:
            departments.append(Department(item["id"], item["name"], item["owner_user_id"], usd(item["balance"])))
            # departments["balance"] = usd(departments["balance"])
            userTotal += item["balance"]

        return render_template("departments.html", departments=departments, userTotal=usd(userTotal))


@app.route("/department-delete", methods=["POST"])
@login_required
def httpDepartmentdelete():
    if not request.form.get("department_id"):
        return apology("must provide department id", 400)

    ret = db.execute("DELETE FROM departments WHERE owner_user_id = ? and id = ?",
                     session["user_id"], request.form.get("department_id"))

    return redirect("/")


@app.route("/select-department", methods=["GET", "POST"])
@login_required
def httpSelectDepartment():
    """Add or Remove Departments"""

    if not (request.args.get("departmentId") is None):
        try:
            departmentId = int(request.args.get("departmentId"))
            departmentName = ""
            departmentBalance = 0.0

            ret = db.execute("SELECT * FROM departments WHERE owner_user_id = ? and id = ?",
                     session["user_id"], departmentId)

            for item in ret:
                departmentName = item["name"]
                departmentBalance = item["balance"]
            session["department_id"] = departmentId
            session["department_name"] = departmentName
            session["department_balance"] = departmentBalance

            loadDepartments()

            return redirect("/")
        except ValueError:
            session["department_id"] = None
            session["department_name"] = None
            session["department_balance"] = None
    else:
        session["department_id"] = None
        session["department_name"] = None
        session["department_balance"] = None


    myDepartments = []
    userTotal = 0

    ret = db.execute("SELECT * FROM departments WHERE owner_user_id = ?",
                     session["user_id"])

    for item in ret:
        myDepartments.append(Department(item["id"], item["name"], item["owner_user_id"], usd(item["balance"])))
        # departments["balance"] = usd(departments["balance"])
        userTotal += item["balance"]

    session["my_departments"] = myDepartments
    session["my_departments_total"] = userTotal

    return render_template("select-department.html", myDepartments=myDepartments, userTotal=usd(userTotal))


@app.route("/transfer", methods=["GET", "POST"])
@login_required
def httpTransfer():
    """Add or Remove Departments"""

    if request.method == "POST":
        result = transferTo(None, request.form.get("to_department_id"), request.form.get("value"), request.form.get("description"))

        if result == False:
            return apology(errorMessage, 400)

        return redirect("/")

    else:
        return render_template("transfer.html", departments=session["departments"], department_id=session["department_id"])

@app.route("/transfers", methods=["GET", "POST"])
@login_required
def httpTransfers():
    """Add or Remove Departments"""

    ret = db.execute("SELECT * FROM (SELECT t.id, strftime('%m-%d-%Y', datetime(t.date, 'unixepoch')) as date, t.value, t.from_department_id, d1.name as from_department_name, t.to_department_id, d2.name as to_department_name, t.description FROM transfers t LEFT JOIN departments d1 ON t.from_department_id = d1.id LEFT JOIN departments d2 ON t.to_department_id = d2.id WHERE from_department_id = ? or to_department_id = ? order by date desc) ORDER BY date ASC",
                     session["department_id"], session["department_id"])

    transfers = []
    total = 0

    for item in ret:
        transfers.append(Transfer(item["id"], item["date"], usd(item["value"]), item["from_department_id"], item["to_department_id"], item["description"], item["from_department_name"], item["to_department_name"]))
        total += item["value"]

    return render_template("transfers.html", transfers=transfers, total=usd(total))


@app.route("/charge", methods=["GET", "POST"])
@login_required
def httpCharge():
    """Add or Remove Departments"""

    if request.method == "POST":
        # Ensure Symbol was submitted
        if not request.form.get("value"):
            return apology("must provide department name", 400)

        value = float(re.sub(r'[^\d.]', '', request.form.get("value")))

        if (value <= 0):
            return apology("Share number must be postive", 400)

        # Insert in the table
        ret = db.execute("INSERT INTO charges (from_department_id, to_department_id, date, value, description) VALUES (?, ?, ?, ?, ?)",
                         session["department_id"],
                         request.form.get("to_department_id"),
                         time.mktime(datetime.datetime.strptime(request.form.get("date"), "%m/%d/%Y").timetuple()),
                         value,
                         request.form.get("description"))

        return redirect("/")

    else:
        return render_template("charge.html", departments=session["departments"], department_id=session["department_id"])


@app.route("/charges", methods=["GET", "POST"])
@login_required
def httpCharges():
    """Add or Remove Departments"""

    ret = db.execute("SELECT c.id, strftime('%m-%d-%Y', datetime(c.date, 'unixepoch')) as date, c.value, c.from_department_id, d1.name as from_department_name, c.to_department_id, d2.name as to_department_name, c.description, c.status FROM charges c LEFT JOIN departments d1 ON c.from_department_id = d1.id LEFT JOIN departments d2 ON c.to_department_id = d2.id WHERE c.from_department_id = ? or c.to_department_id = ?",
                     session["department_id"], session["department_id"])

    charges = []
    total = 0

    for item in ret:
        charges.append(Charge(item["id"], item["from_department_id"], item["to_department_id"], usd(item["value"]), item["date"], item["description"], item["status"], item["from_department_name"], item["to_department_name"]))
        total += item["value"]

    return render_template("charges.html", charges=charges, total=usd(total), department_id=session["department_id"])


@app.route("/charge-accept", methods=["GET"])
@login_required
def httpChargeAccept():
    """Accept Charge"""

    result = chargeAccept(request.args.get("chargeId"))

    if result == False:
        return apology(errorMessage, 400)

    return redirect("/")


@app.route("/charge-decline", methods=["GET"])
@login_required
def httpChargeDecline():
    """Reject Decline"""

    result = chargeDecline(request.args.get("chargeId"))

    if result == False:
        return apology(errorMessage, 400)

    return redirect("/")


@app.route("/transactions", methods=["GET"])
@login_required
def httpTransactions():
    """Show all transactions"""

    ret = db.execute("SELECT t.id, strftime('%m-%d-%Y', datetime(t.date, 'unixepoch')) as date, t.department_id, d.name as departmentName, t.value, t.description, t.type, t.externalId FROM transactions t LEFT JOIN departments d ON t.department_id = d.id where t.department_id = ?", session["department_id"])

    transactions = []
    total = 0

    for item in ret:
        transactions.append(Transaction(item["id"], item["date"], item["department_id"], usd(item["value"]), item["description"], item["type"], item["externalId"], item["departmentName"]))
        total += item["value"]


    return render_template("transactions.html", transactions=transactions, total=usd(total), department_id=session["department_id"])


@app.route("/revenues-expenses", methods=["GET"])
@login_required
def httpRevenuesExpenses():
    """List Transactions of Revenues and Expenses"""

    ret = db.execute("SELECT t.id, strftime('%m-%d-%Y', datetime(t.date, 'unixepoch')) as date, t.department_id, d.name as departmentName, t.value, t.description, t.type, t.externalId FROM transactions t LEFT JOIN departments d ON t.department_id = d.id WHERE t.department_id = ? and (t.type = ? or t.type = ?)", session["department_id"], TransactionType.EXPENSE.value, TransactionType.REVENUE.value)

    transactions = []
    total = 0

    for item in ret:
        transactions.append(Transaction(item["id"], item["date"], item["department_id"], usd(item["value"]), item["description"], item["type"], item["externalId"], item["departmentName"]))
        total += item["value"]


    return render_template("revenues-expenses.html", transactions=transactions, total=usd(total), department_id=session["department_id"])


@app.route("/revenue-expense", methods=["GET", "POST"])
@login_required
def httpRevenueExpense():
    """Add or Remove Transactions of Revenues and Expenses"""

    if request.method == "POST":
        # Ensure Symbol was submitted
        if not request.form.get("type"):
            return apology("must provide transaction type", 400)

        if not request.form.get("value"):
            return apology("must provide department name", 400)

        value = float(re.sub(r'[^\d.]', '', request.form.get("value")))

        if (value <= 0):
            return apology("Value must be postive", 400)

        result = transactionAdd(None, session["department_id"], value, request.form.get("description"), request.form.get("type"), None)

        if result == False:
            return apology(errorMessage, 400)

        return redirect("/")

    else:
        return render_template("revenue-expense.html", transactionType=TransactionType)


@app.route("/report-areas", methods=["GET"])
@login_required
def httpReportAreas():
    """Show all Departments"""

    # To show info from all departments, need to unselect an especific department
    session["department_id"] = None
    session["department_name"] = None
    session["department_balance"] = None

    departmentDB = db.execute("SELECT * from departments")

    reportDepartments = []

    totalResult = 0
    totalExpenses = 0
    totalRevenues = 0

    # def __init__(self, department_id, department_name, total_result, total_revenues, total_expenses):
    for department in departmentDB:
        reportDepartments.append(ReportDepartment(department["id"], department["name"], 0, 0, 0))

    for reportDepartment in reportDepartments:
        ret = db.execute("SELECT t.id, strftime('%m-%d-%Y', datetime(t.date, 'unixepoch')) as date, t.department_id, d.name as departmentName, t.value, t.description, t.type, t.externalId FROM transactions t LEFT JOIN departments d ON t.department_id = d.id where t.department_id = ?",
                          reportDepartment.department_id)

        for item in ret:
            # transactions.append(Transaction(item["id"], item["date"], item["department_id"], usd(item["value"]), item["description"], item["type"], item["externalId"], item["departmentName"]))
            reportDepartment.total_result += item["value"]
            totalResult += item["value"]

            if (item["value"] > 0):
                totalRevenues += item["value"]
                reportDepartment.total_revenues += item["value"]
            elif (item["value"] < 0):
                totalExpenses += item["value"]
                reportDepartment.total_expenses += item["value"]

    return render_template("report-areas.html", reportDepartments=reportDepartments, totalResult=usd(totalResult), totalRevenues=usd(totalRevenues), totalExpenses=usd(totalExpenses))


@app.route("/report-transactions", methods=["GET"])
@login_required
def httpReportTransactions():
    """Show all transactions"""

    # To show info from all departments, need to unselect an especific department
    session["department_id"] = None
    session["department_name"] = None
    session["department_balance"] = None

    ret = db.execute("SELECT t.id, strftime('%m-%d-%Y', datetime(t.date, 'unixepoch')) as date, t.department_id, d.name as departmentName, t.value, t.description, t.type, t.externalId FROM transactions t LEFT JOIN departments d ON t.department_id = d.id")

    transactions = []
    total = 0
    totalExpenses = 0
    totalRevenues = 0
    totalProfit = 0

    for item in ret:
        transactions.append(Transaction(item["id"], item["date"], item["department_id"], usd(item["value"]), item["description"], item["type"], item["externalId"], item["departmentName"]))
        total += item["value"]
        if (item["value"] > 0):
            totalRevenues += item["value"]
        elif (item["value"] < 0):
            totalExpenses += item["value"]

    return render_template("report-transactions.html", transactions=transactions, total=usd(total), department_id=session["department_id"])


def updateBalance(department_id, value):
    """Update Balances"""

    retUpdate = db.execute("UPDATE departments SET balance = (balance + ?) where id = ?",
                                value,
                                department_id)

    loadBalance()

    return True


def updateBalances(from_department_id, to_department_id, value):
    """Update Balances"""

    if value <= 0:
        return setError("value must be positive number")

    # Update FROM Department in the table
    updateBalance(from_department_id, value * -1)

    # Update TO Department in the table
    updateBalance(to_department_id, value)

    return True


def loadDepartments():
    departments = []

    # Load All Departments List diferent than selected department
    ret = db.execute("SELECT id, name, owner_user_id FROM departments")

    for item in ret:
        departments.append(Department(item["id"], item["name"], item["owner_user_id"], None))

    session["departments"] = departments


def loadBalance():
    """Load Balance"""

    departmentName = ""
    departmentBalance = 0.0

    ret = db.execute("SELECT * FROM departments WHERE owner_user_id = ? and id = ?",
             session["user_id"], session["department_id"])

    for item in ret:
        departmentName = item["name"]
        departmentBalance = item["balance"]
    session["department_name"] = departmentName
    session["department_balance"] = departmentBalance


def transactionAdd(date, departmentId, value, description, type, externalId):
    """Transfer Add"""

    if not type:
        return setError("Type must be provided")

    if not departmentId:
        return setError("Department Id must be provided")

    if not value:
        return setError("Value must be provided")

    if date is None:
        date = time.mktime(datetime.datetime.strptime(request.form.get("date"), "%m/%d/%Y").timetuple())

    logging.warning(type)
    # Make sure Expense is negative value
    if int(type) == TransactionType.EXPENSE.value:
        logging.warning("Expense")
        if value > 0:
            value = value * -1
    # Make sure Revenue is positive value
    elif int(type) == TransactionType.REVENUE.value:
        logging.warning("Revenue")
        if value < 0:
            value = value * -1

    if externalId is None:
        # Insert FROM Transaction tables
        ret = db.execute("INSERT INTO transactions (date, department_id, value, description, type) VALUES (?, ?, ?, ?, ?)",
                         date,
                         departmentId,
                         value,
                         description,
                         type)
    else:
        # Insert FROM Transaction tables
        ret = db.execute("INSERT INTO transactions (date, department_id, value, description, type, externalId) VALUES (?, ?, ?, ?, ?, ?)",
                         date,
                         departmentId,
                         value,
                         description,
                         type,
                         externalId)

    updateBalance(departmentId, value)

    return True


def transferTo(date, toDepartmentId, value, description):
    """Transfer To"""

    if not toDepartmentId:
        return setError("Department Id must be provided")

    if not value:
        return setError("Value must be provided")

    value = float(re.sub(r'[^\d.]', '', value))

    if (value <= 0):
        return setError("Value must be postive")

    if date is None:
        date = time.mktime(datetime.datetime.strptime(request.form.get("date"), "%m/%d/%Y").timetuple())

    # Insert TRANSFER
    ret = db.execute("INSERT INTO transfers (from_department_id, to_department_id, date, value, description) VALUES (?, ?, ?, ?, ?)",
                     session["department_id"],
                     toDepartmentId,
                     date,
                     value,
                     description)

    logging.warning(ret)
    # Insert FROM Transaction tables
    transactionAdd(date, session["department_id"], -value, description, TransactionType.TRANSFER.value, ret)

    # Insert TO Transaction tables
    transactionAdd(date, toDepartmentId, value, description, TransactionType.TRANSFER.value, ret)

    return True


def setError(message):
    errorMessage = message
    return False


def chargeAccept(chargeId):
    """Accept Charge"""

    if not chargeId:
        return setError("must provide charge id")

    ret = db.execute("SELECT * FROM charges WHERE id = ?", chargeId)

    if not session["department_id"] == ret[0]["to_department_id"]:
        return setError("you are not allowed")

    if not ret[0]["status"] == 0:
        return setError("you are not allowed")

    # Update in the table
    retUpdate = db.execute("UPDATE charges SET status = 1 WHERE id = ?", chargeId)

    # Insert FROM Transaction tables
    transactionAdd(ret[0]["date"], ret[0]["to_department_id"], -ret[0]["value"], ret[0]["description"], TransactionType.CHARGE.value, chargeId)

    # Insert TO Transaction tables
    transactionAdd(ret[0]["date"], ret[0]["from_department_id"], ret[0]["value"], ret[0]["description"], TransactionType.CHARGE.value, chargeId)

    return True


def chargeDecline(chargeId):
    """Reject Decline"""

    if not chargeId:
        return setError("must provide charge id")

    ret = db.execute("SELECT * FROM charges WHERE id = ?", chargeId)

    if not session["department_id"] == ret[0]["to_department_id"]:
        return setError("you are not allowed")

    if not ret[0]["status"] == 0:
        return setError("you are not allowed")

    # Update in the table
    ret = db.execute("UPDATE charges SET status = 2 WHERE id = ?",
                     chargeId)

    return True