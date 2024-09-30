#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <stdlib.h>
#include <string.h>

char *encode_base64(const unsigned char *input, int length) {
  BIO *bio, *b64;
  BUF_MEM *bufferPtr;

  b64 = BIO_new(BIO_f_base64());
  bio = BIO_new(BIO_s_mem());
  bio = BIO_push(b64, bio);

  BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
  BIO_write(bio, input, length);
  BIO_flush(bio);
  BIO_get_mem_ptr(bio, &bufferPtr);

  char *base64_encoded = malloc(bufferPtr->length + 1);
  if (base64_encoded == NULL) {
    BIO_free_all(bio);
    return NULL;
  }

  memcpy(base64_encoded, bufferPtr->data, bufferPtr->length);
  base64_encoded[bufferPtr->length] = '\0';

  BIO_free_all(bio);

  return base64_encoded;
}

unsigned char *decode_base64(const char *input, int *output_length) {
  BIO *bio, *b64;
  int decoded_size = strlen(input);
  unsigned char *buffer = malloc(decoded_size);

  if (buffer == NULL) {
    return NULL;
  }

  bio = BIO_new_mem_buf(input, -1);
  b64 = BIO_new(BIO_f_base64());
  bio = BIO_push(b64, bio);

  BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
  *output_length = BIO_read(bio, buffer, decoded_size);

  BIO_free_all(bio);

  if (*output_length <= 0) {
    free(buffer);
    return NULL;
  }

  return buffer;
}
