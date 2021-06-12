import time
from soundplayer import SoundPlayer

# Use device with ID 1  (mostly USB audio adapter)
p = SoundPlayer("/home/pi/sound/salza1.wav", 1)        
print("play for 10 s with volume 0.5")
p.play(0.5) # non-blocking, volume = 0.5
print("isPlaying:", p.isPlaying())
time.sleep(10)
print("pause for 5 s")
p.pause()
print("isPlaying:", p.isPlaying())
time.sleep(5)
print("resume for 10 s")
p.resume()
time.sleep(10)
print ("stop")
p.stop()
print("isPlaying:", p.isPlaying())
print =("done")