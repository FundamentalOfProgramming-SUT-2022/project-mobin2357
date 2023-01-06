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
    int Invalid_path = 0;
    if(path[0] != '/' && path[0] != '"'){
        Invalid_path = 1;
    }
    else if(path[0] == '/'){
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

    if(Invalid_path == 0){
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
        else if(path[0] == '"'){
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
        //creating the substrings
        int ss = 0;
        for(int i = 0; i < num; i++){
            ss += sizee[i] + 1;
            char newpath[ss];
            for(int j = 0; j < ss - 1; j++){
                if(path[0] == '/') newpath[j] = path[j + 1];
                if(path[0] == '"') newpath[j] = path[j + 2];
            }
            newpath[ss - 1] = '\0';

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




int main()
{


    char command[100];
    gets(command);
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






