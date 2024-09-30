#include "Conway.h"
//Funciones

int * configuracionInicial()
{
    int * valores = (int*)malloc(2*sizeof(int));
    if(!valores)
    {
        printf("Error al asignar mem");
        return NULL;
    }

        printf("******** CONFIGURAR TABLERO ********");
        printf("\n\n\n");

        printf("Ingrese cantidad de fila [5-60]: \t");
        scanf("%d", &valores[0]);

        printf("\n\n");

        printf("\nIngrese cantidad de coumnas [5-60]: \t");
        scanf("%d", &valores[1]);

        while(valores[0] < 5 || valores[0] > 60){
            printf("\nIngrese nuevamente la cantidad de filas: \t");
            scanf("%d", &valores[0]);
        }
        while(valores[1] < 5 || valores[1] > 60){
            printf("\nIngrese nuevamente la cantidad de columnas: \t");
            scanf("%d", &valores[1]);
        }

        printf("\n\n\n");
        printf("************************************\n");
        system("cls");

    return valores;
}

int ** crearTablero(unsigned int f, unsigned int c)
{
    int ** tab;
    tab = (int **)malloc(f*sizeof(int *));

    if(tab == NULL)
    {
        printf("Error asignacion mem(1)\n");
        return NULL;
    }

    for(int x = 0; x < f; x ++)
    {
        tab[x] = (int *)malloc(sizeof(int)*c);

        if(tab[x] == NULL)
        {
            printf("Error asignacion mem(2)\n");

            for (int y = 0; y < x; y++)
            {
                free(tab[y]); //libero memoria
            }

            free(tab); //libero memoria

            return NULL;
        }
    }

    //inicializo la matriz con 0
    int i = 0;
    int j = 0;
    for(i=0; i<f; i++)
    {
        for(j=0; j<c; j++)
        {
            tab[i][j] = 0;
        }
    }

    //calculo mitad del tablero
    int x = (int)(f-4)/2;
    int y = (int)(c-5)/2;
    //ubico ca�on de planeador seg coordenadas
    patronInicial(tab, x, y);

    return tab;
}

void patronInicial(int ** tablero, int x, int y)
{
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

void cuadrado(SDL_Renderer * renderer, SDL_Rect fillRect, int** tab, int f, int c)
{
    for(int i = 0; i < f; i ++)
    {
        for(int j = 0; j < c; j++)
        {
            if(tab[i][j] == 1)
            {
                fillRect.x = i*15; //posicion
                fillRect.y = j*15; // posicion
                fillRect.h = 15; //alto
                fillRect.w = 15; //ancho

                SDL_SetRenderDrawColor(renderer, 91, 15, 54, .8); //rgba(144, 12, 63, 1)
                SDL_RenderFillRect(renderer, &fillRect);
            }
        }
    }
}

int contarVecinosVivos(int ** tablero, int x, int y, unsigned int f, unsigned int c)
{
    int contVecinos = 0;
    for(int i = x - 1; i <= x + 1; i++  )
    {
        for(int j = y - 1; j <= y + 1; j++)
        {
            if( (i >= 0 && i< f && j >= 0 && j < c) && !(i == x && j == y) ) contVecinos += tablero[i][j]; //Establezco que no se exceda del tablero y que no sean las coordenadas enviadas
        }
    }
    return contVecinos;
}

void calcularSiguienteGeneracion(int ** tablero, int ** proxGen, unsigned int f, unsigned int c)
{
    int vecVivos = 0;
    for(int i = 0; i < f; i++)
    {
        for(int j = 0; j < c; j++)
        {
            vecVivos = contarVecinosVivos(tablero, i, j, f, c); //cantidad de vecinos en esa posicion
            if(tablero[i][j] == 1)
            {
                proxGen[i][j] = (vecVivos == 2 || vecVivos == 3) ? 1 : 0 ; // si los vecVivos son 2 o 3, guardo 1 y si no 0
            }
            else
            {
                proxGen[i][j] = (vecVivos == 3) ? 1 : 0; // si vecVivos es 3, guardo 1 y si no 0
            }
        }
    }
}

void actTablero(int ** tablero, int ** proxGen, unsigned int f, unsigned int c)
{
    for(int i = 0; i < f; i ++)
    {
        for(int j = 0; j < c; j++)
        {
            tablero[i][j] = proxGen[i][j];
        }
    }
}

int cantPoblacion(int ** tablero, unsigned int f, unsigned int c)
{
    int poblacion = 0;
    for(int i = 0; i < f; i++)
    {
        for(int j = 0; j < c; j++)
        {
            if(tablero[i][j] == 1) poblacion++;
        }
    }

    return poblacion;
}

void liberoMem(int ** tablero, int ** proxGen, unsigned int f)
{
    for(int x = 0; x < f; x ++)
    {
        free(tablero[x]); //libero memoria
        free(proxGen[x]);
    }
    free(tablero); //libero memoria
    free(proxGen);
}

void conway(){
 //inicia variables
    unsigned char done;
    //int k;
    int * dimTab = configuracionInicial();

    //Mucha de esta parametrizacion puede venir por linea de comando!!
    int delay = 100;
    SDL_Window* window = NULL; //inicializa una ventana
    SDL_Renderer* renderer = NULL; // inicializa renderizar
    SDL_Event e;
    SDL_Rect fillRect;

    //Inicializando SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL No se ha podido inicializar! SDL_Error: %s\n", SDL_GetError()); //mensaje y muestra el error
        exit(1);
    }

    //Crear window: primero nombre de la ventana, despues las dos posiciones, ancho, alto y visibilidad
    window = SDL_CreateWindow("Juego de la vida - Conway",
                              SDL_WINDOWPOS_UNDEFINED, //Posicion
                              SDL_WINDOWPOS_UNDEFINED, //Posicion
                              800, //ancho
                              600, //alto
                              SDL_WINDOW_SHOWN); //Bandera visible

    //valido que se pudo abrir la ventana
    if (!window)
    {
        SDL_Log("Error en la creacion de la ventana: %s\n", SDL_GetError()); //mensaje y muestra el error
        SDL_Quit(); // limpia y libera todos los recursos
        exit(1);
    }

    // Renderizamos sobre la ventana, Creamos el lienzo - le pasamos la ventana, -1, aceleracion por hw
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // -1 localiza automaticamente
    if (!renderer)
    {
        SDL_Log("No se ha podido crear el lienzo! SDL Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window); //Destruye ventana
        SDL_Quit(); //limpia y libera
        exit(1);
    }

    int ** tablero = crearTablero(dimTab[0],dimTab[1]);
    int ** proxGen = crearTablero(dimTab[0],dimTab[1]);

    //Crear evento que muestre pantalla
    while(!done)
    {
        //pregunta si hay eventos en espera
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                done = 1;
            }
        }
        //Pintar lienzo
        SDL_SetRenderDrawColor(renderer, 0x88, 0x24, 0x69, 0x1); //pinto la pantalla
        SDL_RenderClear(renderer); //limpio despues de pintar

        // cudrado
        cuadrado(renderer, fillRect, tablero, dimTab[0], dimTab[1]);

        calcularSiguienteGeneracion(tablero, proxGen, dimTab[0], dimTab[1]);
        actTablero(tablero, proxGen,  dimTab[0], dimTab[1]);

        //Presentar lienzo, si o si para ver si lo pinta
        SDL_RenderPresent(renderer);
        SDL_Delay(delay);
    }

    //destruyo todos los elementos creados
    //Observar ni mas ni menos que destructores, en la asignatura no inventamos nada!
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    free(dimTab);
}
