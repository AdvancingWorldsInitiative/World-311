#include "secure_connect.h"

int Secure_ConnectSocket(Secure_Session *session, Net_Addr addr, Net_Port port)
{
    session->sock = Net_NewSock(NET_TCP);

    if(Net_Connect(session->sock, addr, port))
    {
        Error_Print("Unable to connect session socket.\n");
        return -1;
    }

    return 0;
}

int Secure_SendPublicKey(Secure_Session *session, Secure_PrivKey peer,
    Secure_PubKey pub, Secure_PrivKey priv)
{
    size_t ciphersize = SECURE_PUBKEY_SIZE + crypto_box_SEALBYTES;
    uint8_t *cipher = (uint8_t*)malloc(ciphersize);

    if(crypto_box_seal(cipher, pub, SECURE_PUBKEY_SIZE, peer))
    {
        Error_Print("Unable to encrypt public key.\n");
        free(cipher);
        return -1;
    }

    if(Net_Send(session->sock, cipher, ciphersize) != ciphersize)
    {
        Error_Print("Unable to send public key.\n");
        free(cipher);
        return -1;
    }

    free(cipher);

    return 0;
}
