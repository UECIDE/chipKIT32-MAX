/*
 * Copyright (c) 2015, chipKIT
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of chipKIT nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _DMA_H
#define _DMA_H

// This is what's known as planning ahead. We don't have 32
// DMA channels in any chip *yet*, but why not plan for the
// future?
#if defined(_DCH31CON_CHPRI_POSITION)
    #define _NUM_DMA 32
#elif defined(_DCH30CON_CHPRI_POSITION)
    #define _NUM_DMA 31
#elif defined(_DCH29CON_CHPRI_POSITION)
    #define _NUM_DMA 30
#elif defined(_DCH28CON_CHPRI_POSITION)
    #define _NUM_DMA 29
#elif defined(_DCH27CON_CHPRI_POSITION)
    #define _NUM_DMA 28
#elif defined(_DCH26CON_CHPRI_POSITION)
    #define _NUM_DMA 27
#elif defined(_DCH25CON_CHPRI_POSITION)
    #define _NUM_DMA 26
#elif defined(_DCH24CON_CHPRI_POSITION)
    #define _NUM_DMA 25
#elif defined(_DCH23CON_CHPRI_POSITION)
    #define _NUM_DMA 24
#elif defined(_DCH22CON_CHPRI_POSITION)
    #define _NUM_DMA 23
#elif defined(_DCH21CON_CHPRI_POSITION)
    #define _NUM_DMA 22
#elif defined(_DCH20CON_CHPRI_POSITION)
    #define _NUM_DMA 21
#elif defined(_DCH19CON_CHPRI_POSITION)
    #define _NUM_DMA 20
#elif defined(_DCH18CON_CHPRI_POSITION)
    #define _NUM_DMA 19
#elif defined(_DCH17CON_CHPRI_POSITION)
    #define _NUM_DMA 18
#elif defined(_DCH16CON_CHPRI_POSITION)
    #define _NUM_DMA 17
#elif defined(_DCH15CON_CHPRI_POSITION)
    #define _NUM_DMA 16
#elif defined(_DCH14CON_CHPRI_POSITION)
    #define _NUM_DMA 15
#elif defined(_DCH13CON_CHPRI_POSITION)
    #define _NUM_DMA 14
#elif defined(_DCH12CON_CHPRI_POSITION)
    #define _NUM_DMA 13
#elif defined(_DCH11CON_CHPRI_POSITION)
    #define _NUM_DMA 12
#elif defined(_DCH10CON_CHPRI_POSITION)
    #define _NUM_DMA 11
#elif defined(_DCH9CON_CHPRI_POSITION)
    #define _NUM_DMA 10
#elif defined(_DCH8CON_CHPRI_POSITION)
    #define _NUM_DMA 9
#elif defined(_DCH7CON_CHPRI_POSITION)
    #define _NUM_DMA 8
#elif defined(_DCH6CON_CHPRI_POSITION)
    #define _NUM_DMA 7
#elif defined(_DCH5CON_CHPRI_POSITION)
    #define _NUM_DMA 6
#elif defined(_DCH4CON_CHPRI_POSITION)
    #define _NUM_DMA 5
#elif defined(_DCH3CON_CHPRI_POSITION)
    #define _NUM_DMA 4
#elif defined(_DCH2CON_CHPRI_POSITION)
    #define _NUM_DMA 3
#elif defined(_DCH1CON_CHPRI_POSITION)
    #define _NUM_DMA 2
#elif defined(_DCH0CON_CHPRI_POSITION)
    #define _NUM_DMA 1
#else
    #define _NUM_DMA 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "p32_defs.h"

extern int dma_acquire();
extern int dma_acquire_pref(int pref);
extern int dma_release(int chan);
extern int dma_busy(int chan);
extern void *dma_memcpy(int chan, void *dest, const void *src, size_t n);
extern int dma_transfer(int chan, void *dest, const void *src, int dsize, int ssize, int csize, int irq);
extern int dma_block_finished(int chan);
extern p32_dch *dma_registers(int chan);

#ifdef __cplusplus
}
#endif

#endif
