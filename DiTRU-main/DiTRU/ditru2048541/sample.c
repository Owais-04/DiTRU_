#include "sample.h"
#include <stdio.h>
#include <string.h>
#include "crypto_hash_sha3256.h"


void sample_fixed_type(poly *r, const unsigned char u[SAMPLE_FT_BYTES],int d)
{
  // Assumes NTRU_SAMPLE_FT_BYTES = ceil(30*(n-1)/8)

  int32_t s[ORDER];
  int i;

  // Use 30 bits of u per word
  for (i = 0; i < (ORDER)/4; i++)
  {
    s[4*i+0] =                              (u[15*i+ 0] << 2) + (u[15*i+ 1] << 10) + (u[15*i+ 2] << 18) + ((uint32_t) u[15*i+ 3] << 26);
    s[4*i+1] = ((u[15*i+ 3] & 0xc0) >> 4) + (u[15*i+ 4] << 4) + (u[15*i+ 5] << 12) + (u[15*i+ 6] << 20) + ((uint32_t) u[15*i+ 7] << 28);
    s[4*i+2] = ((u[15*i+ 7] & 0xf0) >> 2) + (u[15*i+ 8] << 6) + (u[15*i+ 9] << 14) + (u[15*i+10] << 22) + ((uint32_t) u[15*i+11] << 30);
    s[4*i+3] =  (u[15*i+11] & 0xfc)       + (u[15*i+12] << 8) + (u[15*i+13] << 16) + ((uint32_t) u[15*i+14] << 24);
  }
  int dif = ORDER-(ORDER/4)*4;
  //printf("dif %d \n", dif);
  i = (ORDER)/4;
if (dif ==1)// (N) = 1 mod 4
{
  s[4*i+0] =                              (u[15*i+ 0] << 2) + (u[15*i+ 1] << 10) + (u[15*i+ 2] << 18) + ((uint32_t) u[15*i+ 3] << 26);
}

else if (dif ==2)// (N) = 2 mod 4
{
  s[4*i+0] =                              (u[15*i+ 0] << 2) + (u[15*i+ 1] << 10) + (u[15*i+ 2] << 18) + ((uint32_t) u[15*i+ 3] << 26);
  s[4*i+1] = ((u[15*i+ 3] & 0xc0) >> 4) + (u[15*i+ 4] << 4) + (u[15*i+ 5] << 12) + (u[15*i+ 6] << 20) + ((uint32_t) u[15*i+ 7] << 28);
}

else// (N) = 3 mod 4

{ 
  s[4*i+0] =                              (u[15*i+ 0] << 2) + (u[15*i+ 1] << 10) + (u[15*i+ 2] << 18) + ((uint32_t) u[15*i+ 3] << 26);
  s[4*i+1] = ((u[15*i+ 3] & 0xc0) >> 4) + (u[15*i+ 4] << 4) + (u[15*i+ 5] << 12) + (u[15*i+ 6] << 20) + ((uint32_t) u[15*i+ 7] << 28);
  s[4*i+2] = ((u[15*i+ 7] & 0xf0) >> 2) + (u[15*i+ 8] << 6) + (u[15*i+ 9] << 14) + (u[15*i+10] << 22) + ((uint32_t) u[15*i+11] << 30);
}
  

  
  for (i = 0; i<d; i++) {s[i] |=  1;}

  for (i = d; i<2*d; i++) {s[i] |=  2;}
 
  crypto_sort_int32(s,ORDER);

  for(i=0; i<ORDER; i++)
    r->coeffs[i] = ((uint16_t) (s[i] & 3));


  //r->coeffs[NTRU_N-1] = 0;
}



void sample_r(poly *r, const poly *h, const poly *m){
  unsigned char r_seed[LENGTH_OF_HASH*2];

  
  sha3_512(r_seed, (unsigned char *)(m), ORDER);
  sha3_512(r_seed+LENGTH_OF_HASH, (unsigned char *)(h), ORDER);
  // generate uniformbytes of NTRU_SAMPLE_FT_BYTES by hashing the r_seed
  int filled=0;
  unsigned char uniformbytes[SAMPLE_FT_BYTES];
  sha3_512(r_seed, r_seed, LENGTH_OF_HASH*2);
  while(filled<SAMPLE_FT_BYTES)
  {    
    int rem=SAMPLE_FT_BYTES-filled;
    if(rem>=LENGTH_OF_HASH){
      memcpy(uniformbytes+filled, r_seed, LENGTH_OF_HASH);
    }
    else{
      memcpy(uniformbytes+filled, r_seed, rem);
    }
    filled+=LENGTH_OF_HASH;
    sha3_512(r_seed, r_seed, LENGTH_OF_HASH);
  }
  sample_fixed_type(r, uniformbytes, d_f); //dr=df
}


void sample_mask(poly *mask, unsigned char t_seed[LENGTH_OF_HASH]){

  //printf("inside sample mask \n");
  unsigned char uniformbytes[SAMPLE_IID_BYTES];
  int filled=0;
  sha3_512(t_seed, t_seed, LENGTH_OF_HASH);
  while(filled<SAMPLE_IID_BYTES){    
    int rem=SAMPLE_IID_BYTES-filled;
    if(rem>=LENGTH_OF_HASH){
      memcpy(uniformbytes+filled, t_seed, LENGTH_OF_HASH);
    }
    else{
      memcpy(uniformbytes+filled, t_seed, rem);
    }
    filled+=LENGTH_OF_HASH;
    sha3_512(t_seed, t_seed, LENGTH_OF_HASH);
  }
  sample_iid(mask, uniformbytes);
  
}
