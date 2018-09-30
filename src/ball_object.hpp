#ifndef BALL_OBJECT
#define BALL_OBJECT

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.hpp"
#include "game_object.hpp"
#include "sprite_renderer.hpp"

class BallObject : public GameObject
{
  public:
    GLfloat Radius;
    GLboolean Stuck;

    BallObject();
    BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);

    glm::vec2 Move(GLfloat dt, GLuint window_width);
    void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif
