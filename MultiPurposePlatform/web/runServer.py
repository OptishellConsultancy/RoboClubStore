from serverRuntime import app, subprocess

#Runtime
def execv(command, path):
    if(len(path) > 0):
        command = '%s%s' % (path, command)
    result = subprocess.run(['/bin/bash', '-c', command],
                            stdout=subprocess.PIPE, encoding='UTF-8')
    print(result.stdout)


app.config.update(PERMANENT_SESSION_LIFETIME=600)

if __name__ == '__main__':
    # shellESpeak("Web server starting")
    execv('start.sh', '/home/pi/Desktop/RPi_Cam_Web_Interface/')
    execv('killWebServer.sh 2223',
          '/home/pi/Desktop/RoboClubStore/MultiPurposePlatform/web/')
    app.config.update(
        SESSION_COOKIE_SECURE=True,
        SESSION_COOKIE_HTTPONLY=True,
        SESSION_COOKIE_SAMESITE='Lax',
    )
    app.run(debug=False, port=2223, host='0.0.0.0')
