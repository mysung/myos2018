/* Copyright abandoned; this code is in the public domain. */
/* Provided to GNUnet by peter@horizon.com */

/*
 * Adapted for GeekOS by David Hovemeyer
 * Code downloaded from OVM (http://www.ovmj.org)
 */

#include <geekos/crc32.h>
#include <geekos/kassert.h>

#define POLYNOMIAL (ulong_t)0xedb88320
static ulong_t crc_table[256];

/*
 * This routine writes each crc_table entry exactly once,
 * with the correct final value.  Thus, it is safe to call
 * even on a table that someone else is using concurrently.
 */
void Init_CRC32(void) {
    unsigned int i, j;
    ulong_t h = 1;
    crc_table[0] = 0;
    for(i = 128; i; i >>= 1) {
        h = (h >> 1) ^ ((h & 1) ? POLYNOMIAL : 0);
        /* h is now crc_table[i] */
        for(j = 0; j < 256; j += 2 * i)
            crc_table[i + j] = crc_table[j] ^ h;
    }
}

/*
 * This computes the standard preset and inverted CRC, as used
 * by most networking standards.  Start by passing in an initial
 * chaining value of 0, and then pass in the return value from the
 * previous crc32() call.  The final return value is the CRC.
 * Note that this is a little-endian CRC, which is best used with
 * data transmitted lsbit-first, and it should, itself, be appended
 * to data in little-endian byte and bit order to preserve the
 * property of detecting all burst errors of length 32 bits or less.
 */
ulong_t crc32(ulong_t crc, char const *buf, size_t len) {
    KASSERT(crc_table[255] != 0);
    crc ^= 0xffffffff;
    while (len--)
        crc = (crc >> 8) ^ crc_table[(crc ^ *buf++) & 0xff];
    return crc ^ 0xffffffff;
}

/* end of crc32.c */
