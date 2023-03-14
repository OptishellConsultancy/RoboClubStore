#!/usr/bin/env python3

import subprocess
from flask_sqlalchemy import SQLAlchemy
from flask_login import LoginManager
from flask import Flask
from flask import Blueprint, render_template


db = SQLAlchemy()
main = Blueprint('main', __name__)
app = Flask(__name__)

# refactored with authentication via https://github.com/do-community/flask_auth_scotch/tree/master/project


def execv(command, path):
    if(len(path) > 0):
        command = '%s%s' % (path, command)
    result = subprocess.run(['/bin/bash', '-c', command],
                            stdout=subprocess.PIPE, encoding='UTF-8')
    print(result.stdout)




def create_app():
    app.config.update(PERMANENT_SESSION_LIFETIME=600)

    app.config.update(
        SESSION_COOKIE_SECURE=True,
        SESSION_COOKIE_HTTPONLY=True,
        SESSION_COOKIE_SAMESITE='Lax',
    )
    app.config['SECRET_KEY'] = '9OLWxND4o83j4K4iuopO'
    app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///db.sqlite'

    db.init_app(app)
    login_manager = LoginManager()
    login_manager.login_view = 'auth.login'
    login_manager.init_app(app)

    @login_manager.user_loader
    def load_user(user_id):
        # since the user_id is just the primary key of our user table, use it in the query for the user
        return User.query.get(int(user_id))

    # blueprint for auth routes in our app
    from auth import auth
    app.register_blueprint(auth)

    # blueprint for non-auth parts of app    
    app.register_blueprint(main)

    db.create_all(app = app) 
    return app


if __name__ == '__main__':
    # shellESpeak("Web server starting")
    execv('start.sh', '/home/pi/Desktop/RPi_Cam_Web_Interface/')
    execv('killWebServer.sh 2223',
          '/home/pi/Desktop/RoboClubStore/MultiPurposePlatform/web/')


    app = create_app()    
    app.run(debug=False, port=2223, host='0.0.0.0')
