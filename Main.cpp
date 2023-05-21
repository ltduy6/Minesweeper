#include <time.h>
#include "Game.h"

using namespace sf;

int main()
{
    // Init srand
    std::srand(static_cast<unsigned>(time(NULL)));

    // Game
    Game game; 
    while (game.running())
    {
        game.update(); 

        game.render();
    }
    return 0;
}
