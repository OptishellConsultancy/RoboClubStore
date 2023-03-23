


from flask import Flask, url_for
from flask_sqlalchemy import SQLAlchemy
from flask_login import LoginManager 
import os

dbName = r"db.sqlite"
# init SQLAlchemy so we can use it later in our models
db = SQLAlchemy()

from models import User
from auth import auth as auth_blueprint
from main import main as main_blueprint
from serverRuntime import srt as serverRuntime_blueprint

def create_app():
    app = Flask(__name__)
    app.config.update(PERMANENT_SESSION_LIFETIME=600)

    # app.config.update(
    #     SESSION_COOKIE_SECURE=True,Z
    #     SESSION_COOKIE_HTTPONLY=True,
    #     SESSION_COOKIE_SAMESITE='Lax',
    # )

    app.config['SECRET_KEY'] = '9OLWxND4o83j4K4iuopO'
    app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///' + os.path.join(app.root_path, dbName)
    print("sqldatabaseurl:" + app.config['SQLALCHEMY_DATABASE_URI'])

    db.init_app(app)

    login_manager = LoginManager()
    login_manager.login_view = 'auth.login'
    login_manager.init_app(app)



    @login_manager.user_loader
    def load_user(user_id):
        # since the user_id is just the primary key of our user table, use it in the query for the user
        return User.query.get(int(user_id))

    # blueprint for auth routes in our app    
    app.register_blueprint(auth_blueprint)

    # blueprint for non-auth parts of app    
    app.register_blueprint(main_blueprint)

    # Server runtime
    app.register_blueprint(serverRuntime_blueprint)

    return app