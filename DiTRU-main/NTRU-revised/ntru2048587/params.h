#ifndef PARAMS_H
#define PARAMS_H

#define NTRU
#define ORDER 587 
#define LOGQ 11
#define N 587  //for dihedral group N is half the order
#define LENGTH_OF_HASH 64
#ifndef CRYPTO_NAMESPACE
#define CRYPTO_NAMESPACE(s) s
#endif

// #ifdef NTRU_HPS
//  N  (ORDER)
// #endif

// #ifdef DiTRU
// N  (ORDER/2)
// #endif
// /* Do not modify below this line */

#define PAD32(X) ((((X) + 31)/32)*32)

#define Q (1 << LOGQ)


#define d_f 195  //weight of d_f and d_r
#define d_g ((ORDER)/3) //weight of d_g 
#define WEIGHT (d_f+d_g)
#define SEEDBYTES       32
#define PRFKEYBYTES     32
#define SHAREDKEYBYTES  32

#define SAMPLE_IID_BYTES  (ORDER)
#define SAMPLE_FT_BYTES   ((30*(ORDER)+7)/8)
#define SAMPLE_FG_BYTES   (SAMPLE_FT_BYTES+SAMPLE_FT_BYTES)
#define SAMPLE_RM_BYTES   (SAMPLE_IID_BYTES+SAMPLE_FT_BYTES)

#define PACK_DEG (ORDER)
#define PACK_TRINARY_BYTES    ((PACK_DEG+4)/5)

#define PPKE_PUBLICKEYBYTES ((LOGQ*PACK_DEG+7)/8)
#define PPKE_CIPHERTEXTBYTES ((LOGQ*PACK_DEG+7)/8)
#define PPKE_SECRETKEYBYTES (PACK_TRINARY_BYTES)
#define PPKE_MESSAGEBYTES (PACK_TRINARY_BYTES)



#define PUBLICKEYBYTES  (PPKE_PUBLICKEYBYTES)
#define SECRETKEYBYTES  (PPKE_SECRETKEYBYTES)
#define CIPHERTEXTBYTES (PPKE_CIPHERTEXTBYTES)
#endif
