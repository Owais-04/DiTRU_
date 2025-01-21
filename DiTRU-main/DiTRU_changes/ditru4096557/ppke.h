#ifndef PPKE_H
#define PPKE_H

#include "params.h"
#include "poly.h"



#define ppke_keypair CRYPTO_NAMESPACE(ppke_keypair)
int ppke_keypair(unsigned char *pk,
                   unsigned char *sk,
                   unsigned char seed[SEEDBYTES]);

#define ppke_enc CRYPTO_NAMESPACE(ppke_enc)
void ppke_enc(unsigned char *c,
               const poly *r,
               poly *m,
               const poly *h);

#define ppke_dec CRYPTO_NAMESPACE(ppke_dec)
int ppke_dec(poly *m,
              const unsigned char *ciphertext,
              const poly *secretkey);
#endif
