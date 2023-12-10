#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define NHANDS 1024

typedef enum {
    high = 0,
    one_pair = 1,
    two_pairs = 2,
    three = 3,
    full_house = 4,
    four  = 5,
    five  = 6,
} type;

typedef struct {
    type t;
    int bid;
    int val;
} hand;

static int compare_hands(const hand *ha, const hand *hb) {
    int diff = (int)(ha->t) - (int)(hb->t);

    if (diff == 0) diff = ha->val - hb->val;

    return diff;
}

static type find_best_type(const char cs[5]) {
    type t = high;

    bool seen[5] = {0};

    for (int i = 0; i < 5; ++i) {
        if (seen[i]) continue;

        int nc = 0;

        for (int j = 0; j < 5; ++j) {
            if (cs[j] == cs[i]) {
                ++nc;
                seen[j] = true;
            }
        }

        if      (nc == 5)                  t = five;
        else if (nc == 4 && t != five)     t = four;
        else if (nc == 3 && t == one_pair) t = full_house;
        else if (nc == 3)                  t = three;
        else if (nc == 2 && t == three)    t = full_house;
        else if (nc == 2 && t == one_pair) t = two_pairs;
        else if (nc == 2)                  t = one_pair;
    }

    return t;
}

static int val_from_c(char c) {
    switch (c) {
    case 'A': return 12;
    case 'K': return 11;
    case 'Q': return 10;
    case 'J': return 9;
    case 'T': return 8;
    case '9': return 7;
    case '8': return 6;
    case '7': return 5;
    case '6': return 4;
    case '5': return 3;
    case '4': return 2;
    case '3': return 1;
    case '2': return 0;
    default: assert(0 && "unknown card");
    }
}

static int val_from_hand(const char cs[5]) {
    int val = 1;

    for (int i = 0; i < 5; ++i)
        val = val * 13 + val_from_c(cs[i]);

    return val;
}

int main(void) {
    char *line = NULL;
    size_t ngetline;

    int nchars;

    hand hs[NHANDS] = {0};
    int nhs = 0;

    while ((nchars = (int)getline(&line, &ngetline, stdin)) > 0) {
        if (line[nchars - 1] == '\n') line[--nchars] = 0;
        assert(nchars > 5 + 1);

        hand h = {0};

        h.bid = (int)strtol(line + 5, NULL, 10);
        h.val = val_from_hand(line);
        h.t   = find_best_type(line);

        assert(nhs < NHANDS);
        hs[nhs++] = h;
    }

    free(line);

    qsort(hs, (size_t)nhs, sizeof(hs[0]), (int (*)(const void*, const void*))compare_hands);

    int64_t sum = 0;

    for (int i = 0; i < nhs; ++i)
        sum += (i + 1) * hs[i].bid;

    printf("%lld\n", sum);

    return 0;
}
