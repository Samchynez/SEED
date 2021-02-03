#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "tommath.h"
#include "seed.h"
#include "kisa_seed.h"

int main()
{
    mp_int val;
    int res;
    res = mp_init(&val);

    struct key* kek = malloc(sizeof(*kek));
    struct mp_seed_key* kek2 = malloc(sizeof(*kek2));

    const u_int32_t initial_text [4] = {0x0001000, 0x04050607, 0x08090a0b, 0x0c0d0e0f};
    u_int32_t key [4] = {0x00000000, 0x00000000, 0x00000000, 0x00000000};
    u_int32_t encrypt_text [4];
    u_int32_t decrypt_text [4];

    printf("\nInitial text: ");
    for (int i = 0; i < 4; i++)
    {
        printf("%x ", initial_text[i]);
    }

    /* TEST 1 */

    printf("\n\n\t1st TEST: 128-bit integer SEED");
    key_schedule_set(kek, key);
    seed_encrypt (kek, initial_text, encrypt_text);
    printf("\n\nEncrypted text: ");
    for (int i = 0; i < 4; i++){
        printf("%x ", encrypt_text[i]);
    }

    seed_decrypt(kek, encrypt_text, decrypt_text);
    printf("\nDecrypted text: ");
    for (int i = 0; i < 4; i++){
        printf("%x ", decrypt_text[i]);
    }

    /* TEST 2 */

    struct mp_seed_text mp_initial_text;
    res = mp_init_u32(&mp_initial_text.text[0], initial_text[0]);
    res = mp_init_u32(&mp_initial_text.text[1], initial_text[1]);
    res = mp_init_u32(&mp_initial_text.text[2], initial_text[2]);
    res = mp_init_u32(&mp_initial_text.text[3], initial_text[3]);

    struct mp_seed_text mp_enc_text;
    res = mp_init_u32(&mp_enc_text.text[0], 0x00000000);
    res = mp_init_u32(&mp_enc_text.text[1], 0x00000000);
    res = mp_init_u32(&mp_enc_text.text[2], 0x00000000);
    res = mp_init_u32(&mp_enc_text.text[3], 0x00000000);

    struct mp_seed_text mp_dec_text;
    res = mp_init_u32(&mp_dec_text.text[0], 0x00000000);
    res = mp_init_u32(&mp_dec_text.text[1], 0x00000000);
    res = mp_init_u32(&mp_dec_text.text[2], 0x00000000);
    res = mp_init_u32(&mp_dec_text.text[3], 0x00000000);

    printf("\n\n\t2nd TEST: 128-bit integer SEED (libtommath edition)\n");

    res = mp_seed_encrypt(kek2, &mp_initial_text, &mp_enc_text);
    printf("\nEncrypted text: ");
    for (int i = 0; i < 4; i++){
        printf("%x ", mp_get_i32(&mp_enc_text.text[i]));
    }

    res = mp_seed_decrypt(kek2, &mp_enc_text, &mp_dec_text);
    printf("\nDecrypted text: ");
    for (int i = 0; i < 4; i++){
        printf("%x ", mp_get_i32(&mp_dec_text.text[i]));
    }

    mp_clear_multi(&mp_initial_text.text[0], &mp_initial_text.text[1], &mp_initial_text.text[2], &mp_initial_text.text[3], NULL);
    mp_clear_multi(&mp_enc_text.text[0], &mp_enc_text.text[1], &mp_enc_text.text[2], &mp_enc_text.text[3], NULL);

    /* TEST 3 */

    printf("\n\n\t3nd TEST: 128-bit integer SEED (libtommath edition)\n");

    struct mp_seed_text mp_enc_text2;
    res = mp_init_u32(&mp_enc_text2.text[0], encrypt_text[0]);
    res = mp_init_u32(&mp_enc_text2.text[1], encrypt_text[1]);
    res = mp_init_u32(&mp_enc_text2.text[2], encrypt_text[2]);
    res = mp_init_u32(&mp_enc_text2.text[3], encrypt_text[3]);

    mp_set_u32(&mp_dec_text.text[0], 0x00000000);
    mp_set_u32(&mp_dec_text.text[1], 0x00000000);
    mp_set_u32(&mp_dec_text.text[2], 0x00000000);
    mp_set_u32(&mp_dec_text.text[3], 0x00000000);

    printf("\nInitial encrypted text: ");
    for (int i = 0; i < 4; i++){
        printf("%x ", encrypt_text[i]);
    }

    res = mp_seed_decrypt(kek2, &mp_enc_text2, &mp_dec_text);
    printf("\nDecrypted text: ");
    for (int i = 0; i < 4; i++){
        printf("%x ", mp_get_i32(&mp_dec_text.text[i]));
    }
    printf("\n\n");

    mp_clear_multi(&mp_dec_text.text[0], &mp_dec_text.text[1], &mp_dec_text.text[2], &mp_dec_text.text[3], NULL);
    mp_clear_multi(&mp_enc_text2.text[0], &mp_enc_text2.text[1], &mp_enc_text2.text[2], &mp_enc_text2.text[3], NULL);

    free(kek);
    free(kek2);
    return 0;
}
