/*
 * Common functions for reordering audio channels.
 * Imported from Mplayer
 *
 * Copyright (C) 2007 Ulion <ulion A gmail P com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef REORDER_CH_H
#define REORDER_CH_H

// L   - Left
// R   - Right
// C   - Center
// Ls  - Left Surround
// Rs  - Right Surround
// Cs  - Center Surround
// Rls - Rear Left Surround
// Rrs - Rear Right Surround

#define AF_LFE   (1<<7)

#define AF_CHANNEL_LAYOUT_MONO   ((100<<8)|1)
#define AF_CHANNEL_LAYOUT_STEREO ((101<<8)|2)
#define AF_CHANNEL_LAYOUT_1_0 AF_CHANNEL_LAYOUT_MONO   // C
#define AF_CHANNEL_LAYOUT_2_0 AF_CHANNEL_LAYOUT_STEREO // L R
#define AF_CHANNEL_LAYOUT_2_1   ((102<<8)|3)           // L R LFE
#define AF_CHANNEL_LAYOUT_3_0_A ((103<<8)|3)           // L R C
#define AF_CHANNEL_LAYOUT_3_0_B ((104<<8)|3)           // C L R
#define AF_CHANNEL_LAYOUT_4_0_A ((105<<8)|4)           // L R C Cs
#define AF_CHANNEL_LAYOUT_4_0_B ((106<<8)|4)           // C L R Cs
#define AF_CHANNEL_LAYOUT_4_0_C ((107<<8)|4)           // L R Ls Rs
#define AF_CHANNEL_LAYOUT_5_0_A ((108<<8)|5)           // L R C Ls Rs
#define AF_CHANNEL_LAYOUT_5_0_B ((109<<8)|5)           // L R Ls Rs C
#define AF_CHANNEL_LAYOUT_5_0_C ((110<<8)|5)           // L C R Ls Rs
#define AF_CHANNEL_LAYOUT_5_0_D ((111<<8)|5)           // C L R Ls Rs
#define AF_CHANNEL_LAYOUT_5_1_A ((112<<8)|6|AF_LFE)    // L R C LFE Ls  Rs
#define AF_CHANNEL_LAYOUT_5_1_B ((113<<8)|6|AF_LFE)    // L R Ls Rs C   LFE
#define AF_CHANNEL_LAYOUT_5_1_C ((114<<8)|6|AF_LFE)    // L C R Ls Rs   LFE
#define AF_CHANNEL_LAYOUT_5_1_D ((115<<8)|6|AF_LFE)    // C L R Ls Rs   LFE
#define AF_CHANNEL_LAYOUT_5_1_E ((116<<8)|6|AF_LFE)    // LFE L C R Ls  Rs
#define AF_CHANNEL_LAYOUT_6_1_A ((117<<8)|7|AF_LFE)    // L R C LFE Ls  Rs Cs
#define AF_CHANNEL_LAYOUT_7_1_A ((118<<8)|8|AF_LFE)    // L R C LFE Rls Rrs Ls Rs
#define AF_CHANNEL_LAYOUT_7_1_B ((119<<8)|8|AF_LFE)    // L R C LFE Ls  Rs Rls Rrs
#define AF_CHANNEL_LAYOUT_7_1_C ((120<<8)|8|AF_LFE)    // L R C Ls  Rls Rrs Rs LFE

#define AF_CHANNEL_LAYOUT_FFDSHOW_5CH_DEFAULT AF_CHANNEL_LAYOUT_5_0_A
#define AF_CHANNEL_LAYOUT_FFDSHOW_6CH_DEFAULT AF_CHANNEL_LAYOUT_5_1_A
#define AF_CHANNEL_LAYOUT_FFDSHOW_7CH_DEFAULT AF_CHANNEL_LAYOUT_6_1_A
#define AF_CHANNEL_LAYOUT_FFDSHOW_8CH_DEFAULT AF_CHANNEL_LAYOUT_7_1_A
#define AF_CHANNEL_LAYOUT_LPCM_8CH_DEFAULT AF_CHANNEL_LAYOUT_7_1_C

#define AF_CHANNEL_MASK  0xFF
#define AF_GET_CH_NUM(A) ((A)&0x7F)
#define AF_GET_CH_NUM_WITH_LFE(A) ((A)&0xFF)
#define AF_IS_SAME_CH_NUM(A,B) (((A)&0xFF)==((B)&0xFF))
#define AF_IS_LAYOUT_SPECIFIED(A) ((A)&0xFFFFF800)
#define AF_IS_LAYOUT_UNSPECIFIED(A) (!AF_IS_LAYOUT_SPECIFIED(A))

/// Optimized channel reorder between channel layouts with same channel number.
void reorder_channel_copy(void *src,
                          int src_layout,
                          void *dest,
                          int dest_layout,
                          int samples,
                          int samplesize);

/// Same with reorder_channel_copy, but works on single buffer.
void reorder_channel(void *buf,
                     int src_layout,
                     int dest_layout,
                     int samples,
                     int samplesize);

#endif /* REORDER_CH_H */
