#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <ctype.h>

enum flags { ENCRYPT=1, DECRYPT, _NO_FILE=0, _FILE};

void shift(char *original_string, int shift, int flag) {

    unsigned int *new_string_tmp = malloc((strlen(original_string)-1)*sizeof(int)*2);
    char* new_string = strdup(original_string);

    int i=0;

    for(char *c = original_string; *c; c++, i++) {
        new_string_tmp[i] = (int)*c;

        if(isspace(new_string_tmp[i])) { new_string_tmp[i]=' '; continue; }

        switch(flag) {
            case ENCRYPT:
                new_string_tmp[i]+=shift;
                while(new_string_tmp[i] > 'z') new_string_tmp[i]-=26;
                break;

            case DECRYPT:
                new_string_tmp[i]-=shift;
                while(new_string_tmp[i] < 'a') new_string_tmp[i]+=26;
                break;
        }

        new_string[i] = new_string_tmp[i];

    }
    printf("\"%s\" -> \"%s\"", original_string, new_string);

    free(new_string_tmp);
    free(new_string);
}


int main(int argc, char **argv) {

    FILE *fp;
    int opt, file=0, flag_action=0;
    int _F_FLAG=0, _S_FLAG=0, _D_FLAG=0, _E_FLAG=0;

    if((argc - optind) < 1) exit(1);

    while((opt = getopt(argc, argv, "fsdeh")) != -1) {
        switch(opt) {
            case 'f':
                _F_FLAG=1;
                file=_FILE;
                break;

            case 's':
                _S_FLAG=1;
                file=_NO_FILE;
                break;

            case 'd':
                _D_FLAG=1;
                flag_action=DECRYPT;
                break;

            case 'e':
                _E_FLAG=1;
                flag_action=ENCRYPT;
                break;

            case 'h':
                puts("-s - read from standard input");
                puts("-f - read from a file");
                puts("-e - encrypt");
                puts("-d - decrypt");
                puts("-h - open this menu");
                exit(0);

            default: exit(1);
        }
    }

    if(_S_FLAG != 1 && _F_FLAG != 1) errx(1, "no input provided");
    if(_E_FLAG != 1 && _D_FLAG != 1) errx(1, "no operation provided");

    if(file) {

        fp=fopen(argv[optind+1], "r");
        if(!fp) errx(1,"error opening a file");
        
        fseek(fp, 0L, SEEK_END);
        size_t fsz = ftell(fp)-1;
        rewind(fp);

        char* buf = malloc(fsz);
        fread(buf, 1, fsz, fp);

        if(flag_action == ENCRYPT || flag_action == DECRYPT ) shift(buf, atoi(argv[optind]), flag_action);
            
        free(buf);
    }

    else if(argv[optind] == NULL || argv[optind+1] == NULL) errx(1, "shift value is not provided.");

    if(!file)
        if(flag_action == ENCRYPT || flag_action == DECRYPT ) shift(argv[optind+1], atoi(argv[optind]), flag_action);

    printf("\n");
}
