#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "game_level.hpp"
#include "ball_object.hpp"

const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const GLfloat BALL_RADIUS = 12.5f;

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
    void DoCollisions();

    std::vector<GameLevel> Levels;
    GLuint Level;

  private:
    GLboolean CheckCollision(GameObject &one, GameObject &two);
    GLboolean CheckCollision(BallObject &one, GameObject &two);
};

#endif
