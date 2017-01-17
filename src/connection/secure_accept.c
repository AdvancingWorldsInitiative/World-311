#include "secure_accept.h"

int Secure_AcceptSocket(Secure_Session *session, Secure_Server server)
{
   return session->sock = Net_Accept(server);
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
