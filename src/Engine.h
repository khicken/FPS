#ifndef ENGINE_H
#define ENGINE_H

#include "Main.h"

enum GameState {
    GAME_INGAME,
    GAME_TITLE,
    GAME_PAUSED,
    GAME_OPTIONS
};

class Engine {
public:
    GameState state;
    // window related variables
    unsigned int WINDOW_WIDTH, WINDOW_HEIGHT, MONITOR_WIDTH, MONITOR_HEIGHT;
    bool fullscreen;
    bool keys[1024];

    Engine(unsigned int windowwidth, unsigned int windowheight);
    ~Engine();
    void windowInit(GLFWwindow* window); // configs some window elements and such
    void init(); // loads textures, shaders, and other objects
    void update();
    void render();
    void pollEvents(float dt);
private:
};

#endif