/*
 * ppp-comp.h - Definitions for doing PPP packet compression.
 *
 * Copyright (c) 1994 The Australian National University.
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, provided that the above copyright
 * notice appears in all copies.  This software is provided without any
 * warranty, express or implied. The Australian National University
 * makes no representations about the suitability of this software for
 * any purpose.
 *
 * IN NO EVENT SHALL THE AUSTRALIAN NATIONAL UNIVERSITY BE LIABLE TO ANY
 * PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
 * THE AUSTRALIAN NATIONAL UNIVERSITY HAVE BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * THE AUSTRALIAN NATIONAL UNIVERSITY SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE AUSTRALIAN NATIONAL UNIVERSITY HAS NO
 * OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS,
 * OR MODIFICATIONS.
 *
 * $Id: ppp-comp.h,v 1.7 1995/05/01 01:43:37 paulus Exp $
 */

#ifndef _NET_PPP_COMP_H
#define _NET_PPP_COMP_H

/*
 * The following symbols control whether we include code for
 * various compression methods.
 */
#ifndef DO_BSD_COMPRESS
#define DO_BSD_COMPRESS	0	/* by default, include BSD-Compress */
#endif

/*
 * Structure giving methods for compression/decompression.
 */
#define PACKETPTR        struct mbuf *

struct compressor {
	int	compress_proto;	/* CCP compression protocol number */

	/* Allocate space for a compressor (transmit side) */
	void	*(*comp_alloc); __P((u_char *options, int opt_len));
	/* Free space used by a compressor */
	void	(*comp_free); __P((void *state));
	/* Initialize a compressor */
	int	(*comp_init); __P((void *state, u_char *options, int opt_len,
				  int unit, int hdrlen, int debug));
	/* Reset a compressor */
	void	(*comp_reset); __P((void *state));
	/* Compress a packet */
	int	(*compress); __P((void *state, PACKETPTR *mret,
				 PACKETPTR mp, int orig_len, int max_len, int proto));
	/* Return compression statistics */
	void	(*comp_stat); __P((void *state, struct compstat *stats));

	/* Allocate space for a decompressor (receive side) */
	void	*(*decomp_alloc); __P((u_char *options, int opt_len));
	/* Free space used by a decompressor */
	void	(*decomp_free); __P((void *state));
	/* Initialize a decompressor */
	int	(*decomp_init); __P((void *state, u_char *options, int opt_len,
				    int unit, int hdrlen, int mru, int debug));
	/* Reset a decompressor */
	void	(*decomp_reset); __P((void *state));
	/* Decompress a packet. */
	int	(*decompress); __P((void *state, PACKETPTR mp,
				   PACKETPTR *dmpp));
	/* Update state for an incompressible packet received */
	void	(*incomp); __P((void *state, PACKETPTR mp, int proto));
	/* Return decompression statistics */
	void	(*decomp_stat); __P((void *state, struct compstat *stats));
};

/*
 * Return values for decompress routine.
 * We need to make these distinctions so that we can disable certain
 * useful functionality, namely sending a CCP reset-request as a result
 * of an error detected after decompression.  This is to avoid infringing
 * a patent held by Motorola.
 * Don't you just lurve software patents.
 */
#define DECOMP_OK		0	/* everything went OK */
#define DECOMP_ERROR		1	/* error detected before decomp. */
#define DECOMP_FATALERROR	2	/* error detected after decomp. */

/*
 * CCP codes.
 */
#define CCP_CONFREQ	1
#define CCP_CONFACK	2
#define CCP_TERMREQ	5
#define CCP_TERMACK	6
#define CCP_RESETREQ	14
#define CCP_RESETACK	15

/*
 * Max # bytes for a CCP option
 */
#define CCP_MAX_OPTION_LENGTH	32

/*
 * Parts of a CCP packet.
 */
#define CCP_CODE(dp)		((dp)[0])
#define CCP_ID(dp)		((dp)[1])
#define CCP_LENGTH(dp)		(((dp)[2] << 8) + (dp)[3])
#define CCP_HDRLEN		4

#define CCP_OPT_CODE(dp)	((dp)[0])
#define CCP_OPT_LENGTH(dp)	((dp)[1])
#define CCP_OPT_MINLEN		2

/*
 * Definitions for BSD-Compress.
 */
#define CI_BSD_COMPRESS		21	/* config. option for BSD-Compress */
#define CILEN_BSD_COMPRESS	3	/* length of config. option */

/* Macros for handling the 3rd byte of the BSD-Compress config option. */
#define BSD_NBITS(x)		((x) & 0x1F)	/* number of bits requested */
#define BSD_VERSION(x)		((x) >> 5)	/* version of option format */
#define BSD_CURRENT_VERSION	1		/* current version number */
#define BSD_MAKE_OPT(v, n)	(((v) << 5) | (n))

#define BSD_MIN_BITS		9	/* smallest code size supported */
#define BSD_MAX_BITS		15	/* largest code size supported */

#endif /* _NET_PPP_COMP_H */
