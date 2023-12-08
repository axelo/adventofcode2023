#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static int64_t parse_number(int nchars, const char line[nchars]) {
    int64_t num = 0;

    for (int i = 0; i < nchars; ++i)
        if (isdigit(line[i])) num = num * 10 + (line[i] - '0');

    return num;
}

int main(void) {
    char *line = NULL;
    size_t ngetline;

    int nchars = (int)getline(&line, &ngetline, stdin);
    int64_t time = parse_number(nchars, line);

    nchars = (int)getline(&line, &ngetline, stdin);
    int64_t dist = parse_number(nchars, line);

    int64_t nways = 0;

    for (int64_t v = 1; v <= time; ++v) {
        int64_t d = v * (time - v);

        if (d > dist) ++nways;
    }

    free(line);

    printf("%lld\n", nways);

    return 0;
}
