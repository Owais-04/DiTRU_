/* Based on supercop-20200702/crypto_core/invhrss701/simpler/core.c */

#include "poly.h"

/* return -1 if x<0 and y<0; otherwise return 0 */
static inline int16_t both_negative_mask(int16_t x,int16_t y)
{
  return (x & y) >> 15;
}

void poly_R2_inv(poly *r, const poly *a)
{
  poly f, g, v, w;
  size_t i, loop;
  int16_t delta,sign,swap,t;

  for (i = 0;i < N;++i) v.coeffs[i] = 0;
  for (i = 0;i < N;++i) w.coeffs[i] = 0;
  w.coeffs[0] = 1;

  for (i = 0;i < N;++i) f.coeffs[i] = 1;
  for (i = 0;i < N-1;++i) g.coeffs[N-2-i] = (a->coeffs[i] ^ a->coeffs[N-1]) & 1;
  g.coeffs[N-1] = 0;

  delta = 1;

  for (loop = 0;loop < 2*(N-1)-1;++loop) {
    for (i = N-1;i > 0;--i) v.coeffs[i] = v.coeffs[i-1];
    v.coeffs[0] = 0;

    sign = g.coeffs[0] & f.coeffs[0];
    swap = both_negative_mask(-delta,-(int16_t) g.coeffs[0]);
    delta ^= swap & (delta ^ -delta);
    delta += 1;

    for (i = 0;i < N;++i) {
      t = swap&(f.coeffs[i]^g.coeffs[i]); f.coeffs[i] ^= t; g.coeffs[i] ^= t;
      t = swap&(v.coeffs[i]^w.coeffs[i]); v.coeffs[i] ^= t; w.coeffs[i] ^= t;
    }

    for (i = 0;i < N;++i) g.coeffs[i] = g.coeffs[i]^(sign&f.coeffs[i]);
    for (i = 0;i < N;++i) w.coeffs[i] = w.coeffs[i]^(sign&v.coeffs[i]);
    for (i = 0;i < N-1;++i) g.coeffs[i] = g.coeffs[i+1];
    g.coeffs[N-1] = 0;
  }

  for (i = 0;i < N-1;++i) r->coeffs[i] = v.coeffs[N-2-i];
  r->coeffs[N-1] = 0;
}







#define POLY_R2_ADD(I,A,B,S,N)        \
   for(I=0; I<N; I++)        \
   { A.coeffs[I] ^= B.coeffs[I] * S;  }

#define POLY_S3_FMADD(I,A,B,S,N)                    \
   for(I=0; I<N; I++)                            \
   { A.coeffs[I] = mod3(A.coeffs[I] + S * B.coeffs[I]); }

/* If swap is 0 then don't swap, If swap is 1 then swap*/
static void cswappoly(poly *a, poly *b, int swap, int n)
{
  int i;
  uint16_t t;
  swap = -swap;
  for(i=0;i<n;i++)
  {
    t = (a->coeffs[i] ^ b->coeffs[i]) & swap;
    a->coeffs[i] ^= t;
    b->coeffs[i] ^= t;
  }
}


static inline void poly_divx(poly *a, int s, int n)
{
  int i;

  for(i=1; i<n; i++)
    a->coeffs[i-1] = (s * a->coeffs[i]) | (!s * a->coeffs[i-1]);
  a->coeffs[n-1] = (!s * a->coeffs[n-1]);
}

static inline void poly_mulx(poly *a, int s, int n)
{
  int i;

  for(i=1; i<n; i++)
    a->coeffs[n-i] = (s * a->coeffs[n-i-1]) | (!s * a->coeffs[n-i]);
  a->coeffs[0] = (!s * a->coeffs[0]);
}


void poly_R2_inv_v2(poly *r, const poly *a)
{
  /* Schroeppel--Orman--O'Malley--Spatscheck
   * "Almost Inverse" algorithm as described
   * by Silverman in NTRU Tech Report #14 */
  // with several modifications to make it run in constant-time

  //Take care N should be the order of the group for cyclic and half the order for dihedral
  int i, j;
  int k = 0;
  uint16_t degf = N-1;
  uint16_t degg = N;
  int sign, t, swap;
  int done = 0;
  poly b, f, g;
  poly *c = r; // save some stack space
  poly *temp_r = &f;

  /* b(X) := 1 */
  for(i=1; i<N+1; i++)
    b.coeffs[i] = 0;
  b.coeffs[0] = 1;

  /* c(X) := 0 */
  for(i=0; i<N+1; i++)
    c->coeffs[i] = 0;

  /* f(X) := a(X) */
  for(i=0; i<N; i++)
    f.coeffs[i] = a->coeffs[i] & 1;
  f.coeffs[N]=0;

  /* g(X) := 1 + X^{N} */ //over R2
  for(i=0; i<N; i++)
    g.coeffs[i] = 0;
  g.coeffs[0]=1;
  g.coeffs[N]=1;

  for(j=0;j<2*(N)-1;j++)
  {
    sign = f.coeffs[0];
    swap = sign & !done & ((degf - degg) >> 15);
    cswappoly(&f, &g, swap,N+1);
    cswappoly(&b, c, swap,N+1);
    t = (degf ^ degg) & (-swap);
    degf ^= t;
    degg ^= t;
    POLY_R2_ADD(i, f, g, sign*(!done),N+1);
    POLY_R2_ADD(i, b, (*c), sign*(!done),N+1);
    poly_divx(&f, !done, N+1);
    poly_mulx(c, !done, N+1);
    degf -= !done;
    k += !done;
    done = 1 - (((uint16_t)(-degf)) >> 15);
 }
}


