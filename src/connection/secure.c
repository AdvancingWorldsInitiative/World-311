#include "secure.h"

int Secure_GenKeys(Secure_PubKey *pub, Secure_PrivKey *priv)
{
    *pub = (uint8_t*)malloc(SECURE_PUBKEY_SIZE);
    *priv = (uint8_t*)malloc(SECURE_PRIVKEY_SIZE);

    return crypto_box_keypair(*pub, *priv);
}

Secure_Session *Secure_Connect(Secure_PubKey peer, Secure_PubKey pub, Secure_PrivKey priv,
    Net_Addr addr, Net_Port port)
{
    Secure_Session *out = (Secure_Session*)malloc(sizeof(Secure_Session));

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

    if(Net_Send(out->sock, pub, SECURE_PUBKEY_SIZE) != SECURE_PUBKEY_SIZE)
    {
        Secure_Close(out);
        return NULL;
    }

    return out;
}

Secure_Session *Secure_Accept(Net_Sock sock, Secure_PubKey pub, Secure_PrivKey priv)
{
    Secure_Session *out = (Secure_Session*)malloc(sizeof(Secure_Session));

    Secure_PubKey peer = (Secure_PubKey)malloc(SECURE_PUBKEY_SIZE);

    out->sock = Net_Accept(sock);

    if(sock == -1)
    {
        free(out);
        Error_Print("Unable to accept connection.\n");
        return NULL;
    }

    if(Net_Recv(out->sock, peer, SECURE_PUBKEY_SIZE) != SECURE_PUBKEY_SIZE)
    {
        Secure_Close(out);
        Error_Print("Received key is the wrong size.\n");
        return NULL;
    }

    out->key = (uint8_t*)malloc(crypto_box_BEFORENMBYTES);

    if(crypto_box_beforenm(out->key, peer, priv))
    {
        Secure_Close(out);
        Error_Print("Unable to create shared session key.\n");
        return NULL;
    }

    return out;
}

int Secure_Send(Secure_Session *session, const uint8_t *data, size_t len)
{
    int sent;

    Secure_MsgSize size;
    uint8_t *cyphertext = (uint8_t*)malloc(len + crypto_box_MACBYTES);
    uint8_t *cyphersize = (uint8_t*)malloc(sizeof(Secure_MsgSize) + crypto_box_MACBYTES);
    uint8_t *nonce = (uint8_t*)malloc(crypto_box_NONCEBYTES);

    randombytes_buf(nonce, crypto_box_NONCEBYTES);

    if(len > SECURE_MAX_MSGSIZE)
    {
        free(cyphertext);
        free(cyphersize);
        free(nonce);
        return -1;
    }

    size.value = htonl(len);

    if(crypto_box_easy_afternm(cyphersize, size.bytes, sizeof(Secure_MsgSize),
        nonce, session->key))
    {
        free(cyphertext);
        free(cyphersize);
        free(nonce);
        Error_Print("Unable to encrypt message size.\n");
        return -1;
    }

    nonce[0] ^= 1;

    if(crypto_box_easy_afternm(cyphertext, data, len, nonce, session->key))
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

    if(Net_Send(session->sock, cyphersize, sizeof(Secure_MsgSize) + crypto_box_MACBYTES)
        != sizeof(Secure_MsgSize) + crypto_box_MACBYTES)
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

size_t Secure_Recv(Secure_Session *session, uint8_t **data)
{
    size_t out;

    Secure_MsgSize size;
    uint8_t *cyphertext;
    uint8_t *cyphersize = (uint8_t*)malloc(sizeof(Secure_MsgSize) + crypto_box_MACBYTES);
    uint8_t *nonce = (uint8_t*)malloc(crypto_box_NONCEBYTES);

    if(Net_Recv(session->sock, nonce, crypto_box_NONCEBYTES)
        != crypto_box_NONCEBYTES)
    {
        free(cyphersize);
        free(nonce);
        return -1;
    }

    if(Net_Recv(session->sock, cyphersize, sizeof(Secure_MsgSize) + crypto_box_MACBYTES)
        != sizeof(Secure_MsgSize) + crypto_box_MACBYTES)
    {
        free(cyphersize);
        free(nonce);
        return -1;
    }

    if(crypto_box_open_easy_afternm(size.bytes, cyphersize,
        sizeof(Secure_MsgSize) + crypto_box_MACBYTES, nonce, session->key))
    {
        free(cyphersize);
        free(nonce);
        return -1;
    }

    free(cyphersize);

    out = ntohl(size.value);

    if(out > SECURE_MAX_MSGSIZE)
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

    if(crypto_box_open_easy_afternm(*data, cyphertext,
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

void Secure_Close(Secure_Session *session)
{
    Net_Close(session->sock);

    free(session->key);

    free(session->nonce);
}
