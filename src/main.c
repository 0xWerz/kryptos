#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xor.h"

int main(int argc, char *argv[]) {
  if (argc != 5) {
    printf("Usage: %s <action> <method> <message> <key>\n", argv[0]);
    return 1;
  }
  const char *action = argv[1];
  const char *method = argv[2];
  const char *message = argv[3];
  const char *key = argv[4];

  char *result = NULL;
  if (strcmp(method, "xor") == 0) {
    if (strcmp(action, "encrypt") == 0) {
      result = xor_encrypt(message, key);
    } else if (strcmp(action, "decrypt") == 0) {
      result = xor_decrypt(message, key);
    } else {
      printf("Unsupported action: %s\n", action);
      return 1;
    }
  } else {
    printf("Unsupported method: %s\n", method);
    return 1;
  }

  if (result == NULL) {
    printf("Error: Operation failed\n");
    return 1;
  }

  printf("%s\n", result);
  free(result);

  return 0;
}
