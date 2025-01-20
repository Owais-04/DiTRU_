#include "poly.h"
#include<stdio.h>

void poly_Sq_tobytes(unsigned char *r, const poly *a)
{
  int i,j;

  for(i=0;i<PACK_DEG/2;i++)
  {
    r[3 * i + 0] = (unsigned char) ( MODQ(a->coeffs[2 * i + 0]) & 0xff);
    r[3 * i + 1] = (unsigned char) ((MODQ(a->coeffs[2 * i + 0]) >>  8) | ((MODQ(a->coeffs[2 * i + 1]) & 0x0f) << 4));
    r[3 * i + 2] = (unsigned char) ((MODQ(a->coeffs[2 * i + 1]) >>  4));
  }

}

void poly_Sq_frombytes(poly *r, const unsigned char *a)
{
  int i;
  for(i=0;i<PACK_DEG/2;i++)
  {
    r->coeffs[2*i+0] = (a[3*i+ 0] >> 0) | (((uint16_t)a[3*i+ 1] & 0x0f) << 8);
    r->coeffs[2*i+1] = (a[3*i+ 1] >> 4) | (((uint16_t)a[3*i+ 2] & 0xff) << 4);
  }

}