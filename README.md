# Harvard CS50

## Links

CS50 IDE: 
- https://ide.cs50.io/94dea8b8adf046a0b7572b2a86378955

ed chat
- https://edstem.org/us/courses/176/discussion/13252

Exercises Submited
- https://submit.cs50.io/users/kazuuu/cs50/

Github
- https://github.com/me50/kazuuu/tree/b956030d233c364641242217b870b688a61b796a

CS50 - C Lang Help
https://manual.cs50.io/

# BIZ MONEY: ACCOUNTING PLATFORM
#### Video Demo:  <URL HERE>
#### Description:
Hello Everybody, my name is Marcello Kazuo and I am from Sao Paulo BRAZIL. Today i´m gonna present my CS50 Final Project

This is an Accounting Platform for one company to keep tracking all expenses and revenues organized by departments. The advangtage here
is that not only we can keep tracking of expenses and revenues, as will be able to track the money flow beetween departments.

Each user on this software will be a manager of one or more departments.

So after the manager sign in, he will be asked to to select the department he wants to manage or if he doesn't have a
department yet he will be able to create one.

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
- Sqlite3
- Python
- Flask
- HTML / Javascript
- Bootstrap

#### File Structure:
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





As a business, you’ll inevitably have income as well as overhead. You’ll pay for expenses like an office rental, software licenses, and vendor invoices. And you’ll receive revenue, also known as your “cash.”

https://www.waveapps.com/