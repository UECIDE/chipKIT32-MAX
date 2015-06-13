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

#include <xc.h>
#include <dma.h>
#include <p32_defs.h>

static uint32_t dmaChannelBitmap;

#define KVA_2_PA(v)             (((uint32_t) (v)) & 0x1fffffff)


/*! Acquire a DMA channel.  
 *
 *  This function will check through the dmaChannelBitmap for an
 *  available DMA channel and return the number of that channel,
 *  setting the bit in the bitmap to show the channel is now
 *  allocated.  If no channels are available it returns -1.
 */
int dma_acquire() {
    return dma_acquire_pref(0);
}

int dma_acquire_pref(int pref) {
#if (_NUM_DMA > 0) 
    // First check our preferred channel
    if (pref < _NUM_DMA) {
        if ((dmaChannelBitmap & (1 << pref)) == 0) {
            if (dmaChannelBitmap == 0) { // First DMA assignment - turn on the DMA system
                DMACON = DMA_CON_ON;
            }
            dmaChannelBitmap |= (1 << pref);
            // Set the channel to the reset defaults (or what the datasheet says they are anyway)
            p32_dch *dch = (p32_dch *)&DCH0CON;
            dch[pref].dchcon.reg = 0x00000000;
            dch[pref].dchecon.reg = 0x00FFFF00;
            dch[pref].dchint.reg = 0x00000000;
            dch[pref].dchssa.reg = 0x00000000;
            dch[pref].dchdsa.reg = 0x00000000;
            dch[pref].dchssiz.reg = 0x00000000;
            dch[pref].dchdsiz.reg = 0x00000000;
            dch[pref].dchcsiz.reg = 0x00000000;
            dch[pref].dchdat.reg = 0x00000000;
            return pref;
        }
    }

    int i;
    for (i = 0; i < _NUM_DMA; i++) {
        if ((dmaChannelBitmap & (1 << i)) == 0) {
            if (dmaChannelBitmap == 0) { // First DMA assignment - turn on the DMA system
                DMACON = DMA_CON_ON;
            }
            dmaChannelBitmap |= (1 << i);
            // Set the channel to the reset defaults (or what the datasheet says they are anyway)
            p32_dch *dch = (p32_dch *)&DCH0CON;
            dch[i].dchcon.reg = 0x00000000;
            dch[i].dchecon.reg = 0x00FFFF00;
            dch[i].dchint.reg = 0x00000000;
            dch[i].dchssa.reg = 0x00000000;
            dch[i].dchdsa.reg = 0x00000000;
            dch[i].dchssiz.reg = 0x00000000;
            dch[i].dchdsiz.reg = 0x00000000;
            dch[i].dchcsiz.reg = 0x00000000;
            dch[i].dchdat.reg = 0x00000000;
            return i;
        }
    }
#endif
    return -1;
}

/*! Release an acquired DMA channel
 *
 *  Pass the ID of a channel (provided by dma_acquire) and the
 *  channel will be released back into the pool.  If an invalid
 *  channel was specified it returns -1, otherwise it returns 0.
 */
int dma_release(int chan) {
    if ((chan < 0) || (chan >= _NUM_DMA)) {
        return -1;
    }
#if (_NUM_DMA > 0)
    dmaChannelBitmap &= ~(1 << chan);
    if (dmaChannelBitmap == 0) { // Last channel disabled - turn off DMA
        DMACON = 0;
    }
    // Set the channel to the reset defaults (or what the datasheet says they are anyway)
    p32_dch *dch = (p32_dch *)&DCH0CON;
    dch[chan].dchcon.reg = 0x00000000;
    dch[chan].dchecon.reg = 0x00FFFF00;
    dch[chan].dchint.reg = 0x00000000;
    dch[chan].dchssa.reg = 0x00000000;
    dch[chan].dchdsa.reg = 0x00000000;
    dch[chan].dchssiz.reg = 0x00000000;
    dch[chan].dchdsiz.reg = 0x00000000;
    dch[chan].dchcsiz.reg = 0x00000000;
    dch[chan].dchdat.reg = 0x00000000;
#endif
    return 0;
}

/*! Test to see if a DMA channel is busy
 *
 *  Returns 1 if busy, 0 if not busy or -1 if an invalid channel
 *  was specified (or there is no DMA on this chip).
 */
int dma_busy(int chan) {
#if (_NUM_DMA > 0)
    if ((chan < 0) || (chan >= _NUM_DMA)) {
        return -1;
    }
    p32_dch *dch = (p32_dch *)&DCH0CON;
    if ((dch[chan].dchcon.reg & DCH_CON_CHBUSY) == 0) {
        return 0;
    }
    return 1;
#else
    return -1;
#endif
}

/*! DMA Memory Copy
 *
 *  This is a DMA enabled version of memcpy(). It takes the
 *  same parameters as memcpy() with the addition of a DMA channel
 *  number to perform the copy with.  Returns either the pointer to
 *  dest, or NULL if the copy could not be performed.  If there is
 *  no DMA a normal memcpy() will be performed.
 *
 *  If the channel is busy it blocks waiting for the current operation
 *  to complete.
 */
void *dma_memcpy(int chan, void *dest, const void *src, size_t n) {
#if (_NUM_DMA > 0)
    if ((chan < 0) || (chan >= _NUM_DMA)) {
        return NULL;
    }
    p32_dch *dch = (p32_dch *)&DCH0CON;
    while ((dch[chan].dchcon.reg & DCH_CON_CHBUSY) != 0) {
        asm volatile("nop");
    }
    dch[chan].dchcon.reg = 0;
    dch[chan].dchecon.reg = 0x00FFFF00;
    dch[chan].dchint.reg = 0;
    dch[chan].dchssa.reg = KVA_2_PA(src);
    dch[chan].dchdsa.reg = KVA_2_PA(dest);
    dch[chan].dchssiz.reg = n;
    dch[chan].dchdsiz.reg = n;
    dch[chan].dchcsiz.reg = n;
    dch[chan].dchcon.set = DCH_CON_CHEN;
    dch[chan].dchecon.set = DCH_ECON_CFORCE;
    return dest;
#else
    return memcpy(dest, src, n);
#endif
}

/*! DMA transfer to or from a peripheral
 *
 *  Read a block of data from a peripheral using an interrupt to trigger each
 *  transfer.  Good for things like ADC sampling where a timer sets the sampling rate.
 *
 *  Pass it the channel, destination buffer and source location, destination and source sizes,
 *  transfer cell size and IRQ to trigger the transfers.
 */
int dma_transfer(int chan, void *dest, const void *src, int dsize, int ssize, int csize, int irq) {
#if (_NUM_DMA > 0)
    if ((chan < 0) || (chan >= _NUM_DMA)) {
        return 0;
    }
    p32_dch *dch = (p32_dch *)&DCH0CON;
    while ((dch[chan].dchcon.reg & DCH_CON_CHBUSY) != 0) {
        asm volatile("nop");
    }

    dch[chan].dchcon.reg = 0;
    dch[chan].dchecon.reg = 0x00FF0000;
    dch[chan].dchecon.set = DCH_ECON_CHSIRQ(irq);
    dch[chan].dchecon.set = DCH_ECON_SIRQEN;
    dch[chan].dchint.reg = 0;
    dch[chan].dchssa.reg = KVA_2_PA(src);
    dch[chan].dchdsa.reg = KVA_2_PA(dest);
    dch[chan].dchssiz.reg = ssize;
    dch[chan].dchdsiz.reg = dsize;
    dch[chan].dchcsiz.reg = csize;
    dch[chan].dchcon.set = DCH_CON_CHEN;
#else
    return -1;
#endif
}

/*! Block transfer complete
 *
 *  Single-shot function to find if a transfer has finished. Examines
 *  the interrupt flag status.
 */
int dma_block_finished(int chan) {
#if (_NUM_DMA > 0)
    if ((chan < 0) || (chan >= _NUM_DMA)) {
        return -1;
    }
    p32_dch *dch = (p32_dch *)&DCH0CON;
    if ((dch[chan].dchint.reg & DCH_INT_CHBCIF) == 0) {
        return 0;
    }
    return 1;
#else
    return -1;
#endif
}

p32_dch *dma_registers(int chan) {
#if (_NUM_DMA > 0)
    if ((chan < 0) || (chan >= _NUM_DMA)) {
        return NULL;
    }
    p32_dch *dch = (p32_dch *)&DCH0CON;
    return &dch[chan];
#else
    return NULL;
#endif
}
