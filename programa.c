#include <string.h>
#include <sys/types.h> 
#include <unistd.h> 
struct datos {
 char nom[20];
 int edad;
};
char verifica(struct datos *d){
 char sig[5];
 strcpy(d->nom,strcat(d->nom, d->nom));
 strcpy(sig,"---");
 strcat(d->nom, sig);
 return d->nom[0];
}
void main(){
 struct datos d1; int p;
 char x;
 strcpy(d1.nom," !123"); d1.edad=15;
 if((p=fork()) == 0) {
    printf("\nProcesoHijo 1\n");
	d1.edad += 2; 
	fork();
	printf("ProcesoHijo 2\n");
	printf("** %c %d %s", verifica(&d1), d1.edad, d1.nom); 
	exit(0); 
	fork();
 }else if (p>0){
        printf("\nProcesoPadre 1\n");
		fork();
		printf("\nProcesoPadre 2\n");
		d1.nom[0] = "$"; 
		d1.edad += 1; 
		printf("%c --- %d ... %s",verifica(&d1), d1.edad,d1.nom);
	}
	exit(1);
}
