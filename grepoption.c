#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dir.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <windows.h>

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
            while(path[k] != '/' && path[k] != ' ' && path[k] != '\n' && path[k] != NULL){
                k++;
            }
            if(path[k] == '/'){
                num++;
                k++;
            }
            else if(path[k] == ' ' || path[k] == '\n' || path[k] == NULL){
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
                while(path[j] != '/' && path[j] != ' ' && path[j] != '\n' && path[j] != NULL){
                    j++;
                    sizee[n]++;
                }
                if(path[j] == '/'){
                    j++;
                    n++;
                }
                else if(path[j] == ' ' || path[j] == '\n' || path[j] == NULL){
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
                while(path[j] != NULL && path[j] != '\0'){
                    j++;
                    sizee[n]++;
                }
                if(path[j] == '\0' || path[j] == NULL){
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
                mkdir(newpath);
            }
            else if(i == num - 1){
                if(exist(newpath))   printf("The file already exist");
                else{
                    FILE *fff;
                    fff = fopen(newpath, "a");
                    fff = fclose(fff);
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
        while(filepath[i + 1] != '\0' && filepath[i + 1] != NULL){
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
                i++;
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
                u++;
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
        preserve[save_size - l + line] = '\0';

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
        while(filepath[i + 1] != '\0' && filepath[i + 1] != NULL){
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
                u++;
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
                ua++;
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
                uuu++;
            }
            uuu++;
        }

        if(f == 'f'){
            int save_size = size_file - i - size;

            fseek(fff, 0, SEEK_SET);
            char *remain;
            remain = (char*) malloc(sizeof(char) * i);
            size_t rresult = fread(remain, 1, i, fff);
            remain[i - line + 1] = '\0';

            fseek(fff, i + size, SEEK_SET);
            char *preserve;
            preserve = (char*) malloc(sizeof(char) * save_size);
            size_t result = fread(preserve, 1, save_size, fff);
            preserve[save_size - l + lll] = '\0';
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
            remain[i - size - li + 1] = '\0';

            fseek(fff, i, SEEK_SET);
            char *preserve;
            preserve = (char*) malloc(sizeof(char) * save_size);
            size_t result = fread(preserve, 1, save_size, fff);
            preserve[save_size - l + line] = '\0';
            fclose(fff);

            FILE *ff;
            ff = fopen(filepath, "w");
            fputs(remain, ff);
            fputs(preserve, ff);
            fclose(ff);
        }
    }
}

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
                i++;
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
        OpenClipboard(0);
        EmptyClipboard();
        const char* op = str;
        const size_t ln = strlen(op) + 1;
        HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE, ln);
        memcpy(GlobalLock(h), op, ln);
        GlobalUnlock(h);
        SetClipboardData(CF_TEXT, h);
        CloseClipboard();

        fclose(fff);
    }
}

void ppaste(char filepath[], int pos1, int pos2){
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
}

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
        while(filepath[i + 1] != '\0' && filepath[i + 1] != NULL){
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
                i++;
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
                    i += 2;
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
                i++;
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
        while(filepath[i + 1] != '\0' && filepath[i + 1] != NULL){
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
                return -1;
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
                return -1;
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
                return -1;
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
                                    return 0;
                                }
                            }
                        while(fgetc(fff) == str1[j]){
                            if(j == strlen(str1) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    count++;
                                if(count == at){
                                    rremove(filepath, line, pos, strlen(str1), 'f');
                                    Insert(filepath, str2, line, pos, 0);
                                    return 0;
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
                                    return 0;
                                }
                                if(count == at){
                                    rremove(filepath, line, pos, strlen(str1), 'f');
                                    Insert(filepath, str2, line, pos, 0);
                                    return 0;
                                }
                            }
                        while(fgetc(fff) == str1[j]){
                            if(j == strlen(str1) - 1){
                                char c = fgetc(fff);
                                if(c == ' ' || c == '\n' || c == EOF || c == '\0')    count++;
                                if(count == at){
                                    rremove(filepath, line, pos, strlen(str1), 'f');
                                    Insert(filepath, str2, line, pos, 0);
                                    return 0;
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
                            return 0;
                        }
                        while(fgetc(fff) == str1[j]){
                            if(j == strlen(str1) - 1)    count++;
                            if(count == at){
                                rremove(filepath, line, pos, strlen(str1), 'f');
                                Insert(filepath, str2, line, pos, 0);
                                return 0;
                            }
                            j++;
                        }
                    }
                    i++;
                    fseek(fff, i, SEEK_SET);
                }
            }
            return -1;
        }
        fclose(fff);
    }
}

int main()
{
    char command[100];
    gets(command);
    //creatfile command
    if(Is_sub(command, "createfile --file ", 18) == 0){
        char path[100];
        int i = 0;
        while(command[i + 18] != NULL && command[i + 18] != '\n'){
            path[i] = command[i + 18];
            i++;
        }
        path[i] = command[i + 18];
        createfile(path);
    }
    //cat command
    if(Is_sub(command, "cat --file ", 11) == 0){
        char path[100];
        int i = 0;
        while(command[i + 11] != NULL && command[i + 11] != '\n'){
            path[i] = command[i + 11];
            i++;
        }
        path[i] = command[i + 11];
        cat(path);
    }
    //insert command
    if(Is_sub(command, "insertstr --file ", 17) == 0){
        char path[100];
        char str[100];
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
        while(command[l + i + j + k] != '\0' && command[l + i + j + k] != NULL && command[l + i + j + k] != '\n'){
            pos2[l] = command[l + i + j + k];
            l++;
        }
        int x = atoi(pos1);
        int y = atoi(pos2);

        Insert(path, str, x, y, giome);
    }

    //remove command
    if(Is_sub(command, "removestr --file ", 17) == 0){
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
    if(Is_sub(command, "copystr --file ", 15) == 0){
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
    }

    //cut command
    if(Is_sub(command, "cutstr --file ", 14) == 0){
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
    }

    //paste command
    if(Is_sub(command, "pastestr --file ", 16) == 0){
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
    }

    //indent command
    if(Is_sub(command, "auto-indent ", 12) == 0){
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
    if(Is_sub(command, "compare ", 8) == 0){
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
            while(command[i + 8 + j] != NULL && command[i + 8 + j] != '\n'){
                path2[j] = command[i + j + 8];
                j++;
            }
            path2[j] = command[i + j + 8];
        }
        if(command[8] == '"'){}
        comparator(path1, path2);
    }

    //find command
    if(Is_sub(command, "find --str ", 11) == 0){
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
            while(command[i + j + 2 + k] != '\0' && command[i + j + 2 + k] != ' ' && command[i + j + 2 + k] != '\n'){
                prop1[k] = command[i + j + 2 + k];
                k++;
            }
            prop1[k] = '\0';
            if(prop1[0] == 'a' && prop1[1] == 't' && prop1[2] == '\0'/* prop1 == "at" */){
                int o = 0;
                while(command[i + j + 3 + k + o] != '\0' && command[i + j + 3 + k + o] != ' ' && command[i + j + 3 + k + o] != '\n'){
                    att[o] = command[i + j + 3 + k + o];
                    o++;
                }
                at = atoi(att);
                prop = 'a';
            }
            else if(/* prop1 == "count" */ prop1[0] == 'c' && prop1[1] == 'o' && prop1[2] == 'u' && prop1[3] == 'n' && prop1[4] == 't' && prop1[5] == '\0')    prop = 'c';
            else if(/* prop1 == "byword" */ prop1[0] == 'b' && prop1[1] == 'y' && prop1[2] == 'w' && prop1[3] == 'o' && prop1[4] == 'r' && prop1[5] == 'd' && prop1[6] == '\0')    prop = 'b';
            else if(/* prop1 == "all" */ prop1[0] == 'a' && prop1[1] == 'l' && prop1[2] == 'l' && prop1[3] == '\0')    prop = 'l';
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
        else if(prop != 'l')    printf("%d", x);
    }

    //replace command
    if(Is_sub(command, "replace --str1 ", 15) == 0){
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
        int o = 0;
        char prop1[100];
        while(command[i + j + 2 + o + k] != '\0' && command[i + j + o + k + 2] != ' ' && command[i + j + o + k + 2] != '\n'){
            prop1[o] = command[i + j + 2 + k + o];
            o++;
        }
        prop1[o] = '\0';
        if(prop1[0] == 'a' && prop1[1] == 't' && prop1[2] == '\0'/* prop1 == "at" */){
            int y = 0;
            while(command[i + j + 3 + k + o + y] != '\0' && command[i + j + 3 + k + o + y] != ' ' && command[i + j + 3 + k + o + y] != '\n'){
                att[y] = command[i + j + 3 + k + o + y];
                y++;
            }
            at = atoi(att);
        }
        else if(/* prop1 == "all" */ !(prop1[0] == 'a' && prop1[1] == 'l' && prop1[2] == 'l' && prop1[3] == '\0')){
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
    if(Is_sub(command, "grep --str ", 11) == 0){
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
    if(Is_sub(command, "grep -c --str ", 14) == 0){
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
    if(Is_sub(command, "grep -l --str ", 14) == 0){
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
                        printf("%s\n", file);
                    }
                }
                i++;
            }
            j++;
            fclose(fff);
        }
    }

/*

        char file[] = "fuf.txt";
        char str1[] = "ee";
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
                        printf("%c", t);
                        if(t == '\n')   break;
                    }
                }
            }
            i++;
        }





/*


        char filepath[] = "fuf.txt";
        char str1[] = "ree";
        char str2[] = "booo";
        char str3[] = "all";
        int mode = 2;
        int at = 0;

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
                return -1;
            }
            if(mode == 2){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
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
                return -1;
            }
        }////






/*

        char filepath[] = "fuf.txt";
        int mode = 1;
        char str[] = "salam";



        FILE *fff;
        fff = fopen(filepath, "r+");
        fseek(fff, 0, SEEK_END);
        int file_size = ftell(fff);


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
                //keep the count of words
                char w = fgetc(fff);
                if(w == ' ' || w == '\n'){
                    char q = fgetc(fff);
                    if(q != ' ' && q != '\n')  word_count++;
                }//
                fseek(fff, i, SEEK_SET);

                while(i <= file_size){
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
                //keep the count of words
                char w = fgetc(fff);
                if(w == ' ' || w == '\n'){
                    char q = fgetc(fff);
                    if(q != ' ' && q != '\n')  word_count++;
                }//
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


    /*int x = ffind("fuf.txt", "beautifuly", 1, 'n');
    printf("%d", x);/*
    char filepath[] = "fuf.txt";
    char str[] = "salam";
    int mode = 1;
    char prop = 'n';
    if(!exist(filepath))    printf("The file is not found");
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
                        char c = fgetc(fff);
                        if(c != ' ' && c != '\n'){
                            i++;
                            fseek(fff, i, SEEK_SET);
                            continue;
                        }
                        fseek(fff, i, SEEK_SET);
                    }
                    char bin = fgetc(fff);
                    if(bin == str[0]){
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
            if(mode == 3){
                int i = 0;
                fseek(fff, i, SEEK_SET);
                while(i <= file_size){
                    if(i > 0){
                        fseek(fff, i - 1, SEEK_SET);
                        if(fgetc(fff) != ' ' && fgetc(fff) != '\n'){
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
        }
    }
*/












/*    char filepath[] = "ko.txt";


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
        while(i < 100){
            fseek(fff, i, SEEK_SET);
            c = fgetc(fff);
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
                i++;
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
                    i += 2;
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
                i++;
            }
            i++;
        }
        fclose(fff);
    }



//    Insert(filepath, " ", 1, 8, 0);
    int pos1 = 2;
    int pos2 = 4;
    int giome = 0;
    char str[] = "jbdvl  vasjg asgh";
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
                i++;
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
                u++;
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
        preserve[save_size - l + line] = '\0';

        fseek(fff, i, SEEK_SET);
        fputs(str, fff);
        fputs(preserve, fff);
        fclose(fff);
    }




/*        char filepath[] = "nil.txt";
        int pos1 = 3;
        int pos2 = 8;
        char f = 'f';
        int size = 7;


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
                u++;
            }
            u++;
        }
        //calculating the number of lines for b case
        int li = 1;
        int ua = 0;
        while(ua < i - size - 1){
            fseek(fff, ua, SEEK_SET);
            if(fgetc(fff) == '\n'){
                li++;
                ua++;
            }
            ua++;
        }

        if(f == 'f'){
            int save_size = size_file - i - size;

            fseek(fff, 0, SEEK_SET);
            char *remain;
            remain = (char*) malloc(sizeof(char) * i);
            size_t rresult = fread(remain, 1, i, fff);
            remain[i - line + 1] = '\0';

            fseek(fff, i + size, SEEK_SET);
            char *preserve;
            preserve = (char*) malloc(sizeof(char) * save_size);
            size_t result = fread(preserve, 1, save_size, fff);
            preserve[save_size - l + line] = '\0';
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
            remain[i - size - li + 1] = '\0';

            fseek(fff, i, SEEK_SET);
            char *preserve;
            preserve = (char*) malloc(sizeof(char) * save_size);
            size_t result = fread(preserve, 1, save_size, fff);
            preserve[save_size - l + line] = '\0';
            fclose(fff);

            FILE *ff;
            ff = fopen(filepath, "w");
            fputs(remain, ff);
            fputs(preserve, ff);
            fclose//        gets(str);
//        FILE *fff;
//        fff = fopen(filepath, "r+");
//        int size_file;
//        int line = 1;
//        int i = 0;
//        char c;
//        while(line < pos1){
//            fseek(fff, i, SEEK_SET);
//            if(fgetc(fff) == '\n'){
//                line++;
//                i++;
//            }
//            i++;
//        }
//        i += pos2;
//
//        fseek(fff, 0, SEEK_END);
//        size_file = ftell(fff);
//        int save_size = size_file - i;
//
//        fseek(fff, i, SEEK_SET);
//        char *preserve;
//        preserve = (char*) malloc(sizeof(char) * save_size);
//        size_t result = fread(preserve, 1, save_size, fff);
//
//        fseek(fff, i, SEEK_SET);
//        fputs(str, fff);
//        fputs(preserve, fff);
//        fclose(fff);

*/
    return 0;
}



