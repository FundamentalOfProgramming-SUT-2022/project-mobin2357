#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dir.h>
#include <sys/stat.h>
#include <stdbool.h>

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


void Insert(char filepath[], char str[], int pos1, int pos2){
    FILE *fff;
    char line[512];
    long i;
    fff = fopen(filepath, "w");
    fputs("hellwo flekw\n ns23\n34scdndj \nj", fff);
    fff = fclose(fff);
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
//
//    FILE *fff;
//
//    char c;
//
//    fff = fopen("fuf.txt", "r");
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






