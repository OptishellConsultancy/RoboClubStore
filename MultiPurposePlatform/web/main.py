
from flask import Blueprint, render_template, request
from flask_login import login_required, current_user
import os

main = Blueprint('main', __name__)



@main.route('/')
@login_required
def login():
        return render_template('login.html')
   
@main.route('/profile')
@login_required
def profile():
    return render_template('profile.html', name=current_user.name)

@main.route('/serverRuntime')
@login_required
def serverRuntime():
    if "profile" in request.referrer :
        return render_template('serverRuntime.html')
    else:
        return render_template('deniedPage.html')