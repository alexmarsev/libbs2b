/*-
 * Copyright (c) 2005 Boris Mikhaylov
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#if defined( _O_BINARY ) || defined( _O_RAW )
#include <io.h>
#endif

#include "bs2b.h"

static void print_usage( char *progname )
{
	fprintf( stderr, "\n"
		"    Bauer stereophonic-to-binaural DSP stream converter. "
		"Version %s\n"
		"    PCM stdin-stdout, 44100Hz, 16bit\n",
		BS2B_VERSION_STR );
	fprintf( stderr, "Usage : %s [-x]\n", progname );
	fprintf( stderr, "\n"
		"    'x' is number of:\n"
		"    1,2,3 - Low to High crossfeed levels,\n"
		"    4,5,6 - Low to High crossfeed levels of 'Easy' version\n"
		"    The default crossfeed level is 6\n" );
} /* print_usage() */

int main( int argc, char *argv[] )
{
	t_bs2bdp bs2bdp;
	char     *progname, *tmpstr;
	uint32_t srate;
	uint32_t level;
	short    sample[ 2 ];

	tmpstr = strrchr( argv[0], '/' );
	tmpstr = tmpstr ? tmpstr + 1 : argv[ 0 ];
	progname = strrchr( tmpstr, '\\' );
	progname = progname ? progname + 1 : tmpstr;

	if( argc > 2 )
	{
		print_usage( progname );
		return 1;
	}

	level = BS2B_DEFAULT_CLEVEL;

	if( argc == 2 )
	{
		if( ( !argv[ 1 ][ 1 ] ) || ( argv[ 1 ][ 0 ] != '-' ) )
		{
			print_usage( progname );
			return 1;
		}

		switch( argv[ 1 ][ 1 ] )
		{
		case '1':
			level = BS2B_LOW_CLEVEL;
			break;

		case '2':
			level = BS2B_MIDDLE_CLEVEL;
			break;

		case '3':
			level = BS2B_HIGH_CLEVEL;
			break;

		case '4':
			level = BS2B_LOW_ECLEVEL;
			break;

		case '5':
			level = BS2B_MIDDLE_ECLEVEL;
			break;
		} /* switch */
	}

	srate = 44100;

	bs2bdp = bs2b_open();

	bs2b_set_srate( bs2bdp, srate );
	bs2b_set_level( bs2bdp, level );

#if defined( _O_BINARY )
	_setmode( _fileno( stdin ),  _O_BINARY );
	_setmode( _fileno( stdout ), _O_BINARY );
#elif defined( _O_RAW )
	_setmode( _fileno( stdin ),  _O_RAW );
	_setmode( _fileno( stdout ), _O_RAW );
#endif

	while( 2 == fread( sample, sizeof( short ), 2, stdin ) )
	{
		bs2b_cross_feed_s16ne( bs2bdp, sample, 1 );
		fwrite( sample, sizeof( short ), 2, stdout );
	}

	bs2b_close( bs2bdp );
	bs2bdp = 0;

	return 0 ;
} /* main() */
