/* Repositorio: https://github.com//vladivz/tp3Info/ */
#include <stdio.h>
#include <stdlib.h> 
int TAM;

int main(void) {
    printf("colocar la cantidad de productos que desea cargar\n");
    scanf("%d", &TAM);
    int *codigos = (int*) malloc(TAM * sizeof(int));
    double *precios = (double*) malloc(TAM * sizeof(double));

    for (int i = 0; i < TAM; i++) {
        
        printf("Ingrese codigo de barras (%d-%d)\n", i+1, TAM);
    
        scanf("%d", &codigos[i]);
        while (codigos[i] < 0 || codigos[i]>999999999) {
            printf("codigo invalido\n");
            scanf("%d", &codigos[i]);
        }
        printf("ingrese valor del producto\n");
        scanf("%lf",&precios[i]);
        long centavos = (long)(precios[i]*100);
        while(precios[i]<0||(precios[i] * 100 != centavos)){
            printf("precio invalido\n");
            scanf("%lf", &precios[i]);
            centavos = (long)(precios[i]*100);
        }


    }
    for (int i = 0; i < TAM; i++){
        printf("producto [%d]: $%.2f\n",codigos[i],precios[i]);
    }
    int DirMayor = 0;
    int DirMenor = 0;

    for (int i = 1; i < TAM; i++) {
    if (precios[i] > precios[DirMayor]) {
        DirMayor = i;
    }
    if (precios[i] < precios[DirMenor]) {
        DirMenor = i;
    }
}
    printf("mas caro: [%d] $%.2f\n", codigos[DirMayor], precios[DirMayor]);
    printf("mas barato: [%d] $%.2f\n", codigos[DirMenor], precios[DirMenor]);
    free(codigos);
    free(precios);

    return 0;
}

