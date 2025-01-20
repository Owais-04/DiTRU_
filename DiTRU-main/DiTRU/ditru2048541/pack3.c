#include "poly.h"
#include <stdio.h>
#include "helper.h"
void poly_S3_tobytes(unsigned char msg[PPKE_MESSAGEBYTES], const poly *a)
{
  int i;
  unsigned char c;
#if PACK_DEG > (PACK_DEG / 5) * 5  // if 5 does not divide ORDER
  int j;
#endif

  for(i=0; i<PACK_DEG/5; i++)
  {
    c =        a->coeffs[5*i+4] & 255;
    c = (3*c + a->coeffs[5*i+3]) & 255;
    c = (3*c + a->coeffs[5*i+2]) & 255;
    c = (3*c + a->coeffs[5*i+1]) & 255;
    c = (3*c + a->coeffs[5*i+0]) & 255;
    msg[i] = c;
  }
#if PACK_DEG > (PACK_DEG / 5) * 5  // if 5 does not divide NTRU_N-1
  i = PACK_DEG/5;
  c = 0;
  for(j = PACK_DEG - (5*i) - 1; j>=0; j--)
    c = (3*c + a->coeffs[5*i+j]) & 255;
  msg[i] = c;
#endif
}

void poly_S3_frombytes(poly *r, const unsigned char msg[PPKE_MESSAGEBYTES])
{
  
  //printf("inside poly 3 fun \n");
  //printBstr("secret key (packed f)= ", (unsigned char)msg, PACK_TRINARY_BYTES);
  int i;
  unsigned char c;
#if PACK_DEG > (PACK_DEG / 5) * 5  // if 5 does not divide NTRU_N-1
  int j;
#endif

  for(i=0; i<PACK_DEG/5; i++)
  {
    c = msg[i];
    r->coeffs[5*i+0] = c;
    r->coeffs[5*i+1] = c * 171 >> 9;  // this is division by 3
    r->coeffs[5*i+2] = c * 57 >> 9;  // division by 3^2
    r->coeffs[5*i+3] = c * 19 >> 9;  // division by 3^3
    r->coeffs[5*i+4] = c * 203 >> 14;  // etc.
  }
#if PACK_DEG > (PACK_DEG / 5) * 5  // if 5 does not ORDER
  i = PACK_DEG/5;
  c = msg[i];
  for(j=0; (5*i+j)<PACK_DEG; j++)
  {
    r->coeffs[5*i+j] = c;
    c = c * 171 >> 9;
  }
#endif
  //r->coeffs[NTRU_N-1] = 0; //*commented because last coefficient is not zero in our case*
  poly_mod_3(r);  
}

