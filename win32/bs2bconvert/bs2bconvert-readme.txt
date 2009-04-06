Bauer stereophonic-to-binaural DSP (bs2b)
Copyright (c) 2005  Boris Mikhaylov <http://www.tmn.ru/~bor>

The Bauer stereophonic-to-binaural DSP (bs2b) is designed
to improve headphone listening of stereo audio records.
Project source code and description is available at
http://bs2b.sourceforge.net/

bs2bconvert.exe - win32 x86 binary.
libsndfile-1.dll - libsndfile library v1.0.19.

libsndfile is copyright by Erik de Castro Lopo.
http://www.mega-nerd.com/libsndfile/

Usage : bs2bconvert.exe [-x] <input file> <output file>
 x=d|c|m:
   d - default preset     - 700Hz/260us, 4.5 dB;
   c - Chu Moy's preset   - 700Hz/260us, 6.0 dB;
   m - Jan Meier's preset - 650Hz/280us, 9.5 dB.
