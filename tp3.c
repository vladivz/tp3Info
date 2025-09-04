/*2. Realizar un programa en lenguaje C que solicite al usuario el ingreso de peso y altura y
devuelva dicho índice y una tabla de referencia.
Ejemplo de salida del programa:

Ingrese el peso en kg: 78
Ingrese la altura en metros: 1.87

Su índice de masa corporal es: 22.31

Índice| Condición
-----------------------------
<18.5 | Bajo peso
18.5 a 24.9 | Normal
25.0 a 29.9 | Sobrepeso
>=30| Obesidad
*/
#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS //esto es una macro para sacar un error que me esta generando visual studio

int main() {
    float peso, altura, IMC; //definicion de variables como flotantes
    printf("colocar su peso en kg\n");
    scanf( "%f", &peso); //ESCANEAR O OBTENER DATO DE PESO
    printf("colocar su altura en metros\n");
    scanf("%f", &altura);
    //calculo del IMC
    IMC = (peso) / (altura * altura);
    printf("su IMC es: %.2f\n\n", IMC);
    //tabla que pide la consigna
    printf("Indice | Condicion\n");
    printf("------------------------\n");
    printf("<18.5  | Bajo peso\n");
    printf("18.5 a 24.9 | Normal\n");
    printf("25.0 a 29.9 | Sobrepeso\n");
    printf(">=30   | Obesidad\n");
    printf("\nSegun su IMC, su condicion es: ");
    //condicion nueva:
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