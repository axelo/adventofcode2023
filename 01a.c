#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static int8_t find_first_last_digit_as_int(int len, const char* line) {
    int8_t f = -1;
    int8_t l = -1;

    for (int i = 0; i < len; ++i) {
        if (line[i] >= '0' && line[i] <= '9') {
            l = line[i] - '0';
            if (f < 0) f = l;
        }
    }

    assert(f >= 0 && l >= 0);

    return (f * 10) + l;
}

int main(void) {
    char *line = NULL;
    size_t ngetline = 0;
    int nchars = 0;

    int64_t sum = 0;

    while ((nchars = (int)getline(&line, &ngetline, stdin)) > 0) {
        int nline = nchars - 1;
        line[nline] = '\0';

        sum += find_first_last_digit_as_int(nline, line);
    }

    free(line);

    printf("%lld\n", sum);
}
