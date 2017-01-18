#include "secure_makenonce.h"

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
}
