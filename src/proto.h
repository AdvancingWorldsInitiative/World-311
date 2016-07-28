#ifndef _PROTO_H
#define _PROTO_H

#include <stdint.h>
#include "network.h"

#define PROTO_DEFAULT_K 20
#define PROTO_DEFAULT_ID_SIZE 256

#define PROTO_RPC 0
#define PROTO_RPC_PING 0
#define PROTO_RPC_FINDNODE 1

typedef uint8_t* Proto_PeerID;

typedef struct
{
    Proto_PeerID id;
    Net_Addr addr;
} Proto_Contact;

typedef struct
{
    int k;
    Proto_Contact **list;
} Proto_Bucket;

typedef struct
{
    int k;
    int idsize;
    Proto_Contact *local;
    Proto_Bucket **buckets;
} Proto_Table;

extern Proto_Contact *Proto_NewContact();
extern void Proto_FreeContact(Proto_Contact *contact);

extern Proto_Bucket *Proto_NewBucket(int k);
extern void Proto_FreeBucket(Proto_Bucket *bucket);

extern Proto_Table *Proto_NewTable(int k, int idsize);
extern void Proto_FreeTable(Proto_Table *table);

extern int Proto_AddContactToBucket(Proto_Contact *add, Proto_Bucket *to);
extern int Proto_RemoveContactFromBucket(Proto_Contact *rm, Proto_Bucket *from);
extern int Proto_BumpContactInBucket(Proto_Contact *bump, Proto_Bucket *in);

extern int Proto_AddContactToTable(Proto_Contact *add, Proto_Table *to);

extern Proto_Contact *Proto_LookupPeer(Proto_PeerID *id);
extern int Proto_SendMessage(Proto_Contact *to, uint8_t *data);

extern int Proto_RPC_Ping(Proto_Contact *to);
extern Proto_Contact *Proto_RPC_FindPeer(Proto_Contact *to);

#endif
