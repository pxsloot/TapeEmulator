TapeEmulator library
====================

I have some old hardware lying around (zx81, KIM-1 and a MPF-1), but no
cassette player to load software into the old farts. I didn't want to
muck around with software on my laptop to load the thousands of '.p' files.
Well, I could have, but then I wouldn't have this Arduino project.

The sounds on a (data) cassette tape are created by toggling 1 bit. So
I grabbed my Arduino, hooked 1 pin to a small LM386 amplifier and began
toggling one bit.

FIXME:
-----
While the proof of concept sketch worked, I'm in the middle of creating
a library from it. Things will not compile at this point.


TODO:
----

* learn proper C/C++
* cleanup the internal format of the program
* add an SD card to get the files from (now hardcoded)
* LCD screen to browse/select file to upload
* add 'save' feature
* create code for more machines. Only works for MPF-1 now.

