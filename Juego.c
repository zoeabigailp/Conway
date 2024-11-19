#include "Juego.h"
int inicializarTablero_Archivo(const char *nombre, int **tablero)
{

    //variables:
    char linea[100];
    tCoordenadas c;

    FILE * arch = fopen(nombre, "rt");
    if(!arch)
    {
        return -1;
    }

    //
   while(fgets(linea, sizeof(linea), arch))
   {

        trozar(linea, &c);
        tablero[c.x][c.y] = 1;
   }
   fclose(arch);
   return 1;

}
int crearPatron_archivo(const char * nombre)
{
    FILE * archivo = fopen(nombre, "wt");
    if(!archivo)
    {
        printf("ERROR EN LA CARGA DEL PATRON");
        return 0;
    }
    fprintf(archivo, "1|2\n");
    fprintf(archivo, "1|3\n");
    fprintf(archivo, "1|4\n");
    fprintf(archivo, "2|1\n");
    fprintf(archivo, "2|2\n");
    fprintf(archivo, "2|5\n");
    fprintf(archivo, "2|6\n");
    fprintf(archivo, "3|2\n");

    fclose(archivo);
    return 1;
}
void trozar(char * linea, tCoordenadas * c)
{
    char * aux = strchr(linea , '\n');
    *aux = '\0';

    aux = strrchr(linea, '|');
    sscanf(aux+1, "%d", &c->y);
    *aux = '\0';

    sscanf(linea, "%d", &c->x);
    *aux = '\0';
}
void patron_canonPlaneador(int **tablero, unsigned int fila, unsigned int col)
{
    //NO
    int x = (fila - 3) / 2;
    int y = (col - 3) / 2;

    tablero[x + 1][y + 2] = 1;
    tablero[x + 1][y + 3] = 1;
    tablero[x + 1][y + 4] = 1;
    tablero[x + 2][y + 1] = 1;
    tablero[x + 2][y + 2] = 1;
    tablero[x + 2][y + 5] = 1;
    tablero[x + 2][y + 6] = 1;
    tablero[x + 3][y + 2] = 1;
}
void inicializarTablero(int **tablero, unsigned int fila, unsigned int col)
{
    int x = (fila - 3) / 2; // el 3 va por el tamaño del patron
    int y = (col - 3) / 2;

    tablero[x][y] = 1;
    tablero[x][y + 1] = 1;
    tablero[x+1][y+1] = 1;
    tablero[x+2][y+1] = 1;
    tablero[x+1][y+2] = 1;
}
void procesarTablero(int **tablero, unsigned int fila, unsigned int col)
{
    int** tableroSiguiente = crearTablero(fila, col);
    int cantVecinos = 0;
    for(int i = 0; i<fila; i++)
    {
        for(int j = 0; j<col; j++)
        {
            cantVecinos = cantVecinosVivos(tablero, i, j, fila, col);
            if( (cantVecinos== 2 || cantVecinos==3)  && tablero[i][j] == 1 )
            {
                //permanece viva
                tableroSiguiente[i][j] = 1;
            }
            else if (tablero[i][j] == 0 && cantVecinos == 3)
            {
                tableroSiguiente[i][j] = 1;
            }
            else
            {
                tableroSiguiente[i][j] = 0;
            }
        }
    }

    //copia de tableros
    for(int i = 0; i< fila; i++)
    {
        for(int j = 0; j< col; j++)
        {
            tablero[i][j]  = tableroSiguiente[i][j];
        }
    }

    free(tableroSiguiente);
}
int cantVecinosVivos(int ** tablero, int x, int y, unsigned int fila, unsigned int col)
{
    int cantVivas = 0;
    for(int i = x-1; i<= x+1; i++)
    {
        for(int j = y -1; j<= y+1; j++)
        {
            /*
                evito excederme de los limites del tablero:
                i no pueden ser nunca negativas (significa que esto al limite del lado izquierdo del tablero)
                j no pueden ser nunca negativas (significa que esto al limite del lado superior del tablero)
                i no pueden exceder a la cantidad de filas (significa que estoy al limite del lado derecho)
                j no pueden exceder a la cantidad de columnas (significa que estoy al limite del lado inferior del tablero)
            */
            if( i >=0 && i<fila  && j>=0 && j<col) //
            {
                if(tablero[i][j] == 1 && !(i == x && j == y) )
                {
                    cantVivas++;
                }
            }
        }
    }
    return cantVivas;
}
void destruirTablero(int **tablero, unsigned int fila)
{
    for(int i = 0; i<fila; i++)
    {
        free(tablero[i]);
    }

    free(tablero);
}
int** crearTablero(unsigned int fila, unsigned int col)
{
    int ** tablero = (int**)calloc(fila, sizeof(int *));

    if(tablero == NULL)
    {
        printf("\nError en la asignacion de memoria para las filas\n");
        return NULL; //Estoy en duda.
    }

    for(int i=0; i<fila; i++)
    {
        tablero[i] = (int *)calloc(col, sizeof(int));
        if(tablero[i] == NULL)
        {
            for(int j = 0; j < i; j++)
            {
                free(tablero[j]);
            }

            free(tablero);
            printf("\nError en la asignacion de memoria para las columnas\n");
            return NULL;
        }
    }

    return tablero;
}
void configurarTablero(int * config)
{
    printf("\n----------- CONFIGURACIONES INICIALES -----------\n");

    ///Ingresar filas:
    printf("\nIngrese un numero de filas entre 5 y 80: ");
    scanf("%d", config);
    while(*config <5 || *config > 80)
    {
        printf("\nNumero de filas invalido, vuelva intentar: ");
        scanf("%d", config);
    }

    ///Ingresar columnas:
    printf("\nIngrese un numero de columnas entre 5 y 60:");
    config++;
    scanf("%d", config);
    while(*config <5 || *config > 60)
    {
        printf("\nNumero de columnas invalido, vuelva intentar: ");
        scanf("%d", config);
    }

    printf("\n-------------------------------------------------\n");
}


//Renderizados
void imprimirTablero(int **tablero, unsigned int fila, int unsigned col, SDL_Renderer * renderer )
{
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 0xFF);
    SDL_RenderClear(renderer);
    for(int i = 0; i <fila; i++)
    {
        for(int j = 0; j < col; j++)
        {
            if(tablero[i][j] != 0)
            {
                drawSquare(renderer,  i * TAM_SQUARE, j * TAM_SQUARE);
            }
        }
    }

    SDL_RenderPresent(renderer);
}
void drawSquare(SDL_Renderer * renderer, unsigned int posX, unsigned int posY)
{
    SDL_Rect square;
    //posicion
    square.x = posX;
    square.y = posY;
    //Tamaño
    square.h = TAM_SQUARE;
    square.w = TAM_SQUARE;

    //Lo cargo al render:
    SDL_SetRenderDrawColor(renderer, 0, 200, 0, 0xFF);
    SDL_RenderFillRect(renderer, &square);
}
