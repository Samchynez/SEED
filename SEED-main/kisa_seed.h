#ifndef KISA_SEED_H
#define KISA_SEED_H

#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include <sys/types.h>
#include "const.h"

struct key
{
    u_int32_t ks[32];
};

u_int16_t G(u_int32_t x)
{
    u_int32_t z = SS0[(x) & 0xFF] ^ SS1[((x) >> 8) & 0xFF] ^ SS2[((x) >> 16) & 0xFF] ^ SS3[((x) >> 24) & 0xFF];
    return z;
};

void F(u_int32_t Lvs, u_int32_t Rvs, u_int32_t ki0, u_int32_t ki1, u_int32_t* t1, u_int32_t* t2)
{
    u_int32_t Rv, Lv;
    Lv = Rvs;
    Rv = Lvs;

    *t1 = ((G(( G(( G((Rv ^ ki0) ^ (Lv ^ ki1)) + (Rv ^ ki0)) % M) + G((Rv ^ ki0) ^ (Lv ^ ki1))) % M) + G(( G((Rv ^ ki0) ^ (Lv ^ ki1)) + (Rv ^ ki0)) % M)) % M) & 0xFFFFFFFF;
    *t2 = G(( G(( G((Rv ^ ki0) ^ (Lv ^ ki1)) + (Rv ^ ki0)) % M) + G((Rv ^ ki0) ^ (Lv ^ ki1))) % M) & 0xFFFFFFFF;
};

void key_schedule_set(struct key *context, u_int32_t key[])
{
    u_int32_t t, key0, key1, key2, key3;

    key0 = key[0];
    key1 = key[1];
    key2 = key[2];
    key3 = key[3];

    for (int i = 0; i < 16; i++){
        t = (((key0 + key2) % M) - table_key[i]) % M;
        context->ks[2*i] = G(t);

        t = (((key1 - key3) % M) + table_key[i]) % M;
        context->ks[2*i +1] = G(t);

        if ((i % 2) != 0)
        {
            t = (key3 << 8) | (key2 >> 24);
            key2 = (key2 << 8) | (key3 >> 24);
            key3 = t;
        }
        else
        {
            t = (key1 >> 8) | (key0 << 24);
            key0 = (key0 >> 8) | (key1 << 24);
            key1 = t;
        }

    }
};

void seed_encrypt(struct key *context, const u_int32_t input[], u_int32_t output[])
{
    u_int32_t *ks;
    u_int32_t t1 = 0;
    u_int32_t t2 = 1;
    u_int32_t Lv1 = input[0];
    u_int32_t Lv2 = input[1];
    u_int32_t Rv1 = input[2];
    u_int32_t Rv2 = input[3];

    ks = context->ks;

    for (int i = 0; i < 16; i++){
        F(Rv1, Rv2, ks[2*i], ks[2*i + 1], &t1, &t2);
        t1 ^= Lv1;
        t2 ^= Lv2;
        Lv1 = Rv1;
        Lv2 = Rv2;
        Rv1 = t1;
        Rv2 = t2;
    }

    output[2] = Lv1;
    output[3] = Lv2;
    output[0] = Rv1;
    output[1] = Rv2;

};

void seed_decrypt(struct key *context, const u_int32_t input[], u_int32_t output[])
{
    u_int32_t *ks;
    u_int32_t t1 = 0;
    u_int32_t t2 = 1;
    u_int32_t Lv1 = input[0];
    u_int32_t Lv2 = input[1];
    u_int32_t Rv1 = input[2];
    u_int32_t Rv2 = input[3];

    ks = context->ks;

    for (int i = 0; i < 16; i++)
    {
        F(Rv1, Rv2, ks[30 - 2*i], ks[31 - 2*i], &t1, &t2);
        t1 ^= Lv1;
        t2 ^= Lv2;
        Lv1 = Rv1;
        Lv2 = Rv2;
        Rv1 = t1;
        Rv2 = t2;
    }

    output[2] = Lv1;
    output[3] = Lv2;
    output[0] = Rv1;
    output[1] = Rv2;
};

#endif // KISA_SEED_H
