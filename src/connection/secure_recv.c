#include "secure_recv.h"

#include <string.h>

Secure_PlainText Secure_Decrypt(Secure_Session *session,
    Secure_CipherText ciphertext, size_t len)
{
    Secure_PlainText out = Secure_NewPlainText(len - SECURE_CIPHER_OVERHEAD);

    if(crypto_box_open_easy_afternm(out, ciphertext, len, session->nonce, session->key))
    {
        Error_Print("Unable to decrypt ciphertext.\n");
        Secure_FreePlainText(out);
        return NULL;
    }

    Secure_ProgressNonce(session->nonce);

    return out;
}

int Secure_TestRecvSize(size_t len)
{
    if(len > SECURE_MAX_MSGSIZE)
    {
        Error_Print("Message is too long to receive.\n");
        return -1;
    }

    return 0;
}

size_t Secure_RecvSize(Secure_Session *session)
{
    Secure_MsgSize size;
    Secure_CipherText ciphersize;
    Secure_PlainText plainsize;

    ciphersize = Secure_NewCipherText(sizeof(Secure_MsgSize));

    if(Net_Recv(session->sock, ciphersize, sizeof(Secure_MsgSize) + SECURE_CIPHER_OVERHEAD)
        != sizeof(Secure_MsgSize) + SECURE_CIPHER_OVERHEAD)
    {
        Secure_FreeCipherText(ciphersize);
        return -1;
    }

    plainsize = Secure_Decrypt(session, ciphersize,
        sizeof(Secure_MsgSize) + SECURE_CIPHER_OVERHEAD);

    Secure_FreeCipherText(ciphersize);

    if(plainsize == NULL)
        return -1;

    memcpy(size.bytes, plainsize, sizeof(Secure_MsgSize));

    Secure_FreePlainText(plainsize);

    return ntohl(size.value);
}

size_t Secure_RecvMessage(Secure_Session *session, uint8_t **data, size_t len)
{
    size_t out;

    Secure_CipherText ciphertext;
    Secure_PlainText plaintext;

    ciphertext = Secure_NewCipherText(len);

    out = Net_Recv(session->sock, ciphertext, len + SECURE_CIPHER_OVERHEAD);

    if(out == -1)
    {
        Secure_FreeCipherText(ciphertext);
        return -1;
    }

    *data = Secure_Decrypt(session, ciphertext, out);

    Secure_FreeCipherText(ciphertext);

    if(*data == NULL)
        return -1;

    return out - SECURE_CIPHER_OVERHEAD;
}
