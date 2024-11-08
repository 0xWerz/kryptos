
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MD5_DIGEST_LENGTH 16
#define BLOCK_SIZE 64 // 512 bits

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

static const uint32_t r[] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

static const uint32_t k[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

static void md5_transform(uint32_t state[4], const uint8_t block[64]) {
    uint32_t a = state[0];
    uint32_t b = state[1]; 
    uint32_t c = state[2];
    uint32_t d = state[3];
    uint32_t m[16];
    uint32_t i, j;

    for(i = 0, j = 0; i < 16; ++i, j += 4)
        m[i] = (uint32_t)(block[j] | (block[j+1] << 8) | (block[j+2] << 16) | (block[j+3] << 24));

    for(i = 0; i < 64; i++) {
        uint32_t f, g;

        if (i < 16) {
            f = (b & c) | ((~b) & d);
            g = i;
        } else if (i < 32) {
            f = (d & b) | ((~d) & c);
            g = (5*i + 1) % 16;
        } else if (i < 48) {
            f = b ^ c ^ d;
            g = (3*i + 5) % 16;
        } else {
            f = c ^ (b | (~d));
            g = (7*i) % 16;
        }

        uint32_t temp = d;
        d = c;
        c = b;
        b = b + LEFTROTATE((a + f + k[i] + m[g]), r[i]);
        a = temp;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}

char *md5_hash(const char *message) {
    if (message == NULL) {
        return NULL;
    }

    uint32_t state[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};
    uint64_t bitlen = strlen(message) * 8;
    size_t msglen = strlen(message);
    size_t new_len = ((((msglen + 8) / 64) + 1) * 64) - 8;
    
    uint8_t *msg = calloc(new_len + 64, 1);
    if (!msg) return NULL;
    
    memcpy(msg, message, msglen);
    msg[msglen] = 0x80; 
    
    uint64_t *bits = (uint64_t *)(msg + new_len);
    *bits = bitlen;

    for(size_t offset = 0; offset < new_len; offset += 64) {
        md5_transform(state, msg + offset);
    }

    free(msg);

    unsigned char digest[MD5_DIGEST_LENGTH];
    for(int i = 0; i < 4; i++) {
        digest[i*4] = state[i] & 0xFF;
        digest[i*4 + 1] = (state[i] >> 8) & 0xFF;
        digest[i*4 + 2] = (state[i] >> 16) & 0xFF;
        digest[i*4 + 3] = (state[i] >> 24) & 0xFF;
    }

    char *hash = malloc(33); 
    if (!hash) return NULL;

    for(int i = 0; i < 16; i++) {
        sprintf(hash + (i * 2), "%02x", digest[i]);
    }
    hash[32] = '\0';

    return hash;
}
