#include<stdlib.h>
#include<stdio.h>
#include "ppke.h"
#include "sample.h"
#include <string.h>
#include "params.h"
#include "ccacrypto.h"
#include "poly.h"
#include "crypto_hash_sha3256.h"
#include "rng.h"
#include "poly.h"




int ntru_kem_keypair(unsigned char *pk,unsigned char *sk);
int ntru_kem_enc(unsigned char *ct,unsigned char *ss,const unsigned char *pk);
int ntru_kem_dec(unsigned char *ss,const unsigned char* ct, char* sk,char *pk);