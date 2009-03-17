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

#ifndef BS2BCLASS_H
#define BS2BCLASS_H

#include "bs2b.h"

class bs2b_base
{
private:
	t_bs2bdp bs2bdp;

public:
	bs2b_base();
	~bs2b_base();

	void        set_level( uint32_t level );
	uint32_t    get_level();
	void        set_srate( uint32_t srate );
	uint32_t    get_srate();
	void        clear();
	bool        is_clear();
	char const *runtime_version( void );

	void cross_feed( double   *sample, int n = 1 );
	void cross_feed( float    *sample, int n = 1 );
	void cross_feed( int32_t  *sample, int n = 1 );
	void cross_feed( int16_t  *sample, int n = 1 );
	void cross_feed( int8_t   *sample, int n = 1 );
	void cross_feed( uint8_t  *sample, int n = 1 );
	void cross_feed( int24_t  *sample, int n = 1 );

	void cross_feed_be( double   *sample, int n = 1 );
	void cross_feed_be( float    *sample, int n = 1 );
	void cross_feed_be( int32_t  *sample, int n = 1 );
	void cross_feed_be( int16_t  *sample, int n = 1 );
	void cross_feed_be( int8_t   *sample, int n = 1 );
	void cross_feed_be( uint8_t  *sample, int n = 1 );
	void cross_feed_be( int24_t  *sample, int n = 1 );

	void cross_feed_le( double   *sample, int n = 1 );
	void cross_feed_le( float    *sample, int n = 1 );
	void cross_feed_le( int32_t  *sample, int n = 1 );
	void cross_feed_le( int16_t  *sample, int n = 1 );
	void cross_feed_le( int8_t   *sample, int n = 1 );
	void cross_feed_le( uint8_t  *sample, int n = 1 );
	void cross_feed_le( int24_t  *sample, int n = 1 );
}; // class bs2b_base

#endif // BS2BCLASS_H
