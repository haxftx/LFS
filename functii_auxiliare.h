int comp_nod(Arbore *arb, char b)
{//compar un nod de arbore cu un char
    return (arb->info - '0') - (b -'0');
}

int nr_slash(char *str)
{//calculez numarul de '/' dintr-un string
    int i, count = 0;
    for(i = 1; i < strlen(str); i++)
    {
        if(str[i] == '/') count++;
    }
    return count;
}

void print_name(Arbore *arb)
{//afiseaza namele directorului sau fisierului
    if(!arb) return;
    if(!arb->ant) return;
    if(arb->info == '/') return;
    print_name(arb->ant);
    printf("%c", arb->info);
}

void add_list_sort(Arbore *arb, char inf, char is_file)
{//adaug in lista sortata dupa informatii din arbore
    List *cel, *p, *ant = NULL;
    cel = AlocL(inf, is_file);//aloc celula
    if(!cel)
        return;
    cel->nod->ant = arb;
    p = arb->urm;
    while(p && comp_nod(p->nod, inf) < 0)
    {//caut locul inserarii celulei
        ant = p;
        p = p->urm;
    }
    cel->urm = p;
    if(!ant)
    {
        arb->urm = cel;
    }
    else
    {
        ant->urm = cel;
    }
}

Arbore *find_nod(List *l, char c)
{//caut nodul intr-o lista
    while(l)
    {
        if(comp_nod(l->nod, c) == 0)
        {
            return l->nod;
        }
        else if(comp_nod(l->nod, c) > 0)
        {
            break;
        }
        l = l->urm;
    }
    return NULL;
}

Arbore *find_way(Arbore *director, char *way, int nr_slash)
{//caut o cale in director dupa numarul de direcroare
    int i, count = 0;
    Arbore *aux = director;
    for(i = 0; i < strlen(way); i++)
    {
        if(i == 0 && way[0] == '/') continue;
        aux = find_nod(aux->urm, way[i]);
        if(!aux) return NULL;
        if(way[i] == '/') count++;
        if(count == nr_slash) break;
    }
    return aux;
}
