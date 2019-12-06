#include <stdio.h>
#include <string.h>

#define stdin_no 0

int main(int argc, char **argv)
{

        char *ptr;

        for (ptr = argv[0] + strlen(argv[0]) - 1; 
             strchr("ascitohex-", *ptr) && ptr != *argv; 
             ptr--) 
                ;               /* empty */

        ptr++;

        if (!strncmp(ptr, "ascii-to-hex", strlen("ascii-to-hex"))) {
                char buf;
                int n;

                while ((n = read(stdin_no, &buf, 1)) == 1) 
                        printf("%02x", buf, 2);
        } else if (!strncmp(ptr, "hex-to-ascii", strlen("hex-to-ascii"))) {
                char buf[2];
                int n;

                while ((n = read(stdin_no, &buf, 2) == 2)) {
                        int byte = (int) strtol(buf, NULL, 16);
                        printf("%c", byte);
                }
        } else  {
                return -1;
        }

        return 0;
}
