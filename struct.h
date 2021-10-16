#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define EOL '\n' //sfarsit de linie

typedef struct arb Arbore;
typedef struct list List;

struct arb
{//arborele
    char info, is_file;
    List *urm;
    Arbore *ant;
};

struct list
{//lista
    Arbore *nod;
    List *urm;
};

typedef struct filesystem
{//sistemul de fisiere
    Arbore *root;
    Arbore *current_directory;
}Tfile;

//definire funtii utilizate
Arbore *find_nod(List *, char);
Arbore *find_way(Arbore *, char *, int);
Arbore *find_way(Arbore*, char*, int);
Arbore *cd(Arbore*, Arbore*);
int comp_nod(Arbore*, char);
int nr_slash(char*);
int nr_slash(char *);
void print_name(Arbore*);
void add_list_sort(Arbore *, char, char);
void ls(Arbore*, char, int);
void function_ls(Tfile*);
void mkdir_touch(Arbore*, Arbore*, char*, char);
void funtion_mkdir_touch(Tfile *, char);
void mv(Tfile*);
void cp(Arbore*);
void rm(Arbore*);
void rmdir(Arbore*);
