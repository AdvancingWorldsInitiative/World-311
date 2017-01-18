#ifndef _SECURE_CONNECT_H
#define _SECURE_CONNECT_H

#include "secure_types.h"

extern int Secure_ConnectSocket(Secure_Session *session, Net_Addr addr, Net_Port port);

extern int Secure_ConnectMakeNonce(Secure_Session *session);

extern int Secure_SendPublicKey(Secure_Session *session, Secure_PrivKey peer,
    Secure_PubKey pub, Secure_PrivKey priv);

#endif
