#ifdef __MINGW32__
#define SDL_MAIN_HANDLED
#include <SDL_main.h>
#endif
#include "Conway.h"

int main(int argc, char *argv[])
{

    conway();

    return 0;
}
