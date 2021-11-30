# [CS50 FINAL PROJECT] BIZ MONEY: ACCOUNTING PLATFORM
#### Video Demo:  https://youtu.be/bW-LEU8I8Ik
#### Description:
Author: Marcello Kazuo Miyajima
Location: Sao Paulo, Brazil
Date: 10/07/2021

This is an Accounting Web Application called Biz Money.

With this Web App one company will be able to track all expenses and revenues organized by departments.

The advangtage here is that not only we can keep track of expenses and revenues, as will be able to track the money flow beetween departments.

Each user on this software is a manager of one or more departments.

So after the manager sign in, he will be asked to to select the department he wants to manage or if he doesn't have any
department he will be able to create one.

Once all set, the manager will register all transactions like revenues, payments, salaries, vendor invoices, office
rental.

After all managers have registered all transactions from all departments we will be able to see the result from the Company and also each department.

#### Features
- Register multiple Users
- Register multiple Departments
- Register Revenues / Deposits
- Register Expenses / Payments
- One Department can charge another Department
- One Department can directly transfer a value to another Department
- Report of all transactions from Departments and from all company.
- Report of the expenses, revenues and results of departments and all company.


#### Technologies
This Web app was developed using the same technologies and design that we have learned in Python lessons:

- Sqlite3
- Python
- Flask
- HTML / Javascript
- Bootstrap

#### File Structure:
./README.md -> This file
./application.py -> It's Python main file with all routes and business logic
./bizmoney.db -> SQLite3 Database
./helpers.py -> Some generic functions
./static/bizmoney.js -> The Javascript to maimpulate data and menus
./static/favicon.ico -> The app icon that appears on the Web Browser
./style.css -> All CSS Styling
./templates/apology.html -> Webpage of Error Messages
./templates/charge.html -> Webpage to the a department charges another department
./templates/charges.html -> Webpage to list all charges of a department
./templates/departments.html -> Webpage to list all departments from a User
./templates/index.html -> Webpage as dashboard with Balance and actions links.
./templates/layout.html -> Website Main Layout that contains headers, menu, footers.
./templates/login.html -> Login form to authenticate
./templates/register.html -> Register form to create a new user.
./templates/report-areas.html -> Webpage to show results, expenses and revenues from all Deparments and all the company.
./templates/report-transactions.html -> Webpage to list transactions from all Departments and all the company.
./templates/revenue-expense.html -> Webpage to add a Revenue or Expense in a Department.
./templates/revenues-expenses.html -> Webpage to list Revenues and Expenses in a Department.
./templates/select-department.html -> Webpage to select a department to manage.
./templates/transactions.html -> List of all transactions from a Department
./templates/transfer.html -> Webpage to Transfer a value to another Department.
./templates/transfers.html -> Webpage to list transfers in a Department.
