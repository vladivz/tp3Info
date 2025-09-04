#include <stdio.h>
//declaracion de funciones
float calcularAreaRectangulo(float longitud, float altura);
float calcularPerimetroRectangulo(float longitud, float altura);
float calcularDiagonalRectangulo(float longitud, float altura);
float calcularAreaCirculo(float radio);
float calcularPerimetroCirculo(float radio);
void imprimirResultados(float area, float perimetro);
//desarrollo de funciones
float calcularAreaRectangulo(float longitud, float altura) {
    return longitud * altura;
}

float calcularPerimetroRectangulo(float longitud, float altura) {
    return 2.0f * (longitud + altura);
}

float calcularDiagonalRectangulo(float longitud, float altura) {
    float diagonal = longitud * longitud + altura * altura;
    return diagonal;
}

float calcularAreaCirculo(float radio) {
    const float PI = 3.14159f;
    return PI * radio * radio;
}

float calcularPerimetroCirculo(float radio) {
    const float PI = 3.14159f;
    return 2.0f * PI * radio;
}

void imprimirResultados(float area, float perimetro) {
    printf("\nEl area es: %.2f\n", area);
    printf("El perimetro es: %.2f\n", perimetro);
}

int main(void) {
    int opcion = 0;
    float longitud, altura, radio;
    float area, perimetro;

    do {
        printf("Ingrese la figura que desea calcular (1: rectangulo, 2: circulo): \n ");
        scanf_s("%d", &opcion);
       
    } while (opcion != 1 && opcion != 2);

    if (opcion == 1) {
        printf("Opcion de rectangulo seleccionada\n");
        printf("Ingrese la longitud del rectangulo: \n ");
        scanf_s("%f", &longitud);
        printf("Ingrese la altura del rectangulo: \n");
        scanf_s("%f", &altura);

        area = calcularAreaRectangulo(longitud, altura);
        perimetro = calcularPerimetroRectangulo(longitud, altura);
        imprimirResultados(area, perimetro);
        printf("La diagonal del rectangulo es: %.2f\n", calcularDiagonalRectangulo(longitud, altura));
    }
    else { 
        printf("Opcion de circulo seleccionada\n");
        printf("Ingrese el radio del circulo: \n");
        scanf_s("%f", &radio);

        area = calcularAreaCirculo(radio);
        perimetro = calcularPerimetroCirculo(radio);
        imprimirResultados(area, perimetro);
    }

    return 0;
}
