#include "proto.h"
#include <stdlib.h>
#include <string.h>

Proto_PeerID Proto_NewPeerID()
{
    return (Proto_PeerID)malloc(sizeof(uint8_t) * PROTO_PEERID_SIZE);
}

void Proto_FreePeerID(Proto_PeerID *id)
{
    free(*id);
}

Proto_PeerID Proto_XorPeerID(Proto_PeerID first, Proto_PeerID second)
{
    int i;
    Proto_PeerID out = Proto_NewPeerID();

    for(i = 0; i < PROTO_PEERID_SIZE; i++)
        out[i] = first[i] ^ second[i];

    return out;
}

Proto_Contact *Proto_NewContact()
{
    return (Proto_Contact*)malloc(sizeof(Proto_Contact));
}

void Proto_FreeContact(Proto_Contact *contact)
{
    free(contact);
}

Proto_Bucket *Proto_NewBucket(int k)
{
    Proto_Bucket *out = (Proto_Bucket*)malloc(sizeof(Proto_Bucket));

    out->k = k;

    out->list = (Proto_Contact**)malloc(k * sizeof(Proto_Contact*));
    memset(out->list, 0, k * sizeof(Proto_Contact*));

    return out;
}

void Proto_FreeBucket(Proto_Bucket *bucket)
{
    int i;

    if(bucket == NULL)
        return;

    for(i = 0; i < bucket->k; i++)
        if(bucket->list[i] != NULL)
            Proto_FreeContact(bucket->list[i]);

    free(bucket->list);

    free(bucket);
}

Proto_Table *Proto_NewTable(int k, int idsize)
{
    Proto_Table *out = (Proto_Table*)malloc(sizeof(Proto_Table));

    out->k = k;
    out->idsize = idsize;

    out->buckets = (Proto_Bucket**)malloc(idsize * sizeof(Proto_Bucket*));
    memset(out->buckets, 0, idsize * sizeof(Proto_Bucket*));

    return out;
}

void Proto_FreeTable(Proto_Table *table)
{
     int i;

     if(table == NULL)
         return;

     for(i = 0; i < table->idsize; i++)
         if(table->buckets[i] != NULL)
             Proto_FreeBucket(table->buckets[i]);

     free(table->buckets);

     free(table);
}

int Proto_AddContactToBucket(Proto_Contact *add, Proto_Bucket *to)
{
    int i;

    if(add == NULL || to == NULL)
        return -1;

    for(i = 0; i < to->k; i++)
    {
        if(to->list[i] == NULL)
        {
            to->list[i] = add;
            return 0;
        }
    }

    return -1;
}

int Proto_RemoveContactFromBucket(Proto_Contact *rm, Proto_Bucket *from)
{
    int index;

    if(from == NULL || rm == NULL)
        return -1;

    for(index = 0; index <= from->k; index++)
    {
        if(index == from->k)
            return -1;

        if(from->list[index] == rm)
            break;
    }

    memmove(&from->list[index], &from->list[index + 1],
        (from->k - index - 1) * sizeof(Proto_Contact*));

    from->list[from->k - 1] = NULL;

    return 0;
}

int Proto_BumpContactInBucket(Proto_Contact *bump, Proto_Bucket *in)
{
    int index;

    for(index = 0; index <= in->k; index++)
    {
        if(index == in->k)
            return -1;

        if(in->list[index] == bump)
            break;
    }
}

Proto_Contact *Proto_GetClosestInBucket(Proto_PeerID *id, Proto_Bucket *in)
{
}

int Proto_AddContactToTable(Proto_Contact *add, Proto_Table *to)
{
    int i;

    for(i = 0; i < to->idsize * 8; i++)
    {
        if(to->local->id[i / 8] >> i % 8 & 1 == add->id[i / 8] >> i % 8 & 1)
        {
            if(to->buckets[i] == NULL)
                to->buckets[i] = Proto_NewBucket(to->k);

            if(Proto_AddContactToBucket(add, to->buckets[i]))
                return -1;
            else
                return 0;
        }
    }

    return -1;
}

Proto_Contact *Proto_GetClosestInTable(Proto_PeerID *id, Proto_Table *in)
{
}

Proto_Contact *Proto_LookupPeer(Proto_PeerID *id)
{
}

int Proto_SendMessage(Proto_Contact *to, uint8_t *data, long len)
{
}

int Proto_RPC_Ping(Proto_Contact *to)
{
}

Proto_Contact *Proto_RPC_FindNode(Proto_Contact *to)
{
}
