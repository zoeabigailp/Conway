#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef __MINGW32__
#define SDL_MAIN_HANDLED
#include <SDL_main.h>
#endif
#include <SDL.h>
#include "Juego.h"


int main(int argc, char *argv[])
{


    ///Inicializar variables
    unsigned char done;
    int delay = 100;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer  = NULL;
    SDL_Event e;
    int config[2];

    ///Inicializando SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL NO SE PUDO INICIALIZAR, ERROR: %s", SDL_GetError() );
        return 1;
    }

    ///Configurar tablero inicial
    configurarTablero(config);


    ///Crear window = primero el nombre de la ventana, la posiciones, tamaño y visibilidad
    window = SDL_CreateWindow(
                 "Juego Conway",
                 SDL_WINDOWPOS_UNDEFINED,
                 SDL_WINDOWPOS_UNDEFINED,
                 800,
                 600,
                 SDL_WINDOW_SHOWN
             );
    //Validar que se creo la ventana
    if(window == NULL)
    {
        SDL_Log("No se puede crear la ventana %s", SDL_GetError());
        SDL_Quit(); //limpia y libera todo los recursos de SDL
        return -1;
    }

    ///Crear renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer)
    {
        SDL_Log("No se pudo crear el lienzo, %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    ///Inicializar tablero
    int **tablero = crearTablero(config[INDEX_F], config[INDEX_C]);
    /*
    crearPatron_archivo(argv[1]);
    inicializarTablero_Archivo(argv[1], tablero);
    */
    inicializarTablero(tablero, config[INDEX_F], config[INDEX_C]);



    ///Evento principal
    while(!done)
    {

        //Capturador de eventos
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                done = 1;
            }

        }

        //
        imprimirTablero(tablero, config[INDEX_F], config[INDEX_C], renderer);

        //Proceso tablero
        procesarTablero(tablero, config[INDEX_F], config[INDEX_C]);

        //DELAY
        SDL_Delay(delay);

    }

    /// Finalizo el programa:
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    destruirTablero(tablero, config[INDEX_F]);

    return 0;

}



