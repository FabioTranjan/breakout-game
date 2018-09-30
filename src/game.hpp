#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "game_level.hpp"

enum GameState {
  GAME_ACTIVE,
  GAME_MENU,
  GAME_WIN
};

class Game {
  public:
    GameState State;
    GLboolean Keys[1024];
    GLuint Width, Height;

    Game(GLuint width, GLuint height);
    ~Game();

    void Init();
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();

    std::vector<GameLevel> Levels;
    GLuint Level;
};

#endif
