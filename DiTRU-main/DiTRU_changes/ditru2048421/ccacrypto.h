#ifndef CCACRYPTO_H
#define CCACRYPTO_H

#include "params.h"
#include "poly.h"

// #define owcpa_samplemsg CRYPTO_NAMESPACE(owcpa_samplemsg)
// void owcpa_samplemsg(unsigned char msg[NTRU_OWCPA_MSGBYTES],
//                      const unsigned char seed[NTRU_SEEDBYTES]);

#define CCA_keypair CRYPTO_NAMESPACE(CCA_keypair)
int CCA_keypair(unsigned char *pk,
                   unsigned char *sk);

#define CCA_enc CRYPTO_NAMESPACE(CCA_enc)
int CCA_enc(unsigned char *c,
               const char *m,
               const unsigned char *pk);

#define CCA_dec CRYPTO_NAMESPACE(CCA_dec)
int CCA_dec(unsigned char *m,
              const unsigned char *ciphertext,
              const unsigned char *secretkey,
              const unsigned char *publickey);
#endif