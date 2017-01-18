#ifndef _SECURE_MAKENONCE_H
#define _SECURE_MAKENONCE_H

#include "secure_types.h"

extern Secure_Nonce Secure_SendNoncePart(Secure_Session *session);
extern Secure_Nonce Secure_RecvNoncePart(Secure_Session *session);

extern Secure_Nonce Secure_MakeNonce(Secure_Nonce firstpart, Secure_Nonce secondpart);

#endif
