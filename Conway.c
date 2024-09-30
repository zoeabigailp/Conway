#include "Conway.h"
//Funciones

int ** crearTablero(unsigned int f, unsigned int c){
    int ** tab;
    tab = (int **)malloc(f*sizeof(int *));

    if(tab == NULL){
        printf("Error asignacion mem(1)\n");
        return NULL;
    }

    for(int x = 0; x < f; x ++){
        tab[x] = (int *)malloc(sizeof(int)*c);

        if(tab[x] == NULL){
            printf("Error asignacion mem(2)\n");

            for (int y = 0; y < x; y++) {
                free(tab[y]); //libero memoria
            }

            free(tab); //libero memoria

            return NULL;
        }
    }

    //inicializo la matriz con 0
    int i = 0;
    int j = 0;
    for(i=0; i<f; i++){
        for(j=0; j<c; j++){
            tab[i][j] = 0;
        }
    }

    //calculo mitad del tablero
    int x = (int)(f-4)/2;
    int y = (int)(c-5)/2;
    //ubico ca�on de planeador seg coordenadas
    caPlaneador(tab, x, y);

    return tab;
}

void caPlaneador(int ** tablero, int x, int y){
    tablero[x + 1][y + 1] = 1;
    tablero[x + 1][y + 2] = 1;
    tablero[x + 2][y + 2] = 1;
    tablero[x + 3][y + 2] = 1;
    tablero[x + 2][y + 3] = 1;
    /*tablero[4][3] = 1;
    tablero[4][4] = 1;
    tablero[5][4] = 1;
    tablero[6][4] = 1;
    tablero[5][5] = 1;*/
}

int contarVecinosVivos(int ** tablero, int x, int y, unsigned int f, unsigned int c){
    int contVecinos = 0;
    for(int i = x - 1; i <= x + 1; i++  ){
        for(int j = y - 1; j <= y + 1; j++){
            if( (i >= 0 && i< f && j >= 0 && j < c) && !(i == x && j == y) ) contVecinos += tablero[i][j]; //Establezco que no se exceda del tablero y que no sean las coordenadas enviadas
        }
    }
    return contVecinos;
}

void calcularSiguienteGeneracion(int ** tablero, int ** proxGen, unsigned int f, unsigned int c){
    int vecVivos = 0;
    for(int i = 0; i < f; i++){
        for(int j = 0; j < c; j++){
            vecVivos = contarVecinosVivos(tablero, i, j, f, c); //cantidad de vecinos en esa posicion
            if(tablero[i][j] == 1){
                proxGen[i][j] = (vecVivos == 2 || vecVivos == 3) ? 1 : 0 ; // si los vecVivos son 2 o 3, guardo 1 y si no 0
            }
            else{
                proxGen[i][j] = (vecVivos == 3) ? 1 : 0; // si vecVivos es 3, guardo 1 y si no 0
            }
        }
    }
}

void actTablero(int ** tablero, int ** proxGen, unsigned int f, unsigned int c){
    for(int i = 0; i < f; i ++){
        for(int j = 0; j < c; j++){
            tablero[i][j] = proxGen[i][j];
        }
    }
}

int cantPoblacion(int ** tablero, unsigned int f, unsigned int c){
    int poblacion = 0;
    for(int i = 0; i < f; i++){
        for(int j = 0; j < c; j++){
            if(tablero[i][j] == 1) poblacion++;
        }
    }

    return poblacion;
}

void liberoMem(int ** tablero, int ** proxGen, unsigned int f){
    for(int x = 0; x < f; x ++){
        free(tablero[x]); //libero memoria
        free(proxGen[x]);
    }
    free(tablero); //libero memoria
    free(proxGen);
}
