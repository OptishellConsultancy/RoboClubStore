to compile and upload:

bash compileAndUpload.sh

run python3 PySerialArdunioIF.py.py the press F7. to begin API runtime.

See APIExamples.txt  

http://jamiespi:2223/ - web interface to the API

http://optishell.duckdns.org:2223/ - duck dns variant

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
Make sure LIBRESPOT_DEVICE="plughw:4,0" - or any other device

for debugging check the service:
'sudo systemctl status raspotify'

sudo systemctl reboot
sudo apt install raspotify

https://pimylifeup.com/raspberry-pi-spotify/

sudo nano /etc/raspotify/conf
#LIBRESPOT_USERNAME="<Your username>"
#LIBRESPOT_PASSWORD="<Your username>"

//--------------------------------------
Sound test:
NOTE: espeak is required for this framework - 'sudo apt-get install speak'
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

Test with: 'speaker-test -c2 -twav -l3 -D plughw:4,0' - where '4' is the sound card index
Update the following to set new defaults:
sudo nano /etc/asound.conf - enter: 'defaults.pcm.card 4 defaults.ctl.card 4'

//--------------------------------------
Pipewire - bookworm > raspbian audio info:
systemctl status pipewire-pulse
//--------------------------------------
Duck dns guide!
https://pimylifeup.com/raspberry-pi-duck-dns/
.Go duckdns, login get token for chosen domain (https://www.duckdns.org/domains)
.Create domain 
sudo mkdir /opt/duckdns/
sudo mkdir /var/log/duckdns/
sudo nano /opt/duckdns/duck.sh
 > Copy the following line:
   echo url="https://www.duckdns.org/update?domains=<DOMAIN>&token=<TOKEN>" | curl -k -o /var/log/duckdns/duck.log -K -
   
sudo chmod 700 /opt/duckdns/duck.sh
sudo crontab -e
And add: 
*/3 * * * * /opt/duckdns/duck.sh >/dev/null 2>&1

Startup addition:
sudo nano /etc/profile
sudo /opt/duckdns/duck.sh &

//--------------------------------------
git ssh add
https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent