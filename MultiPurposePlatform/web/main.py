
from flask import Blueprint, render_template
from flask_login import login_required, current_user

main = Blueprint('main', __name__)

@main.route('/')
@login_required
def index():
    return render_template('serverRuntime.html')

@main.route('/profile')
def profile():
    print("current_user:" + current_user)
    return render_template('profile.html', name=current_user.name)