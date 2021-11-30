import os
import logging

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

@app.route("/", methods=["GET", "POST"])
def index():
    # logging.warning(request.method);
    if request.method == "POST":

        # Add the user's entry into the database
        name = request.form.get("inputName")
        month = request.form.get("inputMonth")
        day = request.form.get("inputDay")
        db.execute("INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?)", name, month, day)

        return redirect("/")

    else:

        # Display the entries in the database on index.html
        result = db.execute("SELECT * FROM birthdays")
        return render_template("index.html", birthdays=result)
        # return render_template("index.html", name=request.args.get("name", "world"))


@app.route("/delete", methods=["GET"])
def delete():

    # logging.warning(request.args.get("id"))
    # DELETE AN ESPECIF BIRTHDAY
    id = request.args.get("id")
    db.execute("DELETE FROM birthdays WHERE id = ?", id)

    return redirect("/")

