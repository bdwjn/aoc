#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <immintrin.h>

void scenic_dist_32xN(uint8_t *_data, uint8_t *count, int64_t stride, uint64_t n) {
	const __m256i ones = _mm256_set_epi8(
		0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
		0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01 );
	const __m256i offs = _mm256_set_epi8(
		0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,
		0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8,0xC8 );
	__m256i result, gt_mask, eq_mask, data;
	
	__m256i d[9] = {
		_mm256_setzero_si256(), _mm256_setzero_si256(), _mm256_setzero_si256(),
		_mm256_setzero_si256(), _mm256_setzero_si256(), _mm256_setzero_si256(),
		_mm256_setzero_si256(), _mm256_setzero_si256(), _mm256_setzero_si256()
	};

	do {
		data    = _mm256_loadu_si256((__m256i*)_data);
		data    = _mm256_add_epi8(data, offs);
		result  = _mm256_setzero_si256();
		gt_mask = _mm256_setzero_si256();

		__builtin_prefetch(_data + 4 * stride);

		for (int i=0; i<9; i++) {
			eq_mask = _mm256_cmpeq_epi8(data, ones);
			gt_mask = _mm256_or_si256(gt_mask, eq_mask);
			eq_mask = _mm256_and_si256(eq_mask, d[i]);
			result  = _mm256_or_si256(result, eq_mask);
			d[i]    = _mm256_andnot_si256(gt_mask, d[i]);
			d[i]    = _mm256_add_epi8(d[i], ones);
			data    = _mm256_add_epi8(data, ones);
		}

		eq_mask = _mm256_andnot_si256(gt_mask, ones);
		result  = _mm256_or_si256(result, eq_mask);

		_mm256_storeu_si256((__m256i*)count, result);

		_data += stride;
		count += stride;
	} while (--n);
}


int main(void) {

	uint8_t *filedata = malloc(1<<30);
	int size = fread(filedata, 1, 1<<30, stdin);

	int W = 0; do W++; while (filedata[W] - '\n');
	int H = size / (W+1);

	/* Create 128-byte aligned matrix */
	int align      = 128;
	int stride     = (W + 64 + align - 1) & ~(align - 1);
	int tstride    = (H + 64 + align - 1) & ~(align - 1);
	uint8_t *data  = aligned_alloc(align, stride * H);
	uint8_t *count = aligned_alloc(align, 2 * stride * H);
	uint8_t *tdata = aligned_alloc(align, tstride * W);
	uint8_t *tcount= aligned_alloc(align, 2 * tstride * W);

	for (int y=0; y<H; y++) {
		memcpy(data + stride * y, filedata + (W+1) * y, W);
	}

	/* north view distance */
	for (int col=0; col<W; col += 32) {
		scenic_dist_32xN(data + col, count + col, stride, H);
	}
	for (int col=0; col<W; col += 32) {
		scenic_dist_32xN(data + col + stride * (H-1), count + col + stride * (2*H-1), -stride, H);
	}

	/* matrix transposition */
	for (int y=0; y<H-64; y+=64) {
		for (int x=0; x<W; x+=64) {
			for (int i=0; i<64; i++) {
				for (int j=0; j<64; j++) {
					x+=i; y+=j;
					tdata[tstride * x + y] = data[x + stride * y];
					x-=i; y-=j;
				}
			}
		}
	}

	/* west/east view distance */
	for (int col=0; col<H; col += 32) {
		scenic_dist_32xN(tdata + col, tcount + col, tstride, W);
	}
	for (int col=0; col<H; col += 32) {
		scenic_dist_32xN(tdata + col + tstride * (W-1), tcount + col + tstride * (2*W-1), -tstride, W);
	}

	int best = 0;

	/* viewing distances are now in 4 separate arrays, multiply them together to find the best tree */	
	for (int y=0; y<H-64; y+=64) {
		for (int x=0; x<W; x+=64) {
			for (int i=0; i<64; i++) {
				__builtin_prefetch(count + x + 256 + stride * y);
				__builtin_prefetch(count + x + 256 + stride * (H+y+i));
				__builtin_prefetch(tcount + y + tstride * (x+i+32));
				__builtin_prefetch(tcount + y + tstride * (x+i+W+32));

				for (int j=0; j<64; j++) {
					int north = count[x + i + stride * (y+j)];
					int south = count[x + i + stride * (y+j+H)];
					int west  = tcount[y + j + tstride * (x+i)];
					int east  = tcount[y + j + tstride * (x+i+W)];
					int score = north * south * west * east;

					if (score > best) best = score;
				}
			}
		}
	}

	printf("Part 2 answer: %d\n", best);

#	ifdef  __SANITIZE_ADDRESS__ 
	free(filedata); free(data); free(count); free(tdata); free(tcount);
#	endif
}
