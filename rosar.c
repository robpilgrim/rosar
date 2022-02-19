#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>

enum flags { ENCRYPT=1, DECRYPT, _NO_FILE=0, _FILE};

void shift(char *original_string, int shift, int flag) {
    unsigned char *new_string = strdup(original_string);
    shift %= 26;
    for (unsigned char *c = original_string, *p = new_string; *c; c++, p++) {
        *p = (unsigned char)*c;

        if(isspace(*p)) continue;
        switch(flag) {
            case ENCRYPT:
                *p += shift;
                 while (*p > 'z') *p -= 26;                              
                 break;
            case DECRYPT:
                *p -= shift;
                while (*p < 'a') *p += 26;
                break;
        }
    }                                                   
    printf("\"%s\" -> \"%s\"", original_string, new_string);                  
    free(new_string);
}
 


int main(int argc, char **argv) {

    FILE *fp;
    int opt, file=0, flag_action=0;
    int _D_FLAG=-1, _E_FLAG=-1;
    char* FLAG_VALUE, *INPUT_VALUE;

    if((argc - optind) < 1) exit(1);

    while((opt = getopt(argc, argv, "f:s:d:e:h")) != -1) {
        switch(opt) {
            case 'f':
                INPUT_VALUE=optarg;
                file=_FILE;
                break;

            case 's':
                INPUT_VALUE=optarg;
                file=_NO_FILE;
                break;

            case 'd':
                FLAG_VALUE=optarg;
                flag_action=DECRYPT;
                break;

            case 'e':
                FLAG_VALUE=optarg;
                flag_action=ENCRYPT;
                break;

            case 'h':
                puts("-s - read from a string");
                puts("-f - read from a file");
                puts("-e - encrypt");
                puts("-d - decrypt");
                puts("-h - open this menu");
                exit(0);

            default: exit(1);
        }
    }

    if(file) {

        fp=fopen(INPUT_VALUE, "r");
        if(!fp) errx(1,"error opening a file");
        
        fseek(fp, 0L, SEEK_END);
        size_t fsz = ftell(fp)-1;
        rewind(fp);

        char* buf = malloc(fsz);
        fread(buf, 1, fsz, fp);

        if(flag_action == ENCRYPT || flag_action == DECRYPT ) shift(buf, strtoimax(FLAG_VALUE, NULL, 0), flag_action);
            
        free(buf);
    }


    else if(!file)
        if(flag_action == ENCRYPT || flag_action == DECRYPT ) shift(INPUT_VALUE, strtoimax(FLAG_VALUE, NULL, 0), flag_action);

    printf("\n");
}
