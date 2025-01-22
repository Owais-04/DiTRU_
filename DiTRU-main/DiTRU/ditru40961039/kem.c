#include<stdio.h>
#include "kem.h"
#include "api.h"


int ntru_kem_keypair(unsigned char *pk,unsigned char *sk)
{
   int i= CCA_keypair(pk,sk);
   return i;
} 


int  ntru_kem_enc(unsigned char *ct,unsigned char *ss,const unsigned char *pk){
   // unsigned char* secret_seed=malloc(SAMPLE_IID_BYTES);
      
  // unsigned char* shared_secret=malloc(PACK_TRINARY_BYTES);
  unsigned char secret_seed[SAMPLE_IID_BYTES];
  unsigned char shared_secret[PACK_TRINARY_BYTES];
   poly x1;
   poly * shared_secret_3 =&x1;
   
    
    randombytes(secret_seed,SAMPLE_IID_BYTES);
    sample_iid(shared_secret_3,secret_seed);
    poly_S3_tobytes(shared_secret,shared_secret_3);
   int i = CCA_enc(ct, shared_secret,pk);
    crypto_hash_sha3512(shared_secret, shared_secret, PACK_TRINARY_BYTES);
    // crypto_hash_sha3512(shared_secret+CRYPTO_BYTES,pk, PUBLICKEYBYTES);
    // crypto_hash_sha3512(shared_secret,  shared_secret,CRYPTO_BYTES+LENGTH_OF_HASH);
    memcpy(ss,shared_secret,LENGTH_OF_HASH);
    // free(shared_secret);
    // free(secret_seed);
    return i;
    
}
int ntru_kem_dec(unsigned char *ss,const unsigned char* ct, char* sk,char *pk)
{   
  //  unsigned char* shared_secret=malloc(PACK_TRINARY_BYTES);
  unsigned char shared_secret[PACK_TRINARY_BYTES];
   int i= CCA_dec(shared_secret,ct,sk,pk);
    crypto_hash_sha3512(shared_secret, shared_secret, PACK_TRINARY_BYTES);
    // crypto_hash_sha3512(shared_secret+CRYPTO_BYTES,pk,CRYPTO_BYTES);
    // crypto_hash_sha3512(shared_secret,shared_secret,CRYPTO_BYTES+LENGTH_OF_HASH);
    memcpy(ss,shared_secret,LENGTH_OF_HASH);
    // free(shared_secret);
return 0;
}