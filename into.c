/*
 * into.c: Description
 *
 * Copyright: (c) 2018 Jacco van Schaik (jacco@jaccovanschaik.net)
 * Created:   2018-11-18
 * Version:   $Id: into.c 1 2018-11-18 18:14:20Z jacco $
 *
 * This software is distributed under the terms of the MIT license. See
 * http://www.opensource.org/licenses/mit-license.php for details.
 */

#include <libgen.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void Usage(FILE *fp, char *argv0, int exit_code, const char *msg, ...)
{
    if (msg) {
        va_list ap;

        va_start(ap, msg);
        vfprintf(fp, msg, ap);
        va_end(ap);
    }

    fprintf(fp, "Usage: %s <output_file>\n", basename(argv0));
    fprintf(fp, "Writes input on its stdin to <output_file>, but only if it is different.\n");

    exit(exit_code);
}

int main(int argc, char *argv[])
{
    if (argc == 1) {
        Usage(stderr, argv[0], 0, NULL);
    }

    int debug = 0, must_write = 0;

    FILE *fp;
    struct stat statbuf;
    int c;

    int input_size = 0;
    int input_used = 0;
    char *input_data = NULL;

    while ((c = fgetc(stdin)) != EOF) {
        if (input_used == input_size) {
            input_size = (input_size == 0) ? 1024 : input_size * 2;

            input_data = realloc(input_data, input_size);
        }

        input_data[input_used++] = c;
    }

    if ((fp = fopen(argv[1], "r")) == NULL) {
        if (errno = ENOENT) {
            must_write = 1;
        }
        else {
            perror(argv[1]);
            return 1;
        }
    }
    else if (fstat(fileno(fp), &statbuf) != 0) {
        perror(argv[1]);
        return 1;
    }
    else if (statbuf.st_size != input_size) {
        must_write = 1;
    }
    else {
        for (int i = 0; i < input_size; i++) {
            c = fgetc(fp);

            if (c == EOF) {
                fprintf(stderr, "Unexpected end of file on %s after %d bytes.\n", argv[1], i);
                exit(1);
            }
            else if (c != input_data[i]) {
                must_write = 1;
                break;
            }
        }

        fclose(fp);
    }

    if (must_write) {
        if ((fp = fopen(argv[1], "w")) == NULL) {
            perror(argv[1]);
            return 1;
        }
        else {
            fwrite(input_data, input_used, 1, fp);
        }
    }
}
