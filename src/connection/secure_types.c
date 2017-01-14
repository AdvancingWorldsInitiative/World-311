#include "secure_types.h"

void Secure_FreePubKey(Secure_PubKey pub)
{
    free(pub);
}

void Secure_FreePrivKey(Secure_PrivKey priv)
{
    free(priv);
}

void Secure_FreeKeyPair(Secure_PubKey pub, Secure_PrivKey priv)
{
    Secure_FreePubKey(pub);
    Secure_FreePrivKey(priv);
}
