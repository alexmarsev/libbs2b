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

#include "bs2bclass.h"

bs2b_base::bs2b_base()
{
	bs2bdp = bs2b_open();
}

bs2b_base::~bs2b_base()
{
	bs2b_close( bs2bdp );
}

void bs2b_base::set_level( uint32_t level )
{
	bs2b_set_level( bs2bdp, level );
}

uint32_t bs2b_base::get_level()
{
	return bs2b_get_level( bs2bdp );
}

void bs2b_base::set_srate( uint32_t srate )
{
	bs2b_set_srate( bs2bdp, srate );
}

uint32_t bs2b_base::get_srate()
{
	return bs2b_get_srate( bs2bdp );
}

void bs2b_base::clear()
{
	bs2b_clear( bs2bdp );
}

bool bs2b_base::is_clear()
{
	return( bs2b_is_clear( bs2bdp ) ? true : false );
}

char const *bs2b_base::runtime_version( void )
{
	return bs2b_runtime_version();
}

void bs2b_base::cross_feed( double *sample, int n )
{
	bs2b_cross_feed_d( bs2bdp, sample, n );
}

void bs2b_base::cross_feed_be( double *sample, int n )
{
	bs2b_cross_feed_dbe( bs2bdp, sample, n );
}

void bs2b_base::cross_feed_le( double *sample, int n )
{
	bs2b_cross_feed_dle( bs2bdp, sample, n );
}

void bs2b_base::cross_feed( float *sample, int n )
{
	bs2b_cross_feed_f( bs2bdp, sample, n );
}

void bs2b_base::cross_feed_be( float *sample, int n )
{
	bs2b_cross_feed_fbe( bs2bdp, sample, n );
}

void bs2b_base::cross_feed_le( float *sample, int n )
{
	bs2b_cross_feed_fle( bs2bdp, sample, n );
}

void bs2b_base::cross_feed( int32_t *sample, int n )
{
	bs2b_cross_feed_s32( bs2bdp, sample, n );
}

void bs2b_base::cross_feed_be( int32_t *sample, int n )
{
	bs2b_cross_feed_s32be( bs2bdp, sample, n );
}

void bs2b_base::cross_feed_le( int32_t *sample, int n )
{
	bs2b_cross_feed_s32le( bs2bdp, sample, n );
}

void bs2b_base::cross_feed( int16_t *sample, int n )
{
	bs2b_cross_feed_s16( bs2bdp, sample, n );
}

void bs2b_base::cross_feed_be( int16_t *sample, int n )
{
	bs2b_cross_feed_s16be( bs2bdp, sample, n );
}

void bs2b_base::cross_feed_le( int16_t *sample, int n )
{
	bs2b_cross_feed_s16le( bs2bdp, sample, n );
}

void bs2b_base::cross_feed( int8_t *sample, int n )
{
	bs2b_cross_feed_s8( bs2bdp, sample, n );
}

void bs2b_base::cross_feed( uint8_t *sample, int n )
{
	bs2b_cross_feed_u8( bs2bdp, sample, n );
}

void bs2b_base::cross_feed( int24_t *sample, int n )
{
	bs2b_cross_feed_s24( bs2bdp, sample, n );
}

void bs2b_base::cross_feed_be( int24_t *sample, int n )
{
	bs2b_cross_feed_s24be( bs2bdp, sample, n );
}

void bs2b_base::cross_feed_le( int24_t *sample, int n )
{
	bs2b_cross_feed_s24le( bs2bdp, sample, n );
}
