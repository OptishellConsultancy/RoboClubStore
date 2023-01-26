to compile and upload:


bash compileAndUpload.sh

run python3 PySerialArdunioIF.py.py the press F7. to begin API runtime.

See APIExamples.txt  

http://jamiespi:2222/ - web interface to the API


Access dedicate webcam interface:
http://jamiespi:8000/html/


Note to augment startup scripts go to:
sudo nano /etc/profile


Backup startup profile script commands:
sudo bash    /home/pi/Desktop/RoboClubStore/MultiPurposePlatform/web/killWebServer.sh 2223
sudo python3 /home/pi/Desktop/RoboClubStore/MultiPurposePlatform/PlatformStartupAndGPIORuntimes.py &
sudo python3 /home/pi/Desktop/RoboClubStore/MultiPurposePlatform/web/runServer.py runServer.py &
bash ./home/pi/Desktop/RPi_Cam_Web_Interface/start.sh &


To kill server manually run, from web folder:
sudo bash ./MultiPurposePlatform/web/killWebServer.sh 2223

for low level API
sudo python3 MultiPurposePlatform/PySerialArdunioIF.py


To recompile level API:
bash ./compileAndUpload.sh

To run server manually:
sudo python3 MultiPurposePlatform/web/runServer.py

