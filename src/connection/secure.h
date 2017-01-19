#ifndef _SECURE_H
#define _SECURE_H

#include "secure_types.h"
#include "secure_connect.h"
#include "secure_accept.h"
#include "secure_send.h"
#include "secure_recv.h"

#include <sodium.h>
#include <stdint.h>
#include "network.h"

extern Secure_Session *Secure_Connect(Secure_PubKey peer, Secure_PubKey pub,
    Secure_PrivKey priv, Net_Addr addr, Net_Port port);

extern Secure_Server Secure_StartServer(Net_Port port);
extern Secure_Session *Secure_Accept(Secure_Server server, Secure_PubKey pub,
    Secure_PrivKey priv);

extern int Secure_Send(Secure_Session *session, const uint8_t *data, size_t len);
extern size_t Secure_Recv(Secure_Session *session, uint8_t **data);

extern void Secure_Close(Secure_Session *session);

#endif
