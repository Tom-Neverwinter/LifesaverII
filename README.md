# LifesaverII
Ghostbusters Lifesaver II clone

"The Lifegard II PASS Alarm (Personal Alert Safety System) emits a high-pitched alert of 95 decibels when its wearer lies motionless for over 30 seconds. Primarily used by firefighters, the Lifegard II was manufactured by The Antenna Specialists Co. in 1988.

Introduced in Ghostbusters II, this device was worn with the black pistol belts during the montage and the battle with Vigo." https://www.gbfans.com/equipment/uniform/lifegard-2-pass-alarm/

As this piece of equipment is getting harder to find I have attempted to make a more functional and faithful recreation of this piece of hardware. I DO NOT Recommend using this for its "original safety purpose" as it was found to put live in more danger when they fell off equipment, however as a prop it is fantastic.

shell:
https://www.thingiverse.com/thing:943444/makes

You need an arduino UNO or digispark and a piezo or regular speaker (small)
one battery for power and a switch to turn it on or off.



After trying to figure out how to make one cheap and simple I went through many many iterations

These are my results:

I found this repo which did what I generally wanted and used cost effective parts: https://github.com/robsoncouto/arduino-songs

Then all I had to do was figure out how to make the input.

To do this you follow this tutorial:

https://www.instructables.com/Translate-Songs-to-Be-Played-on-Arduino/

however because you must subscribe to download the format we need, we instead download musescore 3

Take your mp3 or whatever format make it single channel with audacity. simple is better. 

followed this tutorial, decreased the size significantly:

https://www.youtube.com/watch?v=F28Znry0qcw

set format to 16 bit pcm audio  and project hertz to 8000

import our audio as midi. yay good ole midi...

https://www.bearaudiotool.com/mp3-to-midi

then export as musicxml format 

rename "musicxml" to "mxl"

convert using https://github.com/MrRedBeard/DotNet-MXL-Parsing-for-Arduino (seems broken atm sadly or I have failed to format it properly.)

then paste it in and away you go.
