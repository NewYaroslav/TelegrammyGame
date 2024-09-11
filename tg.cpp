#include <iostream>
#define GAME_DEBUG 0
#include "include/Game.hpp"

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

EXTERN EMSCRIPTEN_KEEPALIVE
void init_game() {
    Game::get_instance();
}

EXTERN EMSCRIPTEN_KEEPALIVE
void update_game() {
    Game::get_instance().run();
}

EXTERN EMSCRIPTEN_KEEPALIVE
void on_key_press(int key, int mode) {
    Game::get_instance().handle_input(key, mode);
}

EXTERN EMSCRIPTEN_KEEPALIVE
void cleanup_game() {

}

