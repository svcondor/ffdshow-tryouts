/*
 * Common code between AC3 encoder and decoder
 * Copyright (c) 2000, 2001, 2002 Fabrice Bellard.
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file ac3.h
 * Common code between AC3 encoder and decoder.
 */

#define AC3_MAX_CODED_FRAME_SIZE 3840 /* in bytes */
#define AC3_MAX_CHANNELS 6 /* including LFE channel */

#define NB_BLOCKS 6 /* number of PCM blocks inside an AC3 frame */
#define AC3_FRAME_SIZE (NB_BLOCKS * 256)

/* exponent encoding strategy */
#define EXP_REUSE 0
#define EXP_NEW   1

#define EXP_D15   1
#define EXP_D25   2
#define EXP_D45   3

typedef struct AC3BitAllocParameters {
    int fscod; /* frequency */
    int halfratecod;
    int sgain, sdecay, fdecay, dbknee, floor;
    int cplfleak, cplsleak;
} AC3BitAllocParameters;

extern const uint16_t ff_ac3_freqs[3];
extern const uint16_t ff_ac3_bitratetab[19];
extern const int16_t ff_ac3_window[256];
extern const uint8_t ff_sdecaytab[4];
extern const uint8_t ff_fdecaytab[4];
extern const uint16_t ff_sgaintab[4];
extern const uint16_t ff_dbkneetab[4];
extern const int16_t ff_floortab[8];
extern const uint16_t ff_fgaintab[8];

void ac3_common_init(void);
void ac3_parametric_bit_allocation(AC3BitAllocParameters *s, uint8_t *bap,
                                   int8_t *exp, int start, int end,
                                   int snroffset, int fgain, int is_lfe,
                                   int deltbae,int deltnseg,
                                   uint8_t *deltoffst, uint8_t *deltlen, uint8_t *deltba);
