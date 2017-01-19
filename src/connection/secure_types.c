#include "secure_types.h"
#include <string.h>

int Secure_GenKeys(Secure_PubKey pub, Secure_PrivKey priv)
{
    if(crypto_box_keypair(pub, priv))
    {
        Error_Print("Unable to generate keypair.\n");
        return -1;
    }

    return 0;
}

int Secure_GenSharedKey(Secure_SharedKey sharedkey, Secure_PubKey peer,
    Secure_PrivKey priv)
{
    if(crypto_box_beforenm(sharedkey, peer, priv))
    {
        Error_Print("Unable to generate a shared key.\n");
        return -1;
    }

    return 0;
}

int Secure_GenNonce(Secure_Nonce nonce)
{
    randombytes_buf(nonce, SECURE_NONCE_SIZE);
}

Secure_Session *Secure_NewSession()
{
    Secure_Session *out = (Secure_Session*)malloc(sizeof(Secure_Session));
    memset(out, 0, sizeof(Secure_Session));
    return out;
}


Secure_PubKey Secure_NewPubKey()
{
    return (Secure_PubKey)malloc(SECURE_PUBKEY_SIZE);
}

Secure_PrivKey Secure_NewPrivKey()
{
    return (Secure_PrivKey)malloc(SECURE_PRIVKEY_SIZE);
}
Secure_SharedKey Secure_NewSharedKey()
{
    return (Secure_SharedKey)malloc(SECURE_SHAREDKEY_SIZE);
}

Secure_Nonce Secure_NewNonce()
{
    return (Secure_Nonce)malloc(SECURE_NONCE_SIZE);
}

Secure_PlainText Secure_NewPlainText(size_t len)
{
    return (Secure_PlainText)malloc(len);
}

Secure_CipherText Secure_NewCipherText(size_t plaintextsize)
{
    return (uint8_t*)malloc(plaintextsize + SECURE_CIPHER_OVERHEAD);
}

void Secure_FreeSession(Secure_Session *session)
{
    free(session->key);

    free(session->nonce);

    free(session);
}

void Secure_FreePubKey(Secure_PubKey pub)
{
    free(pub);
}

void Secure_FreePrivKey(Secure_PrivKey priv)
{
    free(priv);
}

void Secure_FreeKeyPair(Secure_PubKey pub, Secure_PrivKey priv)
{
    Secure_FreePubKey(pub);
    Secure_FreePrivKey(priv);
}

void Secure_FreeNonce(Secure_Nonce nonce)
{
    free(nonce);
}

void Secure_FreePlainText(Secure_PlainText plaintext)
{
    free(plaintext);
}

void Secure_FreeCipherText(Secure_CipherText ciphertext)
{
    free(ciphertext);
}
