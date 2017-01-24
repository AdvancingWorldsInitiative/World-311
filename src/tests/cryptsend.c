#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../connection/secure.h"
#include "../connection/network.h"

static int Connector();
static int Acceptor();

#define KEY_FILE "peerkey"

int main(int argc, char *argv[])
{
    if(argc < 2)
        return -1;

    if(strcmp(argv[1], "-c") == 0)
        return Connector();

    if(strcmp(argv[1], "-a") == 0)
        return Acceptor();

    Error_Print("Please specify -c or -a\n");

    return -1;
}

int Connector()
{
    int i;

    Secure_PubKey peerpub = Secure_NewPubKey();

    const char *message = "The test passed.";

    Secure_PubKey pub = Secure_NewPubKey();
    Secure_PrivKey priv = Secure_NewPrivKey();

    FILE *file = fopen(KEY_FILE, "rb");

    Secure_Session *session = NULL;

    fread(peerpub, SECURE_PUBKEY_SIZE, 1, file);

    fclose(file);

    Secure_GenKeys(pub, priv);

    Error_Print("Pubkey:  ");
    for(i = 0; i < SECURE_PUBKEY_SIZE; i++)
        Error_Print("%.2x", pub[i]);

    Error_Print("\nPrivkey: ");
    for(i = 0; i < SECURE_PUBKEY_SIZE; i++)
        Error_Print("%.2x", priv[i]);
    Error_Print("\n");

    session = Secure_Connect(peerpub, pub, priv, in6addr_loopback, 5558);

    if(session == NULL)
    {
        Error_Print("Session is NULL.\n");
        return -1;
    }

    Secure_Send(session, message, strlen(message) + 1);

    Secure_Close(session);

    Secure_FreePubKey(pub);
    Secure_FreePrivKey(priv);

    return 0;
}

int Acceptor()
{
    uint8_t *plain;

    Secure_Server listen;

    Secure_Session *peerses;

    FILE *file = fopen(KEY_FILE, "wb");

    Secure_PubKey peerpub = Secure_NewPubKey();
    Secure_PubKey peerpriv = Secure_NewPrivKey();

    Secure_GenKeys(peerpub, peerpriv);

    if(fwrite(peerpub, sizeof(uint8_t), SECURE_PUBKEY_SIZE, file) != SECURE_PUBKEY_SIZE)
    {
        Error_Print("Unable to write public key to a file.\n");
        return -1;
    }

    fclose(file);

    listen = Secure_StartServer(5558);

    peerses = Secure_Accept(listen, peerpub, peerpriv);

    if(peerses == NULL)
    {
        Error_Print("Session didn't start.\n");
        return -1;
    }

    Secure_Recv(peerses, &plain);
    Error_Print("Message: %s\n", plain);

    Secure_FreePubKey(peerpub);
    Secure_FreePrivKey(peerpriv);

    Secure_Close(peerses);

    free(plain);

    return 0;
}
