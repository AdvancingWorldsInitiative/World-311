#ifndef _SECURE_H
#define _SECURE_H

#include <stdint.h>
#include "network.h"

#define SEC_HASH_SIZE 32
typedef uint8_t* Sec_Hash;

#define SEC_SYMKEY_SIZE 32
typedef uint8_t* Sec_SymKey;

#define SEC_PUBKEY_SIZE 32
typedef uint8_t* Sec_PubKey;

#define SEC_PRIVKEY_SIZE 32
typedef uint8_t* Sec_PrivKey;

int Sec_EncryptWithPub(Sec_PubKey *key, void *data, int len, void *out);
int Sec_DecryptWithPriv(Sec_PrivKey *key, void *data, int len, void *out);

#endif
