/* Name: Chenyao Lou
 * ID: 2016011343
 */

#include "cachelab.h"
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

#define ADDR unsigned int

int miss_count = 0, hit_count = 0, eviction_count = 0;
int s, E, b;
int verbose = 0;
char *tracefile;

typedef struct {
    int valid;
    int tag;
    int last_visit;
}CacheLine;

typedef struct {
    CacheLine *lines;
}CacheSet;

CacheSet *sets;

void print_usage(const char *exe) {
    printf("Usage: %s [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n", exe);
    exit(-1);
}

int get_set(ADDR addr) {
    return (addr >> b) & ((1ll << s) - 1);
}

int get_tag(ADDR addr) {
    return addr >> (b + s);
}

void cache(ADDR addr, int size, int *hit, int *miss, int *eviction) {
    // we will ignore size by assuming that the access is aligned properly
    static int time = 0;
    time++;

    int set = get_set(addr);
    int tag = get_tag(addr);

    // whether hit?
    for (int i = 0; i < E; i++) if (sets[set].lines[i].valid) {
        if (sets[set].lines[i].tag == tag) {
            (*hit) += 1;
            sets[set].lines[i].last_visit = time;
            return;
        }
    }

    // find ans empty cache line or least used line
    (*miss) += 1;
    int mi = 1e9, which = -1;
    for (int i = 0; i < E; i++) {
        if (sets[set].lines[i].valid == 0) {
            sets[set].lines[i].valid = 1;
            sets[set].lines[i].tag = tag;
            sets[set].lines[i].last_visit = time;
            return;
        } else {
            if (sets[set].lines[i].last_visit < mi) {
                mi = sets[set].lines[i].last_visit;
                which = i;
            }
        }
    }

    // evict existing line
    (*eviction) += 1;
    sets[set].lines[which].valid = 1;
    sets[set].lines[which].tag = tag;
    sets[set].lines[which].last_visit = time;
}

int main(int argc, char *argv[]) {
    int opt;

    while((opt = getopt(argc, argv, "vs:E:b:t:")) != -1) {
        switch(opt) {
            case 'v':
                verbose = 1;
                break;
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);;
                break;
            case 'b':
                b = atoi(optarg);;
                break;
            case 't':
                tracefile = optarg;
                break;
            default: /* -h or unknown option*/
                print_usage(argv[0]);
        }
    }
    if (!tracefile || s <= 0 || b <= 0 || E <= 0) {
        print_usage(argv[0]);
    }

    FILE *tf;
    tf = fopen(tracefile, "r");
    if (!tf) {
        print_usage(argv[0]);
    }

    // allocating cache sets and cache lines.
    sets = malloc(sizeof(CacheSet) * (1<<s));
    for (int i = 0; i < (1<<s); i++) {
        sets[i].lines = malloc(sizeof(CacheLine) * E);
        for (int j = 0; j < E; j++) {
            sets[i].lines[j].valid = 0;
        }
    }

    char ops[20];
    ADDR addr;
    int size;
    int hit, miss, eviction, ignore;
    while (fscanf(tf, "%s %x,%d", ops, &addr, &size) == 3) {
        hit = 0;
        miss = 0;
        eviction = 0;
        ignore = 0;

        switch (ops[0]) {
            case 'I':
                ignore = 1;
                break;
            case 'L':
                cache(addr, size, &hit, &miss, &eviction);
                break;
            case 'M':
                cache(addr, size, &hit, &miss, &eviction);
                cache(addr, size, &hit, &miss, &eviction);
                break;
            case 'S':
                cache(addr, size, &hit, &miss, &eviction);
                break;
        }

        hit_count += hit;
        miss_count += miss;
        eviction_count += eviction;

        // print details
        if (verbose) {
            printf("%s %x,%d set=%d, tag = %d ", ops, addr, size, get_set(addr), get_tag(addr));
            if (miss) printf("miss ");
            if (eviction) printf("eviction ");
            if (hit == 1) printf("hit ");
            if (hit == 2) printf("hit hit ");
            if (ignore) printf("ignore ");
            printf("\n");
        }
    }

    printSummary(hit_count, miss_count, eviction_count);
    return 0;
}
