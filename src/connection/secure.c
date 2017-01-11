#include "secure.h"

int Sec_GenKeys(Sec_PubKey *pub, Sec_PrivKey *priv)
{
    *pub = (uint8_t*)malloc(SEC_PUBKEY_SIZE);
    *priv = (uint8_t*)malloc(SEC_PRIVKEY_SIZE);

    return crypto_box_keypair(*pub, *priv);
}

void Sec_FreePubKey(Sec_PubKey pub)
{
    free(pub);
}

void Sec_FreePrivKey(Sec_PrivKey priv)
{
    free(priv);
}

void Sec_FreeKeyPair(Sec_PubKey pub, Sec_PrivKey priv)
{
    Sec_FreePubKey(pub);
    Sec_FreePrivKey(priv);
}

Sec_Session *Sec_Connect(Sec_PubKey peer, Sec_PubKey pub, Sec_PrivKey priv,
    Net_Addr addr, Net_Port port)
{
    Sec_Session *out = (Sec_Session*)malloc(sizeof(Sec_Session));

    if(out == NULL)
    {
        Error_Print("Unable to allocate session.\n");
        return out;
    }

    out->sock = Net_NewSock(NET_TCP);

    if(out->sock == -1)
    {
        Error_Print("Unable to open session socket.\n");
        free(out);
        return NULL;
    }

    if(Net_Connect(out->sock, addr, port))
    {
        free(out);
        return NULL;
    }

    out->key = (uint8_t*)malloc(crypto_box_BEFORENMBYTES);

    if(out->key == NULL)
    {
        Net_Close(out->sock);
        free(out);
        Error_Print("Unable to allocate shared session key.\n");
        return NULL;
    }

    if(crypto_box_beforenm(out->key, peer, priv))
    {
        Net_Close(out->sock);
        sodium_free(out->key);
        free(out);
        Error_Print("Unable to create shared session key.\n");
        return NULL;
    }

    if(Net_Send(out->sock, pub, SEC_PUBKEY_SIZE) != SEC_PUBKEY_SIZE)
    {
        Sec_Close(out);
        return NULL;
    }

    return out;
}

Sec_Session *Sec_Accept(Net_Sock sock, Sec_PubKey pub, Sec_PrivKey priv)
{
    Sec_Session *out = (Sec_Session*)malloc(sizeof(Sec_Session));

    Sec_PubKey peer = (Sec_PubKey)malloc(SEC_PUBKEY_SIZE);

    out->sock = Net_Accept(sock);

    if(sock == -1)
    {
        free(out);
        Error_Print("Unable to accept connection.\n");
        return NULL;
    }

    if(Net_Recv(out->sock, peer, SEC_PUBKEY_SIZE) != SEC_PUBKEY_SIZE)
    {
        Sec_Close(out);
        Error_Print("Received key is the wrong size.\n");
        return NULL;
    }

    out->key = (uint8_t*)malloc(crypto_box_BEFORENMBYTES);

    if(crypto_box_beforenm(out->key, peer, priv))
    {
        Sec_Close(out);
        Error_Print("Unable to create shared session key.\n");
        return NULL;
    }

    return out;
}

int Sec_Send(Sec_Session *session, const uint8_t *data, size_t len)
{
    int sent;

    Sec_Size size;
    uint8_t *cyphertext = (uint8_t*)malloc(len + crypto_box_MACBYTES);
    uint8_t *cyphersize = (uint8_t*)malloc(sizeof(Sec_Size) + crypto_box_MACBYTES);
    uint8_t *nonce = (uint8_t*)malloc(crypto_box_NONCEBYTES);

    randombytes_buf(nonce, crypto_box_NONCEBYTES);

    if(len > SEC_MAX_SIZE)
    {
        free(cyphertext);
        free(cyphersize);
        free(nonce);
        return -1;
    }

    size.value = htonl(len);

    if(crypto_box_afternm(cyphersize, size.bytes, sizeof(Sec_Size),
        nonce, session->key))
    {
        free(cyphertext);
        free(cyphersize);
        free(nonce);
        Error_Print("Unable to encrypt message size.\n");
        return -1;
    }

    nonce[0] ^= 1;

    if(crypto_box_afternm(cyphertext, data, len, nonce, session->key))
    {
        free(cyphertext);
        free(cyphersize);
        free(nonce);
        Error_Print("Unable to encrypt message.\n");
        return -1;
    }

    nonce[0] ^= 1;

    if(Net_Send(session->sock, nonce, crypto_box_NONCEBYTES)
        != crypto_box_NONCEBYTES)
    {
        free(cyphertext);
        free(cyphersize);
        free(nonce);
        return -1;
    }

    if(Net_Send(session->sock, cyphersize, sizeof(Sec_Size) + crypto_box_MACBYTES)
        != sizeof(Sec_Size) + crypto_box_MACBYTES)
    {
        free(cyphertext);
        free(cyphersize);
        free(nonce);
        return -1;
    }

    sent = Net_Send(session->sock, cyphertext, len + crypto_box_MACBYTES);

    free(cyphertext);
    free(cyphersize);
    free(nonce);

    return sent;
}

size_t Sec_Recv(Sec_Session *session, uint8_t **data)
{
    size_t out;

    Sec_Size size;
    uint8_t *cyphertext;
    uint8_t *cyphersize = (uint8_t*)malloc(sizeof(Sec_Size) + crypto_box_MACBYTES);
    uint8_t *nonce = (uint8_t*)malloc(crypto_box_NONCEBYTES);

    if(Net_Recv(session->sock, nonce, crypto_box_NONCEBYTES)
        != crypto_box_NONCEBYTES)
    {
        free(cyphersize);
        free(nonce);
        return -1;
    }

    if(Net_Recv(session->sock, cyphersize, sizeof(Sec_Size) + crypto_box_MACBYTES)
        != sizeof(Sec_Size) + crypto_box_MACBYTES)
    {
        free(cyphersize);
        free(nonce);
        return -1;
    }

    if(crypto_box_open_afternm(size.bytes, cyphersize,
        sizeof(Sec_Size) + crypto_box_MACBYTES, nonce, session->key))
    {
        free(cyphersize);
        free(nonce);
        return -1;
    }

    free(cyphersize);

    out = ntohl(size.value);

    if(out > SEC_MAX_SIZE)
    {
        free(nonce);
        return -1;
    }

    cyphertext = (uint8_t*)malloc(out + crypto_box_MACBYTES);

    if(Net_Recv(session->sock, cyphertext, out + crypto_box_MACBYTES)
        != out + crypto_box_MACBYTES)
    {
        free(cyphertext);
        free(nonce);
        return -1;
    }

    nonce[0] ^= 1;

    *data = (uint8_t*)malloc(out);

    if(*data == NULL)
    {
        free(cyphertext);
        free(nonce);
        Error_Print("Unable to allocate incoming message buffer.\n");
        return -1;
    }

    if(crypto_box_open_afternm(*data, cyphertext,
        out + crypto_box_MACBYTES, nonce, session->key))
    {
        free(cyphertext);
        free(nonce);
        free(*data);
        *data = NULL;
        return -1;
    }

    free(cyphertext);
    free(nonce);

    return out;
}

void Sec_Close(Sec_Session *session)
{
    Net_Close(session->sock);

    free(session->key);
}
