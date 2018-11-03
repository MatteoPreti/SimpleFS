//Matteo Preti
//Versione Finale e corretta che passa tutti i Test Cases del server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MaxSize 1024
#define MAX 255

int indice=0;
int ultimo=0;
int figlio=0;
int caso;
int altezza=0;
int Finindex;
char * Globalstr;


typedef struct Lista{ // Struttura che uso per la find e per i metadati delle directory
   char * percorso;
   struct Lista *next;
   struct Lista *prec;
} listaordinata ;

typedef struct Nodo{ //Struttura che rappresenta il nodo del file system
    char  *Nome;
    char  *Contenuto;
    char * Percorso;
    int    File; //Lo utilizzo per distinguere tra file e directory
    int Nfigli;
    struct Nodo * Padre;
    struct Nodo ** figli;
    struct Nodo * next;
    struct Nodo * prec;
    listaordinata* Metadati;

} nodo ;

nodo * last;

nodo * New();
void inizializza(nodo * no);
char *createStr(char *word);
char *createStr2(char*word);
int FuncHash(char * percorso);
int ValChar(char c);
void NewNodeB(nodo* root, char * percorso, char * nome, int index );
void NewNodeA(nodo* root, char * percorso, char * nome, int index );
char *SpezzaStringa (char *percorso);
void create_dir(nodo * root, char * percorso);
void create(nodo * root, char * percorso);
void read(nodo * root, char * percorso);
void write(nodo * root, char * percorso, char * contenuto);
void delete(nodo * root, char * percorso);
void elimina(nodo * root);
void find(nodo * root, char * risorsa);
nodo * ScorriLista(nodo * root, char * nome );
listaordinata * creaLista(listaordinata * list,char* aggiungi);
int argomenti(char* numero);
nodo * TrovaPercorso(nodo * root, char * percorso);
void delete_r(nodo * root, char * percorso);
void printLista(listaordinata * list);
listaordinata* Trova(nodo * root, listaordinata* meta , char * risorsa, int index, listaordinata* lista);

void EliminaElem(listaordinata *lista, char * nome,  nodo * nodo);

int main()
{
    int numero=0;
    int ok=0;
    nodo *t;
    t =New(); //creo la radice del FS
    char comando[14];
    char percorso[3000];
    char contenuto[260];

    int esci=1;

    do{
        scanf("%s", &comando);
         numero=argomenti(comando);
         if(numero==1)scanf("%s", &percorso);
        if (numero==2){
            scanf("%s", &percorso);
            scanf("%s", &contenuto);
         }

        switch(caso){
            case 1:
                 create(t, percorso);
                 break;
            case 2:
                create_dir(t, percorso);
                 break;
            case 3:
                 read(t, percorso);
                 break;
            case 4:
                if(strlen(contenuto)<258) write(t, percorso, contenuto);
                else printf("no\n");
                 break;
            case 5:
                 delete(t, percorso);
                 break;
            case 6:
                delete_r(t, percorso);
                 break;
            case 7:
                find(t, percorso);
                 break;
            case 8:
                 ok=1;
                 break;
            case 9:

                 break;

        }

    if(ok)
        esci=0;
    }while(esci);


}

nodo * New(){
    nodo * new;
    new= (nodo *) malloc(sizeof(nodo));
    nodo ** prova=malloc(MaxSize*sizeof(nodo*));
    new->Percorso=createStr("/");
    new->figli=prova;
    new->Nfigli=0;
    new->Padre=NULL;
    new->File=0;
    new->Metadati=NULL;
    new->prec=NULL;
    new->next=NULL;
    inizializza(new);
    return new;

}

void inizializza(nodo * no){

    int i;
    for(i=0; i < MaxSize; i++)
        no->figli[i]=NULL;

}
void NewNodeB(nodo* root, char * percorso, char * nome, int index ){ //funzione che mi crea il nodo directory

    nodo * scorri, *prece;
    nodo * new= (nodo *)malloc(sizeof(nodo));
      if(root->figli[index]==NULL){
            root->figli[index]=new;
            new->next=NULL;
            new->prec=NULL;
    }

    else{

        scorri=root->figli[index];
        while(scorri!=NULL){
            if(!strcmp(scorri->Nome, nome)){
                printf("no\n");
                free(new);
                return;
            }
            prece=scorri;
            scorri=scorri->next;
        }
        prece->next=new;
        new->next=NULL;
        new->prec=prece;

    }

    nodo ** prova=malloc(MaxSize*sizeof(nodo*));
    root->Nfigli++;
    new->figli=prova;
    new->Padre=root;
    new->Nfigli=0;
    new->File=0;
    new->Metadati=NULL;
    new->Contenuto=NULL;
    new->Nome=createStr(nome);
    new->Percorso=createStr(percorso);
    last=new;
    new->Padre->Metadati=creaLista( new->Padre->Metadati, new->Nome);
    inizializza(new);
    printf("ok\n");

}

void NewNodeA(nodo* root, char * percorso, char * nome, int index ){ // funzione che mi crea il nodo File

    nodo * scorri, *prece;
    nodo * new= (nodo *)malloc(sizeof(nodo));
    if(root->figli[index]==NULL){
        root->figli[index]=new;
        new->next=NULL;
        new->prec=NULL;
    }
    else{
        scorri=root->figli[index];
        while(scorri!=NULL){
                if(!strcmp(scorri->Nome, nome)){
                printf("no\n");
                free(new);
                return;
            }
            prece=scorri;
            scorri=scorri->next;
        }
        prece->next=new;
        new->next=NULL;
        new->prec=prece;

    }
    new->Padre=root;
    new->Nfigli=0;
    new->File=1;
    root->Nfigli++;
    new->Metadati=NULL;
    new->Contenuto=createStr("");
    new->figli=NULL;
    new->Nome=createStr(nome);
    new->Percorso=createStr(percorso);
    last=NULL;
    new->Padre->Metadati=creaLista(new->Padre->Metadati, new->Nome );
    printf("ok\n");

}

char *createStr(char *word){ // crea un vettore di char che contiene word e restit. un puntatore

char *w =calloc((strlen(word)+1), sizeof(char));
strcpy(w, word);
return w;

}

char *createStr2(char*word){ // stessa funzione di createStr ma omette le virgolette (per la write)
    int numero, i=0;
 char *w =calloc((strlen(word)-1), sizeof(char));
 for(numero=0; word[numero]!='\0'; numero++){
   if(word[numero]!='"'){
        w[i]=word[numero];
        i++;
    }}
    w[i]='\0';
return w;

}

char *SpezzaStringa (char *percorso){ //Funzione che serve a spezzare le stringhe dei percorsi
    char *parola;
    parola = calloc((strlen(percorso)), sizeof(char));
    int i;
    i=0;
    ultimo=0;
    if(percorso[indice]=='/') indice++;
    if(percorso[indice]=='\0') {
        return NULL;
    }

    for(; percorso[indice]!='/' && percorso[indice]!='\0'; indice++){
            parola[i]=percorso[indice];
            i++;
    }
    if(percorso[indice]=='/')
        ultimo=0;
    else ultimo=1;
    parola[i]='\0';

    return parola;

}

int FuncHash(char * percorso){ //funzione di Hash
    int index=0, i=0;
    while(percorso[i]!='\0'){
        index = (( 27 * index + ValChar(percorso[i]))) % MaxSize;
        i++;
    }
    return index;

}

int ValChar(char c){

  return (c-'a')+1;

}

void create_dir(nodo * root, char * percorso){ //Trova se e dove va creata la directory, invocando quindi la NewNode

    nodo* new;
    new = TrovaPercorso(root, percorso);

    if(altezza<256 && new!=NULL && new->File==0 && new->Nfigli<1024 ){
        NewNodeB(new, percorso, Globalstr , Finindex);
        free(Globalstr);
    }
    else{
        printf("no\n");
        free(Globalstr);
    }

}

void create(nodo * root, char * percorso){ //Simile alla create_dir ma per i File
  nodo* new;
    new = TrovaPercorso(root, percorso);

    if(altezza<256  && new!=NULL && new->File==0 && new->Nfigli<1024 ){
        NewNodeA(new, percorso, Globalstr , Finindex);
        free(Globalstr);
    }
    else{
        printf("no\n");
        free(Globalstr);
    }

}

void read(nodo * root, char * percorso){
    nodo* trovato;
    trovato = TrovaPercorso(root, percorso);
    if(trovato!=NULL && trovato->figli[Finindex]!=NULL  ){
        trovato=ScorriLista(trovato->figli[Finindex], Globalstr);
        if(trovato!=NULL && trovato->File==1){
            printf("contenuto %s\n", trovato->Contenuto);
            free(Globalstr);
            return;
        }
    }
        printf("no\n");
        free(Globalstr);

}

void write(nodo * root, char * percorso, char * stringa){

nodo* trovato;
int numero;
    trovato = TrovaPercorso(root, percorso);
    if(trovato!=NULL && trovato->figli[Finindex]!=NULL ){
        trovato=ScorriLista(trovato->figli[Finindex], Globalstr);
        if(trovato!=NULL && trovato->File==1){

            free(trovato->Contenuto);
            trovato->Contenuto=createStr2(stringa);
            for(numero=0; trovato->Contenuto[numero]!='\0'; numero++);
            printf("ok %d\n", numero);
            free(Globalstr);
            return;
        }
    }
        printf("no\n");
        free(Globalstr);

}

void delete(nodo * root, char * percorso){

    nodo* trovato;
    int ok=0;
    trovato = TrovaPercorso(root, percorso);
    if(trovato!=NULL && trovato->figli[Finindex]!=NULL ){
        trovato=ScorriLista(trovato->figli[Finindex], Globalstr);
        if(trovato!=NULL  &&  trovato->Metadati==NULL){
            EliminaElem(trovato->Padre->Metadati, trovato->Nome, trovato->Padre);

            if(!(trovato->File))free(trovato->figli);
            if(trovato->File)free(trovato->Contenuto);
            if(!ok && trovato->prec==NULL){ //Se il nodo da eliminare è il primo della lista
                    trovato->Padre->figli[Finindex]=trovato->next;
                    if(trovato->next!=NULL) trovato->next->prec=NULL;
                    ok=1;
            }
            if(!ok &&  trovato->prec!=NULL){
                    trovato->prec->next=trovato->next;
                    if(trovato->next!=NULL) trovato->next->prec=trovato->prec;
                    ok=1;
            }
            free(trovato->Nome);
            free(trovato);
            printf("ok\n");
            free(Globalstr);
            return;
            }
    }
        printf("no\n");
        free(Globalstr);

}

int argomenti(char* numero){ //Funzione collegata al main che permettere di riconoscere i vari comandi di input

   caso=0;

        if(!strcmp(numero, "create")){
            caso=1;

            return 1;

        }
        if(!strcmp(numero, "create_dir")){
            caso=2;
            return 1;
        }
        if(!strcmp(numero, "read")){
            caso=3;
            return 1;
        }
        if(!strcmp(numero, "write")){
            caso=4;
            return 2;
        }
        if(!strcmp(numero, "delete")){
            caso=5;
            return 1;
        }
        if(!strcmp(numero, "delete_r")){
            caso=6;
            return 1;
        }
        if(!strcmp(numero, "find")){
            caso=7;
            return 1;
        }
        if(!strcmp(numero, "exit")){
            caso=8;
            return 0;
        }
return 10;

}

nodo * TrovaPercorso(nodo * root, char * percorso){ //funzione che scandisce i vari percorsi
    indice=0;
    ultimo=0;
    altezza=1;
    int uno=0;
    int index=0;
    char * copia, *sonda;

    copia=createStr(percorso);

    while(altezza<254){

        if(!uno && altezza>80){
        uno=1;
        indice=0;
            if(last!=NULL && !strncmp(percorso, last->Percorso, strlen(last->Percorso))){
                indice=strlen(last->Percorso)+1;
                sonda=SpezzaStringa(copia);
                if(ultimo){
                    Globalstr=sonda;
                    free(copia);
                    Finindex=index;
                    return last;
                }

                Globalstr=sonda;
                free(copia);
                return NULL;
            }
        }

        sonda=SpezzaStringa(copia);
        altezza++;

        if(strlen(sonda)>255){
            Globalstr=sonda;
            free(copia);
            Finindex=0;
            return NULL;
        }
        index=FuncHash(sonda);

        if(ultimo)
        {
                Globalstr=sonda;
                free(copia);
                Finindex=index;

                return root;}

        else
            if(!ultimo && root->figli[index]!=NULL ) {
                root=ScorriLista(root->figli[index], sonda);

                if(root==NULL){
                  Globalstr=sonda;
                  free(copia);
                  return NULL;
                }
                free(sonda);
            }
            else {
                Globalstr=sonda;
                free(copia);
                return NULL;
        }

    }
    return NULL;

}

void delete_r(nodo * root, char * percorso){
    nodo* trovato;
    int ok=0;

    trovato = TrovaPercorso(root, percorso);

    if(trovato!=NULL && trovato->figli[Finindex]!=NULL ){
        trovato=ScorriLista(trovato->figli[Finindex], Globalstr);
        if(trovato!=NULL  ){
            if(!ok && trovato->prec==NULL){ //Se il nodo da eliminare è il primo della lista
                trovato->Padre->figli[Finindex]=trovato->next;
                if(trovato->next!=NULL) trovato->next->prec=NULL;
                ok=1;
            }
            if(!ok &&  trovato->prec!=NULL){
                trovato->prec->next=trovato->next;
                if(trovato->next!=NULL) trovato->next->prec=trovato->prec;
                ok=1;
            }
            elimina(trovato);
            printf("ok\n");
            free(Globalstr);
            return;
        }
    }
    printf("no\n");
    free(Globalstr);

}

void elimina(nodo * root){ //funzione a cui si appoggia la delete_r
    int i;
    if(!(root->File))
        for(i=0; i < MaxSize; i++){
            if(root->figli[i]!=NULL){
               if(root->figli[i]->next!=NULL)
                    elimina(root->figli[i]->next);
               elimina(root->figli[i]);

            }
        }

        free(root->Nome);
        root->Padre=NULL;
        if(root->File)free(root->Contenuto);
        if(!(root->File))free(root->figli);
        free(root);
        root=NULL;
}

void find(nodo * root, char * risorsa){
    char * parol, *copia;
    indice=0;
    int index;
    listaordinata* lista=NULL;

    copia=createStr(risorsa);
    parol=SpezzaStringa(copia);
    index=FuncHash(parol);
    lista=Trova(root, root->Metadati, risorsa, index, lista);

    if(lista!=NULL)
        printLista(lista);
    else printf("no\n");

}

listaordinata * creaLista(listaordinata * lista ,char * aggiungi){ //Questa e le funzioni seguenti servono a gestire le liste(find e metadati)
    listaordinata * elemento, *scorri;

    if(lista==NULL){
        elemento = malloc(sizeof(listaordinata));
        lista=elemento;
        lista->prec=NULL;
        lista->next=NULL;
        lista->percorso=aggiungi;
        return lista;
    }
    elemento=lista;
    scorri=lista->prec;
    while(elemento!=NULL && strcmp(aggiungi, elemento->percorso)>0){
        scorri=elemento;
        elemento=elemento->next;
    }
    elemento = malloc(sizeof(listaordinata));
    if(scorri==NULL) {
        lista->prec=elemento;
        elemento->next=lista;
        lista=elemento;
    }
    else {
        elemento->next=scorri->next;
        scorri->next=elemento;
    }
    elemento->prec=scorri;
    elemento->percorso=aggiungi;
    return lista;

}
void printLista(listaordinata * list){
    listaordinata* scorri;
    scorri=list;
    while(scorri!=NULL){
            if(scorri->next!=NULL && !(strcmp(scorri->percorso, scorri->next->percorso)))
                scorri=scorri->next;
        printf("ok %s\n", scorri->percorso);
        scorri=scorri->next;
    }

}

listaordinata* Trova(nodo * root, listaordinata* meta , char * risorsa, int index, listaordinata* lista){
    listaordinata * scorri;
    int findex, ok;

    scorri=meta;
    if(scorri==NULL)return NULL;
    if(scorri!=NULL && scorri->next!=NULL)
        lista=Trova(root, scorri->next, risorsa, index, lista);

    findex=FuncHash(scorri->percorso);

    if(root!=NULL && root->figli[findex]!=NULL && root->figli[findex]->Metadati!=NULL)
        lista=Trova(root->figli[findex], root->figli[findex]->Metadati, risorsa, index, lista );
    if(root!=NULL && root->figli[findex]!=NULL && root->figli[findex]->Nome!=NULL){
        ok=strcmp(risorsa, root->figli[findex]->Nome);
    }
    if(!ok){
        lista=creaLista(lista, root->figli[findex]->Percorso);
    }
    return lista;

}

nodo * ScorriLista(nodo * root, char * nome ){
    nodo * scorri=root;
    while(scorri!=NULL){
        if(!(strcmp(nome, scorri->Nome)))
            return scorri;
        scorri=scorri->next;
    }
    return NULL;

}


void EliminaElem(listaordinata *lista, char * nome, nodo * nodo){
    listaordinata *trovato=lista, *prece;
    int ok=1;
    while (trovato!=NULL && ok){
        ok=strcmp(trovato->percorso, nome);
        prece=trovato;
        trovato=trovato->next;
    }
    if(trovato==NULL)return;
    if(!ok)trovato=prece;
    if(!ok && trovato->prec==NULL){
        nodo->Metadati=trovato->next;
        if(trovato->next!=NULL) trovato->next->prec=NULL;
        ok=1;
    }
    if(!ok &&  trovato->prec!=NULL){
        trovato->prec->next=trovato->next;
        if(trovato->next!=NULL) trovato->next->prec=trovato->prec;
        ok=1;
    }

}
