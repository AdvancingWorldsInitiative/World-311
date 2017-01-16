#include "secure_connect.h"

int Secure_ConnectSocket(Secure_Session *session, Net_Addr addr, Net_Port port)
{
    session->sock = Net_NewSock(NET_TCP);

    if(session->sock == -1)
    {
        Error_Print("Unable to open session socket.\n");
        return -1;
    }

    if(Net_Connect(session->sock, addr, port))
    {
        Error_Print("Unable to connect session socket.\n");
        return -1;
    }

    return 0;
}

int Secure_SendPublicKey(Secure_Session *session, Secure_PubKey pub)
{
    if(Net_Send(session->sock, pub, SECURE_PUBKEY_SIZE) != SECURE_PUBKEY_SIZE)
    {
        Error_Print("Unable to send public key.\n");
        return -1;
    }

    return 0;
}
