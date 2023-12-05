#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NSPELLED_DIGITS 10

static const char *spelled_digits[NSPELLED_DIGITS] = {
    "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
};

static int find_digit(int start, int nline, const char* line, int8_t *out_digit) {
    for (int i = start; i < nline; ++i) {
        if (line[i] >= '0' && line[i] <= '9') {
            *out_digit = line[i] - '0';
            return i;
        }

        for (int8_t j = 0; j < NSPELLED_DIGITS; ++j) {
            if (strnstr(line + i, spelled_digits[j], strlen(spelled_digits[j])) != NULL) {
                *out_digit = j;
                return i;
            }
        }
    }

    return nline;
}

int main(void) {
    char *line = NULL;
    size_t ngetline = 0;
    int nchars = 0;

    int64_t sum = 0;

    while ((nchars = (int)getline(&line, &ngetline, stdin)) > 0) {
        int nline = nchars - 1;
        line[nline] = '\0';

        int8_t f = -1;
        find_digit(0, nline, line, &f);

        int8_t l = -1;
        int pos = -1;

        while ((pos = find_digit(pos + 1, nline, line, &l)) < nline);

        assert(f != -1 && l != -1);

        sum += (f * 10) + l;
    }

    free(line);

    printf("%lld\n", sum);
}
