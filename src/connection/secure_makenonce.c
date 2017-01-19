#include "secure_makenonce.h"

#include <string.h>
#include <stdint.h>

#include "network.h"

Secure_Nonce Secure_SendNoncePart(Secure_Session *session)
{
    Secure_Nonce out = Secure_NewNonce();

    if(Secure_GenNonce(out))
    {
        Secure_FreeNonce(out);
        return NULL;
    }

    if(Net_Send(session->sock, out, SECURE_NONCE_SIZE) != SECURE_NONCE_SIZE)
    {
        Error_Print("Unable to send nonce.\n");
        Secure_FreeNonce(out);
        return NULL;
    }

    return out;
}

Secure_Nonce Secure_RecvNoncePart(Secure_Session *session)
{
    Secure_Nonce out = Secure_NewNonce();

    if(Net_Recv(session->sock, out, SECURE_NONCE_SIZE) != SECURE_NONCE_SIZE)
    {
        Error_Print("Unable to receive nonce.\n");
        Secure_FreeNonce(out);
        return NULL;
    }

    return out;
}

Secure_Nonce Secure_MakeNonce(Secure_Nonce firstpart, Secure_Nonce secondpart)
{
    Secure_Nonce out = Secure_NewNonce();

    uint8_t *together = (uint8_t*)malloc(SECURE_NONCE_SIZE * 2);

    memcpy(together, firstpart, SECURE_NONCE_SIZE);

    memcpy(together + SECURE_NONCE_SIZE, secondpart, SECURE_NONCE_SIZE);

    if(crypto_generichash(out, SECURE_NONCE_SIZE, together, SECURE_NONCE_SIZE * 2,
        NULL, 0))
    {
        Error_Print("Unable to hash nonce.\n");
        Secure_FreeNonce(out);
        free(together);
        return NULL;
    }

    free(together);

    return out;
}

int Secure_ProgressNonce(Secure_Nonce nonce)
{
    if(crypto_generichash(nonce, SECURE_NONCE_SIZE, nonce, SECURE_NONCE_SIZE, NULL, 0))
    {
        Error_Print("Unable to progress nonce.\n");
        return -1;
    }

    return 0;
}
