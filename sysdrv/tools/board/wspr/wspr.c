/*
 * Copyright 2018 Alexander Fasching
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "debug.h"


const uint8_t sync_vector[] = {
    0x03, 0x71, 0xa4, 0x07, 0xa4, 0x40, 0xb3, 0x58, 0x58, 0x95, 0x34, 0x56,
    0x04, 0xc9, 0xcd, 0xe2, 0xa0, 0x0c, 0x58, 0x63, 0x00
};


/**
 * Calculate the parity bits from the value of the shift register.
 * The parity bits are returned in bit0 and bit1 of the returned value.
 */
static uint8_t get_parity(uint32_t * shiftreg0, uint32_t * shiftreg1) {
    // use Galois-type shift register calculation
    // for this to work, polynomials have to be bit-reversed
    const uint32_t rev_poly0 = 0x8ACA0B4E;  // invert lowest bit because it
    const uint32_t rev_poly1 = 0xE23C8626;  // is set by having Data put there

    uint8_t par0;
    uint8_t par1;

    // if new data bit is 1, apply polynomial to SR
    // new data bit is the same in both SRs, so we only need to test LSB once
    if(*shiftreg0 & 1){
        *shiftreg0 ^= rev_poly0;
        *shiftreg1 ^= rev_poly1;
    }
    // parity is at MSB
    par0 = *shiftreg0 & 0x80000000 ? 1 : 0;
    // implement shift in constant
    par1 = *shiftreg1 & 0x80000000 ? 2 : 0;
    return par1 | par0;
}


static void set_bit(uint8_t *bitvector, int index, uint8_t value) {
    if (value)
        bitvector[index >> 3] |= (1 << (index & 0x07));
    else
        bitvector[index >> 3] &= ~(1 << (index & 0x07));
}

static uint8_t get_bit(const uint8_t *bitvector, int index) {
    return !!(bitvector[index >> 3] & (1 << (index & 0x07)));
}

static uint8_t bit_reverse(uint8_t byte) {
    uint8_t result = 0;
    for (int i = 0; i < 8; i++) {
        if (byte & (1 << i))
            result |= (1 << (7 - i));
    }
    return result;
}


//#ifdef DEBUG
static void print_vector(const char *name, const uint8_t *bitvec) {
    printf("%s:", name);
    for (int i = 0; i < 162; i++) {
        if (i % 30 == 0)
            printf("\n      ");
        printf("%d", get_bit(bitvec, i));

        if (i % 30 != 29)
            printf(" ");
    }
    printf("\n");
}
//#endif


/**
 * @brief Encode a WSPR message
 * @param call Callsign as string in the format AABCCC
 * @param loc 4 character QTH locator as string
 * @param power Power in dBm
 */
int wspr_encode(uint8_t *result, const char *call, const char *loc, int power) {
    uint32_t call_enc = 0;

    /* Map the callsign to integers between 0 and 36. */
    uint8_t call_mapped[6];
    for (int i = 0; i < 6; i++) {
        if ('0' <= call[i] && call[i] <= '9')
            call_mapped[i] = call[i] - '0';
        else if ('A' <= call[i] && call[i] <= 'Z')
            call_mapped[i] = 10 + call[i] - 'A';
        else if (call[i] == ' ')
            call_mapped[i] = 36;
        else
            return 1;
    }

    /* Encode the mapped callsign. */
    call_enc += call_mapped[0];
    call_enc = 36 * call_enc + call_mapped[1];
    call_enc = 10 * call_enc + call_mapped[2];
    call_enc = 27 * call_enc + (call_mapped[3] - 10);
    call_enc = 27 * call_enc + (call_mapped[4] - 10);
    call_enc = 27 * call_enc + (call_mapped[5] - 10);

    /* Encode the grid locator. */
    uint32_t loc_pwr_enc = (179 - 10 * (loc[0] - 'A') - (loc[2] - '0')) * 180 +
                                  10 * (loc[1] - 'A') + (loc[3] - '0');
    /* Encode the transmit power. */
    loc_pwr_enc = 128 * loc_pwr_enc + power + 64;

    /* Forward Error Correction */
    uint32_t shiftreg0 = 0;
    uint32_t shiftreg1 = 0;
    uint8_t packed[21] = {0};

    for (int i = 0; i < 50; i++) {
        uint8_t bit;
        if (i < 28) {
            uint8_t index = 27 - i;
            bit = !!(call_enc & (1UL << index));
        } else {
            uint8_t index = 21 - (i - 28);
            bit = !!(loc_pwr_enc & (1UL << index));
        }

        shiftreg0 = (shiftreg0 << 1) | bit;
        shiftreg1 = (shiftreg1 << 1) | bit;

        uint32_t parity = get_parity(&shiftreg0, &shiftreg1);
        uint8_t par0 = !!(parity & (1UL << 0));
        uint8_t par1 = !!(parity & (1UL << 1));

        set_bit(packed, 2 * i, par0);
        set_bit(packed, 2 * i + 1, par1);
    }
    for (int i = 50; i < 81; i++) {
        shiftreg0 <<= 1;
        shiftreg1 <<= 1;

        uint32_t parity = get_parity(&shiftreg0, &shiftreg1);
        uint8_t par0 = !!(parity & (1 << 0));
        uint8_t par1 = !!(parity & (1 << 1));

        set_bit(packed, 2 * i, par0);
        set_bit(packed, 2 * i + 1, par1);
    }

    /* Shuffle the result. */
    memset(result, 0, 21);
    uint8_t p = 0;
    for (int i = 0; i < 256; i++) {
        uint8_t j = bit_reverse(i);
        if (j < 162) {
            set_bit(result, j, get_bit(packed, p));
            p++;
        }
        if (p >= 162)
            break;
    }

//#ifdef DEBUG
    //print_vector("Data symbols", result);
    //printf("\n");
    //print_vector("Sync symbols", sync_vector);
//#endif
    return 0;
}


uint8_t wspr_get_tone(const uint8_t *enc, uint8_t index) {
    return get_bit(sync_vector, index) + 2 * get_bit(enc, index);
}
