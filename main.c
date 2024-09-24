//Librerias y macros

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAG "\033[35m"
#define FIN "\033[0m"
#define ERROR_MEM -1
#define TODO_OK 0

//Prototipos

int conway(); //
int ** crearTablero(unsigned int f, unsigned int c); // creo el tablero
void caPlaneador(int ** tablero, int x, int y); // creo el cañon planeador
int contarVecinosVivos(int ** tablero, int x, int y, unsigned int f, unsigned int c); // Cuenta la cantidad de vecinos vivos, seg posicion pasada
void sigGen(int ** tablero, int ** proxGen, unsigned int f, unsigned int c); // Calcula la siguiente generarion
void actTablero(int ** tablero, int ** proxGen, unsigned int f, unsigned int c); // Actualiza el tablero original respecto a la proxGen
int cantPoblacion(int ** tablero, unsigned int f, unsigned int c);//Cuenta la poblacion
void mostrarTablero(int ** tablero, unsigned int f, unsigned int c); //Muestra del tablero
void liberoMem(int ** tablero, int ** proxGen, unsigned int f); // Libero memoria


int main()
{
    conway();
    return TODO_OK;
}

//Funciones

int conway(){
    //valores iniciales del programa
    int f = 0;
    int c = 0;
    printf("ingrese cantidad de fila: \t");
    scanf("%d", &f);
    printf("\ningrese cantidad de columnas: \t");
    scanf("%d", &c);

    system("cls");

    //creo los tableros con las filas y columnas dadas
    int ** tablero = crearTablero(f,c);
    int ** proxGen = crearTablero(f,c);

    //libero memoria si falla
    if(!tablero) {
        liberoMem(tablero, proxGen, f);
        return ERROR_MEM;
    }
    if(!proxGen) {
        liberoMem(tablero, proxGen, f);
        return ERROR_MEM;
    }



    //comienza el programa
    int poblacion = 0;
    int generacion = 0;

    while(1){

        system("cls");
        printf("Generacion: \t%d\n", generacion);
        poblacion = cantPoblacion(tablero, f, c);
        printf("Poblacion: \t%d\n", poblacion);

        mostrarTablero(tablero, f, c);

        sigGen(tablero, proxGen, f, c);
        actTablero(tablero, proxGen, f, c);

        generacion++;

        usleep(500000); // 0.5  seg = 500000
    }

    liberoMem(tablero, proxGen, f);
    return TODO_OK;
}

int ** crearTablero(unsigned int f, unsigned int c){
    int ** tab;
    tab = (int **)malloc(f*sizeof(int *));

    if(tab == NULL){
        printf("Error asignacion mem(1)");
        free(tab);
        return ERROR_MEM;
    }

    for(int x = 0; x < f; x ++){
        tab[x] = (int *)malloc(sizeof(int)*c);

        if(tab[x] == NULL){
            printf("Error asignacion mem(2)");

            for (int y = 0; y < x; y++) {
                free(tab[y]); //libero memoria
            }

            free(tab); //libero memoria

            return ERROR_MEM;
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
    //ubico cañon de planeador seg coordenadas
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

void sigGen(int ** tablero, int ** proxGen, unsigned int f, unsigned int c){
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

void mostrarTablero(int ** tablero, unsigned int f, unsigned int c){
    for(int i=0; i<f; i++){
        for(int j=0; j<c; j++){
            (tablero[i][j] == 1) ? printf(MAG"[#]\t"FIN) : printf("[ ]\t"); //\033[35m[#]\t\033[0m magenta
        }
        printf("\n");
    }
}

void liberoMem(int ** tablero, int ** proxGen, unsigned int f){
    for(int x = 0; x < f; x ++){
        free(tablero[x]); //libero memoria
        free(proxGen[x]);
    }
    free(tablero); //libero memoria
    free(proxGen);
}
