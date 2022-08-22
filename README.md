to compile and upload:


bash compileAndUpload.sh

run python3 PySAI.py the press F7. to begin API runtime.

See APIExamples.txt  

http://jamiespi:2222/ - web interface to the API


Access dedicate webcam interface:
http://jamiespi:8000/html/


Note to augment startup scripts go to:
sudo nano /etc/profile


Backup startup profile script commands:
sudo python3 /home/pi/Desktop/RoboClubStore/PlatformStartupAndGPIORuntimes.py &
sudo python3 /home/pi/Desktop/RoboClubStore/web/runServer.py runServer.py &
bash ./home/pi/Desktop/RPi_Cam_Web_Interface/start.sh &


To kill server manually run, from web folder:
sudo bash ./web/killWebServer.sh 2223

for low level API
sudo python3 PySAI.py


To recompile level API:
bash ./compileAndUpload.sh

To run server manually:
sudo python3 web/runServer.py