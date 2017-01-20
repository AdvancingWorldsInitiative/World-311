#include "secure.h"

Secure_Session *Secure_Connect(Secure_PubKey peer, Secure_PubKey pub, Secure_PrivKey priv,
    Net_Addr addr, Net_Port port)
{
    Secure_Session *out = Secure_NewSession();

    if(out == NULL)
    {
        Error_Print("Unable to allocate session.\n");
        return out;
    }

    if(Secure_ConnectSocket(out, addr, port))
    {
        Secure_FreeSession(out);
        return NULL;
    }

    if(Secure_ConnectMakeNonce(out))
    {
        Secure_Close(out);
        return NULL;
    }

    out->key = Secure_NewSharedKey();

    if(Secure_GenSharedKey(out->key, peer, priv))
    {
        Secure_Close(out);
        return NULL;
    }

    if(Secure_SendPublicKey(out, peer, pub, priv))
    {
        Secure_Close(out);
        return NULL;
    }

    return out;
}

Secure_Server Secure_StartServer(Net_Port port)
{
    Secure_Server out = Net_NewSock(NET_TCP);

    Net_StartServer(out, port, NET_TCP);

    return out;
}

Secure_Session *Secure_Accept(Secure_Server server, Secure_PubKey pub,
    Secure_PrivKey priv)
{
    Secure_Session *out = Secure_NewSession();

    Secure_PubKey peer = Secure_NewPubKey();

    if(Secure_AcceptSocket(out, server))
    {
        Secure_FreeSession(out);
        return NULL;
    }

    if(Secure_AcceptMakeNonce(out))
    {
        Secure_Close(out);
        return NULL;
    }

    if(Secure_RecvPublicKey(out, peer, pub, priv))
    {
        Secure_Close(out);
        return NULL;
    }

    out->key = Secure_NewSharedKey();

    if(Secure_GenSharedKey(out->key, peer, priv))
    {
        Secure_Close(out);
        return NULL;
    }

    Secure_FreePubKey(peer);

    return out;
}

int Secure_Send(Secure_Session *session, const uint8_t *data, size_t len)
{
    if(Secure_TestSendSize(len))
        return -1;

    if(Secure_SendSize(session, len))
        return -1;

    return Secure_SendMessage(session, data, len);
}

size_t Secure_Recv(Secure_Session *session, uint8_t **data)
{
    size_t len;

    len = Secure_RecvSize(session);

    if(len == -1)
        return len;

    if(Secure_TestRecvSize(len))
        return -1;

    return Secure_RecvMessage(session, data, len);
}

void Secure_Close(Secure_Session *session)
{
    Net_Close(session->sock);

    Secure_FreeSession(session);
}
