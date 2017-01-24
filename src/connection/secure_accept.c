#include "secure_accept.h"

#include "secure_makenonce.h"

int Secure_AcceptSocket(Secure_Session *session, Secure_Server server)
{
    session->sock = Net_Accept(server);

    if(session->sock == -1)
        return -1;

    return 0;
}

int Secure_AcceptMakeNonce(Secure_Session *session)
{
    Secure_Nonce firstpart, secondpart;

    firstpart = Secure_RecvNoncePart(session);

    if(firstpart == NULL)
        return -1;

    secondpart = Secure_SendNoncePart(session);

    if(secondpart == NULL)
    {
        Secure_FreeNonce(firstpart);
        return -1;
    }

    session->nonce = Secure_MakeNonce(firstpart, secondpart);

    if(session->nonce == NULL)
    {
        Secure_FreeNonce(firstpart);
        Secure_FreeNonce(secondpart);
        return -1;
    }

    return 0;
}

int Secure_RecvPublicKey(Secure_Session *session, Secure_PubKey peer,
    Secure_PubKey pub, Secure_PrivKey priv)
{
    size_t ciphersize = SECURE_PUBKEY_SIZE + crypto_box_SEALBYTES;
    uint8_t *cipher = (uint8_t*)malloc(ciphersize);

    if(Net_Recv(session->sock, cipher, ciphersize) != ciphersize)
    {
        Error_Print("Received key is the wrong size.\n");
        free(cipher);
        return -1;
    }

    if(crypto_box_seal_open(peer, cipher, ciphersize, pub, priv))
    {
        Error_Print("Unable to decrypt received public key.\n");
        free(cipher);
        return -1;
    }

    free(cipher);

    return 0;
}
