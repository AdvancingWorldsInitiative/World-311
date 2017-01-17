#include <string.h>
#include "../connection/secure.h"
#include "../connection/network.h"

int main(int argc, char *argv[])
{
    const char *message = "The test passed.";
    int i;
    Secure_PubKey pub = Secure_NewPubKey();
    Secure_PrivKey priv = Secure_NewPrivKey();
    Secure_PubKey peerpub = Secure_NewPubKey();
    Secure_PubKey peerpriv = Secure_NewPrivKey();

    Secure_Session *session;
    Secure_Session *peerses;

    Secure_Server listen;
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

    listen = Secure_StartServer(5558);

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

    Secure_FreePubKey(pub);
    Secure_FreePrivKey(priv);
    Secure_FreePubKey(peerpub);
    Secure_FreePrivKey(peerpriv);

    return 0;
}
