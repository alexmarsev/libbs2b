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

#ifndef BS2B_H
#define BS2B_H

#include <limits.h>

#include "bs2bversion.h"

#if HAVE_STDINT_H
# include <stdint.h>
#else
typedef signed   char    int8_t;
typedef unsigned char    uint8_t;
typedef signed   short   int16_t;
typedef unsigned short   uint16_t;
# if UINT_MAX == 0xffff /* 16 bit compiler */
typedef signed   long    int32_t;
typedef unsigned long    uint32_t;
# else /* UINT_MAX != 0xffff */ /* 32/64 bit compiler */
typedef signed   int     int32_t;
typedef unsigned int     uint32_t;
# endif
#endif /* HAVE_STDINT_H */

typedef struct
{
#ifdef WORDS_BIGENDIAN
	int8_t  octet2;
	uint8_t octet1;
	uint8_t octet0;
#else
	uint8_t octet0;
	uint8_t octet1;
	int8_t  octet2;
#endif /* WORDS_BIGENDIAN */
} int24_t;

typedef struct
{
#ifdef WORDS_BIGENDIAN
	uint8_t octet2;
	uint8_t octet1;
	uint8_t octet0;
#else
	uint8_t octet0;
	uint8_t octet1;
	uint8_t octet2;
#endif /* WORDS_BIGENDIAN */
} uint24_t;

/* Number of crossfeed levels */
#define BS2B_CLEVELS           3

/* Normal crossfeed levels */
#define BS2B_HIGH_CLEVEL       3
#define BS2B_MIDDLE_CLEVEL     2
#define BS2B_LOW_CLEVEL        1

/* Easy crossfeed levels */
#define BS2B_HIGH_ECLEVEL      BS2B_HIGH_CLEVEL    + BS2B_CLEVELS
#define BS2B_MIDDLE_ECLEVEL    BS2B_MIDDLE_CLEVEL  + BS2B_CLEVELS
#define BS2B_LOW_ECLEVEL       BS2B_LOW_CLEVEL     + BS2B_CLEVELS

/* Default crossfeed levels */
#define BS2B_DEFAULT_CLEVEL    BS2B_HIGH_ECLEVEL
/* Default sample rate (Hz) */
#define BS2B_DEFAULT_SRATE     44100

typedef struct
{
	uint32_t level;              /* Crossfeed level */
	uint32_t srate;              /* Sample rate (Hz) */
	double a0_lo, b1_lo;         /* Lowpass IIR filter coefficients */
	double a0_hi, a1_hi, b1_hi;  /* Highboost IIR filter coefficients */
	double gain;                 /* Global gain against overloading */
	/* Buffer of last filtered sample: [0] 1-st channel, [1] 2-d channel */
	struct { double asis[ 2 ], lo[ 2 ], hi[ 2 ]; } lfs;
} t_bs2bd;

typedef t_bs2bd *t_bs2bdp;

#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */

/* Allocates and sets a data to defaults.
 * Return NULL on error.
 */
t_bs2bdp bs2b_open( void );

/* Close */
void bs2b_close( t_bs2bdp bs2bdp );

/* Sets a new coefficients with new crossfeed level value.
 * level - crossfeed level of *LEVEL values.
 */
void bs2b_set_level( t_bs2bdp bs2bdp, uint32_t level );

/* Return current crossfeed level value */
uint32_t bs2b_get_level( t_bs2bdp bs2bdp );

/* Clear buffers and sets a new coefficients with new sample rate value.
 * srate - sample rate by Hz.
 */
void bs2b_set_srate( t_bs2bdp bs2bdp, uint32_t srate );

/* Return current sample rate value */
uint32_t bs2b_get_srate( t_bs2bdp bs2bdp );

/* Clear buffer */
void bs2b_clear( t_bs2bdp bs2bdp );

/* Return 1 if buffer is clear */
int bs2b_is_clear( t_bs2bdp bs2bdp );

/* Crossfeeds one stereo sample that are pointed by sample.
 * [0] - first channel, [1] - second channel.
 * Returns crossfided samle by sample pointer.
 */

/* sample poits to double floats native endians */
void bs2b_cross_feed_dne( t_bs2bdp bs2bdp, double *sample );

/* sample poits to floats native endians */
void bs2b_cross_feed_fne( t_bs2bdp bs2bdp, float *sample );

/* sample poits to 32bit signed integers native endians */
void bs2b_cross_feed_s32ne( t_bs2bdp bs2bdp, int32_t *sample );

/* sample poits to 16bit signed integers native endians */
void bs2b_cross_feed_s16ne( t_bs2bdp bs2bdp, int16_t *sample );

/* sample poits to 8bit signed integers */
void bs2b_cross_feed_s8( t_bs2bdp bs2bdp, int8_t *sample );

/* sample poits to 8bit unsigned integers */
void bs2b_cross_feed_u8( t_bs2bdp bs2bdp, uint8_t *sample );

/* sample poits to 24bit signed integers native endians */
void bs2b_cross_feed_s24ne( t_bs2bdp bs2bdp, int24_t *sample );

/* Return bs2b version string */
char const *bs2b_runtime_version( void );

#ifdef __cplusplus
}	/* extern "C" */
#endif /* __cplusplus */

#endif	/* BS2B_H */
