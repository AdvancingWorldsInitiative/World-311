#ifndef _SECURE_H
#define _SECURE_H

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

extern int Secure_GenKeys(Secure_PubKey *pub, Secure_PrivKey *priv);

extern void Secure_FreePubKey(Secure_PubKey pub);
extern void Secure_FreePrivKey(Secure_PrivKey priv);
extern void Secure_FreeKeyPair(Secure_PubKey pub, Secure_PrivKey priv);

extern Secure_Session *Secure_Connect(Secure_PubKey peer, Secure_PubKey pub, Secure_PrivKey priv,
    Net_Addr addr, Net_Port port);

extern Secure_Session *Secure_Accept(Net_Sock sock, Secure_PubKey pub, Secure_PrivKey priv);

extern int Secure_Send(Secure_Session *session, const uint8_t *data, size_t len);

extern size_t Secure_Recv(Secure_Session *session, uint8_t **data);

extern void Secure_Close(Secure_Session *session);

#endif
