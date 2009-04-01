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
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#if defined( _O_BINARY ) || defined( _O_RAW )
#include <io.h>
#endif

#include "bs2b.h"

static void print_usage( char *progname )
{
	fprintf( stderr, "\n"
		"Bauer stereophonic-to-binaural DSP stream converter. Version %s\n"
		"LPCM stdin-stdout\n\n",
		BS2B_VERSION_STR );
	fprintf( stderr, "Usage : %s [-h] [-u] [-e E] [-b B] [-r R] [-l L]\n",
		progname );
	fprintf( stderr,
		"-h - this help.\n"
		"-u - unsigned data. Default is signed.\n"
		"-e - endians, E = b|l|n (big|little|native). Default is native.\n"
		"-b - bits per integer sample, B=8|16|24|32. Default is 16 bit.\n"
		"-r - sample rate, R = <value by kHz>. Default is 44.1 kHz.\n"
		"-l - crossfeed level, L=d|c|m:\n"
		"     d - default preset     - 700Hz/260us, 4.5 dB;\n"
		"     c - Chu Moy's preset   - 700Hz/260us, 6.0 dB;\n"
		"     m - Jan Meier's preset - 650Hz/280us, 9.5 dB.\n" );
} /* print_usage() */

int main( int argc, char *argv[] )
{
	int i;
	char *progname, *tmpstr;

	t_bs2bdp bs2bdp;

	uint32_t srate = 44100;
	uint32_t level = 'd';
	int bits = 16;
	int unsigned_flag = 0;
	int endians = 'n';

	tmpstr = strrchr( argv[0], '/' );
	tmpstr = tmpstr ? tmpstr + 1 : argv[ 0 ];
	progname = strrchr( tmpstr, '\\' );
	progname = progname ? progname + 1 : tmpstr;

	for( i = 1; i < argc; i++ )
	{
		double df;

		if( '-' != argv[ i ][ 0 ] )
		{
			print_usage( progname );
			return 1;
		}
		else
		{
			switch( argv[ i ][ 1 ] )
			{
				case 'h':
					print_usage( progname );
					return 1;

				case 'u':
					unsigned_flag = 1;
					break;

				case 'e':
					if( ++i >= argc )
					{
						print_usage( progname );
						return 1;
					}
					endians = argv[ i ][ 0 ];
					if( endians != 'n' &&
						endians != 'b' &&
						endians != 'l' )
					{
						print_usage( progname );
						return 1;
					}
					break;

				case 'b':
					if( ++i >= argc )
					{
						print_usage( progname );
						return 1;
					}
					bits = atoi( argv[ i ] );
					if( bits != 8 &&
						bits != 16 &&
						bits != 24 &&
						bits != 32 )
					{
						print_usage( progname );
						return 1;
					}
					break;

				case 'r':
					if( ++i >= argc )
					{
						print_usage( progname );
						return 1;
					}
					df = atof( argv[ i ] );
					srate = ( uint32_t )( df * 1000 );
					if( srate > 384000 || srate < 2000 )
					{
						print_usage( progname );
						return 1;
					}
					break;

				case 'l':
					if( ++i >= argc )
					{
						print_usage( progname );
						return 1;
					}
					level = argv[ i ][ 0 ];
					if( level != 'd' &&
						level != 'c' &&
						level != 'm' )
					{
						print_usage( progname );
						return 1;
					}
					break;
				default:
					print_usage( progname );
					return 1;
			} /* swith */
		} /* if */
	} /* for */

	switch( level )
	{
	case 'c':
		level = BS2B_CMOY_CLEVEL;
		break;
	case 'm':
		level = BS2B_JMEIER_CLEVEL;
		break;
	default:
		level = BS2B_DEFAULT_CLEVEL;
		break;
	} /* switch */

	#if defined( _O_BINARY )
	_setmode( _fileno( stdin ),  _O_BINARY );
	_setmode( _fileno( stdout ), _O_BINARY );
	#elif defined( _O_RAW )
	_setmode( _fileno( stdin ),  _O_RAW );
	_setmode( _fileno( stdout ), _O_RAW );
	#endif

	bs2bdp = bs2b_open();

	bs2b_set_srate( bs2bdp, srate );
	bs2b_set_level( bs2bdp, level );

	switch( bits )
	{
	case 8:
		{
			int8_t sample[ 2 ];
			while( 2 == fread( sample, sizeof( int8_t ), 2, stdin ) )
			{
				if( unsigned_flag )
					bs2b_cross_feed_u8( bs2bdp, sample, 1 );
				else
					bs2b_cross_feed_s8( bs2bdp, sample, 1 );

				fwrite( sample, sizeof( int8_t ), 2, stdout );
			} /* while */
		}
		break;

	case 16:
		{
			int16_t sample[ 2 ];
			while( 2 == fread( sample, sizeof( int16_t ), 2, stdin ) )
			{
				switch( endians )
				{
				case 'b':
					{
						if( unsigned_flag )
							bs2b_cross_feed_u16be( bs2bdp, sample, 1 );
						else
							bs2b_cross_feed_s16be( bs2bdp, sample, 1 );
					}
					break;

				case 'l':
					{
						if( unsigned_flag )
							bs2b_cross_feed_u16le( bs2bdp, sample, 1 );
						else
							bs2b_cross_feed_s16le( bs2bdp, sample, 1 );
					}
					break;

				default:
					{
						if( unsigned_flag )
							bs2b_cross_feed_u16( bs2bdp, sample, 1 );
						else
							bs2b_cross_feed_s16( bs2bdp, sample, 1 );
					}
					break;
				} /* switch( endians ) */

				fwrite( sample, sizeof( int16_t ), 2, stdout );
			} /* while */
		}
		break;

	case 24:
		{
			bs2b_int24_t sample[ 2 ];
			while( 2 == fread( sample, sizeof( bs2b_int24_t ), 2, stdin ) )
			{
				switch( endians )
				{
				case 'b':
					{
						if( unsigned_flag )
							bs2b_cross_feed_u24be( bs2bdp, ( bs2b_uint24_t * )sample, 1 );
						else
							bs2b_cross_feed_s24be( bs2bdp, sample, 1 );
					}
					break;

				case 'l':
					{
						if( unsigned_flag )
							bs2b_cross_feed_u24le( bs2bdp, ( bs2b_uint24_t * )sample, 1 );
						else
							bs2b_cross_feed_s24le( bs2bdp, sample, 1 );
					}
					break;

				default:
					{
						if( unsigned_flag )
							bs2b_cross_feed_u24( bs2bdp, ( bs2b_uint24_t * )sample, 1 );
						else
							bs2b_cross_feed_s24( bs2bdp, sample, 1 );
					}
					break;
				} /* switch( endians ) */

				fwrite( sample, sizeof( bs2b_int24_t ), 2, stdout );
			} /* while */
		}
		break;

	case 32:
		{
			int32_t sample[ 2 ];
			while( 2 == fread( sample, sizeof( int32_t ), 2, stdin ) )
			{
				switch( endians )
				{
				case 'b':
					{
						if( unsigned_flag )
							bs2b_cross_feed_u32be( bs2bdp, sample, 1 );
						else
							bs2b_cross_feed_s32be( bs2bdp, sample, 1 );
					}
					break;

				case 'l':
					{
						if( unsigned_flag )
							bs2b_cross_feed_u32le( bs2bdp, sample, 1 );
						else
							bs2b_cross_feed_s32le( bs2bdp, sample, 1 );
					}
					break;

				default:
					{
						if( unsigned_flag )
							bs2b_cross_feed_u32( bs2bdp, sample, 1 );
						else
							bs2b_cross_feed_s32( bs2bdp, sample, 1 );
					}
					break;
				} /* switch( endians ) */

				fwrite( sample, sizeof( int32_t ), 2, stdout );
			} /* while */
		}
		break;

	default:
		break;
	} /* switch( bits ) */

	bs2b_close( bs2bdp );
	bs2bdp = 0;

	return 0 ;
} /* main() */
