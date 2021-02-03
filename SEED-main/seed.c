#include <stdint.h>
#include <inttypes.h>

#include "tommath.h"
#include "seed.h"
#include "const.h"

u_int16_t mp_G(u_int32_t x)
{
    u_int32_t z = SS0[(x) & 0xFF] ^ SS1[((x) >> 8) & 0xFF] ^ SS2[((x) >> 16) & 0xFF] ^ SS3[((x) >> 24) & 0xFF];
    return z;
}

void mp_F(mp_int Lvs, mp_int Rvs, u_int32_t ki0, u_int32_t ki1, mp_int T1, mp_int T2)
{
    u_int32_t t1 = mp_get_u32(&T1);
    u_int32_t t2 = mp_get_u32(&T2);
    u_int32_t Rv = mp_get_u32(&Rvs);
    u_int32_t Lv = mp_get_u32(&Lvs);

    t1 = ((mp_G(( mp_G(( mp_G((Rv ^ ki0) ^ (Lv ^ ki1)) + (Rv ^ ki0)) % M) + mp_G((Rv ^ ki0) ^ (Lv ^ ki1))) % M) + mp_G(( mp_G((Rv ^ ki0) ^ (Lv ^ ki1)) + (Rv ^ ki0)) % M)) % M) & 0xFFFFFFFF;
    t2 = mp_G(( mp_G(( mp_G((Rv ^ ki0) ^ (Lv ^ ki1)) + (Rv ^ ki0)) % M) + mp_G((Rv ^ ki0) ^ (Lv ^ ki1))) % M) & 0xFFFFFFFF;

    mp_set_u32 (&T1, t1);
    mp_set_u32 (&T2, t2);
}

void mp_key_schedule_set(struct mp_seed_key *context, struct mp_seed_text *key)
{
    u_int32_t t, key0, key1, key2, key3;

    key0 = mp_get_u32(&key->text[0]);
    key1 = mp_get_u32(&key->text[1]);
    key2 = mp_get_u32(&key->text[2]);
    key3 = mp_get_u32(&key->text[3]);

    for (int i = 0; i < 16; i++)
    {
        t = (((key0 + key2) % M) - table_key[i]) % M;
        context->ks[2*i] = mp_G(t);

        t = (((key1 - key3) % M) + table_key[i]) % M;
        context->ks[2*i +1] = mp_G(t);

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
}

mp_err mp_seed_encrypt(struct mp_seed_key *key, struct mp_seed_text *in_text, struct mp_seed_text *out_text)
{
    mp_err err;
    mp_int t1, t2, Lv1, Lv2, Rv1, Rv2;
    //u_int32_t ki0, ki1;

    err = mp_init_u32(&t1, 0);
    err = mp_init_u32(&t2, 1);
    err = mp_init_copy(&Lv1, &in_text->text[0]);
    err = mp_init_copy(&Lv2, &in_text->text[1]);
    err = mp_init_copy(&Rv1, &in_text->text[2]);
    err = mp_init_copy(&Rv2, &in_text->text[3]);

    for (int i=0; i<16; i++)
    {
        // F function
        mp_F(Rv1, Rv2, key->ks[2*i], key->ks[2*i+1], t1, t2);

        err = mp_xor(&t1, &Lv1, &t1); // t1 ^= Lv1;
        err = mp_xor(&t2, &Lv2, &t2); // t2 ^= Lv2;
        err = mp_copy(&Rv1, &Lv1);
        err = mp_copy(&Rv2, &Lv2);
        err = mp_copy(&t1, &Rv1);
        err = mp_copy(&t2, &Rv2);
    }

    err = mp_copy(&Lv1, &out_text->text[2]);
    err = mp_copy(&Lv2, &out_text->text[3]);
    err = mp_copy(&Rv1, &out_text->text[0]);
    err = mp_copy(&Rv2, &out_text->text[1]);

    mp_clear_multi(&t1, &t2, &Lv1, &Lv2, &Rv1, &Rv2, NULL);
    return err;
}

mp_err mp_seed_decrypt(struct mp_seed_key *key, struct mp_seed_text *in_text, struct mp_seed_text *out_text)
{
    mp_err err;
    mp_int t1, t2, Lv1, Lv2, Rv1, Rv2;
    // u_int32_t ki0, ki1;

    err = mp_init_u32(&t1, 0);
    err = mp_init_u32(&t2, 1);
    err = mp_init_copy(&Lv1, &in_text->text[0]);
    err = mp_init_copy(&Lv2, &in_text->text[1]);
    err = mp_init_copy(&Rv1, &in_text->text[2]);
    err = mp_init_copy(&Rv2, &in_text->text[3]);

    for (int i=0; i<16; i++)
    {
        // F function
        //ki0 = mp_get_u32(&key->ks[30 - 2*i]);
        //ki1 = mp_get_u32(&key->ks[31 - 2*i]);
        mp_F(Rv1, Rv2, key->ks[30 - 2*i], key->ks[31 - 2*i], t1, t2);

        err = mp_xor(&t1, &Lv1, &t1); // t1 ^= Lv1;
        err = mp_xor(&t2, &Lv2, &t2); // t2 ^= Lv2;
        err = mp_copy(&Rv1, &Lv1);
        err = mp_copy(&Rv2, &Lv2);
        err = mp_copy(&t1, &Rv1);
        err = mp_copy(&t2, &Rv2);
    }

    err = mp_copy(&Lv1, &out_text->text[2]);
    err = mp_copy(&Lv2, &out_text->text[3]);
    err = mp_copy(&Rv1, &out_text->text[0]);
    err = mp_copy(&Rv2, &out_text->text[1]);

    mp_clear_multi(&t1, &t2, &Lv1, &Lv2, &Rv1, &Rv2, NULL);
    return err;
}
