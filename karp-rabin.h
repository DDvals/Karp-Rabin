#ifndef KARP_RABIN_H_INCLUDED
#define KARP_RABIN_H_INCLUDED
#define _GNU_SOURCE
#define _MATH_DEFINES_DEFINED
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <zlib.h>
#include <gsl/gsl_rng.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include "miller-rabin/sprp32.h"
#include "klib/kseq.h"

#define NCHAR 128
#define NPRIMI 4

unsigned long int* genPrime(int, unsigned long int);
short int isPrime(unsigned long int);
unsigned long int genPrime2(unsigned long int);
unsigned long int horner(char*, int);
short int checkStr(char*, char*);
static char* read_text(char*);
unsigned int* karprabin(char*, char*, double precision);
char* sliceString(char*, int, int);
double findI(size_t, size_t, double);
double findMax(double, double);
short int checkInput(int, char**);


#endif
