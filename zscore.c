#include "zscore.h"

/* ------------------------------------------------------------------------- *
 * Interleave the bits of two 8bits unsigned integers
 * Source: https://graphics.stanford.edu/~seander/bithacks.html#Interleave64bitOps
 * ------------------------------------------------------------------------- */

static uint64_t interleave8(uint8_t m, uint8_t n);

static uint64_t interleave8(uint8_t m, uint8_t n) {
	return (
		((m * 0x0101010101010101ULL & 0x8040201008040201ULL) * 0x0102040810204081ULL >> 49) & 0x5555
	) | (
		((n * 0x0101010101010101ULL & 0x8040201008040201ULL) * 0x0102040810204081ULL >> 48) & 0xAAAA
	);
}

/* ------------------------------------------------------------------------- *
 * Interleave the bits of two 64bits unsigned integers
 * ------------------------------------------------------------------------- */
static uint64_t interleave32(uint32_t m, uint32_t n) {
	uint64_t out = 0;
	for (uint8_t byte = 0; byte < 4; ++byte) {
		out |= interleave8((m >> (byte * 8)) & 0xFF, (n >> (byte * 8)) & 0xFF) << (byte * 16);
	}
	return out;
}

uint64_t zEncode(double latitude, double longitutde)
{
    // Quantification
    latitude += 90.;// Positive
    longitutde += 180.;// Positive
    uint32_t x = (uint32_t) (latitude / 180. * UINT32_MAX);
    uint32_t y = (uint32_t) (longitutde / 360. * UINT32_MAX);
    return interleave32(x, y);
}


