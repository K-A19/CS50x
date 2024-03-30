import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime


from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Allows for the use of lookup in different templates
app.jinja_env.globals.update(lookup=lookup)

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Checks if a table exists for the user's history
    user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    table = db.execute("SELECT name FROM sqlite_master WHERE type='table' AND name=?", user[0]["username"])

    # Creates a table for the users history is there isnt already one
    if not table:
        db.execute(
            "CREATE TABLE ? (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, type TEXT NOT NULL, stock TEXT NOT NULL, price INTEGER NOT NULL, shares INTEGER NOT NULL, time DATETIME NOT NULL)", user[0]["username"])

    # This gets the stocks and amount of shares of those stocks that the user owns
    stocks = db.execute(
        "SELECT DISTINCT stock, SUM(shares) FROM transactions WHERE username = ? GROUP BY stock ORDER BY SUM(shares) DESC", user[0]["username"])

    # Finds the total value of shares in the users possesion
    total = 0
    for stock in stocks:
        if int(stock["SUM(shares)"]) > 0:
            total += lookup(stock['stock'])["price"] * stock['SUM(shares)']
        else:
            stocks.remove(stock)

    # Calculates the users net worth
    net = int(user[0]["cash"]) + total

    # Shares the users name and cash to be displayed in their home page
    return render_template("index.html", username=user[0]["username"], cash=int(user[0]["cash"]), stocks=stocks, total=total, net=net)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # When the route is reached by submitting the purchasing form
    if request.method == "POST":

        # Error checks the symbol provided
        if not request.form.get("symbol"):
            return apology("No stock symbol provided")

        if not lookup(request.form.get("symbol")):
            return apology("Stock symbol invalid")

        # Error checks the number of shares to be purchased
        if not request.form.get("shares"):
            return apology("Number of shares to be purchased not provided")

        # Checks if user input is a letter or decimal
        check = request.form.get("shares").lower()
        if check.islower() or request.form.get("shares").find(".") > 0:
            return apology("Invalid number of shares to be purchased")

        if int(request.form.get("shares")) < 1:
            return apology("Invalid number of shares to be purchased")

        # Calculates the price of purchasing the given number of shares
        stock = lookup(request.form.get("symbol"))
        price = stock["price"]
        per = price
        price *= int(request.form.get("shares"))

        # Makes sure the user has enough money to pay for the selcted amount of shares
        wallet = db.execute("SELECT cash FROM users WHERE id = ? ", session["user_id"])
        if wallet[0]["cash"] < price:
            return apology("Insufficient Funds to purchase selected amount of shares")

        # Gets the date and time of purchase
        now = datetime.now()
        time = now.strftime("%d-%m-%Y %H:%M:%S")

        # Updates all tables to show the influence of the users purchase
        user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        db.execute("INSERT INTO ? ('type', 'stock', 'price', 'shares', 'time') VALUES ('Purchase', ?, ?, ?, ?)",
                   user[0]["username"], request.form.get("symbol"), per, request.form.get("shares"), time)
        db.execute("INSERT INTO transactions ('username', 'stock', 'shares', 'price', 'time') VALUES (?, ?, ?, ?, ?)",
                   user[0]["username"], request.form.get("symbol"), request.form.get("shares"), per, time)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", user[0]["cash"]-price, user[0]["id"])

        # Returns user to the home page
        return redirect("/")

    # When the route is reached by clicking on the buy button or searching it
    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Gets the users transactions history
    user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    history = db.execute("SELECT * FROM ?", user[0]["username"])

    return render_template('history.html', history=history)


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

    # When the route is reached by filling in the quote form
    if request.method == "POST":

        # Makes sure a symbol was inputted
        if not request.form.get("symbol"):
            return apology("No symbol provided")

        stock = lookup(request.form.get("symbol"))

        # Makes sure the symbol is valid
        if stock is None:
            return apology("Invalid symbol provided")

        # Gets data about the stock's price and parent company name
        company_name = stock["name"]
        price = stock["price"]
        return render_template("quote.html", company_name=company_name, price=price, get=False)

    # When the route is reached by clicking on the qoute button or by being search
    return render_template("quote.html", get=True)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # When the user reaches the route by POST due to submitting the registration form
    if request.method == "POST":

        # Makes sure all fields have an input
        if not request.form.get("username"):
            return apology("Must provide a username")

        if not request.form.get("password"):
            return apology("Must provide a password")

        if not request.form.get("confirmation"):
            return apology("Must provide password confirmation")

        # Makes sure username is available
        repeat = db.execute("SELECT * FROM users WHERE username = ? ", request.form.get("username"))
        if len(repeat) != 0:
            return apology("Username already taken")

        # Makes sure the password and confirmation are the same
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("Your password and password confirmation do not match")

        # Registers user if all error checks come back false
        db.execute("INSERT INTO users ('username', 'hash') VALUES (?, ?)", request.form.get(
            "username"), generate_password_hash(request.form.get("password")))

        # Logs the user in
        code = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        session["user_id"] = code[0]["id"]
        return redirect("/")

    # When the user eaches the route by clicking the register button or searching for it
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Finds which stocks the user possibly owns
    user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    stocks = db.execute(
        "SELECT DISTINCT stock, SUM(shares) FROM transactions WHERE username = ? GROUP BY stock ORDER BY SUM(shares) DESC", user[0]["username"])

    # Finds the total value of shares in the users possesion
    for stock in stocks:
        if int(stock["SUM(shares)"]) < 1:
            stocks.remove(stock)

    # When the user reachers th route by submitting the sell form
    if request.method == 'POST':

        # Checks that a valid stock symbol is provided
        if not request.form.get('symbol'):
            return apology('No stock symbol selected')

        if request.form.get('symbol') not in [sub['stock'] for sub in stocks]:
            return apology('You have no shares in this stock')

        # Checks if the number of shares to be sold is valid
        if not request.form.get('shares'):
            return apology('Amount of shares to be sold not provided')

        if int(request.form.get('shares')) < 1:
            return apology('Invalid amount of shares to be sold provided')

        if int(request.form.get('shares')) > int([element for element in stocks if element['stock'] == request.form.get('symbol')][0]['SUM(shares)']):
            return apology('You are trying to sell more shares than you currently own')

        # Calculates the price of selling the given number of shares
        stock = lookup(request.form.get("symbol"))
        price = stock["price"]
        per = price
        price *= int(request.form.get("shares"))

        # Gets the date and time of sale
        now = datetime.now()
        time = now.strftime("%d-%m-%Y %H:%M:%S")

        # Updates all tables to show the influence of the users sale
        db.execute("INSERT INTO ? ('type', 'stock', 'price', 'shares', 'time') VALUES ('Sale', ?, ?, ?, ?)",
                   user[0]["username"], request.form.get("symbol"), per, request.form.get("shares"), time)
        db.execute("INSERT INTO transactions ('username', 'stock', 'shares', 'price', 'time') VALUES (?, ?, ?, ?, ?)",
                   user[0]["username"], request.form.get("symbol"), 0-int(request.form.get("shares")), per, time)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", user[0]["cash"]+price, user[0]["id"])

        # Moves the user back to the home page
        return redirect('/')

    return render_template("sell.html", stocks=stocks)


@app.route("/password", methods=['GET', 'POST'])
def password():

    # When the user reaches the route by submitting the password change form
    if request.method == "POST":

        user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        # Makes sure none of the provided fields are empty
        if not request.form.get('c-password'):
            return apology('Current password not provided')

        if not request.form.get('n-password'):
            return apology('New password not provided')

        if not request.form.get('confirmation'):
            return apology('New password confirmation not provided')

        # Makes sure the user provides their actual password first
        if not check_password_hash(user[0]['hash'], request.form.get('c-password')):
            return apology('Incorrect password inputted')

        # Makes sure the user's new password and password confirmation match
        if not request.form.get('n-password') == request.form.get('confirmation'):
            return apology('New password and new password confirmation do not match')

        # Changes the user's password by updating their password in the users table
        db.execute('UPDATE users SET hash = ? Where id = ?', generate_password_hash(request.form.get('n-password')), user[0]['id'])

        return redirect('/')

    # When the user reaches the route by clicking on the password change button
    return render_template('password.html')