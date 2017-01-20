#ifndef _SECURE_RECV_H
#define _SECURE_RECV_H

#include "secure_types.h"

extern Secure_PlainText Secure_Decrypt(Secure_Session *session,
    Secure_CipherText ciphertext, size_t len);

extern int Secure_TestRecvSize(size_t len);

extern size_t Secure_RecvSize(Secure_Session *session);

extern size_t Secure_RecvMessage(Secure_Session *session, uint8_t **data, size_t len);

#endif
