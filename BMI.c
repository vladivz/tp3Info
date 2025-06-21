/* 2. Realizar un programa en lenguaje C que solicite al usuario el ingreso de peso y altura y
devuelva dicho índice y una tabla de referencia.
Ejemplo de salida del programa:

Ingrese el peso en kg: 78
Ingrese la altura en metros: 1.87

Su índice de masa corporal es: 22.31

Índice | Condición
-----------------------------
<18.5  | Bajo peso
18.5 a 24.9 | Normal
25.0 a 29.9 | Sobrepeso
>=30   | Obesidad

Repositorio: https://github.com/vladivz/tp3Info/tree/tp3-condicion/tp3Info
*/

#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS // esto es una macro para sacar un error que me esta generando visual studio

int main() {
    float peso, altura, IMC; // definicion de variables como flotantes

    // Ingreso del peso, con validación
    do {
        printf("Colocar su peso en kg:\n");
        scanf("%f", &peso);
        if (peso <= 0) {
            printf("ERROR: El peso debe ser un número positivo mayor que cero.\n");
        }
    } while (peso <= 0);

    // Ingreso de la altura, con validación
    do {
        printf("Colocar su altura en metros:\n");
        scanf("%f", &altura);
        if (altura <= 0) {
            printf("ERROR: La altura debe ser un número positivo mayor que cero.\n");
        }
    } while (altura <= 0);

    // Calculo del IMC
    IMC = peso / (altura * altura);

    printf("Su IMC es: %.2f\n\n", IMC);

    // Tabla que pide la consigna
    printf("Indice | Condicion\n");
    printf("------------------------\n");
    printf("<18.5  | Bajo peso\n");
    printf("18.5 a 24.9 | Normal\n");
    printf("25.0 a 29.9 | Sobrepeso\n");
    printf(">=30   | Obesidad\n");

    printf("\nSegun su IMC, su condicion es: ");

    // Evaluación de la condición
    if (IMC < 18.5) {
        printf("Bajo peso\n");
    }
    else if (IMC >= 18.5 && IMC <= 24.9) {
        printf("Normal\n");
    }
    else if (IMC >= 25.0 && IMC <= 29.9) {
        printf("Sobrepeso\n");
    }
    else {
        printf("Obesidad\n");
    }

    return 0;
}
