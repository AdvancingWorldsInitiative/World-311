#ifndef _SECURE_TYPES_H
#define _SECURE_TYPES_H

#include <sodium.h>
#include <stdint.h>
#include "network.h"

#define SECURE_HASH_SIZE 32
typedef uint8_t* Secure_Hash;

#define SECURE_SYMKEY_SIZE 32
typedef uint8_t* Secure_SymKey;

#define SECURE_PUBKEY_SIZE crypto_box_PUBLICKEYBYTES
typedef uint8_t* Secure_PubKey;

#define SECURE_PRIVKEY_SIZE crypto_box_SECRETKEYBYTES
typedef uint8_t* Secure_PrivKey;

#define SECURE_MAX_MSGSIZE 1048576 /* 1 Meg */
typedef union
{
    uint32_t value;
    uint8_t bytes[4];
} Secure_MsgSize;

typedef struct
{
    Net_Sock sock;
    uint8_t* key;
    uint8_t* nonce;
} Secure_Session;

extern void Secure_FreePubKey(Secure_PubKey pub);
extern void Secure_FreePrivKey(Secure_PrivKey priv);
extern void Secure_FreeKeyPair(Secure_PubKey pub, Secure_PrivKey priv);

#endif
