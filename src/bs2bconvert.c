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

/* MSVC: /I "$(ProjectDir)\..\sndfile" */
/* gcc: -I../libsndfile-1.0.17/src */
#include <sndfile.h>

#include "bs2b.h"

#define	 BUFFER_LEN	1024 /* must be multiply of 2 */

static void copy_metadata( SNDFILE *outfile, SNDFILE *infile );
static void copy_data( SNDFILE *outfile, SNDFILE *infile, t_bs2bdp bs2bdp );

static void print_usage( char *progname )
{
	printf( "\n"
		"    Bauer stereophonic-to-binaural DSP converter. "
		"Version %s\n",
		BS2B_VERSION_STR );
	printf( "Usage : %s [-x] <input file> <output file>\n", progname );
	printf( "\n"
		"    'x' is number of:\n"
		"    1,2,3 - Low to High crossfeed levels,\n"
		"    4,5,6 - Low to High crossfeed levels of 'Easy' version\n"
		"    The default crossfeed level is 6\n" );
} /* print_usage() */

int main( int argc, char *argv[] )
{
	char     *progname, *infilename, *outfilename, *tmpstr;
	SNDFILE  *infile = NULL, *outfile = NULL;
	SF_INFO  sfinfo;
	t_bs2bdp bs2bdp;
	long     srate;
	int      level;

	tmpstr = strrchr( argv[ 0 ], '/' );
	tmpstr = tmpstr ? tmpstr + 1 : argv[ 0 ];
	progname = strrchr( tmpstr, '\\' );
	progname = progname ? progname + 1 : tmpstr;

	if( argc < 3 || argc > 4 )
	{
		print_usage( progname );
		return 1;
	}

	infilename  = argv[ argc - 2 ];
	outfilename = argv[ argc - 1 ];

	level = BS2B_DEFAULT_CLEVEL;

	if( argc == 4 )
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

	if( strcmp( infilename, outfilename ) == 0 )
	{
		puts( "Error : Input and output filenames are the same.\n\n" );
		print_usage( progname );
		return 1;
	}
	
	if( ( infile = sf_open( infilename, SFM_READ, &sfinfo ) ) == NULL )
	{
		printf( "Not able to open input file %s.\n", infilename );
		puts( sf_strerror( NULL ) );
		return 1;
	}

	if( sfinfo.channels != 2 )
	{
		puts( "Input file is not a stereo.\n" );
		sf_close( infile );
		return 1;
	}

	srate = sfinfo.samplerate;

	/* Open the output file. */
	if( ( outfile = sf_open( outfilename, SFM_WRITE, &sfinfo ) ) == NULL )
	{
		printf( "Not able to open output file %s : %s\n", outfilename, sf_strerror( NULL ) );
		sf_close( infile );
		return 1;
	}

	{
		char *levels[] = { "low", "middle", "high", "low easy", "middle easy", "high easy" };

		printf( "Converting file %s to file %s\nsample rate=%li, crossfeed level=%s...",
			infilename, outfilename, srate, levels[ level - 1 ] );
	}

	if( !( bs2bdp = bs2b_open() ) )
	{
		puts( "Not able to allocate data\n" );
		sf_close( infile );
		sf_close( outfile );
		return 1;
	}

	bs2b_set_srate( bs2bdp, srate );
	bs2b_set_level( bs2bdp, level );

	copy_metadata( outfile, infile );

	copy_data( outfile, infile, bs2bdp );

	bs2b_close( bs2bdp );
	bs2bdp = 0;

	sf_close( infile );
	sf_close( outfile );

	puts( " done\n" );

	return 0;
} /* main() */

static void copy_metadata( SNDFILE *outfile, SNDFILE *infile )
{
	const char *str;
	int k, err = 0;

	for( k = SF_STR_FIRST; k <= SF_STR_LAST; k++ )
	{
		str = sf_get_string( infile, k );
		if( str != NULL )
			err = sf_set_string( outfile, k, str );
	}
	
	err = sf_set_string( outfile, SF_STR_COMMENT, "CROSSFEEDED" );
} /* copy_metadata() */

static void copy_data( SNDFILE *outfile, SNDFILE *infile, t_bs2bdp bs2bdp )
{
	static double data[ BUFFER_LEN ];
	int items, readcount, k;

	items = BUFFER_LEN;
	readcount = items;

	while( readcount > 0 )
	{
		readcount = (int)sf_read_double( infile, data, items );

		for( k = 0; k < readcount; k += 2 )
			bs2b_cross_feed( bs2bdp, data + k );

		sf_write_double( outfile, data, readcount );
	}

	return;
} /* copy_data() */
