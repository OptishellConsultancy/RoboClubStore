to compile and upload:


bash compileAndUpload.sh

run python3 PySerialArdunioIF.py.py the press F7. to begin API runtime.

See APIExamples.txt  

http://jamiespi:2223/ - web interface to the API

http://optishell.duckdns.org:2223/

//--------------------------------------
Camera info:

libcamera-hello --list-cameras
Test with:  libcamera-vid -t 0

sudo apt install python3-picamera2
sudo apt install libcamera-v4l2
sudo apt install libcamera-tools
sudo apt install -y libopenjp2-7-dev libhdf5-dev
https://www.waveshare.com/wiki/RPi_Camera_V2
//--------------------------------------
If unused ppa's: 
sudo nano /etc/apt/sources.list
sudo apt list --installed | grep -i ppa 
And delete any unused here : /etc/apt/sources.list.d/
//--------------------------------------
Note to augment startup scripts go to:
sudo nano /etc/profile
And copy the following into the file:
sudo bash    /home/pi/RoboClubStore/MultiPurposePlatform/web/killWebServer.sh 2223
sudo python3 /home/pi/RoboClubStore/MultiPurposePlatform/PlatformStartupAndGPIORuntimes.py &
sudo python3 /home/pi/RoboClubStore/MultiPurposePlatform/web/runServer.py runServer.py &


//--------------------------------------
To kill server manually run, from root folder:
sudo bash ./MultiPurposePlatform/web/killWebServer.sh 2223
//--------------------------------------
for low level API
sudo python3 MultiPurposePlatform/PySerialArdunioIF.py
//--------------------------------------
To recompile level API:
bash ./MultiPurposePlatform/compileAndUpload.sh
//--------------------------------------
To run server manually:
sudo python3 MultiPurposePlatform/web/runServer.py
//--------------------------------------
Port forwarding: 
to check currently exposed ports:

netstat -tnlp
To open a port, e.g 445: sudo ufw allow 445

Update requirements.txt with:
pip3 install pipreqs

//--------------------------------------
Note: be sure to install:
sudo apt-get install python3-scipy
pip3 install scipy --force


//--------------------------------------
cd into MultiPurposePlatform/web
python3 -m  pipreqs.pipreqs .

//--------------------------------------
Raspotify:
sudo apt-get -y install curl && curl -sL https://dtcooper.github.io/raspotify/install.sh | sh
-Note if you get isntall issues, donwload directly the deb file from : https://dtcooper.github.io/raspotify/raspotify-latest_arm64.deb
https://github.com/dtcooper/raspotify
'sudo systemctl restart raspotify'
'sudo nano /etc/default/raspotify' to configure
'sudo nano /etc/asound.conf' - make sure device setting look ok

for debugging check the service:
'sudo systemctl status raspotify'

sudo systemctl reboot
sudo apt install raspotify

https://pimylifeup.com/raspberry-pi-spotify/

#LIBRESPOT_USERNAME="<Your username>"
#LIBRESPOT_PASSWORD="<Your username>"

//--------------------------------------
Note on utilising usb soundcard:

Uninstall/reinstall alsa:
  sudo apt-get remove alsa-utilssudo
  sudo apt-get remove alsamixer
  sudo apt-get install alsa-utils
  sudo apt-get install alsamixer
  sudo apt install pulseaudio-module-bluetooth
  pulseaudio --start


To set sound output device, mixed results from:
cat /proc/asound/cards
aplay -l
aplay -L | grep CARD
OR:
pacmd list-cards
dmesg | grep -i snd
lsusb -v
OR:
aplay -lL
OR:
cat /proc/asound/modules

Test with: speaker-test -c2 -twav -l3 -D plughw:4,0 - where '4' is the sound card index
Update the following to set new defaults:
sudo nano /etc/asound.conf - enter: 'defaults.pcm.card 4 defaults.ctl.card 4'


sudo nano /usr/share/alsa/alsa.conf
And change :
defaults.pcm.card 0
defaults.pcm.device 0
to:
defaults.pcm.card <card number>
defaults.pcm.device <card number>

'alsamixer' to configure sound level OR set directly amixer set Master 100%

//--------------------------------------
Duck dns guide!
https://pimylifeup.com/raspberry-pi-duck-dns/
.Go duckdns, login get token for chosen domain
.Create domain 
sudo mkdir /opt/duckdns/
sudo mkdir /var/log/duckdns/
sudo nano /opt/duckdns/duck.sh
 > Copy the following line:
   echo url="https://www.duckdns.org/update?domains=<DOMAIN>&token=<TOKEN> | curl -k -o /var/log/duckdns/duck.log -K -
sudo chmod 700 /opt/duckdns/duck.sh
sudo crontab -e
And add: 
*/3 * * * * /opt/duckdns/duck.sh >/dev/null 2>&1

sudo nano /etc/profile
/opt/duckdns/duck.sh &