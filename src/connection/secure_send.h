#ifndef _SECURE_SEND_H
#define _SECURE_SEND_H

#include "secure_types.h"

extern Secure_CipherText Secure_Encrypt(Secure_Session *session,
    const uint8_t *plaintext, size_t len);

extern int Secure_TestSendSize(size_t len);

extern int Secure_SendSize(Secure_Session *session, size_t len);

extern size_t Secure_SendMessage(Secure_Session *session, const uint8_t *data, size_t len);

#endif
