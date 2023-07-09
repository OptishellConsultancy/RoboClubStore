to compile and upload:


bash compileAndUpload.sh

run python3 PySerialArdunioIF.py.py the press F7. to begin API runtime.

See APIExamples.txt  

http://jamiespi:2223/ - web interface to the API

http://optishell.duckdns.org:2223/

Access dedicate webcam interface:
http://jamiespi:8000/html/


Note to augment startup scripts go to:
sudo nano /etc/profile


Backup startup profile script commands:
sudo bash    /home/pi/Desktop/RoboClubStore/MultiPurposePlatform/web/killWebServer.sh 2223
sudo python3 /home/pi/Desktop/RoboClubStore/MultiPurposePlatform/PlatformStartupAndGPIORuntimes.py &
sudo python3 /home/pi/Desktop/RoboClubStore/MultiPurposePlatform/web/runServer.py runServer.py &
bash ./home/pi/Desktop/RPi_Cam_Web_Interface/start.sh &


To kill server manually run, from root folder:
sudo bash ./MultiPurposePlatform/web/killWebServer.sh 2223

for low level API
sudo python3 MultiPurposePlatform/PySerialArdunioIF.py


To recompile level API:
bash ./MultiPurposePlatform/compileAndUpload.sh

To run server manually:
sudo python3 MultiPurposePlatform/web/runServer.py




Port forwarding: 

to check currently exposed ports:

netstat -tnlp
To open a port, e.g 445: sudo ufw allow 445

Update requirements.txt with:
pip3 install pipreqs


Note: be sure to install:
sudo apt-get install python3-scipy
pip3 install scipy --force

pip3 install PyWavelets==1.4.0

cd into MultiPurposePlatform/web
python3 -m  pipreqs.pipreqs .




Note on utilising usb soundcard:
//------------------------------------

sudo aptitude install libc6=2.31-0ubuntu9.2
sudo apt-get -y install curl && curl -sL https://dtcooper.github.io/raspotify/install.sh | sh
-Note if you get isntall issues, donwload directly the deb file from : https://dtcooper.github.io/raspotify/raspotify-latest_arm64.deb

cat /proc/asound/cards

cat /proc/asound/card1/pcm0p/info
Should give:

card: 1
device: 0
subdevice: 0
stream: PLAYBACK
id: USB Audio
name: USB Audio
subname: subdevice #0
class: 0
subclass: 0
subdevices_count: 1
subdevices_avail: 1


sudo nano /usr/share/alsa/alsa.conf
And change :
defaults.pcm.card 0
defaults.pcm.device 0
to:
defaults.pcm.card 2
defaults.pcm.device 2

The reboot, finally customise volume with:
alsamixer -c 1
Use arrow keys and enter/space to set (Recommended to set to 98)

RECOMMENDED: Alsa config tool: https://github.com/JasonLG1979/asound-conf-wizard
-The built output runtime is included in this repo:
run ./awiz and follow instructions
then run 'speaker-test'
then 'alsamixer' to configure sound level
then 'sudo systemctl restart raspotify'
then 'sudo nano /etc/default/raspotify' to configure
then 'sudo nano /etc/asound.conf' - make sure device setting look ok

for debugging check the service:
'sudo systemctl status raspotify'



If you get stuck:
sudo apt-get purge alsa-utils
sudo apt-get install alsa-utils

NOTE:
There is an issure installing raspotify, the fix is to install this version:


Ensure you're running debian 11: https://linuxize.com/post/how-to-upgrade-debian-10-to-debian-11/ + (sudo apt install gcc-8-base if error on full upgrade)
sudo systemctl reboot
sudo apt install raspotify


Check what version of debian you're on with:
cat /etc/os-release


