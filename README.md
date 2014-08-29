TapeEmulator library
====================

I have some old hardware lying around (zx81, KIM-1 (I know where to find one)
and a MPF-1B), but no cassette player to load software into the old farts. I
didn't want to muck around with software on my laptop to load the thousands
of '.p' files.
Well, I could have, but then there wouldn't be this little Arduino project.

The sounds on a (data) cassette tape are created by toggling 1 bit. An arduino 
can do that, hooked up to a small LM386 amplifier. With this library you can
(will be able) to stream data to vintage computers that use cassette players
to load programs (the ones mentioned above at least).

FIXME:
-----
* created 2 standalone sketches (MPF1 and KIM1). For now, edit the sketch
  and add the program data to make it work.
* the KIM1 sketch will not create the right stream. It's late, can't be
  bothered. WIP

TODO:
----

* make libraries from the sketches.
* learn library stuff. And C++
* come up with an internal data format of the program data
* write a sketch for an SD card to get the program data files from
* and an LCD screen to browse/select file to upload
* add 'save' feature
