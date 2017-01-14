#include <string.h>
#include "../connection/secure.h"
#include "../connection/network.h"

int main(int argc, char *argv[])
{
    const char *message = "The test passed.";
    int i;
    Secure_PubKey pub;
    Secure_PrivKey priv;
    Secure_PubKey peerpub;
    Secure_PubKey peerpriv;
    Secure_Session *session;
    Secure_Session *peerses;
    Net_Sock listen;
    uint8_t *plain;

    Secure_GenKeys(&pub, &priv);
    Secure_GenKeys(&peerpub, &peerpriv);

    Error_Print("Pubkey:  ");
    for(i = 0; i < SECURE_PUBKEY_SIZE; i++)
        Error_Print("%.2x", pub[i]);

    Error_Print("\nPrivkey: ");
    for(i = 0; i < SECURE_PUBKEY_SIZE; i++)
        Error_Print("%.2x", priv[i]);
    Error_Print("\n");

    listen = Net_NewSock(NET_TCP);
    Net_StartServer(listen, 5558, NET_TCP);

    session = Secure_Connect(peerpub, pub, priv, in6addr_loopback, 5558);

    if(session == NULL)
        return -1;

    peerses = Secure_Accept(listen, peerpub, peerpriv);

    if(peerses == NULL)
        return -1;

    Secure_Send(session, message, strlen(message) + 1);
    Secure_Recv(peerses, &plain);
    Error_Print("Message: %s\n", plain);

    Secure_Close(session);
    Secure_Close(peerses);

    return 0;
}
