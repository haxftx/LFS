void pwd(Arbore *a)
{//afisez calea absoloda a directorului
    if(!a->ant)
    {
        printf("/");
        return;
    }
    pwd(a->ant);
    printf("%c", a->info);
}

void funtion_pwd(Arbore *a)
{//afisez directorul de lucru
    if(!a->ant)
        printf("/");
    else
        pwd(a->ant);
    printf("\n");
}

void ls(Arbore *director, char print_type, int first)
{//afiseaza fisierele si directoarele unui director
    if(!director) return;
    if(first && director->info == '/' && director->ant)
    {//daca am ajuns la un director
        print_name(director->ant);//afisez numele
        if(print_type)
        {//daca trebuies si tipul
            printf("/");
        }
        if(director->ant)printf(" ");
        return;
    }
    if(director->is_file)
    {//daca am ajuns la un fisier
        print_name(director);//fisez numele
        if(print_type)
        {//daca trebuie si tipul
            printf("*");
        }
        printf(" ");
    }
    List *p;
    if(director->urm)
    {//fac un pas in adancime
        p = director->urm;
    }
    else return;
    while(p)
    {
        ls(p->nod, print_type, 1);
        p = p->urm;
    }
}

void funtion_ls(Tfile *LFS)
{
    char c;
    c = fgetc(stdin);
    if(c == EOL || c == EOF)
    {//daca e sfarsitul linei sau a fisierului
        if(LFS->current_directory->urm)
            ls(LFS->current_directory, 0, 0);//afisez directorul curent
    }
    else
    {//daca am de aifsat un director sau -F tipul
        char name_director[20];
        if(fscanf(stdin, "%s", name_director));
        else return;
        if(strcmp(name_director, "-F") == 0)
        {//daca trebuie sa afisez tipul
            if(fgetc(stdin) == EOL)
                ls(LFS->current_directory, 1, 0);
            else
            {//daca mai sunt paramtri
                if(fscanf(stdin, "%s", name_director));
                if(fgetc(stdin) != EOL)
                {//daca exista mai mult de un parametru de listat
                    fprintf(stderr, "ls -F %s ", name_director);
                    char c;
                    while((c = fgetc(stdin)) != EOL)
                    {
                        fprintf(stderr, "%c", c);
                    }
                    fprintf(stderr, ": too many arguments\n");
                    return;
                }
                else if(strcmp(name_director, ".") == 0)
                {//daca trebuie de listat directorul curent
                    ls(LFS->current_directory, 1 ,0);
                    printf("\n");
                    return;
                }
                //pentru cazul cand trebuie de afisat un direcor anume
                Arbore *p = find_way(LFS->current_directory,
                                name_director, nr_slash(name_director) + 1);
                if(p)ls(p->urm->nod, 0, 0);
            }
        }
        else if(strcmp(name_director, "..") == 0)
        {//daca trebuie de listat fisierul curent
            Arbore *p = LFS->current_directory->ant;
            while(p->info != '/')
            {
                p = p->ant;
            }
            ls(p, 0, 0);
        }
        else if(strcmp(name_director, "/") == 0)
        {//daca trebuie de listat root
            ls(LFS->root, 0 ,0);
        }
        else 
        {//daca trebuie de listat un fisier anume
            Arbore *p = find_way(LFS->current_directory, name_director,
                                            nr_slash(name_director) + 1);
            if(p)ls(p->urm->nod, 0, 0);
        }
    }
    printf("\n");
}

void mkdir_touch(Arbore *current_directory, Arbore *root, char *name, char fun)
{//creez un fisier sau un director
    int i, exist = 0; 
    Arbore *p = NULL, *aux;
    if(name[0] == '/') aux = root;
    else aux = current_directory;
    if(nr_slash(name) && fun)
    {//daca trebuie sa caut directorul pentru adaugare
        p = find_way(root, name, nr_slash(name));
        if(!p)
        {//daca nu gasesc directorul
            fprintf(stderr, "%s: No such file or directory\n", name);
            return;
        }
    }
    for(i = 0; i < strlen(name); i++)
    {//pentru fiecare caracter din nume
        if(i == 0 && name[0] == '/') continue;
        p = find_nod(aux->urm, name[i]);
        if(p)
        {//daca exista continui cu urmatorul caracter
            aux = p;
            if(i + 1 == strlen(name) && ( (p->is_file && fun == 0)
                                || (p->urm->nod->info == '/' && fun)))
            {//daca am gasit fisierul sau directorul in sistem
                fprintf(stderr, "%s: already exists\n", name);
                return;
            }
            continue;
        }//daca nu exista adaug si merg in adancime
        add_list_sort(aux, name[i], 0);
        aux = find_nod(aux->urm, name[i]);
    }//creez sfarsitul directorului sau fisierului
    if(fun && !exist) add_list_sort(aux, '/', 0);
    else if(!exist)aux->is_file = 1;
}

void funtion_mkdir_touch(Tfile *LFS, char type_file)
{//functia mkdir si touch
    char director[50];
    while(fscanf(stdin, "%s", director))
    {//cat citsc fisiere le adaug in sistem
        mkdir_touch(LFS->current_directory, LFS->root, director, type_file);
        if(fgetc(stdin) == EOL)break;//daca e sfarsitul liniei ma opresc
    }
}

Arbore *cd(Arbore *curent_direcotry, Arbore *root)
{//schimb directorul curent
    char name[20];
    if(fscanf(stdin, "%s", name));
    else return NULL;
    Arbore *aux = curent_direcotry;
    if(name[1] == '.')
    {//daca am .. - directorul precedent
        if(curent_direcotry->ant == NULL)
        {//daca nu exista
            fprintf(stderr, "%s: No such file or directory\n", name);
            return curent_direcotry;
        }
        aux = curent_direcotry->ant;
        while(aux->info != '/')
        {//ajung in directorul precedent
            aux = aux->ant;
        }
        return aux;
    }
    if(name[0] == '/')
    {//daca am o cale absoluta
        aux = root;
    }//caut directorul
    aux = find_way(aux, name, nr_slash(name) + 1);
    if(!aux)
    {//daca nu exista directorul
        fprintf(stderr, "%s: No such file or directory\n", name);
        return curent_direcotry;
    }
    if(aux->is_file || (aux->urm && aux->urm->nod->info != '/'))
    {//daca e fisier
        fprintf(stderr, "%s: Not a directory\n", name);
        return curent_direcotry;
    }
    if(aux->urm)curent_direcotry = aux->urm->nod;
    else curent_direcotry = aux;
    return curent_direcotry;
}

void mv(Tfile *LFS)
{//mut fisierul in directorul dat
    Arbore *aux = LFS->current_directory;
    char name[30], way[30];
    int error = 0;
    if(fgetc(stdin) == EOL)
    {//daca nu sunt paramteri
        fprintf(stderr, "mv: missing operand\n");
        return;
    }
    if(fscanf(stdin,"%s",name));
    if(fgetc(stdin) == EOL)
    {//daca este doar un parametru
        fprintf(stderr, "mv %s: missing operand\n", name);
        return;
    }
    Arbore *source;//caut calea fisierului sursa
    if(name[0] == '/')
        source = find_way(LFS->root, name, nr_slash(name) + 1);
    else source = find_way(aux, name, nr_slash(name) + 1);
    if(!source || (source && !source->is_file))
    {//daca nu exista sursa sau e director
        fprintf(stderr, "%s: No such file or directory\n", name);
        error = 1;
    }
    if(fscanf(stdin, "%s", way));//citesc si caut calea
    Arbore *dest = find_way(LFS->current_directory, way, nr_slash(way) + 1);
    if(!dest && !error)
    {//daca nu exista destinatia
        fprintf(stderr, "%s: No such file or directory\n", way);
        return;
    }
    else if(dest)
    {
        if(dest->is_file && !error)
        {//daca distinatia nu e fisier
            fprintf(stderr, "%s: Not a directory\n", way);
            return;
        }
        if(!error)
        {//mut fiserul in director
            source->is_file = 0;
            mkdir_touch(dest->urm->nod, dest, name, 0);
        }
    }
}

void cp(Arbore *director)
{//copiez fisierul in director
    char name[30], way[30];
    if(fgetc(stdin) == EOL)
    {//daca nu sunt parametri
        fprintf(stderr, "mv: missing operand\n");
        return;
    }
    if(fscanf(stdin,"%s",name));
    if(fgetc(stdin) == EOL)
    {//daca e doar 1 parametru
        fprintf(stderr, "mv %s: missing operand\n", name);
        return;
    }
    //caut fisierul sursa
    Arbore *source = find_way(director, name, nr_slash(name) + 1);
    if(source && source->is_file == 0)
    {//daca nu exista fisierul sursa
        fprintf(stderr, "%s: No such file or directory\n", name);
    }
    if(fscanf(stdin, "%s", way));
    Arbore *dest = find_way(director, way, nr_slash(way) + 1);
    if(!dest || (dest && dest->is_file == 1))
    {//daca nu exista destinatia
        fprintf(stderr, "%s: No such file or directory\n", way);
    }
    else if(dest)
    {//copiez fisierul in destinatie
        mkdir_touch(dest->urm->nod, dest, name, 0);
    }
}

void rm(Arbore *director)
{//sterg un director si continutul acestuia sau fisierul
    char name[30];
    while(fscanf(stdin, "%s", name))
    {//cat citesc parametri de sters, caut directorul
        Arbore *p = find_way(director, name, nr_slash(name) + 1);
        if(!p || (p && p->urm && p->urm->nod->info != '/' && !p->is_file))
        {//daca nu gasesc directorul
            fprintf(stderr, "%s: No such file or directory\n", name);
        }
        if(p && p->is_file)//sterg fisierul
            p->is_file = 0;
        if(fgetc(stdin) == EOL)break;
    }
}

void rmdir(Arbore *director)
{//sterg directorul daca este gol
    char name[30];
    while(fscanf(stdin, "%s", name))
    {//cat citesc directoare de sters
        Arbore *p = find_way(director, name, nr_slash(name) + 1);
        if(!p ||(p && p->is_file == 0))
        {//daca nu exista directorul sau nu e fisier
            if(p && p->urm)//daca nu e gol direcotrul
                fprintf(stderr, "%s: Directory not empty\n", name);
            else fprintf(stderr, "%s: No such file or directory\n", name);
        }
        else
        {//streg directorul
            Arbore *l =p;
            p = NULL;
            DistrArb(&l);
        }
        if(getc(stdin) == EOL) break;
    }
}
