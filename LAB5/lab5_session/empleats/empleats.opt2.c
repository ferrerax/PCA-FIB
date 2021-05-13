#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_NOM 40
#define MAX_DIAS_MES 31 
#define MAX_CATEGORIA 40 
#define MAX_ADDRESS 100
#define MAX_EMPLEATS 4294967295

/*
#define EXTES
*/

typedef struct 
{
  long long int NID;              /* Identification number - rang:0-2^32-1*/
  char Nom[MAX_NOM];  
  char Cognoms[MAX_NOM];  
  int  horesMes[MAX_DIAS_MES]; 
  char Categoria[MAX_CATEGORIA];
  unsigned int ptrClauForaneaDepartament;
  unsigned int ptrClauCategoria;
#ifdef EXTES     
  char Address[MAX_ADDRESS];
  char Nom_parella[MAX_NOM];
  char Cognoms_parella[MAX_NOM];
#endif
} Templeat;

typedef struct {
  unsigned int nid;
  Templeat *empleat;
} ondestas;

int compare(const void *p1, const void *p2)
{
    if (((ondestas *)p1)->nid > ((ondestas*) p2)->nid) return(1);
    else if (((ondestas *)p1)->nid < ((ondestas*) p2)->nid) return(-1);
    else return(0);

}

int main(int argc, char *argv[])
{
	Templeat *empleats;
	unsigned int N = 2000000, i;

	if (argc > 1) N = atoi(argv[1]);

  ondestas *onde = malloc(sizeof(ondestas)*N);
	empleats = (Templeat *) malloc(N*sizeof(Templeat));
	if (empleats == NULL) { fprintf(stderr, "Out of memory\n"); exit(0); }
	memset(empleats, 0, N*sizeof(Templeat));

	/* Random generation */
  unsigned int nid;
	for (i=0; i<N; i++) {
    nid = rand();
    onde[i].nid = nid;
    onde[i].empleat = &empleats[i];
		empleats[i].NID = nid;
		empleats[i].Nom[0] = 'A' + ((i>>3) + (i<<2)) & 0x15;
		empleats[i].Cognoms[0] = 'a' + ((i>>2) + (i<<3)) & 0x15;
		empleats[i].horesMes[i % MAX_DIAS_MES] = i;
		empleats[i].Categoria[i % MAX_CATEGORIA] = (i << 2) * i;
		empleats[i].ptrClauForaneaDepartament = i<<10 + i<<14 + i>>3;
		empleats[i].ptrClauCategoria = i<<24 + i<< 18 + i<<10 + i>>2;
#ifdef EXTES     
                empleats[i].Address[0] = 'A' + ((i>>4) + (i<<2)) & 0x15;
                empleats[i].Nom_parella[0] = 'a' + ((i>>5) + (i<<2)) & 0x15;
  		empleats[i].Cognoms_parella[0] = 'A' + ((i>>5) + (i<<3)) & 0x15;
#endif
	}

	qsort(onde, N, sizeof(ondestas), compare);
	Templeat *buffer = (Templeat *) malloc(N*sizeof(Templeat));
	memset(buffer, 0, N*sizeof(Templeat));
	for (i=0; i<N; i++){
	        //write(1, onde[i].empleat,sizeof(Templeat));
          //memcpy(&buffer[i],onde[i].empleat,sizeof(Templeat)); 
          buffer[i] = *(onde[i].empleat);
          
	}
	write(1, buffer,sizeof(Templeat)*N);

	return 0;
}
