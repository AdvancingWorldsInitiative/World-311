#ifndef _SECURE_ACCEPT_H
#define _SECURE_ACCEPT_H

#include "secure_types.h"

extern int Secure_AcceptSocket(Secure_Session *session, Secure_Server server);

extern int Secure_AcceptMakeNonce(Secure_Session *session);

extern int Secure_RecvPublicKey(Secure_Session *session, Secure_PubKey peer,
    Secure_PubKey pub, Secure_PrivKey priv);

#endif
