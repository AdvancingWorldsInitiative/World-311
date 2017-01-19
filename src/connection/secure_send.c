#include "secure_send.h"

Secure_CipherText Secure_Encrypt(Secure_Session *session,
    const uint8_t *plaintext, size_t len)
{
    Secure_CipherText out = Secure_NewCipherText(len);

    if(crypto_box_easy_afternm(out, plaintext, len,
        session->nonce, session->key))
    {
        Error_Print("Unable to encrypt plaintext.\n");
        Secure_FreeCipherText(out);
        return NULL;
    }

    Secure_ProgressNonce(session->nonce);

    return out;
}

int Secure_TestSendSize(size_t len)
{
    if(len > SECURE_MAX_MSGSIZE)
    {
        Error_Print("Message is too long to be sent.\n");
        return -1;
    }

    return 0;
}

int Secure_SendSize(Secure_Session *session, size_t len)
{
    Secure_MsgSize size;
    Secure_CipherText ciphersize;

    size.value = htonl(len);

    ciphersize = Secure_Encrypt(session, size.bytes, len);

    if(ciphersize == NULL)
        return -1;

    if(Net_Send(session->sock, ciphersize, sizeof(Secure_MsgSize) + SECURE_CIPHER_OVERHEAD)
        != sizeof(Secure_MsgSize) + SECURE_CIPHER_OVERHEAD)
    {
        Secure_FreeCipherText(ciphersize);
        return -1;
    }

    Secure_FreeCipherText(ciphersize);

    return 0;
}

int Secure_SendMessage(Secure_Session *session, const uint8_t *data, size_t len)
{
    int sent;

    Secure_CipherText ciphertext;

    ciphertext = Secure_Encrypt(session, data, len);

    if(ciphertext == NULL)
        return -1;

    sent = Net_Send(session->sock, ciphertext, len + crypto_box_MACBYTES);

    Secure_FreeCipherText(ciphertext);

    return sent;
}
