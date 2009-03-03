Bauer stereophonic-to-binaural DSP (bs2b)
Copyright (c) 2005  Boris Mikhaylov <http://www.tmn.ru/~bor>

The Bauer stereophonic-to-binaural DSP (bs2b) is designed
to improve headphone listening of stereo audio records.
Project source code and description is available at
http://bs2b.sourceforge.net/

bs2bconvert.exe - win32 x86 binary.
libsndfile-1.dll - libsndfile library v1.0.17.

libsndfile is copyright by Erik de Castro Lopo.
http://www.mega-nerd.com/libsndfile/

Usage : bs2bconvert.exe [-x] <input file> <output file>

    'x' is number of:
    1,2,3 - Low to High crossfeed levels,
    4,5,6 - Low to High crossfeed levels of 'Easy' version
    The default crossfeed level is 6
