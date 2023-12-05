#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char *line = NULL;
    size_t ngetline = 0;

    int nchars = 0;

    while ((nchars = (int)getline(&line, &ngetline, stdin)) > 0) {
        if (line[nchars - 1] == '\n') line[--nchars] = 0;

        printf("line: %s\n", line);
    }

    free(line);

    return 0;
}
