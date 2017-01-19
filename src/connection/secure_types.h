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

#define SECURE_SHAREDKEY_SIZE crypto_box_BEFORENMBYTES
typedef uint8_t* Secure_SharedKey;

#define SECURE_NONCE_SIZE crypto_box_NONCEBYTES
typedef uint8_t* Secure_Nonce;

#define SECURE_CIPHER_OVERHEAD crypto_box_MACBYTES
typedef uint8_t* Secure_PlainText;
typedef uint8_t* Secure_CipherText;

#define SECURE_MAX_MSGSIZE 1048576 /* 1 Meg */
typedef union
{
    uint32_t value;
    uint8_t bytes[4];
} Secure_MsgSize;

typedef Net_Sock Secure_Server;

typedef struct
{
    Net_Sock sock;
    uint8_t* key;
    Secure_Nonce nonce;
} Secure_Session;

extern int Secure_GenKeys(Secure_PubKey *pub, Secure_PrivKey *priv);
extern int Secure_GenSharedKey(Secure_SharedKey sharedkey, Secure_PubKey peer,
    Secure_PrivKey priv);
extern int Secure_GenNonce(Secure_Nonce nonce);

extern Secure_Session *Secure_NewSession();
extern Secure_PubKey Secure_NewPubKey();
extern Secure_PrivKey Secure_NewPrivKey();
extern Secure_SharedKey Secure_NewSharedKey();
extern Secure_Nonce Secure_NewNonce();
extern Secure_PlainText Secure_NewPlainText(size_t len);
extern Secure_CipherText Secure_NewCipherText(size_t plaintextsize);

extern void Secure_FreeSession(Secure_Session *session);
extern void Secure_FreePubKey(Secure_PubKey pub);
extern void Secure_FreePrivKey(Secure_PrivKey priv);
extern void Secure_FreeKeyPair(Secure_PubKey pub, Secure_PrivKey priv);
extern void Secure_FreeNonce(Secure_Nonce nonce);
extern void Secure_FreePlainText(Secure_PlainText plaintext);
extern void Secure_FreeCipherText(Secure_CipherText ciphertext);

#endif
