#include <stdio.h>
#include <stdlib.h>

#define N_MAX 100
#define M_MAX 100

struct matrix {
    int filas;
    int columnas;
    float mat[N_MAX][M_MAX];
};

typedef struct matrix matrix_t;

void imprimir_matriz(matrix_t m);
void cargar_matriz(matrix_t *m, char *nombre_archivo);
void sumar_matrices(matrix_t A, matrix_t B, matrix_t *C);
void guardar_matriz(matrix_t m, char *nombre_archivo);

int main(void) {
    matrix_t A = {0};
    cargar_matriz(&A, "mat_A.txt");

    matrix_t B = {0};
    cargar_matriz(&B, "mat_B.txt");

    matrix_t C = {0};
    sumar_matrices(A, B, &C);

    printf("Matriz A:\n");
    imprimir_matriz(A);

    printf("Matriz B:\n");
    imprimir_matriz(B);

    printf("Matriz C = A + B:\n");
    imprimir_matriz(C);

    guardar_matriz(C, "mat_C.txt");

    return 0;
}

void imprimir_matriz(matrix_t m) {
    for (int i = 0; i < m.filas; i++) {
        for (int j = 0; j < m.columnas; j++) {
            printf("%9.2f", m.mat[i][j]);
        }
        printf("\n");
    }
}

void cargar_matriz(matrix_t *m, char *nombre_archivo) {
    FILE *f = fopen(nombre_archivo, "r");
    if (f == NULL) {
        printf("Error: no se pudo abrir el archivo %s\n", nombre_archivo);
        exit(1);
    }

    if (fscanf(f, "%d %d", &m->filas, &m->columnas) != 2) {
        printf("Error: formato invalido en %s (filas/columnas)\n", nombre_archivo);
        fclose(f);
        exit(1);
    }

    if (m->filas < 1 || m->filas > N_MAX || m->columnas < 1 || m->columnas > M_MAX) {
        printf("Error: dimensiones fuera de rango en %s\n", nombre_archivo);
        fclose(f);
        exit(1);
    }

    for (int i = 0; i < m->filas; i++) {
        for (int j = 0; j < m->columnas; j++) {
            if (fscanf(f, "%f", &m->mat[i][j]) != 1) {
                printf("Error: datos insuficientes en %s\n", nombre_archivo);
                fclose(f);
                exit(1);
            }
        }
    }

    fclose(f);
}

void sumar_matrices(matrix_t A, matrix_t B, matrix_t *C) {
    if (A.filas != B.filas || A.columnas != B.columnas) {
        printf("Error: las matrices no tienen las mismas dimensiones.\n");
        exit(1);
    }

    C->filas = A.filas;
    C->columnas = A.columnas;

    for (int i = 0; i < A.filas; i++) {
        for (int j = 0; j < A.columnas; j++) {
            C->mat[i][j] = A.mat[i][j] + B.mat[i][j];
        }
    }
}

void guardar_matriz(matrix_t m, char *nombre_archivo) {
    FILE *f = fopen(nombre_archivo, "w");
    if (f == NULL) {
        printf("Error: no se pudo crear el archivo %s\n", nombre_archivo);
        exit(1);
    }

    fprintf(f, "%d %d\n", m.filas, m.columnas);

    for (int i = 0; i < m.filas; i++) {
        for (int j = 0; j < m.columnas; j++) {
            fprintf(f, "%9.2f", m.mat[i][j]);
        }
        fprintf(f, "\n");
    }

    fclose(f);
}
