#include "struct.h"
#include "alocare_dezalocare.h"
#include "functii_auxiliare.h"
#include "functii_comenzi.h"

void rezolvare(void)
{
    Tfile *LFS = Init();//initializes sistemul de fisiere
    char comand[5];
    int nr_comand;//citesc numarul de comenzi
    if(fscanf(stdin, "%d", &nr_comand));
    else return;
    while(fscanf(stdin, "%s", comand) != EOF)
    {//cat exista comenzi
        if(strcmp(comand, "pwd") == 0)
        {//daca am functia pwd
            funtion_pwd(LFS->current_directory);
            continue;
        }
        if(strcmp(comand, "ls") == 0)
        {//daca am functia ls
            funtion_ls(LFS);
            continue;
        }
        if(strcmp(comand, "cd") == 0)
        {//daca am funtia cd
            LFS->current_directory = cd(LFS->current_directory, LFS->root);
            continue;
        }
        if(strcmp(comand, "mkdir") == 0)
        {//daca am functia mkdir
            funtion_mkdir_touch(LFS, 1);
            continue;
        }
        if(strcmp(comand, "touch") == 0)
        {//daca am functia touch
            funtion_mkdir_touch(LFS, 0);
            continue;
        }
        if(strcmp(comand, "rmdir") == 0)
        {//daca am functia rmdir
            rmdir(LFS->current_directory);
            continue;
        }
        if(strcmp(comand, "rm") == 0)
        {//daca am functia rm
            rm(LFS->current_directory);
            continue;
        }
        if(strcmp(comand, "mv") == 0)
        {//daca am functia mv
            mv(LFS);
            continue;
        }
        if(strcmp(comand, "cp") == 0)
        {//daca am functia cp
            cp(LFS->current_directory);
            continue;
        }
        else
        {//daca nu exista comanda
            fprintf(stderr, "%s: command not found\n", comand);
            while(fgetc(stdin) != EOL);//citesc linia
       }
    }//eliberez memoria
    Eliberare(&LFS);
}

int main(void)
{
    rezolvare();
    return 0;
}