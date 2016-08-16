#ifndef _SECURE_H
#define _SECURE_H

#include <sodium.h>
#include <stdint.h>
#include "network.h"

#define SEC_HASH_SIZE 32
typedef uint8_t* Sec_Hash;

#define SEC_SYMKEY_SIZE 32
typedef uint8_t* Sec_SymKey;

#define SEC_PUBKEY_SIZE crypto_box_PUBLICKEYBYTES
typedef uint8_t* Sec_PubKey;

#define SEC_PRIVKEY_SIZE crypto_box_SECRETKEYBYTES
typedef uint8_t* Sec_PrivKey;

#define SEC_MAX_SIZE 1048576 /* 1 Meg */
typedef union
{
    uint32_t value;
    uint8_t bytes[4];
} Sec_Size;

typedef struct
{
    Net_Sock sock;
    uint8_t* key;
} Sec_Session;

extern int Sec_GenKeys(Sec_PubKey *pub, Sec_PrivKey *priv);

extern void Sec_FreePubKey(Sec_PubKey pub);
extern void Sec_FreePrivKey(Sec_PrivKey priv);
extern void Sec_FreeKeyPair(Sec_PubKey pub, Sec_PrivKey priv);

extern Sec_Session *Sec_Connect(Sec_PubKey peer, Sec_PubKey pub, Sec_PrivKey priv,
    Net_Addr addr, Net_Port port);

extern Sec_Session *Sec_Accept(Net_Sock sock, Sec_PubKey pub, Sec_PrivKey priv);

extern int Sec_Send(Sec_Session *session, const uint8_t *data, size_t len);

extern size_t Sec_Recv(Sec_Session *session, uint8_t **data);

extern void Sec_Close(Sec_Session *session);

#endif
