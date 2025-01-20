
#include "poly.h"
#include "sample.h"
#include "helper.h"
#include "rng.h"
#include "ccacrypto.h"
#include "ppke.h"
#include <stdio.h>



int CCA_keypair(unsigned char *pk,
                   unsigned char *sk)
{
 //The seed is updated until we find a key, that's why not passed as a constant
 unsigned char seed[SAMPLE_FG_BYTES];
 randombytes(seed, SAMPLE_FG_BYTES);
 int loop = ppke_keypair(pk,sk, seed);

 return loop;
}


int CCA_enc(unsigned char *c,
               const char *m,
               const unsigned char *pk)
{
 //We are assuming the message already passed with enough entropy
 poly h,r,mprime;


poly_S3_frombytes(&mprime,m);
unsigned char tmpmsg[PPKE_MESSAGEBYTES];
poly_S3_tobytes(tmpmsg, &mprime);
poly_Sq_frombytes(&h, pk); //unpack the public key
sample_r(&r, &h, &mprime); //sample r based on the hash of (h,m)

ppke_enc(c,&r,&mprime,&h); //mprime is being modified that's why we copied it
return 0;
}


int CCA_dec(unsigned char *m,
              const unsigned char *ciphertext,
              const unsigned char *secretkey,
              const unsigned char *publickey)
{

 /* Do unpacking and check*/
  poly x1,x2,x3,x4;
  poly *h=&x1, *f=&x2, *r=&x3, *message=&x4;
  poly_Sq_frombytes(h,publickey);
  
  poly_S3_frombytes(f,secretkey);
  poly_Z3_to_Zq(f);
  for(int i=0;i<ORDER;i++)
    f->coeffs[i] = MODQ(3*f->coeffs[i]);
  f->coeffs[0] = MODQ(f->coeffs[0]+1);
  ppke_dec(message,ciphertext,f);
  poly *cpy_message = &x2;

  for(int i=0;i<ORDER;i++)
  {
    cpy_message->coeffs[i] = message->coeffs[i];
  }
  sample_r(r, h, cpy_message);
  unsigned char  cprime[CIPHERTEXTBYTES];
  ppke_enc(cprime,r,cpy_message,h);


  for(int i=0;i<CIPHERTEXTBYTES;i++)
  {
    if(cprime[i]!=ciphertext[i])
         {
            printf("decryption failure");
            return -1; //error
         }
        
  }
 //poly_trinary_Zq_to_Z3(message);
 poly_S3_tobytes(m,message);
 return 0; //success
  
}
