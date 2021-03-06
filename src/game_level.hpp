#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "game_object.hpp"
#include "sprite_renderer.hpp"
#include "resource_manager.hpp"

class GameLevel
{
public:
  std::vector<GameObject> Bricks;

  GameLevel() { }

  void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);
  void Draw(SpriteRenderer &renderer);
  GLboolean IsCompleted();

private:
  void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

#endif
