#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NNUMS 32
#define NC 9
#define NRANGES 64

typedef enum {
    none = 0,
    seed,
    soil,
    fertilizer,
    water,
    light,
    temperature,
    humidity,
    location
} C;

typedef struct {
    int64_t d;
    int64_t s;
    int64_t n;
} R;


static int parse_numbers(int i, int nchars, const char line[nchars], int inums, int64_t out_nums[NNUMS]) {
    int nnums = 0;
    int64_t num = -1;

    for (; i < nchars; ++i) {
        if (isdigit(line[i])) num = num < 0 ? (line[i] - '0') : num * 10 + (line[i] - '0');
        else {
            if (num >= 0) {
                assert(inums + nnums < NNUMS);
                out_nums[inums + nnums++] = num;

                num = -1;
            }
        }
    }

    return nnums;
}

static void parse_src_to_dst(char *line, C *src, C *dst) {
    char *lasts;

    *src  = none;
    *dst = none;

    for (char *map = strtok_r(line, "- \n", &lasts);
         map != NULL;
         map = strtok_r(NULL, "- \n", &lasts)) {

        C c = none;

        if (strcmp(map, "to") == 0) continue;
        if (strcmp(map, "map:") == 0) continue;

        if (strcmp(map, "seed") == 0) c = seed;
        if (strcmp(map, "soil") == 0) c = soil;
        if (strcmp(map, "fertilizer") == 0) c = fertilizer;
        if (strcmp(map, "water") == 0) c = water;
        if (strcmp(map, "light") == 0) c = light;
        if (strcmp(map, "temperature") == 0) c = temperature;
        if (strcmp(map, "humidity") == 0) c = humidity;
        if (strcmp(map, "location") == 0) c = location;

        assert(c != none);

        if (*src == none)      *src  = c;
        else if (*dst == none) *dst = c;
    }
}

int main(void) {
    char *line = NULL;
    size_t ngetline;
    int nchars;

    int64_t seeds[NNUMS];
    int nseeds = 0;

    while (nseeds == 0 && (nchars = (int)getline(&line, &ngetline, stdin)) > 0)
        // count zero char to fully parse last digit on line
        nseeds = parse_numbers(0, nchars + 1, line, 0, seeds);

    R map[NC][NC][NRANGES] = {0};
    int nmap[NC][NC] = {0};

    while ((nchars = (int)getline(&line, &ngetline, stdin)) > 0) {
        if (line[0] == '\n') continue;

        C src  = none;
        C dst = none;

        parse_src_to_dst(line, &src, &dst);

        assert(src != none && dst != none);

        int nranges = 0;

        while ((nchars = (int)getline(&line, &ngetline, stdin)) > 0) {
            // printf("line: '%s'\n", line);

            if (line[0] == '\n') break;

            int64_t nums[NNUMS];

            // count zero char to fully parse last digit on line
            int nnums = parse_numbers(0, nchars + 1, line, 0, nums);

            assert(nnums == 3);

            assert(nranges < NRANGES);

            map[src][dst][nranges++] = (R){
                .d = nums[0],
                .s = nums[1],
                .n = nums[2]};
        }

        nmap[src][dst] = nranges;
    }

    int64_t lowest_location = -1;

    for (int si = 0; si < nseeds; ++si) {
        int64_t s = seeds[si];

        for (int src = seed; src < NC - 1; ++src) {
            R *m = map[src][src + 1];

            for (int i = 0; i < nmap[src][src + 1]; ++i) {
                if (s >= m[i].s &&
                    s < (m[i].s + m[i].n)) {

                    s -= m[i].s - m[i].d;
                    break;
                }
            }
        }

        if (lowest_location < 0 || s < lowest_location)
            lowest_location = s;
    }

    free(line);

    printf("%lld\n", lowest_location);

    return 0;
}
