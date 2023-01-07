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

//for this function I considered two \n at the end of lines
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

        fseek(fff, 0, SEEK_END);
        size_file = ftell(fff);
        int save_size = size_file - i;

        fseek(fff, i, SEEK_SET);
        char *preserve;
        preserve = (char*) malloc(sizeof(char) * save_size);
        size_t result = fread(preserve, 1, save_size, fff);

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

//for this function I considered one \n at the end of lines
//this one has bugs
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
        if(f == 'f'){
            int save_size = size_file - i - size;

            fseek(fff, i + size, SEEK_SET);
            char *preserve;
            preserve = (char*) malloc(sizeof(char) * save_size);
            size_t result = fread(preserve, 1, save_size, fff);

            fseek(fff, i, SEEK_SET);
            fputs(preserve, fff);
            fclose(fff);
        }
        else if(f == 'b'){
            int save_size = size_file - i - 1;

            fseek(fff, i + 1, SEEK_SET);
            char *preserve;
            preserve = (char*) malloc(sizeof(char) * save_size);
            size_t result = fread(preserve, 1, save_size, fff);

            fseek(fff, i - size + 1, SEEK_SET);
            fputs(preserve, fff);
            fclose(fff);
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




/*    OpenClipboard(0);
    EmptyClipboard();
    const char* op = "how you? bofwbeofibwf";
    const size_t ln = strlen(op) + 1;
    HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE, ln);
    memcpy(GlobalLock(h), op, ln);
    GlobalUnlock(h);
    SetClipboardData(CF_TEXT, h);
    CloseClipboard();

    getch();

//        system("cls");
    OpenClipboard(0);
    HANDLE in = GetClipboardData(CF_TEXT);
    printf("%s", in);
    CloseClipboard();







//        int pos1 = 2;
//        int pos2 = 0;
//        int size = 8;
//
//        FILE *fff;
//        fff = fopen("root1/hell.txt", "r+");
//        int line = 1;
//        int i = 0;
//        while(line < pos1){
//            fseek(fff, i, SEEK_SET);
//            if(fgetc(fff) == '\n'){
//                line++;
//                i++;
//            }
//            i++;
//        }
//        i += pos2;
//        fseek(fff, 0, SEEK_END);
//        int size_file = ftell(fff);
//
//
//
//
//            int save_size = size_file - i - size;
//
//            fseek(fff, i + size, SEEK_SET);
//            char *preserve;
//            preserve = (char*) malloc(sizeof(char) * save_size);
//            preserve[save_size + 1] = '\0';
//            size_t result = fread(preserve, 1, save_size, fff);
//
//            fseek(fff, i, SEEK_SET);
////            fputs(preserve, fff);
//            fclose(fff);


//    rremove("root1/hell.txt", 5, 9, 6, 'b');
//   Insert("root1/hell.txt", "kiki     kiki", 3, 9);
 /*   char filepath[] = "root1/hell.txt";
    int pos1 = 2;
    int pos2 = 2;
    char str[] = "tttt";
    FILE *fff;
    fff = fopen(filepath, "r+");
    int size_file;
    int place_holder;
    int line = 1;
    int pos = 0;
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
    place_holder = i;

    fseek(fff, 0, SEEK_END);
    size_file = ftell(fff);
    int save_size = size_file - i;

    fseek(fff, i, SEEK_SET);
    char *preserve;
    preserve = (char*) malloc(sizeof(char) * save_size);
    size_t result = fread(preserve, 1, save_size, fff);
    printf("%s", preserve);

    fseek(fff, i, SEEK_SET);
    fputs(str, fff);
    fputs(preserve, fff);
    fclose(fff);

  //  char preserve[size_file - place_holder];
/*    while(1){
        fseek(fff, i, SEEK_SET);
        c = fgetc(fff);
        if(c == '\0' || c == NULL || c == -1)    break;
        preserve[i - place_holder] = c;
        if(c == '\n')   i++;
        i++;
    }

    char save[place_holder - line + 1];
    int j = 0;
    while(j < place_holder){
        fseek(fff, j, SEEK_SET);
        c = fgetc(fff);
        save[j] = c;
        if(c == '\n')  j++;
        j++;
    }
    printf("%s", save);

*/

    /*

    char svae[] = {"blajs jgsgcj\negfk wqg k\nhjkqg j\nve"};
    FILE *ffff;
    ffff = fopen("root1/hell.txt", "w+");
    fputs(svae, ffff);
    fclose(ffff);


/*    FILE *fff;
    fff = fopen("root1/hell.txt", "r");
    int place_holder;
    int line = 1;
    int pos = 0;
    int i = 0;
    char c;
    while(line < 3){
        fseek(fff, i, SEEK_SET);
//        c = fgetc(fff);
//        printf("%c", c);
        if(fgetc(fff) == '\n'){
            line++;
        }
        i++;
    }
    printf("%d\n", i);
    i += 6;
    fseek(fff, i, SEEK_SET);
    char u = fgetc(fff);
    printf("%c", u);
    fclose(fff);
    FILE *fff;
    fff = fopen("nil.txt", "r");
    int place_holder;
    int line = 1;
    int pos = 0;
    int i = 0;
    char c;
    while(i < 300){
        fseek(fff, i, SEEK_SET);
        if(feof(fff))    break;
        if(fgetc(fff) == '\n')    printf("%d", 0);
        else    printf("%d", 1);
        i++;
    }*/









//    FILE *fff;
//
//    char c;
//
//    fff = fopen("fuf.txt", "w+");
//    //fputs("writing first line into the file", fff);
//    fseek(fff, 0, SEEK_END);
//    printf("%d", ftell(fff));
//    fclose(fff);
//    fff = fopen("fuf.txt", "r");
//    //fputs("writing first line into the file", fff);
//    char c;
//    int i = 0;
//    while(1){
//        fseek(fff, i, SEEK_SET);
//        c = fgetc(fff);
//        if(feof(fff))   break;
//        printf("%c", c);
//        i += 2;
//    }
//    fseek(fff, 12, SEEK_SET);
//    char c = fgetc(fff);
//    printf("%c", c);
    //fputs("Lets write something new to it", fff);
////    fff = fopen("fuf.txt", "w+");
////    fputs("writing first line into the file", fff);
//    fseek(fff, 17, SEEK_SET);
//    fputs("tttttttt", fff);
//    fclose(fff);
//    while(1){
//        c = fgetc(fff);
//        if(feof(fff))   break;
//        printf("%c", c);
//    }
//    fclose(fff);
 //   Insert(command, "odsjc", 3, 6);
//    FILE *fff;
//    fff = fopen("milk.txt", "w");
//    fputs("hellwo flekw\n nscdndj \nj", fff);
//    fff = fclose(fff);
//    unsigned char data;
//    fread(&data, 1, 1, fff);
//    printf("%c", data);
//    fff = fclose(fff);
//    FILE *fh_output;
//
//    mkdir("root1");
//
//    fh_output = fopen("iooo.txt", "a");
//
//
//
//    //fputs("A string\n", fh_output);
//    //fprintf(fh_output, "dtat : %d", 5);
//
//    fclose(fh_output);
//
//      mkdir("root1/abc/def/hjhj/grt");
//      mkdir("root1/abc/def/hjhj/grt/re");
//      mkdir("root1/abc/def/hjhj/grt/re/oop");
//      mkdir("root1/abc/def/hjhj/grt/re/oop/gfd");
//      char str1[] = "root7";
//      char str2[] = "root7/vdf";
//      char str3[] = "root7/vdf/vdvd";
//      char str4[] = "root7/vdf/vdvd/iuh";
//      char str5[] = "root7/vdf/vdvd/iuh/skn";
//      char str6[] = "root7/vdf/vdvd/iuh/skn/bkek";
//
//
//      mkdir(str1);
//      mkdir(str2);
//      mkdir(str3);
//      mkdir(str4);
//      mkdir(str5);
//      mkdir(str6);
//    int retval2 = mkdir("root1\\abc\\def\\joi\\reoirr");
//    FILE *fff;
//    fff = fopen("root1\\abc\\def\\joi\\reoir\\beuu.txt", "a");
//    fff = fclose(fff);
//
//    if(retval == -1 || retval2 == -1)   printf("Make Directory failed");

//    int size = 0;
//    int *junk = 0;
//    srand(time(0));
//
//    for(int i = 0 ; i < 1000; i++){
//        size = rand() % 16000;
//        junk = malloc(size * sizeof(int));
//        for(int j = 0; j < size; j++){
//            junk[j] = rand();
//        }
//        free(junk);
//    }
//
//    int *array;
//    array = calloc( 1000 , sizeof(int ));
//    for(int i = 0; i < 1000; i++){
//        printf("%d", array[i]);
//    }
//    free(array);
//
//    printf("\n");
//
//
//
//









    return 0;
}






