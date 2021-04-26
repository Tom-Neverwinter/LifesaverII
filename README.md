# LifesaverII
Ghostbusters Lifesaver II clone


After trying to figure out how to make one cheap and simple I went through many many iterations

These are my results:

I found this repo which did what I generally wanted and used cost effective parts: https://github.com/robsoncouto/arduino-songs

Then all I had to do was figure out how to make the input.

To do this you follow this tutorial:

https://www.instructables.com/Translate-Songs-to-Be-Played-on-Arduino/

however because you must subscribe to download the format we need, we instead download musescore 3

import our audio as midi. yay good ole midi...

https://www.bearaudiotool.com/mp3-to-midi

then export as musicxml format then convert using https://github.com/MrRedBeard/DotNet-MXL-Parsing-for-Arduino/blob/master/MusicXMLParser/MainForm.cs

then paste it in and away you go.
