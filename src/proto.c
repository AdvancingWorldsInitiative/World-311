#include "proto.h"
#include <stdlib.h>
#include <string.h>

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

int Proto_RemoveContactFromBucket(Proto_Bucket *from, unsigned int index)
{
    if(from == NULL || from->list[index] == NULL)
        return -1;

    memmove(&from->list[index], &from->list[index + 1],
        (from->k - index - 1) * sizeof(Proto_Contact*));

    from->list[from->k - 1] = NULL;

    return 0;
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
