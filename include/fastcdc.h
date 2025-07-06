#ifndef  FASTCDC_H
#define  FASTCDC_H

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <zlib.h>
#include "uthash.h"
#include "config.h"

void compute_md5(const unsigned char* data, size_t len, unsigned char* md5_out);

// CDC
void fastCDC_init(int fastcdc_avg_size, int NC_level);
int FastCDC_without_NC(unsigned char *p, int n);
int FastCDC_with_NC(unsigned char *p, int n);

// FSC
int FSC_512(unsigned char *p, int n);
int FSC_4(unsigned char *p, int n);
int FSC_8(unsigned char *p, int n);
int FSC_16(unsigned char *p, int n);

// cloc
int align_chunk_to_enterSymbol(unsigned char* p, int n, int original_chunk_size);
int align_chunk_to_multilineEndDelimiter(unsigned char* p, int n, int original_chunk_size, int, enum LANG);

#endif