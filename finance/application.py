import os
import logging
import sqlite3
import json

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd
from datetime import datetime, timezone

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
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


class Wallet:
    def __init__(self, symbol, name, shares, price, total):
        self.symbol = symbol
        self.name = name
        self.shares = shares
        self.price = price
        self.total = total


class Trade:
    def __init__(self, symbol, shares, price, date):
        self.symbol = symbol
        self.shares = shares
        self.price = price
        self.date = date


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    ret = db.execute("SELECT symbol, sum(shares) as shares FROM trades where user_id = ? group by symbol",
                     session["user_id"])
    portfolio = []
    walletTotal = 0

    for item in ret:
        stock = lookup(item["symbol"])

        # if stock is None:
        #     return apology("Invalid Symbol", 400)

        portfolio.append(Wallet(item["symbol"], stock["name"], item["shares"],
                                usd(stock["price"]), usd(stock["price"] * item["shares"])))
        walletTotal += (stock["price"] * item["shares"])

    return render_template("index.html", portfolio=portfolio, cash=usd(session["cash"]), walletTotal=usd(walletTotal), total=usd(walletTotal + session["cash"]))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        # Ensure Symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)

        # Ensure shares was submitted
        if not request.form.get("shares"):
            return apology("must provide shares", 400)

        # Ensure shares is digit
        try:
            int(request.form.get("shares"))
        except ValueError:
            return apology("must provide integer numbers", 400)

        # Ensure shares is not a negative number
        if int(request.form.get("shares")) < 1:
            return apology("shares cannot be zero or a negative number", 400)

        # Get Stock info and ensure it exists
        result = lookup(request.form.get("symbol"))

        if result is None:
            return apology("Invalid Symbol", 400)

        price = result['price']
        totalValue = price * int(request.form.get("shares"))

        if (totalValue > session["cash"]):
            return apology("Not enough money", 400)

        cash = session["cash"] - totalValue

        # Insert the buy in the table
        ret = db.execute("INSERT INTO trades (symbol, user_id, shares, date, price) VALUES (?, ?, ?, ?, ?)", request.form.get(
            "symbol").upper(), session["user_id"], request.form.get("shares"), (datetime.now(timezone.utc),), price)
        ret = db.execute("UPDATE users SET cash = ? WHERE id = ?", cash,  session["user_id"])
        session["cash"] = cash

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    ret = db.execute("SELECT * FROM trades where user_id = ?", session["user_id"])

    trades = []

    for item in ret:
        trades.append(Trade(item["symbol"], item["shares"], usd(item["price"]), item["date"]))

    return render_template("history.html", trades=trades)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
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
        session["cash"] = rows[0]["cash"]

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


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        result = lookup(request.form.get("symbol"))

        if result is None:
            return apology("Invalid Symbol", 400)

        p = usd(result["price"])

        return render_template("quoted.html", stock=result, price=p)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
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
        session["cash"] = rows[0]["cash"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")
#   return apology("TODO")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        if request.form.get("symbol") is None:
            return apology("Select a symbol", 400)

        stock = lookup(request.form.get("symbol"))

        if stock is None:
            return apology("Invalid Symbol", 400)

        ret = db.execute("SELECT symbol, sum(shares) as shares FROM trades where user_id = ? and symbol = ? group by symbol",
                         session["user_id"], request.form.get("symbol"))

        qtd = int(request.form.get("shares"))

        if (qtd < 1):
            return apology("Share number must be postive", 400)

        if (qtd > int(ret[0]["shares"])):
            return apology("Shares cannot exceed shares you have", 400)

        # Convert qtd to negative because you are selling
        qtd = qtd * (-1)

        price = stock['price']
        totalValue = price * int(request.form.get("shares"))

        cash = session["cash"] + totalValue

        # Insert the sell in the table
        trade = db.execute("INSERT INTO trades (symbol, user_id, shares, date, price) VALUES (?, ?, ?, ?, ?)", request.form.get(
            "symbol").upper(), session["user_id"], qtd, (datetime.now(timezone.utc),), stock["price"])
        ret = db.execute("UPDATE users SET cash = ? WHERE id = ?", cash,  session["user_id"])
        session["cash"] = cash

        return redirect("/")

    else:

        ret = db.execute("SELECT symbol, sum(shares) as shares FROM trades where user_id = ? group by symbol", session["user_id"])

        return render_template("sell.html", portfolio=ret)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
