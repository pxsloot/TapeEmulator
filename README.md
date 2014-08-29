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
* created 2 standalone sketches (MPF1 and KIM1). For now, edit the sketch
  and add the program data.
* the KIM1 sketch won't create the right stream. It's late, can't be
  bothered.

TODO:
----

* learn proper C/C++
* cleanup the internal data format of the program
* add an SD card to get the files from (now hardcoded)
* LCD screen to browse/select file to upload
* add 'save' feature
