
CC=/usr/bin/cc
LDFLAGS=-lcrypto
CFLAGS = -DCRYPTO_NAMESPACE\(s\)=ntru_\#\#s

SOURCES = cmov.c crypto_sort_int32.c fips202.c   pack3.c packq.c ppke.c poly.c poly_mod.c poly_r2_inv.c poly_rq_mul.c  main.c rng.c sample.c sample_iid.c helper.c ccacrypto.c cpucycles.c
HEADERS = api_bytes.h api.h cmov.h crypto_hash_sha3256.h crypto_sort_int32.h ppke.h fips202.h  params.h poly.h rng.h sample.h helper.h ccacrypto.h cpucycles.h

main: $(HEADERS) $(SOURCES)
	$(CC) -DCRYPTO_NAMESPACE\(s\)=ntru_\#\#s -o $@ $(SOURCES) $(LDFLAGS)

.PHONY: clean

clean:
	-rm main
