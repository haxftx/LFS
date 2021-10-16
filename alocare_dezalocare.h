Arbore *Aloc(char info ,char is_file)
{//creez un arbore
    Arbore *p = (Arbore *)malloc(sizeof(Arbore));
    if(!p) return NULL;
    p->info = info;
    p->is_file = is_file;
    p->urm = NULL;
    p->ant = NULL;
    return p;
}

List *AlocL(char info, char is_file)
{//creez o celula de lista
    List *p = (List *)malloc(sizeof(List));
    if(!p)
        return NULL;
    p->nod = Aloc(info, is_file);
    if(!p->nod)
    {
        free(p);
        return NULL;
    }
    p->urm = NULL;
    return p;
}

Tfile *Init(void)
{//initializez sistemul de fisiere
    Tfile *p = (Tfile *)malloc(sizeof(Tfile));
    if(!p) return NULL;
    p->root = Aloc('/', 0);
    if(!p->root)
    {
        free(p);
        return NULL;
    }
    p->current_directory = p->root;
    return p;
}

void DistrArb(Arbore **a)
{//distrug un arbore
    if((*a)->urm == NULL)
    {
        free(*a);
        return;
    }
    List *l, *p = (*a)->urm;
    while(p)
    {
        l = p;
        p = p->urm;
        DistrArb(&(l->nod));
        free(l);
    }
    free(*a);
}

void Eliberare(Tfile **sistem)
{//eliberez memoria sistemului de fisiere
    DistrArb(&((*sistem)->root));
    free(*sistem);
}
