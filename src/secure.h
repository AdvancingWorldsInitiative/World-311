#ifndef _SECURE_H
#define _SECURE_H

#include <stdint.h>
#include "network.h"

#define SEC_HASH_SIZE
typedef uint8_t* Sec_Hash;

typedef uint8_t* Sec_SymKey;
typedef uint8_t* Sec_PubKey;
typedef uint8_t* Sec_PrivKey;

typedef struct
{
    Net_Sock *sock;
    Sec_SymKey *key;
} Sec_Stream;

int Sec_SendWithSym(Net_Sock *sock, Net_Addr *addr, Net_Port port, Sec_SymKey *key, void *data, long len);

int Sec_SendWithPub(Net_Sock *sock, Net_Addr *addr, Net_Port port, Sec_PubKey *key, void *data, long len);

int Sec_RecvWithSym(Net_Sock *sock, void *data, long len);

int Sec_InitStream(Net_Sock *sock, Net_Addr *addr, Net_Port port, Sec_PubKey *key);

#endif
