#include "karp-rabin.h"

KSEQ_INIT(gzFile, gzread);

unsigned short int checkType = 2; //does not use error check and uses only one prime number

int main(int argc, char** argv)
{
    char* filename = NULL;
    char* txt = NULL;
    char* pattern = NULL;
    unsigned int* occurency = NULL;
    double precision = -1;

    if(!checkInput(argc, argv))
    {
        printf("Input error\n");
        exit(1);
    }

    srand(time(NULL));

    filename = calloc(sizeof(char), strlen(argv[1]));
    assert(filename != 0);
    pattern =  calloc(sizeof(char), strlen(argv[2]));
    assert(pattern != 0);
    strcpy(filename, argv[1]);
    strcpy(pattern, argv[2]);
    precision= atof(argv[3]);
    txt = read_text(filename);

    printf("%s %s %e\n\n", filename, pattern, precision);

    occurency = karprabin(txt, pattern, precision);

    if(occurency[0] == 0)
        printf("No occurency found\n");

    else
    {
        printf("Found %d occurency\n", occurency[0]);
        printf("Occurency: ");

        for(int i = 1; i <= occurency[0]; i++)
            printf("%d ", occurency[i]);
    }

    printf("\n");

    free(filename);
    free(pattern);
    free(occurency);
    free(txt);

    return 0;
}

unsigned int* karprabin(char* txt, char* pattern, double precision)
{
    unsigned long int hashPattern, hashText;
    unsigned int* occurency = malloc(20 * sizeof(int)); //We start with 20 occurency, if needed we will add space
    int index = 1; //First value is the occurency number
    char* tmp;
    size_t lenText, lenPattern;
    unsigned long int ptr = 0;
    unsigned int occurencyIdx = 0;
    int mul = 0; //occurency mul
    double primeLen;
    unsigned long int* prime = NULL;
    unsigned long int singlePrime = 1;

    assert(occurency != 0);

    lenText = strlen(txt) - 1;
    lenPattern = strlen(pattern) - 1;

    occurency[0] = 0;

    if(lenPattern > lenText) // The pattern is longer than text, we return 0 occ.
        return occurency;


    //Find limit superior for prime numbers generation
    primeLen = findI(lenPattern, lenText, precision);

    //We have to generate k primes to use k fingerprints
    if(checkType == 1)
    {
        prime = genPrime(NPRIMI, primeLen);
    }
    else //We use only one prime if precision is high or false positives check is needed
    {
        singlePrime = genPrime2(primeLen);
        hashPattern = horner(pattern, singlePrime);
    }


    while(ptr <= lenText - lenPattern)
    {
        tmp = sliceString(txt, ptr, ptr + lenPattern);

        //Horner with k primes
        if(checkType == 1)
        {
            short int ok = 1;

            for(int h = 0; h < NPRIMI; ++h)
                ok = ok & (horner(pattern, prime[h]) == horner(tmp, prime[h]));

            if(ok)
            {
                occurency[0]++;
                occurencyIdx++;
                occurency[occurencyIdx] = ptr;

                if(occurencyIdx >= mul * 20)
                {
                    mul++;
                    occurency = realloc(occurency, 20 * mul * sizeof(int));
                    assert(occurency != 0);
                }
            }
        }
        else
        {
            hashText = horner(tmp, singlePrime);

            if(hashPattern == hashText)
            {
                if(checkType == 0 && !checkStr(tmp, pattern))
                    continue; //if is a false positive we do not save this occurency

                occurency[0]++;
                occurencyIdx++;
                occurency[occurencyIdx] = ptr;

                if(occurencyIdx >= mul * 20)
                {
                    mul++;
                    occurency = realloc(occurency, 20 * mul * sizeof(int));
                    assert(occurency != 0);

                }
            }
        }
        free(tmp);
        ++ptr;
    }

    if(prime != 0)
        free(prime);

    return occurency;
}



double findI(size_t n, size_t m, double precision)
{
    double max, p1, p2;

    p1 = 2.53 / m;
    p2 = 1.0 / n;

    if(precision == -1)
    {
        checkType = 0;
        return n * n * m; // Error check on
    }

    max = findMax(p1, p2);

    if(precision >= max)
        return n * n * m; // n^2 * m


    if(precision >= p1 && precision < p2)
        return n * m * m; // n * m^2


    if(precision >= p2 && precision < p1)
        return n * n * m;


    double i = n*n*m;
    double tmp = pow((n*m / log(n*m + 1)) / (i / log(i+1)), NPRIMI); //(p(nm)/p(i))^k

    if(precision >= tmp)
    {
        checkType = 1;
        return i;
    }

    //Results check to increase precision
    checkType = 0;
    return i;
}

//Find max
double findMax(double a, double b)
{
    if(a > b)
        return a;

    return b;
}

//Cut a string from start to stop
char* sliceString(char* string, int start, int stop)
{
    int len = stop - start +1 ;
    char* ret = malloc(len * sizeof(char));
    int i = start;
    int j = 0;

    assert(ret != 0);

    for(j = 0; i <= stop; i++, j++)
        ret[j] = string[i];

    ret[j] = '\0';

    return ret;
}

//Checks s1 == s2
short int checkStr(char* s1, char* s2)
{
    if(strlen(s1) != strlen(s2))
        return 0;

    size_t len = strlen(s1);

    for(int i = 0; i < len; ++i)
        if(s1[i] != s2[i])
            return 0;

    return 1;
}

// Generates n prime numbers from 1 and max
unsigned long int * genPrime(int n, unsigned long int max)
{
    unsigned long int check = 0;
    unsigned long int *prime = malloc(n * sizeof(unsigned long int));

    assert(prime != 0);

    for(int i = 0; i < n; i++)
        prime[i] = genPrime2(max);

    return prime;
}

//Generates one prime number
unsigned long int genPrime2(unsigned long int max)
{
    unsigned long int check = 1;

    do
    {
        check = (rand() % max) + 1;
    }
    while(check % 2 == 0 || !isPrime(check));

    return check;
}

// Check if n is prime (better method needed :( )
short int isPrime(unsigned long int n)
{
    for(unsigned long int i = 3; i <= n/2; i = i + 2)
        if(n % i == 0)
            return 0;

    return 1;
}

//String hash with Horner rule
unsigned long int horner(char* hashme, int modulo)
{
    unsigned int hash = 0;
    int len = strlen(hashme);

    for(int i = 0; i < len; i++)
        hash = (hash * NCHAR + hashme[i]) % modulo;

    return hash;
}

static char* read_text(char* filename)
{
    gzFile fp;
    kseq_t *seq;
    fp = gzopen(filename, "r");
    //assert(fp != NULL && "Could not open fasta file\n");
    if(fp == NULL)
    {
        printf("Could not open fasta file\n");
        exit(1);
    }

    seq = kseq_init(fp);
    int res = kseq_read(seq);
    assert(res >= 0);
    gzclose(fp);
    return seq->seq.s;
    // kseq_destroy(seq);
}

short int checkInput(int argc, char** argv)
{
    if(argc < 4)
        return 0;

    double tmp = atof(argv[3]);

    if(tmp == 0.0 || tmp < -1) //Precision is NaN or is not correct
        return 0;

    return 1;
}
