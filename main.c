#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef __MINGW32__
    #define SDL_MAIN_HANDLED
    #include <SDL_main.h>
#endif
#include <SDL.h>
#include "Conway.h"

int main(int argc, char *argv[])
{
    //inicia variables
    unsigned char done;
    int k;
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
        return 1;
    }





    //Crear window: primero nombre de la ventana, despues las dos posiciones, ancho, alto y visibilidad
    window = SDL_CreateWindow("Juego de la vida - Conway",
                                                SDL_WINDOWPOS_UNDEFINED, //Posicion
                                                SDL_WINDOWPOS_UNDEFINED, //Posicion
                                                800, //ancho
                                                600, //alto
                                                SDL_WINDOW_SHOWN); //Bandera visible

    //valido que se pudo abrir la ventana
    if (!window) {
        SDL_Log("Error en la creacion de la ventana: %s\n", SDL_GetError()); //mensaje y muestra el error
        SDL_Quit(); // limpia y libera todos los recursos
        return -1;
    }

    // Renderizamos sobre la ventana, Creamos el lienzo - le pasamos la ventana, -1, aceleracion por hw
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // -1 localiza automaticamente
    if (!renderer) {
        SDL_Log("No se ha podido crear el lienzo! SDL Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window); //Destruye ventana
        SDL_Quit(); //limpia y libera
        return -1;
    }

    int f = 40;
    int c = 30;
    int ** tablero = crearTablero(f,c);
    int ** proxGen = crearTablero(f,c);

    //Crear evento que muestre pantalla
    while(!done){
        //pregunta si hay eventos en espera
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                done = 1;
            }
        }
        //Pintar lienzo
        SDL_SetRenderDrawColor(renderer, 0x88, 0x24, 0x69, 0x1); //pinto la pantalla
        SDL_RenderClear(renderer); //limpio despues de pintar

        int posX = 0;
        for(int i = 0; i < f; i ++){
            int posY = 0;
        for(int j = 0; j < c; j++){
            if(tablero[i][j] == 1) {
                fillRect.x = i*20; //posicion
                fillRect.y = j*20; // posicion
                fillRect.h = 20; //alto
                fillRect.w = 20; //ancho

                SDL_SetRenderDrawColor(renderer, 0x144, 0x12, 0x63, 0xFF); //rgba(144, 12, 63, 1)
                SDL_RenderFillRect(renderer, &fillRect);
            }
        }
    }
    calcularSiguienteGeneracion(tablero, proxGen, f, c);
    actTablero(tablero, proxGen, f, c);

    //Presentar lienzo, si o si para ver si lo pinta
    SDL_RenderPresent(renderer);
    SDL_Delay(delay);
    }

    //destruyo todos los elementos creados
    //Observar ni mas ni menos que destructores, en la asignatura no inventamos nada!
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
void imprimirTabDos(int ** tablero, int f, int c, SDL_Renderer * renderer ){
    SDL_Rect fillRect;
}

