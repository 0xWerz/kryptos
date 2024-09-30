#ifndef UTILS_H
#define UTILS_H

char *encode_base64(const unsigned char *input, int length);
unsigned char *decode_base64(const char *input, int *output_length);

#endif  // UTILS_H
