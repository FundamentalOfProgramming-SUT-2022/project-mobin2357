#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>

    
const int rows = 23, cols = 79;

int Is_sub(char a[], char b[], int length_b){
    int i = 0;
    while(i < length_b){
        if(b[i] != a[i])   return 1;
        i++;
    }
    return 0;
}


bool exist(const char *filename){
    struct stat buffer;
    return stat(filename, &buffer) == 0 ? true : false;
}



void createfile(char path[]){
    int num;
    int k = 1;
    int special_case = 0;
    if(path[0] == '/'){
        num = 1;
        while(1){
            while(path[k] != '/' && path[k] != ' ' && path[k] != '\n' && path[k] != '\0'){
                k++;
            }
            if(path[k] == '/'){
                num++;
                k++;
            }
            else if(path[k] == ' ' || path[k] == '\n' || path[k] == '\0'){
                break;
            }
        }
    }
    else if(path[0] == '"'){
        k = 2;
        num = 1;
        while(1){
            while(path[k] != '/' && path[k] != '"'){
                k++;
            }
            if(path[k] == '/'){
                num++;
                k++;
            }
            else if(path[k] == '"'){
                break;
            }
        }
    }
    else   num = 1;

    if(1){
        int sizee[num];
        for(int i = 0; i < num; i++){
            sizee[i] = 0;
        }
        int j = 1;
        int n = 0;
        if(path[0] == '/'){
            while(1){
                while(path[j] != '/' && path[j] != ' ' && path[j] != '\n' && path[j] != '\0'){
                    j++;
                    sizee[n]++;
                }
                if(path[j] == '/'){
                    j++;
                    n++;
                }
                else if(path[j] == ' ' || path[j] == '\n' || path[j] == '\0'){
                    break;
                }
            }
        }
        else if(path[0] == '"' && path[1] == '/'){
            j = 2;
            while(1){
                while(path[j] != '/' && path[j] != '"'){
                    j++;
                    sizee[n]++;
                }
                if(path[j] == '/'){
                    j++;
                    n++;
                }
                else if(path[j] == '"'){
                    break;
                }
            }
        }
        else if(path[0] == '"'){
            j = 1;
            while(1){
                while(path[j] != '/' && path[j] != '"'){
                    j++;
                    sizee[n]++;
                }
                if(path[j] == '/'){
                    j++;
                    n++;
                }
                else if(path[j] == '"'){
                    break;
                }
            }
        }
        else{
            special_case = 1;
            j = 1;
            while(1){
                while(path[j] != '\0'){
                    j++;
                    sizee[n]++;
                }
                if(path[j] == '\0'){
                    break;
                }
            }
        }
        //creating the substrings
        int ss = 0;
        for(int i = 0; i < num; i++){
            ss += sizee[i] + 1;
            char newpath[ss];
            for(int j = 0; j < ss - 1; j++){
                if(path[0] == '/') newpath[j] = path[j + 1];
                else if(path[0] == '"' && path[1] == '/') newpath[j] = path[j + 2];
                else if(path[0] == '"') newpath[j] = path[j + 1];
                else  newpath[j] = path[j];
            }
            newpath[ss - 1] = '\0';
            if(special_case == 1)   newpath[ss - 1] = path[ss - 1];

            if(i < num - 1){
                mkdir(newpath, 0777);
            }
            else if(i == num - 1){
                if(exist(newpath))   printf("The file already exist");
                else{
                    FILE *fff;
                    fff = fopen(newpath, "a");
                    fclose(fff);
                }
            }
        }
    }
}

void Insert(char filepath[], char str[], int pos1, int pos2, int giome){
    if(filepath[0] == '"' && filepath[1] != '/'){
        int i = 0;
        while(filepath[i + 1] != '"'){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '"'){
        int i = 0;
        while(filepath[i + 2] != '"'){
            filepath[i] = filepath[i + 2];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '/'){
        int i = 0;
        while(filepath[i + 1] != '\0'){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    //// pathfile corrected
    if(giome == 1){
        int h = 0;
        while(str[h] != '\0'){
            str[h] = str[h + 1];
            h++;
        }
    }
    //// str corrected
    if(!exist(filepath))    printf("The file is not found");
    else{
        FILE *fff;
        fff = fopen(filepath, "r+");
        int size_file;
        int line = 1;
        int i = 0;
        char c;
        while(line < pos1){
            fseek(fff, i, SEEK_SET);
            if(fgetc(fff) == '\n'){
                line++;
                //i++;
            }
            i++;
        }
        i += pos2;
        //calculating the number of lines
        int l = 1;
        int u = 0;
        while(1){
            fseek(fff, u, SEEK_SET);
            char t = fgetc(fff);
            if(t == EOF)   break;
            if(t == '\n'){
                l++;
                //u++;
            }
            u++;
        }
        fseek(fff, 0, SEEK_END);
        size_file = ftell(fff);
        int save_size = size_file - i;

        fseek(fff, i, SEEK_SET);
        char *preserve;
        preserve = (char*) malloc(sizeof(char) * save_size);
        size_t result = fread(preserve, 1, save_size, fff);
        preserve[save_size] = '\0';

        fseek(fff, i, SEEK_SET);
        fputs(str, fff);
        fputs(preserve, fff);
        fclose(fff);
    }
}

void cat(char filepath[]){
    if(filepath[0] == '"' && filepath[1] != '/'){
        int i = 0;
        while(filepath[i + 1] != '"'){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '"'){
        int i = 0;
        while(filepath[i + 2] != '"'){
            filepath[i] = filepath[i + 2];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '/'){
        int i = 0;
        while(filepath[i + 1] != '\0'){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    FILE *fff;
    int c;
    fff = fopen(filepath, "r");
    while(1){
        c = fgetc(fff);
        if(feof(fff))   break;
        printf("%c", c);
    }
    fclose(fff);
}

void rremove(char filepath[], int pos1, int pos2, int size, char f){
    if(filepath[0] == '"' && filepath[1] != '/'){
        int i = 0;
        while(filepath[i + 1] != '"'){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '"'){
        int i = 0;
        while(filepath[i + 2] != '"'){
            filepath[i] = filepath[i + 2];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '/'){
        int i = 0;
        while(filepath[i + 1] != '\0'){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    //// pathfile corrected

    if(!exist(filepath))    printf("The file is not found");
    else{
        FILE *fff;
        fff = fopen(filepath, "r+");
        int line = 1;
        int i = 0;
        while(line < pos1){
            fseek(fff, i, SEEK_SET);
            if(fgetc(fff) == '\n'){
                line++;
                //i++;
            }
        i++;
        }
        i += pos2;
        fseek(fff, 0, SEEK_END);
        int size_file = ftell(fff);
        //calculating the number of all the lines
        int l = 1;
        int u = 0;
        while(1){
            fseek(fff, u, SEEK_SET);
            char t = fgetc(fff);
            if(t == EOF)   break;
            if(t == '\n'){
                l++;
                //u++;
            }
            u++;
        }
        //calculating the number of lines for b case
        int li = 1;
        int ua = 0;
        while(ua < i - size){
            fseek(fff, ua, SEEK_SET);
            if(fgetc(fff) == '\n'){
                li++;
                //ua++;
            }
            ua++;
        }
        //calculating the number of lines for f case
        int lll = 1;
        int uuu = 0;
        while(uuu < i + size - 1){
            fseek(fff, uuu, SEEK_SET);
            if(fgetc(fff) == '\n'){
                lll++;
                //uuu++;
            }
            uuu++;
        }

        if(f == 'f'){
            int save_size = size_file - i - size;

            fseek(fff, 0, SEEK_SET);
            char *remain;
            remain = (char*) malloc(sizeof(char) * i);
            size_t rresult = fread(remain, 1, i, fff);
            remain[i] = '\0';

            fseek(fff, i + size, SEEK_SET);
            char *preserve;
            preserve = (char*) malloc(sizeof(char) * save_size);
            size_t result = fread(preserve, 1, save_size, fff);
            preserve[save_size] = '\0';
            fclose(fff);

            FILE *ff;
            ff = fopen(filepath, "w");
            fputs(remain, ff);
            fputs(preserve, ff);
            fclose(ff);
        }
        else if(f == 'b'){
            int save_size = size_file - i;

            fseek(fff, 0, SEEK_SET);
            char *remain;
            remain = (char*) malloc(sizeof(char) * (i - size));
            size_t rresult = fread(remain, 1, i - size, fff);
            remain[i - size] = '\0';

            fseek(fff, i, SEEK_SET);
            char *preserve;
            preserve = (char*) malloc(sizeof(char) * save_size);
            size_t result = fread(preserve, 1, save_size, fff);
            preserve[save_size] = '\0';
            fclose(fff);

            FILE *ff;
            ff = fopen(filepath, "w");
            fputs(remain, ff);
            fputs(preserve, ff);
            fclose(ff);
        }
    }
}

void coopy(int x0, int y0, int x, int y, char filepath[], char clip[]){
    int p0 = 0, p = 0;
    int l0 = 0, l = 0;
    FILE *ffd;
    ffd = fopen(filepath, "r");
    fseek(ffd, 0, SEEK_SET);
    while(l0 < y0){
        char m = fgetc(ffd);
        if(m == '\n')   l0++;
        p0++;
    }
    p0 += x0;    
    fseek(ffd, 0, SEEK_SET);
    while(l < y){
        char m = fgetc(ffd);
        if(m == '\n')   l++;
        p++;
    }
    p += x;
    if(p0 <= p){
        fseek(ffd, p0, SEEK_SET);
        int i = p0;
        while(i <= p){
            clip[i - p0] = fgetc(ffd);
            i++;
        }
        clip[i - p0] = '\0';
    }
    if(p0 > p){
        fseek(ffd, p, SEEK_SET);
        int i = p;
        while(i <= p0){
            clip[i - p] = fgetc(ffd);
            i++;
        }
        clip[i - p] = '\0';
    }
    fclose(ffd);
}
/*
void ccopy(char filepath[], int pos1, int pos2, int size, char f){
    if(filepath[0] == '"' && filepath[1] != '/'){
        int i = 0;
        while(filepath[i + 1] != '"'){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '"'){
        int i = 0;
        while(filepath[i + 2] != '"'){
            filepath[i] = filepath[i + 2];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '/'){
        int i = 0;
        while(filepath[i + 1] != '\0' && filepath[i + 1] != NULL){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    //// pathfile corrected

    if(!exist(filepath))    printf("The file is not found");
    else{
        FILE *fff;
        fff = fopen(filepath, "r+");
        int line = 1;
        int i = 0;
        while(line < pos1){
            fseek(fff, i, SEEK_SET);
            if(fgetc(fff) == '\n'){
                line++;
                //i++;
            }
            i++;
        }
        i += pos2;
        char str[100];
        if(f == 'f')    fseek(fff, i, SEEK_SET);
        if(f == 'b')    fseek(fff, i - size + 1, SEEK_SET);
        for(int j = 0; j < size; j++){
            str[j] = fgetc(fff);
        }
        str[size] = '\0';
        

        fclose(fff);
    }
}*/

/*void ppaste(char filepath[], int pos1, int pos2){
    if(filepath[0] == '"' && filepath[1] != '/'){
        int i = 0;
        while(filepath[i + 1] != '"'){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '"'){
        int i = 0;
        while(filepath[i + 2] != '"'){
            filepath[i] = filepath[i + 2];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '/'){
        int i = 0;
        while(filepath[i + 1] != '\0' && filepath[i + 1] != NULL){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    //// pathfile corrected

    if(!exist(filepath))    printf("The file is not found");
    else{
        FILE *fff;
        fff = fopen(filepath, "r+");
        int line = 1;
        int i = 0;
        while(line < pos1){
            fseek(fff, i, SEEK_SET);
            if(fgetc(fff) == '\n'){
                line++;
                i++;
            }
            i++;
        }
        i += pos2;
        char *str;
        OpenClipboard(0);
        HANDLE in = GetClipboardData(CF_TEXT);
        str = (char*)in;
        CloseClipboard();
        fseek(fff, i, SEEK_SET);
        fputs(str, fff);
        fclose(fff);
    }
}*/

int ffind(char filepath[], char str[], int mode, char prop, int at){
    if(filepath[0] == '"' && filepath[1] != '/'){
        int i = 0;
        while(filepath[i + 1] != '"'){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '"'){
        int i = 0;
        while(filepath[i + 2] != '"'){
            filepath[i] = filepath[i + 2];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '/'){
        int i = 0;
        while(filepath[i + 1] != '\0'){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    //// pathfile corrected

    if(!exist(filepath))    return -2;
    else{
        FILE *fff;
        fff = fopen(filepath, "r+");
        fseek(fff, 0, SEEK_END);
        int file_size = ftell(fff);
        if(prop == 'n'){
            if(mode == 1){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char t = fgetc(fff);
                        if(t != ' ' && t != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    return i;
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    return i;
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
                return -1;
            }
            if(mode == 2){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char t = fgetc(fff);
                                if(t == ' ' || t == '\n' || t == EOF || t == '\0')    return i;
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    return i;
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
                return -1;
            }
            if(mode == 3){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char c = fgetc(fff);
                        if(c != ' ' && c != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1)    return i;
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1)    return i;
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
                return -1;
            }
        }////
        else if(prop == 'c'){
            int count = 0;
            if(mode == 1){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char t = fgetc(fff);
                        if(t != ' ' && t != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    count++;
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    count++;
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            if(mode == 2){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char t = fgetc(fff);
                                if(t == ' ' || t == '\n' || t == EOF || t == '\0')    count++;
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    count++;
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            if(mode == 3){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char c = fgetc(fff);
                        if(c != ' ' && c != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1)    count++;
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1)    count++;
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            return count;
        }////
        else if(prop == 'a'){
            int count = 0;
            if(mode == 1){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char t = fgetc(fff);
                        if(t != ' ' && t != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    count++;
                                if(count == at)   return i;
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    count++;
                                if(count == at)   return i;
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            if(mode == 2){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char t = fgetc(fff);
                                if(t == ' ' || t == '\n' || t == EOF || t == '\0')    count++;
                                if(count == at)    return i;
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    count++;
                                if(count == at)    return i;
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            if(mode == 3){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char c = fgetc(fff);
                        if(c != ' ' && c != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1)    count++;
                        if(count == at)   return i;
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1)    count++;
                            if(count == at)   return i;
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            return -1;
        }////
        else if(prop == 'b'){
            int word_count = 1;
            if(mode == 1){
                int i = 0;
                fseek(fff, i, SEEK_SET);

                while(i <= file_size){
                    //keep the count of words
                    char w = fgetc(fff);
                    if(w == ' ' || w == '\n'){
                        fseek(fff, i + 1, SEEK_SET);
                        char q = fgetc(fff);
                        if(q != ' ' && q != '\n')  word_count++;
                    }//
                    fseek(fff, i, SEEK_SET);

                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char t = fgetc(fff);
                        if(t != ' ' && t != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    return word_count;
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    return word_count;
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
                return -1;
            }
            if(mode == 2){
                int i = 0;
                fseek(fff, i, SEEK_SET);

                while(i <= file_size){
                    //keep the count of words
                    char w = fgetc(fff);
                    if(w == ' ' || w == '\n'){
                        fseek(fff, i + 1, SEEK_SET);
                        char q = fgetc(fff);
                        if(q != ' ' && q != '\n')  word_count++;
                    }//
                    fseek(fff, i, SEEK_SET);

                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char t = fgetc(fff);
                                if(t == ' ' || t == '\n' || t == EOF || t == '\0')    return word_count;
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    return word_count;
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
                return -1;
            }
            if(mode == 3){
                int i = 0;
                fseek(fff, i, SEEK_SET);

                while(i <= file_size){
                    //keep the count of words
                    char w = fgetc(fff);
                    if(w == ' ' || w == '\n'){
                        fseek(fff, i + 1, SEEK_SET);
                        char q = fgetc(fff);
                        if(q != ' ' && q != '\n')  word_count++;
                    }//
                    fseek(fff, i, SEEK_SET);

                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char c = fgetc(fff);
                        if(c != ' ' && c != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1)    return word_count;
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1)    return word_count;
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
                return -1;
            }
        }////
        if(prop == 'l'){
            if(mode == 1){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char t = fgetc(fff);
                        if(t != ' ' && t != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    printf("%d ", i);
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    printf("%d ", i);
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            if(mode == 2){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char t = fgetc(fff);
                                if(t == ' ' || t == '\n' || t == EOF || t == '\0')    printf("%d ", i);
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    printf("%d ", i);
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            if(mode == 3){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char c = fgetc(fff);
                        if(c != ' ' && c != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1)    printf("%d ", i);
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1)    printf("%d ", i);
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
        }
        if(prop == 't'){
            int word_count = 1;
            int count = 0;
            if(mode == 1){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    //keep the count of words
                    char w = fgetc(fff);
                    if(w == ' ' || w == '\n'){
                        fseek(fff, i + 1, SEEK_SET);
                        char q = fgetc(fff);
                        if(q != ' ' && q != '\n')  word_count++;
                    }//
                    fseek(fff, i, SEEK_SET);
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char t = fgetc(fff);
                        if(t != ' ' && t != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    count++;
                                if(count == at)   return word_count;
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    count++;
                                if(count == at)   return word_count;
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            if(mode == 2){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    //keep the count of words
                    char w = fgetc(fff);
                    if(w == ' ' || w == '\n'){
                        fseek(fff, i + 1, SEEK_SET);
                        char q = fgetc(fff);
                        if(q != ' ' && q != '\n')  word_count++;
                    }//
                    fseek(fff, i, SEEK_SET);

                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char t = fgetc(fff);
                                if(t == ' ' || t == '\n' || t == EOF || t == '\0')    count++;
                                if(count == at)    return word_count;
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    count++;
                                if(count == at)    return word_count;
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            if(mode == 3){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    //keep the count of words
                    char w = fgetc(fff);
                    if(w == ' ' || w == '\n'){
                        fseek(fff, i + 1, SEEK_SET);
                        char q = fgetc(fff);
                        if(q != ' ' && q != '\n')  word_count++;
                    }//
                    fseek(fff, i, SEEK_SET);

                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char c = fgetc(fff);
                        if(c != ' ' && c != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1)    count++;
                        if(count == at)   return word_count;
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1)    count++;
                            if(count == at)   return word_count;
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            return -1;
        }////
        if(prop == 's'){
            int word_count = 1;
            if(mode == 1){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    //keep the count of words
                    char w = fgetc(fff);
                    if(w == ' ' || w == '\n'){
                        fseek(fff, i + 1, SEEK_SET);
                        char q = fgetc(fff);
                        if(q != ' ' && q != '\n')  word_count++;
                    }//
                    fseek(fff, i, SEEK_SET);
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char t = fgetc(fff);
                        if(t != ' ' && t != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    printf("%d ", word_count);
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    printf("%d ", word_count);
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            if(mode == 2){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    //keep the count of words
                    char w = fgetc(fff);
                    if(w == ' ' || w == '\n'){
                        fseek(fff, i + 1, SEEK_SET);
                        char q = fgetc(fff);
                        if(q != ' ' && q != '\n')  word_count++;
                    }//
                    fseek(fff, i, SEEK_SET);
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char t = fgetc(fff);
                                if(t == ' ' || t == '\n' || t == EOF || t == '\0')    printf("%d ", word_count);
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    printf("%d ", word_count);
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            if(mode == 3){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    //keep the count of words
                    char w = fgetc(fff);
                    if(w == ' ' || w == '\n'){
                        fseek(fff, i + 1, SEEK_SET);
                        char q = fgetc(fff);
                        if(q != ' ' && q != '\n')  word_count++;
                    }//
                    fseek(fff, i, SEEK_SET);
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char c = fgetc(fff);
                        if(c != ' ' && c != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1)    printf("%d ", word_count);
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1)    printf("%d ", word_count);
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
        }////
        fclose(fff);
    }
}

void undo(char filepath[]){

}

void closing_pairs(char filepath[]){
    if(filepath[0] == '"' && filepath[1] != '/'){
        int i = 0;
        while(filepath[i + 1] != '"'){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '"'){
        int i = 0;
        while(filepath[i + 2] != '"'){
            filepath[i] = filepath[i + 2];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '/'){
        int i = 0;
        while(filepath[i + 1] != '\0'){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    //// pathfile corrected
    if(!exist(filepath))    printf("The file is not found");
    else{
        FILE *fff;
        fff = fopen(filepath, "r+");
        fseek(fff, 0, SEEK_SET);
        int indent = 0;
        char c;
        int i = 0;
        int line = 1;
        int pos = -1;
        while(1){
            fseek(fff, i, SEEK_SET);
            c = fgetc(fff);
            if(c == EOF)   break;
            pos++;
            if(pos == 0 && c == ' '){
                rremove(filepath, line, pos, 1, 'f');
                pos = -1;
                continue;
            }
            if(pos == 0 && !(c == '}')){
                for(int l = 0; l < indent; l++){
                    Insert(filepath, "    ", line, 0, 0);
                    pos += 4;
                    i += 4;
                }
            }
            if(c == '\n'){
                line++;
                //i++;
                pos = -1;
            }
            if(c == '{'){
                //checking the characters before {
                int j = 1;
                while(j <= pos - 4*indent){
                    fseek(fff, i - j, SEEK_SET);
                    c = fgetc(fff);
                    if(c == ' '){
                        j++;
                        continue;
                    }
                    rremove(filepath, line, pos - j + 1, j - 1, 'f');
                    pos += 1 - j;
                    i += 1 - j;
                    Insert(filepath, " ", line, pos, 0);
                    pos++;
                    i++;
                    break;
                }
                //after the {
                Insert(filepath, "\n", line, pos + 1, 0);
                indent++;
            }
            if(c == '}'){
                if(pos != 0){
                    Insert(filepath, "\n", line, pos, 0);
                    i += 1;//i+=2;
                    pos = 0;
                    line++;
                }
                indent--;
                for(int l = 0; l < indent; l++){
                    Insert(filepath, "    ", line, 0, 0);
                    pos += 4;
                    i += 4;
                }
                Insert(filepath, "\n", line, pos + 1, 0);
                pos = -1;
                line++;
                i++;
                //i++;
            }
            i++;
        }
        fclose(fff);
    }
}

void comparator(char path1[], char path2[]){

    if(!exist(path1))    printf("file1 is not found");
    else if(!exist(path2))    printf("file2 is not found");
    else{
        FILE *fff1;
        fff1 = fopen(path1, "r");
        FILE *fff2;
        fff2 = fopen(path2, "r");
        int i = 0;
        int j = 0;
        int line = 1;
        int end1 = 0;
        int end2 = 0;
        while(end1 == 0 || end2 == 0){
            //checking the row
            char row1[1000];
            char row2[1000];
            int pos = 0;
            while(1){
                fseek(fff1, i, SEEK_SET);
                row1[pos] = fgetc(fff1);
                if(row1[pos] == '\n' || row1[pos] == EOF){
                    if(row1[pos] == EOF)    end1 = 1;
                    row1[pos] = '\0';
                    i += 2;
                    break;
                }
                pos++;
                i++;
            }
            int poss = 0;
            while(1){
                fseek(fff2, j, SEEK_SET);
                row2[poss] = fgetc(fff2);
                if(row2[poss] == '\n' || row2[poss] == EOF){
                    if(row2[poss] == EOF)    end2 = 1;
                    row2[poss] = '\0';
                    j += 2;
                    break;
                }
                poss++;
                j++;
            }
            if(pos != poss){
                printf("============ #%d ============\n", line);
                printf("%s\n", row1);
                printf("%s\n", row2);
            }
            if(pos == poss){
                int equ = 1;
                for(int k = 0; k <= pos; k++){
                    if(row1[k] != row2[k]){
                        equ = 0;
                        break;
                    }
                }
                if(equ == 0){
                    printf("============ #%d ============\n", line);
                    printf("%s\n", row1);
                    printf("%s\n", row2);
                }
            }
            if(end1 == 0 && end2 == 1){
                printf("<<<<<<<<<<<< #%d - #%d <<<<<<<<<<<<\n", line + 1, line + 1);
                while(1){
                    fseek(fff1, i, SEEK_SET);
                    char t = fgetc(fff1);
                    if(t != EOF)   printf("%c", t);
                    else   break;
                    i++;
                }
                break;
            }
            if(end1 == 1 && end2 == 0){
                printf(">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n", line + 1, line + 1);
                while(1){
                    fseek(fff2, j, SEEK_SET);
                    char t = fgetc(fff2);
                    if(t != EOF)   printf("%c", t);
                    else   break;
                    j++;
                }
                break;
            }
            line++;
        }
        fclose(fff1);
        fclose(fff2);
    }
}

void rreplace(char filepath[], char str1[], char str2[], char str3[], int at, int mode){
    if(filepath[0] == '"' && filepath[1] != '/'){
        int i = 0;
        while(filepath[i + 1] != '"'){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '"'){
        int i = 0;
        while(filepath[i + 2] != '"'){
            filepath[i] = filepath[i + 2];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '/'){
        int i = 0;
        while(filepath[i + 1] != '\0'){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    //// pathfile corrected

    if(!exist(filepath))    printf("the file is not found");
    else{
        FILE *fff;
        fff = fopen(filepath, "r+");
        fseek(fff, 0, SEEK_END);
        int file_size = ftell(fff);
        int line = 1;
        int pos = -1;
        if(str3[0] == 'a' && str3[1] == 'l' && str3[2] == 'l' && str3[3] == '\0'){
            if(mode == 1){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    pos++;
                    fseek(fff, i, SEEK_SET);
                    if(fgetc(fff) == '\n'){
                        fseek(fff, i - 1, SEEK_SET);
                        if(fgetc(fff) == '\n'){
                            line++;
                            pos = -1;
                        }
                    }
                    fseek(fff, i, SEEK_SET);
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char t = fgetc(fff);
                        if(t != ' ' && t != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str1[0]){
                        int j = 1;
                        if(strlen(str1) == 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0'){
                                    rremove(filepath, line, pos, strlen(str1), 'f');
                                    Insert(filepath, str2, line, pos, 0);
                                }
                            }
                        while(fgetc(fff) == str1[j]){
                            if(j == strlen(str1) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0'){
                                    rremove(filepath, line, pos, strlen(str1), 'f');
                                    Insert(filepath, str2, line, pos, 0);
                                }
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
                return;
            }
            if(mode == 2){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    pos++;
                    fseek(fff, i, SEEK_SET);
                    if(fgetc(fff) == '\n'){
                        fseek(fff, i - 1, SEEK_SET);
                        if(fgetc(fff) == '\n'){
                            line++;
                            pos = -1;
                        }
                    }
                    fseek(fff, i, SEEK_SET);
                    if(fgetc(fff) == str1[0]){
                        int j = 1;
                        if(strlen(str1) == 1){
                                char t = fgetc(fff);
                                if(t == ' ' || t == '\n' || t == EOF || t == '\0'){
                                    rremove(filepath, line, pos, strlen(str1), 'f');
                                    Insert(filepath, str2, line, pos, 0);
                                }
                            }
                        while(fgetc(fff) == str1[j]){
                            if(j == strlen(str1) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0'){
                                    rremove(filepath, line, pos, strlen(str1), 'f');
                                    Insert(filepath, str2, line, pos, 0);
                                }
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
                return;
            }
            if(mode == 3){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    pos++;
                    fseek(fff, i, SEEK_SET);
                    if(fgetc(fff) == '\n'){
                        fseek(fff, i - 1, SEEK_SET);
                        if(fgetc(fff) == '\n'){
                            line++;
                            pos = -1;
                        }
                    }
                    fseek(fff, i, SEEK_SET);
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char c = fgetc(fff);
                        if(c != ' ' && c != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str1[0]){
                        int j = 1;
                        if(strlen(str1) == 1){
                            rremove(filepath, line, pos, strlen(str1), 'f');
                            Insert(filepath, str2, line, pos, 0);
                        }
                        while(fgetc(fff) == str1[j]){
                            if(j == strlen(str1) - 1){
                                rremove(filepath, line, pos, strlen(str1), 'f');
                                Insert(filepath, str2, line, pos, 0);
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
                return;
            }
        }////
        if(str3[0] == 'a' && str3[1] == 't' && str3[2] == '\0'){
            int count = 0;
            if(mode == 1){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    pos++;
                    fseek(fff, i, SEEK_SET);
                    if(fgetc(fff) == '\n'){
                        fseek(fff, i - 1, SEEK_SET);
                        if(fgetc(fff) == '\n'){
                            line++;
                            pos = -1;
                        }
                    }
                    fseek(fff, i, SEEK_SET);
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char t = fgetc(fff);
                        if(t != ' ' && t != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str1[0]){
                        int j = 1;
                        if(strlen(str1) == 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    count++;
                                if(count == at){
                                    rremove(filepath, line, pos, strlen(str1), 'f');
                                    Insert(filepath, str2, line, pos, 0);
                                    return;
                                }
                            }
                        while(fgetc(fff) == str1[j]){
                            if(j == strlen(str1) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    count++;
                                if(count == at){
                                    rremove(filepath, line, pos, strlen(str1), 'f');
                                    Insert(filepath, str2, line, pos, 0);
                                    return;
                                }
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            if(mode == 2){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    pos++;
                    fseek(fff, i, SEEK_SET);
                    if(fgetc(fff) == '\n'){
                        fseek(fff, i - 1, SEEK_SET);
                        if(fgetc(fff) == '\n'){
                            line++;
                            pos = -1;
                        }
                    }
                    fseek(fff, i, SEEK_SET);
                    if(fgetc(fff) == str1[0]){
                        int j = 1;
                        if(strlen(str1) == 1){
                                char t = fgetc(fff);
                                if(t == ' ' || t == '\n' || t == EOF || t == '\0'){
                                    rremove(filepath, line, pos, strlen(str1), 'f');
                                    Insert(filepath, str2, line, pos, 0);
                                    return;
                                }
                                if(count == at){
                                    rremove(filepath, line, pos, strlen(str1), 'f');
                                    Insert(filepath, str2, line, pos, 0);
                                    return;
                                }
                            }
                        while(fgetc(fff) == str1[j]){
                            if(j == strlen(str1) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    count++;
                                if(count == at){
                                    rremove(filepath, line, pos, strlen(str1), 'f');
                                    Insert(filepath, str2, line, pos, 0);
                                    return;
                                }
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            if(mode == 3){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    pos++;
                    fseek(fff, i, SEEK_SET);
                    if(fgetc(fff) == '\n'){
                        fseek(fff, i - 1, SEEK_SET);
                        if(fgetc(fff) == '\n'){
                            line++;
                            pos = -1;
                        }
                    }
                    fseek(fff, i, SEEK_SET);
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char c = fgetc(fff);
                        if(c != ' ' && c != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str1[0]){
                        int j = 1;
                        if(strlen(str1) == 1)    count++;
                        if(count == at){
                            rremove(filepath, line, pos, strlen(str1), 'f');
                            Insert(filepath, str2, line, pos, 0);
                            return;
                        }
                        while(fgetc(fff) == str1[j]){
                            if(j == strlen(str1) - 1)    count++;
                            if(count == at){
                                rremove(filepath, line, pos, strlen(str1), 'f');
                                Insert(filepath, str2, line, pos, 0);
                                return;
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            return;
        }
        fclose(fff);
    }
}

void dirtree(char *filepath, const int depth, int deep){
    char newpath[1000];
    struct dirent *dp;
    DIR *dir = opendir(filepath);

    if(!dir)    return;

    while((dp = readdir(dir)) != NULL){
        if(Is_sub(dp->d_name, ".", 1) != 0){
            for(int i = 0; i < 2 * depth; i++){
                if(i % 2 == 0)  printf("|");   //179 is |
                else    printf(" ");
            }
            printf("|---%s\n", dp->d_name);   //195 is |- and 196 is --
            if(depth <= deep || deep == -2){
                //building the newpath
                int j = 0;
                while(j < strlen(filepath)){
                    newpath[j] = filepath[j];
                    j++;
                }
                newpath[j] = '/';
                j++;
                int k = 0;
                while(k < strlen(dp->d_name)){
                    newpath[j + k] = dp->d_name[k];
                    k++;
                }
                newpath[j + k] = '\0';
                dirtree(newpath, depth + 1, deep);
            }
        }
    }
    closedir(dir);
}

int validate(char command[]){
    int ok = 0;
    if(Is_sub(command, "createfile --file ", 18) == 0)    ok = 1;
    else if(Is_sub(command, "cat --file ", 11) == 0)    ok = 1;
    else if(Is_sub(command, "insertstr --file ", 17) == 0)  ok = 1;
    else if(Is_sub(command, "removestr --file ", 17) == 0)  ok = 1;
    else if(Is_sub(command, "copystr --file ", 15) == 0)    ok = 1;
    else if(Is_sub(command, "cutstr --file ", 14) == 0) ok = 1;
    else if(Is_sub(command, "pastestr --file ", 16) == 0)   ok = 1;
    else if(Is_sub(command, "auto-indent ", 12) == 0)   ok = 1;
    else if(Is_sub(command, "find --str ", 11) == 0)    ok = 1;
    else if(Is_sub(command, "replace --str1 ", 15) == 0)    ok = 1;
    else if(Is_sub(command, "compare ", 8) == 0)    ok = 1;
    else if(Is_sub(command, "grep --str ", 11) == 0)    ok = 1;
    else if(Is_sub(command, "grep -c --str ", 14) == 0) ok = 1;
    else if(Is_sub(command, "grep -l --str ", 14) == 0) ok = 1;
    else if(Is_sub(command, "tree ", 5) == 0)   ok = 1;
    return ok;
}

void cat_arman(char filepath[], char input[]){
    int ip = 0;
    if(filepath[0] == '"' && filepath[1] != '/'){
        int i = 0;
        while(filepath[i + 1] != '"'){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '"'){
        int i = 0;
        while(filepath[i + 2] != '"'){
            filepath[i] = filepath[i + 2];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '/'){
        int i = 0;
        while(filepath[i + 1] != '\0'){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    FILE *fff;
    int c;
    fff = fopen(filepath, "r");
    while(1){
        c = fgetc(fff);
        if(feof(fff))   break;
        //printf("%c", c);
        input[ip] = c;
        ip++;
    }
    fclose(fff);
    input[ip] = '\0';
}
void comparator_arman(char path1[], char path2[], char input[]){

    if(!exist(path1))    strcpy(input, "file1 is not found");
    else if(!exist(path2))    strcpy(input, "file2 is not found");
    else{
        strcpy(input, "\0");
        FILE *fff1;
        fff1 = fopen(path1, "r");
        FILE *fff2;
        fff2 = fopen(path2, "r");
        int i = 0;
        int j = 0;
        int line = 1;
        int end1 = 0;
        int end2 = 0;
        while(end1 == 0 || end2 == 0){
            //checking the row
            char row1[1000];
            char row2[1000];
            int pos = 0;
            while(1){
                fseek(fff1, i, SEEK_SET);
                row1[pos] = fgetc(fff1);
                if(row1[pos] == '\n' || row1[pos] == EOF){
                    if(row1[pos] == EOF)    end1 = 1;
                    row1[pos] = '\0';
                    i += 2;
                    break;
                }
                pos++;
                i++;
            }
            int poss = 0;
            while(1){
                fseek(fff2, j, SEEK_SET);
                row2[poss] = fgetc(fff2);
                if(row2[poss] == '\n' || row2[poss] == EOF){
                    if(row2[poss] == EOF)    end2 = 1;
                    row2[poss] = '\0';
                    j += 2;
                    break;
                }
                poss++;
                j++;
            }
            if(pos != poss){
                strcat(input, "============ #");
                char linestr[1000];
                sprintf(linestr, "%d", line);
                strcat(input, linestr);
                strcat(input, " ============\n");
                strcat(input, row1);
                strcat(input, "\n");
                strcat(input, row2);
                strcat(input, "\n");
            }
            if(pos == poss){
                int equ = 1;
                for(int k = 0; k <= pos; k++){
                    if(row1[k] != row2[k]){
                        equ = 0;
                        break;
                    }
                }
                if(equ == 0){
                    strcpy(input, "============ #");
                    char linestr[1000];
                    sprintf(linestr, "%d", line);
                    strcat(input, linestr);
                    strcat(input, " ============\n");
                    strcat(input, row1);
                    strcat(input, "\n");
                    strcat(input, row2);
                    strcat(input, "\n");
                }
            }
            if(end1 == 0 && end2 == 1){
                strcat(input, "<<<<<<<<<<<< #");
                char linestr[1000];
                sprintf(linestr, "%d", line + 1);
                strcat(input, linestr);
                strcat(input, " - #");
                strcat(input, linestr);
                strcat(input, " <<<<<<<<<<<<\n");

                while(1){
                    fseek(fff1, i, SEEK_SET);
                    char t = fgetc(fff1);
                    if(t != EOF)   strncat(input, &t, 1);
                    else   break;
                    i++;
                }
                break;
            }
            if(end1 == 1 && end2 == 0){
                strcat(input, ">>>>>>>>>>>> #");
                char linestr[1000];
                sprintf(linestr, "%d", line + 1);
                strcat(input, linestr);
                strcat(input, " - #");
                strcat(input, linestr);
                strcat(input, " >>>>>>>>>>>>\n");
                while(1){
                    fseek(fff2, j, SEEK_SET);
                    char t = fgetc(fff2);
                    if(t != EOF)   strncat(input, &t, 1);
                    else   break;
                    j++;
                }
                break;
            }
            line++;
        }
        fclose(fff1);
        fclose(fff2);
    }
}
void dirtree_arman(char *filepath, const int depth, int deep, char input[]){
    char newpath[1000];
    struct dirent *dp;
    DIR *dir = opendir(filepath);

    if(!dir)    return;

    while((dp = readdir(dir)) != NULL){
        if(Is_sub(dp->d_name, ".", 1) != 0){
            for(int i = 0; i < 2 * depth; i++){
                if(i % 2 == 0){
                    char q = '|';
                    strncat(input, &q, 1);
                }   //179 is |
                else    strcat(input, " ");
            }
            char w = 195;
            strncat(input, &w, 1);
            w = 196;
            strncat(input, &w, 1);
            strcat(input, dp->d_name);
            strcat(input, "\n");
            //195 is |- and 196 is --
            if(depth <= deep || deep == -2){
                //building the newpath
                int j = 0;
                while(j < strlen(filepath)){
                    newpath[j] = filepath[j];
                    j++;
                }
                newpath[j] = '/';
                j++;
                int k = 0;
                while(k < strlen(dp->d_name)){
                    newpath[j + k] = dp->d_name[k];
                    k++;
                }
                newpath[j + k] = '\0';
                dirtree_arman(newpath, depth + 1, deep, input);
            }
        }
    }
    closedir(dir);
}
void ffind_arman(char filepath[], char str[], int mode, char prop, int at, char input[]){
    if(filepath[0] == '"' && filepath[1] != '/'){
        int i = 0;
        while(filepath[i + 1] != '"'){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '"'){
        int i = 0;
        while(filepath[i + 2] != '"'){
            filepath[i] = filepath[i + 2];
            i++;
        }
        filepath[i] = '\0';
    }
    else if(filepath[0] == '/'){
        int i = 0;
        while(filepath[i + 1] != '\0'){
            filepath[i] = filepath[i + 1];
            i++;
        }
        filepath[i] = '\0';
    }
    //// pathfile corrected

    if(!exist(filepath)){
        strcat(input, "file doesn't exist");
        return;
    }
    else{
        FILE *fff;
        fff = fopen(filepath, "r+");
        fseek(fff, 0, SEEK_END);
        int file_size = ftell(fff);
        if(prop == 's'){
            int word_count = 1;
            if(mode == 1){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    //keep the count of words
                    char w = fgetc(fff);
                    if(w == ' ' || w == '\n'){
                        fseek(fff, i + 1, SEEK_SET);
                        char q = fgetc(fff);
                        if(q != ' ' && q != '\n')  word_count++;
                    }//
                    fseek(fff, i, SEEK_SET);
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char t = fgetc(fff);
                        if(t != ' ' && t != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0'){
                                    char wordstr[1000];
                                    sprintf(wordstr, "%d", word_count);
                                    strcat(input, wordstr);
                                    strcat(input, " ");
                                }
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0'){
                                    char wordstr[1000];
                                    sprintf(wordstr, "%d", word_count);
                                    strcat(input, wordstr);
                                    strcat(input, " ");
                                }
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            if(mode == 2){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    //keep the count of words
                    char w = fgetc(fff);
                    if(w == ' ' || w == '\n'){
                        fseek(fff, i + 1, SEEK_SET);
                        char q = fgetc(fff);
                        if(q != ' ' && q != '\n')  word_count++;
                    }//
                    fseek(fff, i, SEEK_SET);
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char t = fgetc(fff);
                                if(t == ' ' || t == '\n' || t == EOF || t == '\0'){
                                    char wordstr[1000];
                                    sprintf(wordstr, "%d", word_count);
                                    strcat(input, wordstr);
                                    strcat(input, " ");
                                }
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0'){
                                    char wordstr[1000];
                                    sprintf(wordstr, "%d", word_count);
                                    strcat(input, wordstr);
                                    strcat(input, " ");
                                }
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            if(mode == 3){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    //keep the count of words
                    char w = fgetc(fff);
                    if(w == ' ' || w == '\n'){
                        fseek(fff, i + 1, SEEK_SET);
                        char q = fgetc(fff);
                        if(q != ' ' && q != '\n')  word_count++;
                    }//
                    fseek(fff, i, SEEK_SET);
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char c = fgetc(fff);
                        if(c != ' ' && c != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                            char wordstr[1000];
                            sprintf(wordstr, "%d", word_count);
                            strcat(input, wordstr);
                            strcat(input, " ");
                        }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char wordstr[1000];
                                sprintf(wordstr, "%d", word_count);
                                strcat(input, wordstr);
                                strcat(input, " ");
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
        }
        if(prop == 'l'){
            if(mode == 1){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char t = fgetc(fff);
                        if(t != ' ' && t != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0'){
                                    char istr[1000];
                                    sprintf(istr, "%d", i);
                                    strcat(input, istr);
                                    strcat(input, " ");
                                }
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0'){
                                    char istr[1000];
                                    sprintf(istr, "%d", i);
                                    strcat(input, istr);
                                    strcat(input, " ");
                                }
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            if(mode == 2){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char t = fgetc(fff);
                                if(t == ' ' || t == '\n' || t == EOF || t == '\0'){
                                    char istr[1000];
                                    sprintf(istr, "%d", i);
                                    strcat(input, istr);
                                    strcat(input, " ");
                                }
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0'){
                                    char istr[1000];
                                    sprintf(istr, "%d", i);
                                    strcat(input, istr);
                                    strcat(input, " ");
                                }
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            if(mode == 3){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char c = fgetc(fff);
                        if(c != ' ' && c != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                            char istr[1000];
                            sprintf(istr, "%d", i);
                            strcat(input, istr);
                            strcat(input, " ");
                        }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char istr[1000];
                                sprintf(istr, "%d", i);
                                strcat(input, istr);
                                strcat(input, " ");
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
        }
    }
}

void fiind(char filepath[], char str[], int findarray[]){
        //finding * in the str
        int l = strlen(str);
        int e = 0;
        int mode = 1;
        if(str[0] == '*'){
            int k = 0;
            while(str[k + 1] != '\0'){
                str[k] = str[k + 1];
                k++;
            }
            str[k] = '\0';
            mode = 2;
        }
        else if(str[l - 1] == '*' && str[l - 2] != '\\'){
            str[l - 1] = '\0';
            mode = 3;
        }
        else if(str[l - 1] == '*' && str[l - 2] == '\\'){
            mode = 1;
            e = 1;
        }

        //clearing the \*
        if(e == 1)   l++;
        int setare[l - 1];
        for(int r = 0; r < l - 1; r++){
            setare[r] = 0;
        }
        for(int u = 1; u < l - 1; u++){
            if(str[u] == '*' && str[u - 1] == '\\')    setare[u] = 1;
            }
        int s_holder = 0;
        for(int u = 1; u < l - 1; u++){
            if(setare[u] == 1){
                for(int h = u - s_holder; h <= l - s_holder - 2; h++){
                    str[h - 1] = str[h];
                    str[l - 2 - s_holder] = '\0';
                }
                s_holder++;
            }
        }////

        int cnt = 0;
        FILE *fff;
        fff = fopen(filepath, "r+");
        fseek(fff, 0, SEEK_END);
        int file_size = ftell(fff);
            if(mode == 1){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char t = fgetc(fff);
                        if(t != ' ' && t != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    {findarray[cnt] = i; cnt++;}
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    {findarray[cnt] = i; cnt++;}
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            if(mode == 2){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1){
                                char t = fgetc(fff);
                                if(t == ' ' || t == '\n' || t == EOF || t == '\0')    {findarray[cnt] = i; cnt++;}
                            }
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    {findarray[cnt] = i; cnt++;}
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            if(mode == 3){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        char c = fgetc(fff);
                        if(c != ' ' && c != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    if(fgetc(fff) == str[0]){
                        int j = 1;
                        if(strlen(str) == 1)    {findarray[cnt] = i; cnt++;}
                        while(fgetc(fff) == str[j]){
                            if(j == strlen(str) - 1)    {findarray[cnt] = i; cnt++;}
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
        fclose(fff);
        findarray[cnt] = -1;
}


void borrder(){
    init_pair(1, COLOR_MAGENTA, COLOR_CYAN);
    attron(COLOR_PAIR(1));
    for(int i = 0; i < 100; i++){
        move(0, i);
        printw("-");
        move(24, i);
        printw("-");
    }
    for(int i = 0; i < 50; i++){
        move(i, 0);
        printw("|");
        move(i, 99);
        printw("|");
    }
}
//l is natural
int gotoline(int l, char filepath[]){
    FILE *fff;
    fff = fopen(filepath, "r");
    int line = 1;
    int i = 0;
    while(1){
        char c = fgetc(fff);
        if(c == EOF && line < l){
            fclose(fff);
            return -1;
        }
        else if(line == l)    break;
        if(c == '\n')   line++;
        i++;
    }
    fclose(fff);
    return i;
}
//l is not natrual
void emp(int l, int cols){
    //attron(COLOR_PAIR(1));
    move(l, 0);
    for(int o = 0; o < cols; o++){
        printw(" ");
    }
    move(l, 0);
    //attroff(COLOR_PAIR(1));
}

void shower(char filepath[], int rows, int cols){
    move(0, 0);
    FILE *fff;
    fff = fopen(filepath, "a");
    attron(COLOR_PAIR(1));
    for(int i = 0; i < rows - 2; i++){
        printw("%d  ", i + 1);
        int j = gotoline(i + 1, filepath);
        fseek(fff, j, SEEK_SET);
        while(1){
            char c = fgetc(fff);
            printw("%c", c);
            if(c == '\n')   break;
        }
    }
    fclose(fff);
    attroff(COLOR_PAIR(1));
    move(rows - 1, 8);
    attron(COLOR_PAIR(3));
    printw(" %s -", filepath);
    for(int o = 18; o < cols; o++)    printw(" ");
    attroff(COLOR_PAIR(3));
}

int poo(int k){
    int ans = 1;
    int i = 0;
    while(i < k){
        ans *= 10;
        i++;
    }
    return ans;
}

int dig(int n){
    if(n == 0)    return 1;
    int k = 0;
    while(n >= poo(k))    k++;
    return k;
}
//l is natural
void rowprint(int l, char filepath[]){
    int i = 0;
    FILE *ff;
    ff = fopen(filepath, "r");
    fseek(ff, 0, SEEK_SET);
    int line = 1;
    while(line < l){
        char z = fgetc(ff);
        if(z == '\n')   line++;
        i++;
    }
    attron(COLOR_PAIR(1));
    while(1){
        char u = fgetc(ff);
        if(u == '\n' || u == EOF || u == '\0')   break;
        printw("%c", u);
    }
    attroff(COLOR_PAIR(1));
    fclose(ff);
}
//line is not natural
char gopos(char filepath[], int line, int pos){
    FILE *f;
    f = fopen(filepath, "r");
    fseek(f, 0, SEEK_SET);
    int l = 0;
    char d;
    while(l < line){
        d = fgetc(f);
        if(d == '\n')   l++;
    }
    for(int a = 0; a <= pos; a++)   d = fgetc(f);
    fclose(f);
    return d;
}

int totl(char filepath[]){
    FILE *f;
    f = fopen(filepath, "r");
    fseek(f, 0, SEEK_SET);
    int l = 1;
    while(1){
        char b = fgetc(f);
        if(b == '\n')    l++;
        if(b == EOF)    break;
    }
    return l;
}
//l is natural
int rowlen(char filepath[], int l){
    int x = gotoline(l, filepath);
    int u = 0;
    FILE *ff;
    ff = fopen(filepath, "r");
    fseek(ff, x, SEEK_SET);
    while(1){
        char d = fgetc(ff);
        if(d == '\n' || d == EOF || d == '\0')    break;
        u++;
    }
    fclose(ff);
    return u;
}
//y is not natural
int posfind(int y, int x, char filepath[]){
    FILE *fff;
    fff = fopen(filepath, "r");
    int i = 0;
    int l = 0;
    fseek(fff, 0, SEEK_SET);
    while(l < y){
        if(fgetc(fff) == '\n')    l++;
        i++;
    }
    i += x;
    fclose(fff);
    return i;
}

void highlighter(int y0, int x0, int y, int x, char filepath[], int rowkeep){
    int p0 = posfind(y0, x0, filepath);
    int p = posfind(y, x, filepath);

    int mx, my, MX, MY;
    if(y > y0){MY = y; MX = x; my = y0; mx = x0;}
    else if(y < y0){MY = y0; MX = x0; my = y; mx = x;}
    else if(x0 > x){MY = my = y0; MX = x0; mx = x;}
    else{MY = my = y0; MX = x; mx = x0;}
    attron(COLOR_PAIR(2));
    if(my < MY){
        for(int i = mx; i < rowlen(filepath, my + rowkeep); i++){
            move(my, i + dig(rowkeep + my) + 2);
            char c = mvinch(my, i + dig(rowkeep + my) + 2);
            printw("%c", c);
        }
        for(int j = my + 1; j < MY; j++){
            for(int i = 0; i < rowlen(filepath, j + rowkeep); i++){
                move(j, i + dig(rowkeep + j) + 2);
                char c = mvinch(j, i + dig(rowkeep + j) + 2);
                printw("%c", c);
            }
        }
        for(int i = 0; i <= MX; i++){
            move(MY, i + dig(rowkeep + MY) + 2);
            char c = mvinch(MY, i + dig(rowkeep + MY) + 2);
            printw("%c", c);
        }
    }
    else{
        for(int i = mx; i <= MX; i++){
            move(my, i + dig(rowkeep + my) + 2);
            char c = mvinch(my, i + dig(rowkeep + my) + 2);
            printw("%c", c);
        }
    }
    attroff(COLOR_PAIR(2));
}

void undohighlighter(char filepath[], int rowkeep){
    attron(COLOR_PAIR(1));
    for(int i = 0; i < rows - 2; i++){
        for(int j = 0; j < rowlen(filepath, i + rowkeep) + dig(rowkeep + i) + 2; j++){
            move(i, j);
            char c = mvinch(i, j);
            printw("%c", c);
        }
    }
    attroff(COLOR_PAIR(1));
}

//array[0] is natural
void posgive(char filepath[], int i, int array[2]){
    int j = 0;
    int y = 1;
    int x = 0;
    FILE *ff;
    ff = fopen(filepath, "r");
    fseek(ff, 0, SEEK_SET);
    while(j <= i){
        char t = fgetc(ff);
        if(t == '\n'){
            y++;
            x = -1;
        }
        else    x++;
        j++;
    }
    fclose(ff);
    array[0] = y;
    array[1] = x;
}
//pours file1 to file2
void pour(char file1[], char file2[]){
    FILE *fff;
    fff = fopen(file1, "r");
    fseek(fff, 1, SEEK_END);
    int size = ftell(fff);
    fseek(fff, 0, SEEK_SET);
    char *preserve;
    preserve = (char*) malloc(sizeof(char) * size);
    size_t result = fread(preserve, 1, size, fff);
    preserve[size] = '\0';
    fclose(fff);
    FILE *ff;
    ff = fopen(file2, "w");
    fputs(preserve, ff);
    fclose(ff);
}

void is_save(char realpath[]){
    move(rows - 1, 8);
    attron(COLOR_PAIR(3));
    printw(" %s -", realpath);
    for(int o = 18; o < cols; o++)    printw(" ");
    attroff(COLOR_PAIR(3));
}

int main()
{


    /*initscr();
    noecho();
    int x, y;
    x = 10;
    y = 10;

    move(y, x);
    
    printw("heloo world");
    
    //refresh();

    //move(0, 0);
    int c = getch();
    refresh();
    //move(0, 0);
    //printw("%d", c);
    //clear();

    mvprintw(14, 17, "%d", c);
    refresh();

    getch();

    endwin();*/

    int ps[2];
    int pss[2];
    int findarray[100];
    char clipboard[10000];
    char filepath[1000];
    scanf("%s", filepath);
    char realpath[1000];
    strcpy(realpath, filepath);
    strcpy(filepath , ".savingg.txt");
    pour(realpath, filepath);
    initscr();
    start_color();
    noecho();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_CYAN);
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_BLACK, COLOR_WHITE);
    move(0, 0);
    //coordinates of the curser
    int cux = 0;
    int cuy = 0;
    int rowkeep = 1;
    if(exist(realpath) == 1){
        FILE *fff;
        fff = fopen(filepath, "r+");
        attron(COLOR_PAIR(1));
        for(int i = 0; i < rows - 2; i++){
            printw("%d  ", i + 1);
            int j = gotoline(i + 1, filepath);
            fseek(fff, j, SEEK_SET);
            if(i + 1 != totl(filepath)){
                while(1){
                    char c = fgetc(fff);
                    if(c != EOF && c != '\0')   printw("%c", c);
                    if(c == '\n' || c == EOF || c == '\0')   break;
                }
            }
            else{
                for(int q = 0; q < cols; q++){
                    char c = fgetc(fff);
                    if(q < rowlen(filepath, i + 1))    printw("%c", c);
                    else    printw(" ");
                }
            }
            if(i + 1 > totl(filepath)){
                emp(i, cols);
            }
        }
        fclose(fff);
        attroff(COLOR_PAIR(1));

        int save = 0;
        move(rows - 1, 8);
        attron(COLOR_PAIR(3));
        printw(" %s -", realpath);
        for(int o = 18; o < cols; o++)    printw(" ");
        attroff(COLOR_PAIR(3));

        int mode = 1;
        int exit = 0;
        while(1){
            if(mode == 1){
                move(rows - 1, 0);
                attron(COLOR_PAIR(2));
                printw(" NORMAL ");
                attroff(COLOR_PAIR(2));
                move(rows, 0);
                int start = 0;
                char command[100];
                strcpy(command, "\0");
                int redyi = 0;
                int redyv = 0;
                int nextf = -1;
                int firstf;
                int hif = 0;
                int prlen = 0;
                while(1){
                    if(start == 0)    move(cuy, cux + dig(rowkeep + cuy) + 2);
                    char t = getch();
                    if(hif == 1){
                        undohighlighter(filepath, rowkeep);
                        move(cuy, cux + dig(rowkeep + cuy) + 2);
                    }
                    if(hif == 2){
                        undohighlighter(filepath, rowkeep);
                        hif = 0;
                    }
                    if(t == 'n' && hif == 1){
                        if(nextf != -1){
                            int nf[2];
                            int nff[2];
                            posgive(filepath, nextf, nf);
                            nf[0] -= rowkeep;
                            posgive(filepath, nextf + prlen - 1, nff);
                            nff[0] -= rowkeep;
                            if(nf[0] >= 0 && nff[0] >= 0 && nff[0] <= rows - 3 && nf[0] <= rows - 3){
                                highlighter(nf[0], nf[1], nff[0], nff[1], filepath, rowkeep);
                                cux = nf[1];
                                cuy = nf[0];
                                move(cuy, cux + dig(rowkeep + cuy) + 2);
                            }
                            hif = 2;
                            nextf = -1;
                            firstf = -1;
                            continue;
                        }
                        else if(firstf != -1){
                            int nf[2];
                            int nff[2];
                            posgive(filepath, firstf, nf);
                            nf[0] -= rowkeep;
                            posgive(filepath, firstf + prlen - 1, nff);
                            nff[0] -= rowkeep;
                            if(nf[0] >= 0 && nff[0] >= 0 && nff[0] <= rows - 3 && nf[0] <= rows - 3){
                                highlighter(nf[0], nf[1], nff[0], nff[1], filepath, rowkeep);
                                cux = nf[1];
                                cuy = nf[0];
                                move(cuy, cux + dig(rowkeep + cuy) + 2);
                            }
                            hif = 2;
                            nextf = -1;
                            firstf = -1;
                            continue;
                        }
                    }
                    hif = 0;
                    if(t == ':' || t == '/')    start = 1;
                    /////////////
                    if(start == 0){
                        if(t == 'q'){
                            exit = 1;
                            break;
                        }
                        if(t == 'v'){
                            mode = 2;
                            break;
                        }
                        if(t == 'i'){       
                            /*Insert(filepath, clipboard, cuy + 1, cux, 0);
                            FILE *fff;
                            fff = fopen(filepath, "r+");
                            attron(COLOR_PAIR(1));
                            for(int i = 0; i < rows - 2; i++){
                                emp(i, cols);
                                printw("%d  ", i + rowkeep);
                                int j = gotoline(i + rowkeep, filepath);
                                fseek(fff, j, SEEK_SET);
                                while(1){
                                    char c = fgetc(fff);
                                    printw("%c", c);
                                    if(c == '\n' || c == EOF)   break;
                                }
                            }
                            fclose(fff);
                            attroff(COLOR_PAIR(1));*/
                            mode = 3;
                            break;
                        }
                        if(t == 'k'){
                            if(cuy != 3 && cuy != 0){
                                cuy--;
                                if(rowlen(filepath, cuy + rowkeep) >= cux + 1)    move(cuy, cux + dig(rowkeep + cuy) + 2);
                                else{
                                    cux = rowlen(filepath, cuy + rowkeep) - 1;
                                    move(cuy, cux + dig(rowkeep + cuy) + 2);//for when you go to a short line
                                }
                            }
                            else if(rowkeep > 1){
                                rowkeep--;
                                for(int s = 0; s < 21; s++){
                                    move(s, 0);
                                    emp(s, cols);
                                    printw("%d  ", rowkeep + s);
                                    rowprint(rowkeep + s, filepath);
                                }
                                if(rowlen(filepath, cuy + rowkeep) >= cux + 1)    move(cuy, cux + dig(rowkeep + cuy) + 2);
                                else{
                                    cux = rowlen(filepath, cuy + rowkeep) - 1;
                                    move(cuy, cux + dig(rowkeep + cuy) + 2);//for when you go to a short line
                                }
                            }
                            else if(cuy != 0){
                                cuy--;
                                if(rowlen(filepath, cuy + rowkeep) >= cux + 1)    move(cuy, cux + dig(rowkeep + cuy) + 2);
                                else{
                                    cux = rowlen(filepath, cuy + rowkeep) - 1;
                                    move(cuy, cux + dig(rowkeep + cuy) + 2);//for when you go to a short line
                                }
                            }
                        }
                        if(t == 'j'){
                            if(cuy != rows - 6 && cuy != rows - 3){
                                cuy++;
                                if(rowlen(filepath, cuy + rowkeep) >= cux + 1)    move(cuy, cux + dig(rowkeep + cuy) + 2);
                                else{
                                    cux = rowlen(filepath, cuy + rowkeep) - 1;
                                    move(cuy, cux + dig(rowkeep + cuy) + 2);//for when you go to a short line
                                }
                            }
                            else if(rowkeep < totl(filepath) - rows + 3){
                                rowkeep++;
                                for(int s = 0; s < rows - 2; s++){
                                    move(s, 0);
                                    emp(s, cols);
                                    printw("%d  ", rowkeep + s);
                                    rowprint(rowkeep + s, filepath);
                                }
                                if(rowlen(filepath, cuy + rowkeep) >= cux + 1)    move(cuy, cux + dig(rowkeep + cuy) + 2);
                                else{
                                    cux = rowlen(filepath, cuy + rowkeep) - 1;
                                    move(cuy, cux + dig(rowkeep + cuy) + 2);//for when you go to a short line
                                }
                            }
                            else if(cuy != 20){
                                cuy++;
                                if(rowlen(filepath, cuy + rowkeep) >= cux + 1)    move(cuy, cux + dig(rowkeep + cuy) + 2);
                                else{
                                    cux = rowlen(filepath, cuy + rowkeep) - 1;
                                    move(cuy, cux + dig(rowkeep + cuy) + 2);//for when you go to a short line
                                }
                            }
                        }
                        if(t == 'h'){
                            if(cux > 0){
                                cux--;
                                move(cuy, cux + dig(rowkeep + cuy) + 2);
                            }
                        }
                        if(t == 'l'){
                            if(cux < rowlen(filepath, cuy + rowkeep) - 1)    cux++;
                            move(cuy, cux + dig(rowkeep + cuy) + 2);
                        }
                    }//////////////////
                    if(start == 1){
                        move(rows, 0);
                        start = 2;
                    }
                    if(start > 0)    printw("%c", t);
                    
                    if(start > 0 && !(command[0] == '/' && t == ' '))    strncat(command, &t, 1);
                    if(strcmp(command, ":saveas ") == 0){
                        char nam[1000];
                        strcpy(nam, "\0");
                        while(1){
                            char r = getch();
                            if(r == '\n' || r == '\0'){
                                strcat(nam, "\0");
                                emp(rows, cols);
                                break;
                            }
                            strncat(nam, &r, 1);
                            printw("%c", r);
                        }
                        if(rename(realpath, nam) == 0){
                            strcpy(realpath, nam);
                            pour(filepath, realpath);
                            printw("file saved successfuly");
                            move(rows - 1, 8);
                            attron(COLOR_PAIR(3));
                            printw(" %s +", realpath);
                            for(int o = 18; o < cols; o++)    printw(" ");
                            attroff(COLOR_PAIR(3));
                            move(rows, 22);
                            save = 1;
                        }
                        else    printw("Inaccurate name");
                        strcpy(command, "\0");
                        start = 0;
                    }
                    if(strcmp(command, ":save\n") == 0){
                        if(realpath[0] == '\0'){
                            emp(rows, cols);
                            move(rows, 0);
                            printw("Please choose a name for your file");
                        }
                        else{
                            pour(filepath, realpath);
                            move(rows - 1, 8);
                            attron(COLOR_PAIR(3));
                            printw(" %s +", realpath);
                            for(int o = 18; o < cols; o++)    printw(" ");
                            attroff(COLOR_PAIR(3));
                            move(rows, 0);
                            save = 1;
                            nextf = -1;
                        }
                        strcpy(command, "\0");
                        start = 0;
                    }
                    if(strcmp(command, ":open\n") == 0){
                        pour(filepath, realpath);
                        char newfile[100];
                        strcpy(newfile, "newfile");
                        while(1){
                            char n[100];
                            strcpy(n, newfile);
                            strcat(n, ".txt");
                            if(exist(n))    strcat(newfile, "1");
                            else    break;
                        }
                        strcat(newfile, ".txt");
                        move(0, 0);
                        FILE *fff;
                        fff = fopen(newfile, "a");
                        fclose(fff);
                        strcpy(realpath, newfile);
                        attron(COLOR_PAIR(1));
                        for(int i = 0; i < rows - 2; i++){
                            emp(i, cols);
                            move(i, 0);
                            printw("%d", i + 1);
                        }
                        move(rows - 1, 8);
                        attron(COLOR_PAIR(3));
                        printw(" %s -", realpath);
                        for(int o = 18; o < cols; o++)    printw(" ");
                        attroff(COLOR_PAIR(3));
                        emp(rows, cols);
                        is_save(realpath);
                        move(rows, 0);
                        strcpy(command, "\0");
                        start = 0;
                    }
                    if(strcmp(command, ":=\n") == 0){
                        closing_pairs(filepath);
                        FILE *fff;
                        fff = fopen(filepath, "r+");
                        attron(COLOR_PAIR(1));
                        for(int i = 0; i < rows - 2; i++){
                            emp(i, cols);
                            printw("%d  ", i + rowkeep);
                            int j = gotoline(i + rowkeep, filepath);
                            fseek(fff, j, SEEK_SET);
                            if(i + 1 != totl(filepath)){
                                while(1){
                                    char c = fgetc(fff);
                                    if(c != EOF && c != '\0')   printw("%c", c);
                                    if(c == '\n' || c == EOF || c == '\0')   break;
                                }
                            }
                            else{
                                for(int q = 0; q < cols; q++){
                                    char c = fgetc(fff);
                                    if(q < rowlen(filepath, i + 1))    printw("%c", c);
                                    else    printw(" ");
                                }
                            }
                            if(i + 1 > totl(filepath)){
                                emp(i, cols);
                            }
                        }
                        fclose(fff);
                        attroff(COLOR_PAIR(1));
                        is_save(realpath);
                        strcpy(command, "\0");
                        start = 0;
                    }
                    if(command[0] == '/' && t == '\n'){
                        strcat(command, "\0");
                        for(int q = 0; q < strlen(command); q++){
                            command[q] = command[q + 1];
                        }
                        command[strlen(command) - 1] = '\0';
                        fiind(filepath, command, findarray);
                        int nm = 0;
                        while(findarray[nm] != -1)  nm++;
                        for(int v = 0; v < nm; v++){
                            posgive(filepath, findarray[v], ps);
                            ps[0] -= rowkeep;
                            posgive(filepath, findarray[v] + strlen(command) - 1, pss);
                            pss[0] -= rowkeep;
                            if(ps[0] >= 0 && pss[0] >= 0 && pss[0] <= rows - 3 && ps[0] <= rows - 3)    highlighter(ps[0], ps[1], pss[0], pss[1], filepath, rowkeep);
                        }
                        hif = 1;

                        int w = posfind(cuy, cux, filepath);
                        for(int v = 0; v < nm; v++){
                            nextf = -1;
                            if(w <= findarray[v]){
                                nextf = findarray[v];
                                break;
                            }
                        }
                        int pp[2];
                        posgive(filepath, nextf, pp);
                        if(pp[0] - rowkeep > rows - 3)    nextf = -1;
                        prlen = strlen(command);
                        /*if(nm > 0){
                            for(int v = 0; v < nm; v++){
                                firstf = -1;
                                posgive(filepath, firstf, pp);
                                if(pp[0] - rowkeep >= 0){
                                    firstf = findarray[v];
                                    break;
                                }
                            }
                        }*/
                        if(nm > 0)  firstf = findarray[0];

                        ////////////////how can I get the find done??????
                        strcpy(command, "\0");
                        start = 0;
                    }
                    if(strcmp(command, ":p\n") == 0){
                        Insert(filepath, clipboard, cuy + 1, cux, 0);
                        FILE *fff;
                        fff = fopen(filepath, "r+");
                        attron(COLOR_PAIR(1));
                        for(int i = 0; i < rows - 2; i++){
                            emp(i, cols);
                            printw("%d  ", i + rowkeep);
                            int j = gotoline(i + rowkeep, filepath);
                            fseek(fff, j, SEEK_SET);
                            if(i + 1 != totl(filepath)){
                                while(1){
                                    char c = fgetc(fff);
                                    if(c != EOF && c != '\0')   printw("%c", c);
                                    if(c == '\n' || c == EOF || c == '\0')   break;
                                }
                            }
                            else{
                                for(int q = 0; q < cols; q++){
                                    char c = fgetc(fff);
                                    if(q < rowlen(filepath, i + 1))    printw("%c", c);
                                    else    printw(" ");
                                }
                            }
                            if(i + 1 > totl(filepath)){
                                emp(i, cols);
                            }
                        }
                        fclose(fff);
                        attroff(COLOR_PAIR(1));
                        is_save(realpath);
                        move(cuy, cux + dig(rowkeep + cuy) + 2);
                        strcpy(command, "\0");
                        start = 0;
                    }
                    if(t == '\n')   start = 0;
                    if(start == 0){
                        emp(rows, cols);
                        strcpy(command, "\0");
                    }
                }
                if(exit == 1)    break;
            }
            if(mode == 2){//visual mode
                int x0 = cux;
                int y0 = cuy;
                int r0 = rowkeep;
                move(rows - 1, 0);
                attron(COLOR_PAIR(2));
                printw(" VISUAL ");
                attroff(COLOR_PAIR(2));
                move(cuy, cux + dig(rowkeep + cuy) + 2);
                while(1){
                    int y1 = y0 - rowkeep + r0;
                    int x1 = x0;
                    if(y1 > rows - 3)   y1 = rows - 3;
                    if(y1 < 0){
                        y1 = 0;
                        x1 = 0;
                    }
                    highlighter(y1, x1, cuy, cux, filepath, rowkeep);
                    move(cuy, cux + dig(rowkeep + cuy) + 2);
                    char t = getch();
                    if(t == 'q'){
                        exit = 1;
                        break;
                    }
                    if(t == 27){
                        mode = 1;
                        break;
                    }
                    if(t == 'i'){
                        mode = 3;
                        break;
                    }
                    if(t == 'k'){
                        if(cuy != 3 && cuy != 0){
                            cuy--;
                            if(rowlen(filepath, cuy + rowkeep) >= cux + 1)    move(cuy, cux + dig(rowkeep + cuy) + 2);
                            else{
                                cux = rowlen(filepath, cuy + rowkeep) - 1;
                                move(cuy, cux + dig(rowkeep + cuy) + 2);//for when you go to a short line
                            }
                        }
                        else if(rowkeep > 1){
                            rowkeep--;
                            for(int s = 0; s < 21; s++){
                                move(s, 0);
                                emp(s, cols);
                                printw("%d  ", rowkeep + s);
                                rowprint(rowkeep + s, filepath);
                            }
                            if(rowlen(filepath, cuy + rowkeep) >= cux + 1)    move(cuy, cux + dig(rowkeep + cuy) + 2);
                            else{
                                cux = rowlen(filepath, cuy + rowkeep) - 1;
                                move(cuy, cux + dig(rowkeep + cuy) + 2);//for when you go to a short line
                            }
                        }
                        else if(cuy != 0){
                            cuy--;
                            if(rowlen(filepath, cuy + rowkeep) >= cux + 1)    move(cuy, cux + dig(rowkeep + cuy) + 2);
                            else{
                                cux = rowlen(filepath, cuy + rowkeep) - 1;
                                move(cuy, cux + dig(rowkeep + cuy) + 2);//for when you go to a short line
                            }
                        }
                    }
                    if(t == 'j'){
                        if(cuy != rows - 6 && cuy != 20){
                            cuy++;
                            if(rowlen(filepath, cuy + rowkeep) >= cux + 1)    move(cuy, cux + dig(rowkeep + cuy) + 2);
                            else{
                                cux = rowlen(filepath, cuy + rowkeep) - 1;
                                move(cuy, cux + dig(rowkeep + cuy) + 2);//for when you go to a short line
                            }
                        }
                        else if(rowkeep < totl(filepath) - 20){
                            rowkeep++;
                            for(int s = 0; s < 21; s++){
                                move(s, 0);
                                emp(s, cols);
                                printw("%d  ", rowkeep + s);
                                rowprint(rowkeep + s, filepath);
                            }
                            if(rowlen(filepath, cuy + rowkeep) >= cux + 1)    move(cuy, cux + dig(rowkeep + cuy) + 2);
                            else{
                                cux = rowlen(filepath, cuy + rowkeep) - 1;
                                move(cuy, cux + dig(rowkeep + cuy) + 2);//for when you go to a short line
                            }
                        }
                        else if(cuy != 20){
                            cuy++;
                            if(rowlen(filepath, cuy + rowkeep) >= cux + 1)    move(cuy, cux + dig(rowkeep + cuy) + 2);
                            else{
                                cux = rowlen(filepath, cuy + rowkeep) - 1;
                                move(cuy, cux + dig(rowkeep + cuy) + 2);//for when you go to a short line
                            }
                        }
                    }
                    if(t == 'h'){
                        if(cux > 0){
                            cux--;
                            move(cuy, cux + dig(rowkeep + cuy) + 2);
                        }
                    }
                    if(t == 'l'){
                        if(cux < rowlen(filepath, cuy + rowkeep) - 1)    cux++;
                        move(cuy, cux + dig(rowkeep + cuy) + 2);
                    }
                    
                    if(t == 'y'){
                        mode = 1;
                        coopy(x0, y0, cux, cuy, filepath, clipboard);
                        break;
                    }
                    if(t == 'd'){
                        mode = 1;
                        coopy(x0, y0, cux, cuy, filepath, clipboard);
                        int mx, my, MX, MY;
                        if(cuy > y0){MY = cuy; MX = cux; my = y0; mx = x0;}
                        else if(cuy < y0){MY = y0; MX = x0; my = cuy; mx = cux;}
                        else if(x0 > cux){MY = my = y0; MX = x0; mx = cux;}
                        else{MY = my = y0; MX = cux; mx = x0;}
                        rremove(filepath, my + 1, mx, posfind(MY, MX, filepath) - posfind(my, mx, filepath) + 1, 'f');
                        FILE *fff;
                        fff = fopen(filepath, "r+");
                        attron(COLOR_PAIR(1));
                        for(int i = 0; i < rows - 2; i++){
                            emp(i, cols);
                            printw("%d  ", i + rowkeep);
                            int j = gotoline(i + rowkeep, filepath);
                            fseek(fff, j, SEEK_SET);
                            if(i + 1 != totl(filepath)){
                                while(1){
                                    char c = fgetc(fff);
                                    if(c != EOF && c != '\0')   printw("%c", c);
                                    if(c == '\n' || c == EOF || c == '\0')   break;
                                }
                            }
                            else{
                                for(int q = 0; q < cols; q++){
                                    char c = fgetc(fff);
                                    if(q < rowlen(filepath, i + 1))    printw("%c", c);
                                    else    printw(" ");
                                }
                            }
                            if(i + 1 > totl(filepath)){
                                emp(i, cols);
                            }
                        }
                        fclose(fff);
                        attroff(COLOR_PAIR(1));
                        cux = x0;
                        cuy = y0;
                        move(cuy, cux + dig(rowkeep + cuy) + 2);
                        break;
                    }
                    undohighlighter(filepath, rowkeep);
                }
                undohighlighter(filepath, rowkeep);
                if(exit == 1)   break;
            }
            if(mode == 3){//insert mode
                int x0 = cux;
                int y0 = cuy;
                move(rows - 1, 0);
                attron(COLOR_PAIR(2));
                printw(" INSERT ");
                attroff(COLOR_PAIR(2));
                move(cuy, cux + dig(rowkeep + cuy) + 2);
                while(1){
                    char t = getch();
                    if(t == 27){
                        mode = 1;
                        break;
                    }
                    Insert(filepath, &t, cuy + rowkeep, cux, 0);
                    FILE *fff;
                    fff = fopen(filepath, "r+");
                    attron(COLOR_PAIR(1));
                    for(int i = 0; i < rows - 2; i++){
                        emp(i, cols);
                        printw("%d  ", i + rowkeep);
                        int j = gotoline(i + rowkeep, filepath);
                        fseek(fff, j, SEEK_SET);
                        if(i + 1 != totl(filepath)){
                            while(1){
                                char c = fgetc(fff);
                                if(c != EOF && c != '\0')   printw("%c", c);
                                if(c == '\n' || c == EOF || c == '\0')   break;
                            }
                        }
                        else{
                            for(int q = 0; q < cols; q++){
                                char c = fgetc(fff);
                                if(q < rowlen(filepath, i + 1))    printw("%c", c);
                                else    printw(" ");
                            }
                        }
                        if(i + 1 > totl(filepath)){
                            emp(i, cols);
                        }
                    }
                    fclose(fff);
                    attroff(COLOR_PAIR(1));
                    is_save(realpath);
                    cux++;
                    move(cuy, cux + dig(rowkeep + cuy) + 2);
                }
            }
        }
    }








    

    endwin();





/*
    char command[100];
    gets(command);

    //arman command
    int arman = 0;
    char command1[1000];
    char command2[1000];
    int i = 0;
    while(command[i] != '\0' && command[i] != '\n'){
        if(command[i] == ' ' && command[i + 1] == '=' && command[i + 2] == 'D' && command[i + 3] == ' ')    arman = 1;
        if(arman == 1){
            for(int j = 0; j < i; j++){
                command1[j] = command[j];
            }
            command1[i] = '\0';
            for(int j = i + 4; j < strlen(command); j++){
                command2[j - i - 4] = command[j];
            }
            command2[strlen(command) - strlen(command1) - 4] = '\0';
            break;
        }
        i++;
    }
    if(validate(command1) == 1 && validate(command2) == 1 && arman == 1){
        char input[1000];
        strcpy(input, "\0");

        //cat command
        if(Is_sub(command1, "cat --file ", 11) == 0){
            char path[100];
            int i = 0;
            while(command1[i + 11] != '\0' && command1[i + 11] != '\n'){
                path[i] = command1[i + 11];
                i++;
            }
            path[i] = command1[i + 11];
            cat_arman(path, input);
        }
        //comparator command
        if(Is_sub(command1, "compare ", 8) == 0){
            char path1[100];
            char path2[100];
            int i = 0;
            if(command[8] != '"'){
                while(command[i + 8] != ' '){
                    path1[i] = command[i + 8];
                    i++;
                }
                path1[i] = '\0';
                i++;
                int j = 0;
                while(!(command[i + 8 + j] == ' ' && command[i + 9 + j] == '=' && command[i + 10 + j] == 'D' && command[i + 11 + j] == ' ')){
                    path2[j] = command[i + j + 8];
                    j++;
                }
                path2[j] = '\0';
            }
            if(command[8] == '"'){}
            comparator_arman(path1, path2, input);
        }
        //tree command
        else if(Is_sub(command1, "tree ", 5) == 0){
            char d[1000];
            int i = 0;
            while(command1[i + 5] != '\0' && command1[i + 5] != '\n'){
                d[i] = command1[i + 5];
                i++;
            }
            d[i] = '\0';
            int x = atoi(d);
            char loc[1000];
            getcwd(loc, sizeof(loc));

            if(x < -1)  strcat(input, "Invalid depth");
            else    dirtree_arman(loc, 0, x - 1, input);
        }
        //grep command
        else if(Is_sub(command1, "grep --str ", 11) == 0){
            char path[100];
            char str1[100];

            int j = 0;
            int giome = 0;
            if(command1[11] == '"')   giome = 1;
            int s = 0;
            while(!(command1[j + 13] == '-' && command1[j + 12] == ' ')){
                str1[j - s] = command1[j + 11];
                if(str1[j - s] == 'n' && j - s > 0 && str1[j - 1 - s] == '\\' && (str1[j - 2 - s] != '\\' || j - s == 1)){
                    str1[j - 1 - s] = '\n';
                    s++;
                }
                if(str1[j - s] == 'n' && j - s > 1 && str1[j - s - 1] == '\\' && str1[j - s - 2] == '\\'){
                    str1[j - s - 1] = 'n';
                    s++;
                }
                j++;
            }
            if(giome == 0)  str1[j - s] = command1[j + 11];
            if(giome == 1)  str1[j - s] = '\0';
            str1[j + 1 - s] = '\0';

            if(giome == 1){
                int h = 0;
                while(str1[h] != '\0'){
                    str1[h] = str1[h + 1];
                    h++;
                }
            }
            //// str corrected

            j += 21;
            int i = 0;
            int x = 1;
            int count = 0;
            while(x == 1){
                //ith file
                char file[100];
                int k = 0;
                if(command1[j] == '"'){
                    j++;
                    while(command1[j] != '"' && command1[j] != '\0'){
                        file[k] = command1[j];
                        k++;
                        j++;
                    }
                    file[k] = '\0';
                    j++;
                }
                else if(command1[j] == '/'){
                    j++;
                    while(command1[j] != ' ' && command1[j] != '\0'){
                        file[k] = command1[j];
                        j++;
                        k++;
                    }
                    file[k] = '\0';
                }
                else{
                    while(command1[j] != ' ' && command1[j] != '\0'){
                        file[k] = command1[j];
                        k++;
                        j++;
                    }
                    file[k] = '\0';
                }
                if(command1[j] == '\0')   x = 0;
                if(!exist(file)){
                    strcat(input, "unknown file");
                    return 0;
                }
                ////
                FILE *fff;
                fff = fopen(file, "r+");
                int line = 1;
                char c;
                int i = 0;
                int L = 0;
                int again = 0;
                fseek(fff, 0, SEEK_END);
                int size_file = ftell(fff);
                fseek(fff, i, SEEK_SET);
                while(i < size_file){
                    fseek(fff, i, SEEK_SET);
                    c = fgetc(fff);
                    if(c == '\n'){
                        L = i + 2;
                        again = 0;
                        i += 2;
                        continue;
                    }
                    if(c == str1[0] && again == 0){
                        int ans = 1;
                        for(int o = 1; o < strlen(str1); o++){
                            if(fgetc(fff) != str1[o]){
                                ans = 0;
                                break;
                            }
                        }
                        if(ans == 1){
                            again = 1;
                            fseek(fff, L, SEEK_SET);
                            char t;
                            strcat(input, file);
                            strcat(input, ": ");
                            while(1){
                                t = fgetc(fff);
                                if(t == EOF)    t = '\n';
                                strncat(input, &t, 1);
                                if(t == '\n')   break;
                            }
                        }
                    }
                    i++;
                }
                j++;
                fclose(fff);
            }
        }
        //grep c option
        else if(Is_sub(command1, "grep -c --str ", 14) == 0){
            char path[100];
            char str1[100];

            int j = 0;
            int giome = 0;
            if(command1[14] == '"')   giome = 1;
            int s = 0;
            while(!(command1[j + 16] == '-' && command1[j + 15] == ' ')){
                str1[j - s] = command1[j + 14];
                if(str1[j - s] == 'n' && j - s > 0 && str1[j - 1 - s] == '\\' && (str1[j - 2 - s] != '\\' || j - s == 1)){
                    str1[j - 1 - s] = '\n';
                    s++;
                }
                if(str1[j - s] == 'n' && j - s > 1 && str1[j - s - 1] == '\\' && str1[j - s - 2] == '\\'){
                    str1[j - s - 1] = 'n';
                    s++;
                }
                j++;
            }
            if(giome == 0)  str1[j - s] = command1[j + 14];
            if(giome == 1)  str1[j - s] = '\0';
            str1[j + 1 - s] = '\0';

            if(giome == 1){
                int h = 0;
                while(str1[h] != '\0'){
                    str1[h] = str1[h + 1];
                    h++;
                }
            }
            //// str corrected

            j += 24;
            int i = 0;
            int x = 1;
            int count = 0;
            while(x == 1){
                //ith file
                char file[100];
                int k = 0;
                if(command1[j] == '"'){
                    j++;
                    while(command1[j] != '"' && command1[j] != '\0'){
                        file[k] = command1[j];
                        k++;
                        j++;
                    }
                    file[k] = '\0';
                    j++;
                }
                else if(command1[j] == '/'){
                    j++;
                    while(command1[j] != ' ' && command1[j] != '\0'){
                        file[k] = command1[j];
                        j++;
                        k++;
                    }
                    file[k] = '\0';
                }
                else{
                    while(command1[j] != ' ' && command1[j] != '\0'){
                        file[k] = command1[j];
                        k++;
                        j++;
                    }
                    file[k] = '\0';
                }
                if(command1[j] == '\0')   x = 0;
                if(!exist(file)){
                    strcat(input, "unknown file");
                    return 0;
                }
                ////
                FILE *fff;
                fff = fopen(file, "r+");
                int line = 1;
                char c;
                int i = 0;
                int L = 0;
                int again = 0;
                fseek(fff, 0, SEEK_END);
                int size_file = ftell(fff);
                fseek(fff, i, SEEK_SET);
                while(i < size_file){
                    fseek(fff, i, SEEK_SET);
                    c = fgetc(fff);
                    if(c == '\n'){
                        L = i + 2;
                        again = 0;
                        i += 2;
                        continue;
                    }
                    if(c == str1[0] && again == 0){
                        int ans = 1;
                        for(int o = 1; o < strlen(str1); o++){
                            if(fgetc(fff) != str1[o]){
                                ans = 0;
                                break;
                            }
                        }
                        if(ans == 1){
                            again = 1;
                            fseek(fff, L, SEEK_SET);
                            count++;
                        }
                    }
                    i++;
                }
                j++;
                fclose(fff);
            }
            char countstr[1000];
            sprintf(countstr, "%d", count);
            strcat(input, countstr);
        }
        //grep l option
        else if(Is_sub(command1, "grep -l --str ", 14) == 0){
            char path[100];
            char str1[100];

            int j = 0;
            int giome = 0;
            if(command1[14] == '"')   giome = 1;
            int s = 0;
            while(!(command1[j + 16] == '-' && command1[j + 15] == ' ')){
                str1[j - s] = command1[j + 14];
                if(str1[j - s] == 'n' && j - s > 0 && str1[j - 1 - s] == '\\' && (str1[j - 2 - s] != '\\' || j - s == 1)){
                    str1[j - 1 - s] = '\n';
                    s++;
                }
                if(str1[j - s] == 'n' && j - s > 1 && str1[j - s - 1] == '\\' && str1[j - s - 2] == '\\'){
                    str1[j - s - 1] = 'n';
                    s++;
                }
                j++;
            }
            if(giome == 0)  str1[j - s] = command1[j + 14];
            if(giome == 1)  str1[j - s] = '\0';
            str1[j + 1 - s] = '\0';

            if(giome == 1){
                int h = 0;
                while(str1[h] != '\0'){
                    str1[h] = str1[h + 1];
                    h++;
                }
            }
            //// str corrected

            j += 24;
            int i = 0;
            int x = 1;
            int count = 0;
            while(x == 1){
                //ith file
                char file[100];
                int k = 0;
                if(command1[j] == '"'){
                    j++;
                    while(command1[j] != '"' && command1[j] != '\0'){
                        file[k] = command1[j];
                        k++;
                        j++;
                    }
                    file[k] = '\0';
                    j++;
                }
                else if(command1[j] == '/'){
                    j++;
                    while(command1[j] != ' ' && command1[j] != '\0'){
                        file[k] = command1[j];
                        j++;
                        k++;
                    }
                    file[k] = '\0';
                }
                else{
                    while(command1[j] != ' ' && command1[j] != '\0'){
                        file[k] = command1[j];
                        k++;
                        j++;
                    }
                    file[k] = '\0';
                }
                if(command1[j] == '\0')   x = 0;
                if(!exist(file)){
                    strcat(input, "unknown file");
                    return 0;
                }
                ////
                FILE *fff;
                fff = fopen(file, "r+");
                int line = 1;
                char c;
                int i = 0;
                int L = 0;
                int again = 0;
                fseek(fff, 0, SEEK_END);
                int size_file = ftell(fff);
                fseek(fff, i, SEEK_SET);
                while(i < size_file){
                    fseek(fff, i, SEEK_SET);
                    c = fgetc(fff);
                    if(c == '\n'){
                        L = i + 2;
                        //again = 0;
                        i += 2;
                        continue;
                    }
                    if(c == str1[0] && again == 0){
                        int ans = 1;
                        for(int o = 1; o < strlen(str1); o++){
                            if(fgetc(fff) != str1[o]){
                                ans = 0;
                                break;
                            }
                        }
                        if(ans == 1){
                            again = 1;
                            fseek(fff, L, SEEK_SET);
                            count++;
                            strcat(input, file);
                            strcat(input, "\n");
                        }
                    }
                    i++;
                }
                j++;
                fclose(fff);
            }
        }
        //find command
        else if(Is_sub(command1, "find --str ", 11) == 0){
            char path[100];
            char str[100];
            char prop;
            int at = 0;
            char att[100];

            int j = 0;
            int giome = 0;
            if(command1[11] == '"')   giome = 1;
            int s = 0;
            while(command1[j + 13] != '-'){
                str[j - s] = command1[j + 11];
                if(str[j - s] == 'n' && j - s > 0 && str[j - 1 - s] == '\\' && (str[j - 2 - s] != '\\' || j - s == 1)){
                    str[j - 1 - s] = '\n';
                    s++;
                }
                if(str[j - s] == 'n' && j - s > 1 && str[j - s - 1] == '\\' && str[j - s - 2] == '\\'){
                    str[j - s - 1] = 'n';
                    s++;
                }
                j++;
            }
            if(giome == 0)  str[j - s] = command1[j + 11];
            if(giome == 1)  str[j - s] = '\0';
            str[j + 1 - s] = '\0';

            j += 20;
            int i = 0;
            while(command1[i + j] != '\0' && command1[i + j] != '\n' && !(command1[i + j] == ' ' && command1[i + j + 1] == '-')){
                path[i] = command1[i + j];
                i++;
            }
            if(!(command1[i + j] == ' ' && command1[i + j + 1] == '-')){
                path[i] = command1[i + j];
                path[i + 1] = '\0';
                prop = 'n';
            }
            //else if there are properties
            else if(command1[i + j] == ' ' && command1[i + j + 1] == '-'){
                int k = 0;
                char prop1[100];
                while(command1[i + j + 2 + k] != '\0' && command1[i + j + 2 + k] != '\n' && !(command1[i + j + 2 + k] == ' ' && command1[i + j + 1 + k] == 't' && command1[i + j + k] == 'a')){
                    prop1[k] = command1[i + j + 2 + k];
                    k++;
                }
                prop1[k] = '\0';
                if((prop1[0] == 'a' && prop1[1] == 't' && prop1[2] == '\0') || (prop1[0] == 'b' && prop1[1] == 'y' && prop1[2] == 'w' && prop1[3] == 'o' && prop1[4] == 'r' && prop1[5] == 'd' && prop1[6] == ' ' && prop1[7] == '-' && prop1[8] == 'a' && prop1[9] == 't')){
                    int o = 0;
                    while(command1[i + j + 3 + k + o] != '\0' && command1[i + j + 3 + k + o] != ' ' && command1[i + j + 3 + k + o] != '\n'){
                        att[o] = command1[i + j + 3 + k + o];
                        o++;
                    }
                    at = atoi(att);
                    if(command1[i + j + 3 + k + o] == ' ' && command1[i + j + 4 + k + o] == '-' && command1[i + j + 5 + k + o] == 'b' && command1[i + j + 6 + k + o] == 'y' && command1[i + j + 7 + k + o] == 'w' && command1[i + j + 8 + k + o] == 'o' && command1[i + j + 9 + k + o] == 'r' && command1[i + j + 10 + k + o] == 'd' && command1[i + j + 11 + k + o] == '\0')    prop = 't';
                    else if(strcmp(prop1, "at") == 0)    prop = 'a';
                    else if(strcmp(prop1, "byword -at") == 0)   prop = 't';
                }
                else if(prop1[0] == 'c' && prop1[1] == 'o' && prop1[2] == 'u' && prop1[3] == 'n' && prop1[4] == 't' && prop1[5] == '\0')    prop = 'c';
                else if(prop1[0] == 'b' && prop1[1] == 'y' && prop1[2] == 'w' && prop1[3] == 'o' && prop1[4] == 'r' && prop1[5] == 'd' && prop1[6] == '\0')    prop = 'b';
                else if(prop1[0] == 'a' && prop1[1] == 'l' && prop1[2] == 'l' && prop1[3] == '\0')    prop = 'l';
                else if(strcmp(prop1, "byword -all") == 0 || strcmp(prop1, "all -byword") == 0)    prop = 's';
                else{
                    printf("Unvalid property");
                    return 0;
                }
            }////

            if(giome == 1){
                int h = 0;
                while(str[h] != '\0'){
                    str[h] = str[h + 1];
                    h++;
                }
            }
            //// str corrected

            //finding * in the str
            int l = strlen(str);
            int e = 0;
            int mode = 1;
            if(str[0] == '*'){
                int k = 0;
                while(str[k + 1] != '\0'){
                    str[k] = str[k + 1];
                    k++;
                }
                str[k] = '\0';
                mode = 2;
            }
            else if(str[l - 1] == '*' && str[l - 2] != '\\'){
                str[l - 1] = '\0';
                mode = 3;
            }
            else if(str[l - 1] == '*' && str[l - 2] == '\\'){
                mode = 1;
                e = 1;
            }

            //clearing the \*
            if(e == 1)   l++;
            int setare[l - 1];
            for(int r = 0; r < l - 1; r++){
                setare[r] = 0;
            }
            for(int u = 1; u < l - 1; u++){
                if(str[u] == '*' && str[u - 1] == '\\')    setare[u] = 1;
                }
            int s_holder = 0;
            for(int u = 1; u < l - 1; u++){
                if(setare[u] == 1){
                    for(int h = u - s_holder; h <= l - s_holder - 2; h++){
                        str[h - 1] = str[h];
                        str[l - 2 - s_holder] = '\0';
                    }
                    s_holder++;
                }
            }////

            if(prop != 'l' && prop != 's'){
                int x = ffind(path, str, mode, prop, at);
                char xstr[100];
                sprintf(xstr, "%d", x);
                if(x == -2)    strcat(input, "the file is not found");
                else strcat(input, xstr);
            }
            else    ffind_arman(path, str, mode, prop, at, input);
        }


        ////
        //insert command
        if(Is_sub(command2, "insertstr --file ", 17) == 0){
            char path[100];
            char str[1000];
            char pos1[100];
            char pos2[100];
            int i = 0;
            while(command2[i + 19] != '-'){
                path[i] = command2[i + 17];
                i++;
            }
            path[i] = command2[i + 17];
            path[i + 1] = '\0';
            i += 24;
            for(int j = 0; j <= strlen(input); j++){
                str[j] = input[j];
            }
            int k = 0;
            while(command2[k + i] != ':'){
                pos1[k] = command2[k + i];
                k++;
            }
//            pos1[k] = '\0';
            k++;
            int l = 0;
            while(command2[l + i + k] != '\0' && command2[l + i + k] != '\n'){
                pos2[l] = command2[l + i + k];
                l++;
            }
//            pos2[l] = '\0';
            int x = atoi(pos1);
            int y = atoi(pos2);

            Insert(path, str, x, y, 0);
        }
        //find command
        else if(Is_sub(command2, "find --file ", 12) == 0){
            char path[100];
            char prop;
            int at = 0;
            char att[100];

            int j = 12;
            int i = 0;
            while(command2[i + j] != '\0' && command2[i + j] != '\n' && !(command2[i + j] == ' ' && command2[i + j + 1] == '-')){
                if(command2[i + j] > 126 || command2[i + j] < 0)    break;
                path[i] = command2[i + j];
                i++;
            }
            path[i] = '\0';
            if(!(command2[i + j] == ' ' && command2[i + j + 1] == '-')){
                prop = 'n';
            }

            //else if there are properties
            else if(command2[i + j] == ' ' && command2[i + j + 1] == '-'){
                int k = 0;
                char prop1[100];
                while(command2[i + j + 2 + k] != '\0' && command2[i + j + 2 + k] != '\n' && !(command2[i + j + 2 + k] == ' ' && command2[i + j + 1 + k] == 't' && command2[i + j + k] == 'a')){
                    if(command2[i + j + k + 2] < 0)    break;
                    prop1[k] = command2[i + j + 2 + k];
                    k++;
                }
                prop1[k] = '\0';
                if((prop1[0] == 'a' && prop1[1] == 't' && prop1[2] == '\0') || (prop1[0] == 'b' && prop1[1] == 'y' && prop1[2] == 'w' && prop1[3] == 'o' && prop1[4] == 'r' && prop1[5] == 'd' && prop1[6] == ' ' && prop1[7] == '-' && prop1[8] == 'a' && prop1[9] == 't')){
                    int o = 0;
                    while(command2[i + j + 3 + k + o] != '\0' && command2[i + j + 3 + k + o] != ' ' && command2[i + j + 3 + k + o] != '\n'){
                        att[o] = command2[i + j + 3 + k + o];
                        o++;
                    }
                    at = atoi(att);
                    if(command2[i + j + 3 + k + o] == ' ' && command2[i + j + 4 + k + o] == '-' && command2[i + j + 5 + k + o] == 'b' && command2[i + j + 6 + k + o] == 'y' && command2[i + j + 7 + k + o] == 'w' && command2[i + j + 8 + k + o] == 'o' && command2[i + j + 9 + k + o] == 'r' && command2[i + j + 10 + k + o] == 'd' && command2[i + j + 11 + k + o] == '\0')    prop = 't';
                    else if(strcmp(prop1, "at") == 0)    prop = 'a';
                    else if(strcmp(prop1, "byword -at") == 0)   prop = 't';
                }
                else if(prop1[0] == 'c' && prop1[1] == 'o' && prop1[2] == 'u' && prop1[3] == 'n' && prop1[4] == 't' && prop1[5] == '\0')    prop = 'c';
                else if(prop1[0] == 'b' && prop1[1] == 'y' && prop1[2] == 'w' && prop1[3] == 'o' && prop1[4] == 'r' && prop1[5] == 'd' && prop1[6] == '\0')    prop = 'b';
                else if(prop1[0] == 'a' && prop1[1] == 'l' && prop1[2] == 'l' && prop1[3] == '\0')    prop = 'l';
                else if(strcmp(prop1, "byword -all") == 0 || strcmp(prop1, "all -byword") == 0)    prop = 's';
                else{
                    printf("Unvalid property");
                    return 0;
                }
            }////

            if(input[0] == '"'){
                int h = 0;
                while(input[h] != '\0'){
                    input[h] = input[h + 1];
                    h++;
                }
            }
            //// str(input) corrected

            //finding * in the str
            int l = strlen(input);
            int e = 0;
            int mode = 1;
            if(input[0] == '*'){
                int k = 0;
                while(input[k + 1] != '\0'){
                    input[k] = input[k + 1];
                    k++;
                }
                input[k] = '\0';
                mode = 2;
            }
            else if(input[l - 1] == '*' && input[l - 2] != '\\'){
                input[l - 1] = '\0';
                mode = 3;
            }
            else if(input[l - 1] == '*' && input[l - 2] == '\\'){
                mode = 1;
                e = 1;
            }

            //clearing the \*
            if(e == 1)   l++;
            int setare[l - 1];
            for(int r = 0; r < l - 1; r++){
                setare[r] = 0;
            }
            for(int u = 1; u < l - 1; u++){
                if(input[u] == '*' && input[u - 1] == '\\')    setare[u] = 1;
                }
            int s_holder = 0;
            for(int u = 1; u < l - 1; u++){
                if(setare[u] == 1){
                    for(int h = u - s_holder; h <= l - s_holder - 2; h++){
                        input[h - 1] = input[h];
                        input[l - 2 - s_holder] = '\0';
                    }
                    s_holder++;
                }
            }////

            int x = ffind(path, input, mode, prop, at);
            if(x == -2)    printf("The file is not found");
            else if(prop != 'l' && prop != 's')    printf("%d", x);
        }
        //grep command
        else if(Is_sub(command2, "grep --files ", 13) == 0){
        char path[100];

        int j = 0;
        j += 13;
        int i = 0;
        int x = 1;
        int count = 0;
        while(x == 1){
            //ith file
            char file[100];
            int k = 0;
            if(command2[j] == '"'){
                j++;
                while(command2[j] != '"' && command2[j] != '\0'){
                    file[k] = command2[j];
                    k++;
                    j++;
                }
                file[k] = '\0';
                j++;
            }
            else if(command2[j] == '/'){
                j++;
                while(command2[j] != ' ' && command2[j] != '\0'){
                    file[k] = command2[j];
                    j++;
                    k++;
                }
                file[k] = '\0';
            }
            else{
                while(command2[j] != ' ' && command2[j] != '\0'){
                    file[k] = command2[j];
                    k++;
                    j++;
                }
                file[k] = '\0';
            }
            if(command2[j] == '\0')   x = 0;
            if(!exist(file)){
                printf("unknown file");
                return 0;
            }
            ////
            FILE *fff;
            fff = fopen(file, "r+");
            int line = 1;
            char c;
            int i = 0;
            int L = 0;
            int again = 0;
            fseek(fff, 0, SEEK_END);
            int size_file = ftell(fff);
            fseek(fff, i, SEEK_SET);
            while(i < size_file){
                fseek(fff, i, SEEK_SET);
                c = fgetc(fff);
                if(c == '\n'){
                    L = i + 2;
                    again = 0;
                    i += 2;
                    continue;
                }
                if(c == input[0] && again == 0){
                    int ans = 1;
                    for(int o = 1; o < strlen(input); o++){
                        if(fgetc(fff) != input[o]){
                            ans = 0;
                            break;
                        }
                    }
                    if(ans == 1){
                        again = 1;
                        fseek(fff, L, SEEK_SET);
                        char t;
                        printf("%s: ", file);
                        while(1){
                            t = fgetc(fff);
                            if(t == EOF)    t = '\n';
                            printf("%c", t);
                            if(t == '\n')   break;
                        }
                    }
                }
                i++;
            }
            j++;
            fclose(fff);
        }
    }

        //return 0;
    }
    else if(arman == 1 && validate(command1) == 0)  printf("first command is Invalid");
    else if(arman == 1 && validate(command2) == 0)  printf("second command is Invalid");

    else{
    //creatfile command
    if(Is_sub(command, "createfile --file ", 18) == 0){
        char path[100];
        int i = 0;
        while(command[i + 18] != '\0' && command[i + 18] != '\n'){
            path[i] = command[i + 18];
            i++;
        }
        path[i] = command[i + 18];
        createfile(path);
    }
    //cat command
    else if(Is_sub(command, "cat --file ", 11) == 0){
        char path[100];
        int i = 0;
        while(command[i + 11] != '\0' && command[i + 11] != '\n'){
            path[i] = command[i + 11];
            i++;
        }
        path[i] = command[i + 11];
        cat(path);
    }
    //insert command
    else if(Is_sub(command, "insertstr --file ", 17) == 0){
        char path[100];
        char str[1000];
        char pos1[100];
        char pos2[100];
        int i = 0;
        while(command[i + 19] != '-'){
            path[i] = command[i + 17];
            i++;
        }
        path[i] = command[i + 17];
        path[i + 1] = '\0';
        i += 25;
        int j = 0;
        int giome = 0;
        if(command[i] == '"')   giome = 1;
        int s = 0;
        while(command[j + i + 2] != '-'){
            str[j - s] = command[j + i];
            if(str[j - s] == 'n' && j - s > 0 && str[j - 1 - s] == '\\' && (str[j - 2 - s] != '\\' || j - s == 1)){
                str[j - 1 - s] = '\n';
                s++;
            }
            if(str[j - s] == 'n' && j - s > 1 && str[j - s - 1] == '\\' && str[j - s - 2] == '\\'){
                str[j - s - 1] = 'n';
                s++;
            }
            j++;
        }
        if(giome == 0)  str[j - s] = command[j + i];
        if(giome == 1)  str[j - s] = '\0';
        str[j + 1 - s] = '\0';
        j += 8;
        int k = 0;
        while(command[k + j + i] != ':'){
            pos1[k] = command[k + j + i];
            k++;
        }
        k++;
        int l = 0;
        while(command[l + i + j + k] != '\0' && command[l + i + j + k] != '\n'){
            pos2[l] = command[l + i + j + k];
            l++;
        }
        int x = atoi(pos1);
        int y = atoi(pos2);

        Insert(path, str, x, y, giome);
    }

    //remove command
    else if(Is_sub(command, "removestr --file ", 17) == 0){
        char path[100];
        char str[100];
        char pos1[100];
        char pos2[100];
        char flag;
        char size[100];
        int i = 0;
        while(command[i + 19] != '-'){
            path[i] = command[i + 17];
            i++;
        }
        path[i] = command[i + 17];
        path[i + 1] = '\0';
        i += 17;
        int j = 0;
        j += 8;
        int k = 0;
        while(command[k + j + i] != ':'){
            pos1[k] = command[k + j + i];
            k++;
        }
        k++;
        int l = 0;
        while(command[l + i + j + k] != ' '){
            pos2[l] = command[l + i + j + k];
            l++;
        }
        int x = atoi(pos1);
        int y = atoi(pos2);

        l += 7;
        int o = 0;
        while(command[l + i + j + k + o] != ' '){
            size[o] = command[i + j + k + l + o];
            o++;
        }
        int z = atoi(size);
        o += 2;
        flag = command[i + j + k + l + o];

        rremove(path, x, y, z, flag);
    }

    //copy command
    /*else if(Is_sub(command, "copystr --file ", 15) == 0){
        char path[100];
        char str[100];
        char pos1[100];
        char pos2[100];
        char flag;
        char size[100];
        int i = 0;
        while(command[i + 17] != '-'){
            path[i] = command[i + 15];
            i++;
        }
        path[i] = command[i + 15];
        path[i + 1] = '\0';
        i += 15;
        int j = 0;
        j += 8;
        int k = 0;
        while(command[k + j + i] != ':'){
            pos1[k] = command[k + j + i];
            k++;
        }
        k++;
        int l = 0;
        while(command[l + i + j + k] != ' '){
            pos2[l] = command[l + i + j + k];
            l++;
        }
        int x = atoi(pos1);
        int y = atoi(pos2);

        l += 7;
        int o = 0;
        while(command[l + i + j + k + o] != ' '){
            size[o] = command[i + j + k + l + o];
            o++;
        }
        int z = atoi(size);
        o += 2;
        flag = command[i + j + k + l + o];

        ccopy(path, x, y, z, flag);
    }*/

    //cut command
    /*else if(Is_sub(command, "cutstr --file ", 14) == 0){
        char path[100];
        char str[100];
        char pos1[100];
        char pos2[100];
        char flag;
        char size[100];
        int i = 0;
        while(command[i + 16] != '-'){
            path[i] = command[i + 14];
            i++;
        }
        path[i] = command[i + 14];
        path[i + 1] = '\0';
        i += 14;
        int j = 0;
        j += 8;
        int k = 0;
        while(command[k + j + i] != ':'){
            pos1[k] = command[k + j + i];
            k++;
        }
        k++;
        int l = 0;
        while(command[l + i + j + k] != ' '){
            pos2[l] = command[l + i + j + k];
            l++;
        }
        int x = atoi(pos1);
        int y = atoi(pos2);

        l += 7;
        int o = 0;
        while(command[l + i + j + k + o] != ' '){
            size[o] = command[i + j + k + l + o];
            o++;
        }
        int z = atoi(size);
        o += 2;
        flag = command[i + j + k + l + o];

        ccopy(path, x, y, z, flag);
        rremove(path, x, y, z, flag);
    }*/

    //paste command
    /*else if(Is_sub(command, "pastestr --file ", 16) == 0){
        char path[100];
        char pos1[100];
        char pos2[100];
        int i = 0;
        while(command[i + 18] != '-'){
            path[i] = command[i + 16];
            i++;
        }
        path[i] = command[i + 16];
        path[i + 1] = '\0';
        i += 24;
        int k = 0;
        while(command[k + i] != ':'){
            pos1[k] = command[k + i];
            k++;
        }
        k++;
        int l = 0;
        while(command[l + i + k] != '\0' && command[l + i + k] != NULL && command[l + i + k] != '\n'){
            pos2[l] = command[l + i + k];
            l++;
        }
        int x = atoi(pos1);
        int y = atoi(pos2);

        char *str;
        OpenClipboard(0);
        HANDLE in = GetClipboardData(CF_TEXT);
        str = (char*)in;
        CloseClipboard();

        Insert(path, str, x, y, 0);
    }*//*

    //indent command
    else if(Is_sub(command, "auto-indent ", 12) == 0){
        char path[100];
        int i = 0;
        while(command[i + 12] != '\0' && command[i + 12] != '\n'){
            path[i] = command[i + 12];
            i++;
        }
        path[i] = command[i + 12];
        closing_pairs(path);
    }

    //comparator command
    else if(Is_sub(command, "compare ", 8) == 0){
        char path1[100];
        char path2[100];
        int i = 0;
        if(command[8] != '"'){
            while(command[i + 8] != ' '){
                path1[i] = command[i + 8];
                i++;
            }
            path1[i] = '\0';
            i++;
            int j = 0;
            while(command[i + 8 + j] != '\0' && command[i + 8 + j] != '\n'){
                path2[j] = command[i + j + 8];
                j++;
            }
            path2[j] = command[i + j + 8];
        }
        if(command[8] == '"'){}
        comparator(path1, path2);
    }

    //find command
    else if(Is_sub(command, "find --str ", 11) == 0){
        char path[100];
        char str[100];
        char prop;
        int at = 0;
        char att[100];

        int j = 0;
        int giome = 0;
        if(command[11] == '"')   giome = 1;
        int s = 0;
        while(command[j + 13] != '-'){
            str[j - s] = command[j + 11];
            if(str[j - s] == 'n' && j - s > 0 && str[j - 1 - s] == '\\' && (str[j - 2 - s] != '\\' || j - s == 1)){
                str[j - 1 - s] = '\n';
                s++;
            }
            if(str[j - s] == 'n' && j - s > 1 && str[j - s - 1] == '\\' && str[j - s - 2] == '\\'){
                str[j - s - 1] = 'n';
                s++;
            }
            j++;
        }
        if(giome == 0)  str[j - s] = command[j + 11];
        if(giome == 1)  str[j - s] = '\0';
        str[j + 1 - s] = '\0';

        j += 20;
        int i = 0;
        while(command[i + j] != '\0' && command[i + j] != '\n' && !(command[i + j] == ' ' && command[i + j + 1] == '-')){
            path[i] = command[i + j];
            i++;
        }
        if(!(command[i + j] == ' ' && command[i + j + 1] == '-')){
            path[i] = command[i + j];
            path[i + 1] = '\0';
            prop = 'n';
        }
        //else if there are properties
        else if(command[i + j] == ' ' && command[i + j + 1] == '-'){
            int k = 0;
            char prop1[100];
            while(command[i + j + 2 + k] != '\0' && command[i + j + 2 + k] != '\n' && !(command[i + j + 2 + k] == ' ' && command[i + j + 1 + k] == 't' && command[i + j + k] == 'a')){
                prop1[k] = command[i + j + 2 + k];
                k++;
            }
            prop1[k] = '\0';
            if((prop1[0] == 'a' && prop1[1] == 't' && prop1[2] == '\0') || (prop1[0] == 'b' && prop1[1] == 'y' && prop1[2] == 'w' && prop1[3] == 'o' && prop1[4] == 'r' && prop1[5] == 'd' && prop1[6] == ' ' && prop1[7] == '-' && prop1[8] == 'a' && prop1[9] == 't')){
                int o = 0;
                while(command[i + j + 3 + k + o] != '\0' && command[i + j + 3 + k + o] != ' ' && command[i + j + 3 + k + o] != '\n'){
                    att[o] = command[i + j + 3 + k + o];
                    o++;
                }
                at = atoi(att);
                if(command[i + j + 3 + k + o] == ' ' && command[i + j + 4 + k + o] == '-' && command[i + j + 5 + k + o] == 'b' && command[i + j + 6 + k + o] == 'y' && command[i + j + 7 + k + o] == 'w' && command[i + j + 8 + k + o] == 'o' && command[i + j + 9 + k + o] == 'r' && command[i + j + 10 + k + o] == 'd' && command[i + j + 11 + k + o] == '\0')    prop = 't';
                else if(strcmp(prop1, "at") == 0)    prop = 'a';
                else if(strcmp(prop1, "byword -at") == 0)   prop = 't';
            }
            else if(prop1[0] == 'c' && prop1[1] == 'o' && prop1[2] == 'u' && prop1[3] == 'n' && prop1[4] == 't' && prop1[5] == '\0')    prop = 'c';
            else if(prop1[0] == 'b' && prop1[1] == 'y' && prop1[2] == 'w' && prop1[3] == 'o' && prop1[4] == 'r' && prop1[5] == 'd' && prop1[6] == '\0')    prop = 'b';
            else if(prop1[0] == 'a' && prop1[1] == 'l' && prop1[2] == 'l' && prop1[3] == '\0')    prop = 'l';
            else if(strcmp(prop1, "byword -all") == 0 || strcmp(prop1, "all -byword") == 0)    prop = 's';
            else{
                printf("Unvalid property");
                return 0;
            }
        }////

        if(giome == 1){
            int h = 0;
            while(str[h] != '\0'){
                str[h] = str[h + 1];
                h++;
            }
        }
        //// str corrected

        //finding * in the str
        int l = strlen(str);
        int e = 0;
        int mode = 1;
        if(str[0] == '*'){
            int k = 0;
            while(str[k + 1] != '\0'){
                str[k] = str[k + 1];
                k++;
            }
            str[k] = '\0';
            mode = 2;
        }
        else if(str[l - 1] == '*' && str[l - 2] != '\\'){
            str[l - 1] = '\0';
            mode = 3;
        }
        else if(str[l - 1] == '*' && str[l - 2] == '\\'){
            mode = 1;
            e = 1;
        }

        //clearing the \*
        if(e == 1)   l++;
        int setare[l - 1];
        for(int r = 0; r < l - 1; r++){
            setare[r] = 0;
        }
        for(int u = 1; u < l - 1; u++){
            if(str[u] == '*' && str[u - 1] == '\\')    setare[u] = 1;
            }
        int s_holder = 0;
        for(int u = 1; u < l - 1; u++){
            if(setare[u] == 1){
                for(int h = u - s_holder; h <= l - s_holder - 2; h++){
                    str[h - 1] = str[h];
                    str[l - 2 - s_holder] = '\0';
                }
                s_holder++;
            }
        }////

        int x = ffind(path, str, mode, prop, at);
        if(x == -2)    printf("The file is not found");
        else if(prop != 'l' && prop != 's')    printf("%d", x);
    }

    //replace command
    else if(Is_sub(command, "replace --str1 ", 15) == 0){
        char path[100];
        char str1[100];
        char str2[100];
        int at = 0;
        char att[100];

        int j = 0;
        int giome1 = 0;
        int giome2 = 0;
        if(command[15] == '"')   giome1 = 1;
        int s = 0;
        while(!(command[j + 17] == '-' && command[j + 16] == ' ')){
            str1[j - s] = command[j + 15];
            if(str1[j - s] == 'n' && j - s > 0 && str1[j - 1 - s] == '\\' && (str1[j - 2 - s] != '\\' || j - s == 1)){
                str1[j - 1 - s] = '\n';
                s++;
            }
            if(str1[j - s] == 'n' && j - s > 1 && str1[j - s - 1] == '\\' && str1[j - s - 2] == '\\'){
                str1[j - s - 1] = 'n';
                s++;
            }
            j++;
        }
        if(giome1 == 0)  str1[j - s] = command[j + 15];
        if(giome1 == 1)  str1[j - s] = '\0';
        str1[j + 1 - s] = '\0';

        j += 24;
        int i = 0;
        if(command[i + j] == '"')   giome2 = 1;
        int s2 = 0;
        while(!(command[i + j + 2] == '-' && command[i + j + 1] == ' ')){
            str2[i - s2] = command[j + i];
            if(str2[i - s2] == 'n' && i - s2 > 0 && str2[i - 1 - s2] == '\\' && (str2[i - 2 - s2] != '\\' || i - s2 == 1)){
                str2[i - 1 - s2] = '\n';
                s2++;
            }
            if(str2[i - s2] == 'n' && i - s2 > 1 && str2[i - s2 - 1] == '\\' && str2[i - s2 - 2] == '\\'){
                str2[i - s2 - 1] = 'n';
                s2++;
            }
            i++;
        }
        if(giome2 == 0)  str2[i - s2] = command[i + j];
        if(giome2 == 1)  str2[i - s2] = '\0';
        str2[i + 1 - s] = '\0';

        i += 9;
        int k = 0;
        while(!(command[i + j + k] == ' ' && command[i + j + 1 + k] == '-')){
            path[k] = command[i + j + k];
            k++;
        }
        path[k] = '\0';
        int o = 0;
        char prop1[100];
        while(command[i + j + 2 + o + k] != '\0' && command[i + j + o + k + 2] != ' ' && command[i + j + o + k + 2] != '\n'){
            prop1[o] = command[i + j + 2 + k + o];
            o++;
        }
        prop1[o] = '\0';
        if(prop1[0] == 'a' && prop1[1] == 't' && prop1[2] == '\0'){
            int y = 0;
            while(command[i + j + 3 + k + o + y] != '\0' && command[i + j + 3 + k + o + y] != ' ' && command[i + j + 3 + k + o + y] != '\n'){
                att[y] = command[i + j + 3 + k + o + y];
                y++;
            }
            at = atoi(att);
        }
        else if(!(prop1[0] == 'a' && prop1[1] == 'l' && prop1[2] == 'l' && prop1[3] == '\0')){
            printf("Unvalid property");
            return 0;
        }

        if(giome1 == 1){
            int h = 0;
            while(str1[h] != '\0'){
                str1[h] = str1[h + 1];
                h++;
            }
        }
        //// str1 corrected
        if(giome2 == 1){
            int h = 0;
            while(str2[h] != '\0'){
                str2[h] = str2[h + 1];
                h++;
            }
        }
        //// str2 corrected

        //finding * in the str1
        int l = strlen(str1);
        int e = 0;
        int mode = 1;
        if(str1[0] == '*'){
            int k = 0;
            while(str1[k + 1] != '\0'){
                str1[k] = str1[k + 1];
                k++;
            }
            str1[k] = '\0';
            mode = 2;
        }
        else if(str1[l - 1] == '*' && str1[l - 2] != '\\'){
            str1[l - 1] = '\0';
            mode = 3;
        }
        else if(str1[l - 1] == '*' && str1[l - 2] == '\\'){
            mode = 1;
            e = 1;
        }

        //clearing the \*
        if(e == 1)   l++;
        int setare[l - 1];
        for(int r = 0; r < l - 1; r++){
            setare[r] = 0;
        }
        for(int u = 1; u < l - 1; u++){
            if(str1[u] == '*' && str1[u - 1] == '\\')    setare[u] = 1;
            }
        int s_holder = 0;
        for(int u = 1; u < l - 1; u++){
            if(setare[u] == 1){
                for(int h = u - s_holder; h <= l - s_holder - 2; h++){
                    str1[h - 1] = str1[h];
                    str1[l - 2 - s_holder] = '\0';
                }
                s_holder++;
            }
        }////

        rreplace(path, str1, str2, prop1, at, mode);
    }

    //grep command
    else if(Is_sub(command, "grep --str ", 11) == 0){
        char path[100];
        char str1[100];

        int j = 0;
        int giome = 0;
        if(command[11] == '"')   giome = 1;
        int s = 0;
        while(!(command[j + 13] == '-' && command[j + 12] == ' ')){
            str1[j - s] = command[j + 11];
            if(str1[j - s] == 'n' && j - s > 0 && str1[j - 1 - s] == '\\' && (str1[j - 2 - s] != '\\' || j - s == 1)){
                str1[j - 1 - s] = '\n';
                s++;
            }
            if(str1[j - s] == 'n' && j - s > 1 && str1[j - s - 1] == '\\' && str1[j - s - 2] == '\\'){
                str1[j - s - 1] = 'n';
                s++;
            }
            j++;
        }
        if(giome == 0)  str1[j - s] = command[j + 11];
        if(giome == 1)  str1[j - s] = '\0';
        str1[j + 1 - s] = '\0';

        if(giome == 1){
            int h = 0;
            while(str1[h] != '\0'){
                str1[h] = str1[h + 1];
                h++;
            }
        }
        //// str corrected

        j += 21;
        int i = 0;
        int x = 1;
        int count = 0;
        while(x == 1){
            //ith file
            char file[100];
            int k = 0;
            if(command[j] == '"'){
                j++;
                while(command[j] != '"' && command[j] != '\0'){
                    file[k] = command[j];
                    k++;
                    j++;
                }
                file[k] = '\0';
                j++;
            }
            else if(command[j] == '/'){
                j++;
                while(command[j] != ' ' && command[j] != '\0'){
                    file[k] = command[j];
                    j++;
                    k++;
                }
                file[k] = '\0';
            }
            else{
                while(command[j] != ' ' && command[j] != '\0'){
                    file[k] = command[j];
                    k++;
                    j++;
                }
                file[k] = '\0';
            }
            if(command[j] == '\0')   x = 0;
            if(!exist(file)){
                printf("unknown file");
                return 0;
            }
            ////
            FILE *fff;
            fff = fopen(file, "r+");
            int line = 1;
            char c;
            int i = 0;
            int L = 0;
            int again = 0;
            fseek(fff, 0, SEEK_END);
            int size_file = ftell(fff);
            fseek(fff, i, SEEK_SET);
            while(i < size_file){
                fseek(fff, i, SEEK_SET);
                c = fgetc(fff);
                if(c == '\n'){
                    L = i + 2;
                    again = 0;
                    i += 2;
                    continue;
                }
                if(c == str1[0] && again == 0){
                    int ans = 1;
                    for(int o = 1; o < strlen(str1); o++){
                        if(fgetc(fff) != str1[o]){
                            ans = 0;
                            break;
                        }
                    }
                    if(ans == 1){
                        again = 1;
                        fseek(fff, L, SEEK_SET);
                        char t;
                        printf("%s: ", file);
                        while(1){
                            t = fgetc(fff);
                            if(t == EOF)    t = '\n';
                            printf("%c", t);
                            if(t == '\n')   break;
                        }
                    }
                }
                i++;
            }
            j++;
            fclose(fff);
        }
    }

    //grep c option
    else if(Is_sub(command, "grep -c --str ", 14) == 0){
        char path[100];
        char str1[100];

        int j = 0;
        int giome = 0;
        if(command[14] == '"')   giome = 1;
        int s = 0;
        while(!(command[j + 16] == '-' && command[j + 15] == ' ')){
            str1[j - s] = command[j + 14];
            if(str1[j - s] == 'n' && j - s > 0 && str1[j - 1 - s] == '\\' && (str1[j - 2 - s] != '\\' || j - s == 1)){
                str1[j - 1 - s] = '\n';
                s++;
            }
            if(str1[j - s] == 'n' && j - s > 1 && str1[j - s - 1] == '\\' && str1[j - s - 2] == '\\'){
                str1[j - s - 1] = 'n';
                s++;
            }
            j++;
        }
        if(giome == 0)  str1[j - s] = command[j + 14];
        if(giome == 1)  str1[j - s] = '\0';
        str1[j + 1 - s] = '\0';

        if(giome == 1){
            int h = 0;
            while(str1[h] != '\0'){
                str1[h] = str1[h + 1];
                h++;
            }
        }
        //// str corrected

        j += 24;
        int i = 0;
        int x = 1;
        int count = 0;
        while(x == 1){
            //ith file
            char file[100];
            int k = 0;
            if(command[j] == '"'){
                j++;
                while(command[j] != '"' && command[j] != '\0'){
                    file[k] = command[j];
                    k++;
                    j++;
                }
                file[k] = '\0';
                j++;
            }
            else if(command[j] == '/'){
                j++;
                while(command[j] != ' ' && command[j] != '\0'){
                    file[k] = command[j];
                    j++;
                    k++;
                }
                file[k] = '\0';
            }
            else{
                while(command[j] != ' ' && command[j] != '\0'){
                    file[k] = command[j];
                    k++;
                    j++;
                }
                file[k] = '\0';
            }
            if(command[j] == '\0')   x = 0;
            if(!exist(file)){
                printf("unknown file");
                return 0;
            }
            ////
            FILE *fff;
            fff = fopen(file, "r+");
            int line = 1;
            char c;
            int i = 0;
            int L = 0;
            int again = 0;
            fseek(fff, 0, SEEK_END);
            int size_file = ftell(fff);
            fseek(fff, i, SEEK_SET);
            while(i < size_file){
                fseek(fff, i, SEEK_SET);
                c = fgetc(fff);
                if(c == '\n'){
                    L = i + 2;
                    again = 0;
                    i += 2;
                    continue;
                }
                if(c == str1[0] && again == 0){
                    int ans = 1;
                    for(int o = 1; o < strlen(str1); o++){
                        if(fgetc(fff) != str1[o]){
                            ans = 0;
                            break;
                        }
                    }
                    if(ans == 1){
                        again = 1;
                        fseek(fff, L, SEEK_SET);
                        count++;
                    }
                }
                i++;
            }
            j++;
            fclose(fff);
        }
        printf("%d", count);
    }

    //grep l option
    else if(Is_sub(command, "grep -l --str ", 14) == 0){
        char path[100];
        char str1[100];

        int j = 0;
        int giome = 0;
        if(command[14] == '"')   giome = 1;
        int s = 0;
        while(!(command[j + 16] == '-' && command[j + 15] == ' ')){
            str1[j - s] = command[j + 14];
            if(str1[j - s] == 'n' && j - s > 0 && str1[j - 1 - s] == '\\' && (str1[j - 2 - s] != '\\' || j - s == 1)){
                str1[j - 1 - s] = '\n';
                s++;
            }
            if(str1[j - s] == 'n' && j - s > 1 && str1[j - s - 1] == '\\' && str1[j - s - 2] == '\\'){
                str1[j - s - 1] = 'n';
                s++;
            }
            j++;
        }
        if(giome == 0)  str1[j - s] = command[j + 14];
        if(giome == 1)  str1[j - s] = '\0';
        str1[j + 1 - s] = '\0';

        if(giome == 1){
            int h = 0;
            while(str1[h] != '\0'){
                str1[h] = str1[h + 1];
                h++;
            }
        }
        //// str corrected

        j += 24;
        int i = 0;
        int x = 1;
        int count = 0;
        while(x == 1){
            //ith file
            char file[100];
            int k = 0;
            if(command[j] == '"'){
                j++;
                while(command[j] != '"' && command[j] != '\0'){
                    file[k] = command[j];
                    k++;
                    j++;
                }
                file[k] = '\0';
                j++;
            }
            else if(command[j] == '/'){
                j++;
                while(command[j] != ' ' && command[j] != '\0'){
                    file[k] = command[j];
                    j++;
                    k++;
                }
                file[k] = '\0';
            }
            else{
                while(command[j] != ' ' && command[j] != '\0'){
                    file[k] = command[j];
                    k++;
                    j++;
                }
                file[k] = '\0';
            }
            if(command[j] == '\0')   x = 0;
            if(!exist(file)){
                printf("unknown file");
                return 0;
            }
            ////
            FILE *fff;
            fff = fopen(file, "r+");
            int line = 1;
            char c;
            int i = 0;
            int L = 0;
            int again = 0;
            fseek(fff, 0, SEEK_END);
            int size_file = ftell(fff);
            fseek(fff, i, SEEK_SET);
            while(i < size_file){
                fseek(fff, i, SEEK_SET);
                c = fgetc(fff);
                if(c == '\n'){
                    L = i + 2;
                    //again = 0;
                    i += 2;
                    continue;
                }
                if(c == str1[0] && again == 0){
                    int ans = 1;
                    for(int o = 1; o < strlen(str1); o++){
                        if(fgetc(fff) != str1[o]){
                            ans = 0;
                            break;
                        }
                    }
                    if(ans == 1){
                        again = 1;
                        fseek(fff, L, SEEK_SET);
                        count++;
                        printf("%s\n", file);
                    }
                }
                i++;
            }
            j++;
            fclose(fff);
        }
    }

    //directory tree
    else if(Is_sub(command, "tree ", 5) == 0){
        char d[1000];
        int i = 0;
        while(command[i + 5] != '\0' && command[i + 5] != '\n'){
            d[i] = command[i + 5];
            i++;
        }
        d[i] = '\0';
        int x = atoi(d);
        char loc[1000];
        getcwd(loc, sizeof(loc));

        if(x < -1)  printf("Invalid depth");
        else    dirtree(loc, 0, x - 1);
    }

    else    printf("Invalid Command");
    }*/

    return 0;
}
