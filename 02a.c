#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NSETS 10

typedef struct {
    int nr;
    int ng;
    int nb;
} S;

static int parse_sets(char *s, int nsets, S sets[]) {
    char *sets_lasts;
    int i = 0;

    for (char *set = strtok_r(s, ",", &sets_lasts);
         set != NULL;
         set = strtok_r(NULL, ",", &sets_lasts)) {

        char *color;
        int ncubes = (int)strtol(set, &color, 10);
        assert(color != NULL);
        ++color; // skip whitespace

        S s = {0};

        if (strcmp(color, "red") == 0)   s.nr = ncubes;
        if (strcmp(color, "green") == 0) s.ng = ncubes;
        if (strcmp(color, "blue") == 0)  s.nb = ncubes;

        sets[i++] = s;
        assert(i < nsets);
    }

    return i;
}

int main(void) {
    char *line = NULL;
    size_t ngetline = 0;
    int nchars = 0;

    int64_t sum = 0;

    while ((nchars = (int)getline(&line, &ngetline, stdin)) > 0) {
        int nline = nchars - 1;
        line[nline] = line[nline] == '\n' ? '\0' : line[nline];

        int game_id = (int)strtol(line +  5, NULL, 10); // assume 'Game:'

        char * reveals = strstr(line, ":");
        assert(reveals != NULL);

        char *reveals_lasts;

        // only 12 red cubes, 13 green cubes, and 14 blue cubes
        bool any_impossible_set = false;

        for (char *string_sets = strtok_r(reveals + 1, ";", &reveals_lasts);
             string_sets != NULL;
             string_sets = strtok_r(NULL, ";", &reveals_lasts)) {

            S sets[NSETS];
            int nsets = parse_sets(string_sets, NSETS, sets);

            for (int j = 0; j < nsets; ++j) {
                if (sets[j].nr > 12 || sets[j].ng > 13 || sets[j].nb > 14) {
                    any_impossible_set = true;
                }
            }
        }

        if (!any_impossible_set) sum += game_id;
    }

    free(line);

    printf("%lld\n", sum);

    return 0;
}
