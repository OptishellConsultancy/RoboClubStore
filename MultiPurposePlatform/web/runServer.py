#!/usr/bin/env python3

import subprocess
from createApp import create_app, db
import sqlite3
from sqlite3 import Error
from flask import url_for
from waitress import serve
# refactored with authentication via https://github.com/do-community/flask_auth_scotch/tree/master/project

app = None

def execv(command, path):
    if(len(path) > 0):
        command = '%s%s' % (path, command)
    result = subprocess.run(['/bin/bash', '-c', command],
                            stdout=subprocess.PIPE, encoding='UTF-8')
    print(result.stdout)

def create_connection(db_file):
    """ create a database connection to the SQLite database
        specified by db_file
    :param db_file: database file
    :return: Connection object or None
    """
    conn = None
    try:
        conn = sqlite3.connect(db_file)
        return conn
    except Error as e:
        print(e)

    return conn


def create_table(conn, create_table_sql):
    """ create a table from the create_table_sql statement
    :param conn: Connection object
    :param create_table_sql: a CREATE TABLE statement
    :return:
    """
    try:
        c = conn.cursor()
        c.execute(create_table_sql)
        print("Table check/creation complete")
    except Error as e:
        print(e)


def setupTables():
    database = r"MultiPurposePlatform/web/db.sqlite"
    sql_create_projects_table = """ CREATE TABLE IF NOT EXISTS user (
                                        id integer PRIMARY KEY,
                                        email text NOT NULL,
                                        password text,
                                        name text
                                    ); """
    conn = create_connection(database)

    # create tables
    if conn is not None:
        # create projects table
        create_table(conn, sql_create_projects_table)
    else:
        print("Error! cannot create the database connection.")

if __name__ == '__main__':
    # shellESpeak("Web server starting")
    # execv('start.sh', '/home/pi/Desktop/RPi_Cam_Web_Interface/')
    execv('killWebServer.sh 2223',
          '/home/RoboClubStore/MultiPurposePlatform/web/')
    

    setupTables()
    
    app = create_app()

    print("Running server..")
    # app.run(debug=False, port=2223, host='0.0.0.0')  #debug mode   
    serve(app, port=2223)  #production
    
