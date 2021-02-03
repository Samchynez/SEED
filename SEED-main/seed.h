#ifndef SEED_H
#define SEED_H

#include "tommath.h"

/*
 * MP_WUR - warn unused result
 */

struct mp_seed_text {
    mp_int text[4];
};

struct mp_seed_key {
    u_int32_t ks[32];
};

u_int16_t mp_G(u_int32_t x);

void mp_F(mp_int Lvs, mp_int Rvs, u_int32_t ki0, u_int32_t ki1, mp_int T1, mp_int T2);

void mp_key_schedule_set(struct mp_seed_key *context, struct mp_seed_text *key);

mp_err mp_seed_encrypt(struct mp_seed_key *key, struct mp_seed_text *in_text, struct mp_seed_text *out_text) MP_WUR;

mp_err mp_seed_decrypt(struct mp_seed_key *key, struct mp_seed_text *in_text, struct mp_seed_text *out_text) MP_WUR;

#endif // SEED_H
