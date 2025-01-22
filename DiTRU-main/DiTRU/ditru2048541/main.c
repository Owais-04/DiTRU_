//
//  PQCgenKAT_kem.c
//
//  Created by Bassham, Lawrence E (Fed) on 8/29/17.
//  Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "rng.h"
#include "api.h"
#include "helper.h"
#include "ccacrypto.h"
#include "sample.h"
#include "poly.h"
#include "cpucycles.h"
#include "kem.h"

#define MAX_MARKER_LEN 50
#define KAT_SUCCESS 0
#define KAT_FILE_OPEN_ERROR -1
#define KAT_DATA_ERROR -3
#define KAT_CRYPTO_FAILURE -4
#define CPUCYCLES (3.5 * 1000000000.0);

int main()
{

    printf("inside the main function");
    char fn_req[32], fn_rsp[32];
    FILE *fp_req, *fp_rsp;
    unsigned char seed[48];
    unsigned char message_seed[SAMPLE_IID_BYTES];
    unsigned char input_message[PPKE_MESSAGEBYTES];
    unsigned char entropy_input[48];
    unsigned char ct[PPKE_CIPHERTEXTBYTES];
    unsigned char ss[LENGTH_OF_HASH]; // shared secret
    unsigned char ss2[LENGTH_OF_HASH];
    int count;
    int done;
    unsigned char pk[PPKE_CIPHERTEXTBYTES], sk[PPKE_SECRETKEYBYTES];
    int ret_val;
    int key;

    double execution_time_key = 0;
    double encryption_time = 0;
    double decryption_time = 0;
    unsigned long long start_key, end_key;
    unsigned long long start_enc, end_enc;
    unsigned long long start_dec, end_dec;
    unsigned long long timing_overhead;
    // Create the REQUEST file
    sprintf(fn_req, "PQCkemKAT_%d.req", CRYPTO_SECRETKEYBYTES);
    if ((fp_req = fopen(fn_req, "w")) == NULL)
    {
        printf("Couldn't open <%s> for write\n", fn_req);
        return KAT_FILE_OPEN_ERROR;
    }
    sprintf(fn_rsp, "PQCkemKAT_%d.rsp", CRYPTO_SECRETKEYBYTES);
    if ((fp_rsp = fopen(fn_rsp, "w")) == NULL)
    {
        printf("Couldn't open <%s> for write\n", fn_rsp);
        return KAT_FILE_OPEN_ERROR;
    }

    for (int i = 0; i < 48; i++)
    {
        entropy_input[i] = i;
    }
    timing_overhead = cpucycles_overhead();
    int trials = 1000; // the number of trials to execute
    randombytes_init(entropy_input, NULL, 256);
    for (int i = 0; i < trials; i++)
    {
        printf("count %d\n", i);
        fprintf(fp_req, "count = %d\n", i);
        randombytes(seed, 48);
        randombytes(message_seed, SAMPLE_IID_BYTES);
        // printBstr("message seed = ", message_seed, SAMPLE_IID_BYTES);
        poly x1;
        poly *message = &x1;
        sample_iid(message, message_seed);
        poly_S3_tobytes(input_message, message);
        // randombytes(input_message, PPKE_MESSAGEBYTES);
        // printBstr("message seed = ", message_seed, SAMPLE_IID_BYTES);

        // sample_iid(message,message_seed);
        // poly_S3_tobytes(input_message,message);

        fprintBstr(fp_req, "seed = ", seed, 48);
        //fprintBstr(fp_req, "original message = ", input_message, PPKE_MESSAGEBYTES);
        fprintf(fp_req, "pk =\n");
        fprintf(fp_req, "sk =\n");
        fprintf(fp_req, "ct =\n");
        fprintf(fp_req, "ss =\n");
        //        fprintf(fp_req, "decrypted message =\n");
    }
    fclose(fp_req);
    
    // Create the RESPONSE file based on what's in the REQUEST file
    if ((fp_req = fopen(fn_req, "r")) == NULL)
    {
        printf("Couldn't open <%s> for read\n", fn_req);
        return KAT_FILE_OPEN_ERROR;
    }

    fprintf(fp_rsp, "# %s\n\n", CRYPTO_ALGNAME);

    unsigned long long start, end;
    double execution_time;
    start = cpucycles_start();

    done = 0;
    do
    {
        if (FindMarker(fp_req, "count = "))
            fscanf(fp_req, "%d", &count);
        else
        {
            done = 1;
            break;
        }
        fprintf(fp_rsp, "count = %d\n", count);

        if (!ReadHex(fp_req, seed, 48, "seed = "))
        {
            printf("ERROR: unable to read 'seed' from <%s>\n", fn_req);
            return KAT_DATA_ERROR;
        }
        fprintBstr(fp_rsp, "seed = ", seed, 48);

        // if (!ReadHex(fp_req, input_message, PPKE_MESSAGEBYTES, "original message = "))
        // {
        //     printf("ERROR: unable to read 'seed' from <%s>\n", fn_req);
        //     return KAT_DATA_ERROR;
        // }

        //   fprintBstr(fp_rsp, "original message = ", input_message, PPKE_MESSAGEBYTES);

        randombytes_init(seed, NULL, 256);
        // measuring the time for key generation

        start_key = cpucycles_start();

        // Generate the public/private keypair
        // if ( (ret_val = CCA_keypair(pk, sk)) <= 0) {
        // printf("crypto_kem_keypair returned <%d>\n", ret_val);
        // return KAT_CRYPTO_FAILURE;
        //}
        key = ntru_kem_keypair(pk, sk);
        if (key <= 0)
        {
            printf("ntru_kem_keypair returned <%d>\n", key);
        }

        end_key = cpucycles_stop();
        execution_time_key += ((double)(end_key - start_key) / ret_val);
      
        fprintBstr(fp_rsp, "pk = ", pk, PPKE_PUBLICKEYBYTES);
        fprintBstr(fp_rsp, "sk = ", sk, PPKE_SECRETKEYBYTES);

        start_enc = cpucycles_start();
        // enc the key
        // printf("ss= %s\n", ss);

        key = ntru_kem_enc(ct, ss, pk);
        if (key < 0)
        {
            printf("encryption failed.\n");
        }

        //  fprintBstr(fp_rsp, "ss= ", ss, LENGTH_OF_HASH);
        // Encrypt the message
        /* if ( (ret_val = CCA_enc(ct,input_message,pk)) != 0) {
             printf("crypto_kem_enc returned <%d>\n", ret_val);
             return KAT_CRYPTO_FAILURE;
         }*/

        end_enc = cpucycles_stop();

        encryption_time += ((double)(end_enc - start_enc));
        unsigned char decrypted_msg[PPKE_MESSAGEBYTES];

        start_dec = cpucycles_start();
        /*if ( (ret_val = CCA_dec(decrypted_msg,ct,sk,pk)) != 0) {
            printf("crypto_kem_dec returned <%d>\n", ret_val);
            return KAT_CRYPTO_FAILURE;
        }*/

        key = ntru_kem_dec(ss2, ct, sk, pk);

        if (key < 0)
        {
            printf("decryption failed.\n");
        }
        // printf("ss = %s\n", ss);
        end_dec = cpucycles_stop();
        decryption_time += ((double)(end_dec - start_dec));
        fprintBstr(fp_rsp, "ct = ", ct, PPKE_CIPHERTEXTBYTES);
        fprintBstr(fp_rsp, "ss= ", ss2, LENGTH_OF_HASH);

        fprintf(fp_rsp, "\n");

    } while (!done);

    fclose(fp_req);
    fclose(fp_rsp);

    end = cpucycles_stop();

    unsigned long long total_cpu_cycles = ((end - start)) / trials - timing_overhead;
    unsigned long long key_gen_cycles = ((execution_time_key)) / trials - timing_overhead;
    unsigned long long encryption_cycles = ((encryption_time)) / trials - timing_overhead;
    unsigned long long decryption_cycles = ((decryption_time)) / trials - timing_overhead;

    // printf("clock per second: %ld \n", CLOCKS_PER_SEC);

    execution_time = ((double)(end - start) * 1000) / CPUCYCLES;
    execution_time_key = ((double)(execution_time_key) * 1000) / CPUCYCLES;
    encryption_time = ((double)(encryption_time) * 1000) / CPUCYCLES;
    decryption_time = ((double)(decryption_time) * 1000) / CPUCYCLES

                                                               printf("Average CPU cycles for all: %lld \n", total_cpu_cycles);
    printf("Average Time taken to execute in seconds : %f (ms) \n", execution_time / trials);

    printf("Average CPU cycles for key generation: %lld \n", key_gen_cycles);
    printf("Average time for key generation: %f (ms) \n", execution_time_key / trials);

    printf("Average CPU cycles for encryption: %lld \n", encryption_cycles);
    printf("Average time for encryption: %f (ms)\n", encryption_time / trials);

    printf("Average cpu cycles for decryption: %lld \n", decryption_cycles);
    printf("Average time for decryption: %f (ms) \n", decryption_time / trials);

    return KAT_SUCCESS;
}
