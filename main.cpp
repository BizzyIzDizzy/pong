#include <SDL.h>
#include "utils/log.h"
#include "game.h"

int main(int argc, char* argv[]) {
    auto log = utils::get_default_logger();

    pong::game window{};

    if(!window.init())
    {
        log->error("Failed to initialize a window! Exiting!");
        return -1;
    }

    window.run();

    return 0;
}