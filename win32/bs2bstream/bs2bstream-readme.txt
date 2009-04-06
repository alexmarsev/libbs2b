Bauer stereophonic-to-binaural DSP (bs2b)
Copyright (c) 2005  Boris Mikhaylov <http://www.tmn.ru/~bor>

The Bauer stereophonic-to-binaural DSP (bs2b) is designed
to improve headphone listening of stereo audio records.
Project source code and description is available at
http://bs2b.sourceforge.net/

bs2bstream.exe - win32 x86 binary.

Applying bs2b effect to standart input of stereo raw LPCM stream.

Usage : bs2bstream.exe [-h] [-u] [-e E] [-b B] [-r R] [-l L]
-h - this help.
-u - unsigned data. Default is signed.
-e - endians, E = b|l|n (big|little|native). Default is native.
-b - bits per integer sample, B=8|16|24|32. Default is 16 bit.
-r - sample rate, R = <value by kHz>. Default is 44.1 kHz.
-l - crossfeed level, L=d|c|m:
     d - default preset     - 700Hz/260us, 4.5 dB;
     c - Chu Moy's preset   - 700Hz/260us, 6.0 dB;
     m - Jan Meier's preset - 650Hz/280us, 9.5 dB.

Example of usage (with lame):
  lame -t --decode test-44-16.wav - | \
    bs2bstream -l c -b 16 -r 44.1 | \
    lame -r -x -m j -s 44.1 --bitwidth 16 --preset extreme - test.mp3
