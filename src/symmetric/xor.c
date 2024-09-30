#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

char *xor_encrypt(const char *message, const char *key) {
  if (message == NULL || key == NULL) {
    return NULL;
  }

  size_t message_len = strlen(message);
  size_t key_len = strlen(key);

  if (key_len == 0) {
    return NULL;
  }

  unsigned char *xor_result = malloc(message_len);
  if (xor_result == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < message_len; i++) {
    xor_result[i] = message[i] ^ key[i % key_len];
  }

  char *base64_result = encode_base64(xor_result, message_len);

  free(xor_result);

  return base64_result;
}

char *xor_decrypt(const char *message, const char *key) {
  if (message == NULL || key == NULL) {
    return NULL;
  }

  size_t key_len = strlen(key);
  if (key_len == 0) {
    return NULL;
  }

  // decode base64
  int decoded_length;
  unsigned char *decoded_message = decode_base64(message, &decoded_length);

  if (decoded_message == NULL) {
    return NULL;
  }

  // xor
  char *xor_result = malloc(decoded_length + 1);
  if (xor_result == NULL) {
    free(decoded_message);
    return NULL;
  }

  for (int i = 0; i < decoded_length; i++) {
    xor_result[i] = decoded_message[i] ^ key[i % key_len];
  }
  xor_result[decoded_length] = '\0';

  free(decoded_message);

  return xor_result;
}
