#include "../connection/secure.h"

int main(int argc, char *argv[])
{
    int i;
    Sec_PubKey pub;
    Sec_PrivKey priv;
    Sec_PubKey peerpub;
    Sec_PubKey peerpriv;
    Sec_Session *session;
    Sec_Session *peerses;
    Net_Sock listen;
    uint8_t *plain;

    Sec_GenKeys(&pub, &priv);
    Sec_GenKeys(&peerpub, &peerpriv);

    Error_Print("Pubkey:  ");
    for(i = 0; i < SEC_PUBKEY_SIZE; i++)
        Error_Print("%.2x", pub[i]);

    Error_Print("\nPrivkey: ");
    for(i = 0; i < SEC_PUBKEY_SIZE; i++)
        Error_Print("%.2x", priv[i]);
    Error_Print("\n");

    listen = Net_NewSock(NET_TCP);
    Net_StartServer(listen, 5558, NET_TCP);

    session = Sec_Connect(peerpub, pub, priv, in6addr_loopback, 5558);

    if(session == NULL)
        return -1;

    peerses = Sec_Accept(listen, peerpub, peerpriv);

    if(peerses == NULL)
        return -1;

    Sec_Send(session, "secret", 7);
    Sec_Recv(peerses, &plain);
    Error_Print("Message: %s\n", plain);

    Sec_Close(session);
    Sec_Close(peerses);

    return 0;
}
