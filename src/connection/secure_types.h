#ifndef _SECURE_TYPES_H
#define _SECURE_TYPES_H

#include <sodium.h>
#include <stdint.h>
#include "network.h"

#define SEC_HASH_SIZE 32
typedef uint8_t* Secure_Hash;

#define SEC_SYMKEY_SIZE 32
typedef uint8_t* Secure_SymKey;

#define SEC_PUBKEY_SIZE crypto_box_PUBLICKEYBYTES
typedef uint8_t* Secure_PubKey;

#define SEC_PRIVKEY_SIZE crypto_box_SECRETKEYBYTES
typedef uint8_t* Secure_PrivKey;

#define SEC_MAX_SIZE 1048576 /* 1 Meg */
typedef union
{
    uint32_t value;
    uint8_t bytes[4];
} Secure_Size;

typedef struct
{
    Net_Sock sock;
    uint8_t* key;
} Secure_Session;

extern void Secure_FreePubKey(Secure_PubKey pub);
extern void Secure_FreePrivKey(Secure_PrivKey priv);
extern void Secure_FreeKeyPair(Secure_PubKey pub, Secure_PrivKey priv);

#endif
