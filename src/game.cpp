#include "game.hpp"
#include "resource_manager.hpp"
#include "sprite_renderer.hpp"

#include <iostream>

SpriteRenderer *Renderer;
GameObject *Player;
BallObject *Ball;

GLboolean CheckCollision(GameObject &one, GameObject &two);
Collision CheckCollision(BallObject &one, GameObject &two);
Direction VectorDirection(glm::vec2 target);

Game::Game(GLuint width, GLuint height) :
    State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{

}

void Game::Init()
{
  ResourceManager::LoadShader("sprite.vert", "sprite.frag", nullptr, "sprite");

  glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
    static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
  ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
  ResourceManager::GetShader("sprite").SetMatrix4f("projection", projection);

  Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

  ResourceManager::LoadTexture("background.jpg", GL_FALSE, "background");
  ResourceManager::LoadTexture("block.jpg", GL_FALSE, "block");
  ResourceManager::LoadTexture("block_solid.jpg", GL_FALSE, "block_solid");
  ResourceManager::LoadTexture("paddle.jpg", GL_TRUE, "paddle");
  ResourceManager::LoadTexture("awesomeface.jpg", GL_TRUE, "face");

  Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

  GameLevel one; one.Load("one.lvl", this->Width, this->Height * 0.5);
  GameLevel two; two.Load("two.lvl", this->Width, this->Height * 0.5);
  GameLevel three; three.Load("three.lvl", this->Width, this->Height * 0.5);
  GameLevel four; four.Load("four.lvl", this->Width, this->Height * 0.5);

  this->Levels.push_back(one);
  this->Levels.push_back(two);
  this->Levels.push_back(three);
  this->Levels.push_back(four);

  this->Level = 0;

  glm::vec2 playerPos = glm::vec2(
    this->Width / 2 - PLAYER_SIZE.x / 2,
    this->Height - PLAYER_SIZE.y
  );

  Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

  glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
  Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));
}

void Game::Update(GLfloat dt)
{
  Ball->Move(dt, this->Width);
  this->DoCollisions();
}

void Game::ProcessInput(GLfloat dt)
{
  if(this->State == GAME_ACTIVE)
  {
    GLfloat velocity = PLAYER_VELOCITY * dt;
    if(this->Keys[GLFW_KEY_A])
    {
      if(Player->Position.x >= 0)
        Player->Position.x -= velocity;
    }
    if(this->Keys[GLFW_KEY_D])
    {
      if(Player->Position.x <= this->Width - Player->Size.x)
        Player->Position.x += velocity;
    }
    if(this->Keys[GLFW_KEY_SPACE])
        Ball->Stuck = false;
  }
}

void Game::Render()
{
  if(this->State == GAME_ACTIVE)
  {
    Renderer->DrawSprite(ResourceManager::GetTexture("background"),
      glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
    this->Levels[this->Level].Draw(*Renderer);
    Player->Draw(*Renderer);
    Ball->Draw(*Renderer);
  }
}

void Game::DoCollisions()
{
  for(GameObject &box : this->Levels[this->Level].Bricks)
  {
    if(!box.Destroyed)
    {
      Collision collision = CheckCollision(*Ball, box);
      if(std::get<0>(collision))
      {
        if(!box.IsSolid)
          box.Destroyed = GL_TRUE;
        Direction dir = std::get<1>(collision);
        glm::vec2 diff_vector = std::get<2>(collision);
        if(dir == LEFT || dir == RIGHT)
        {
          Ball->Velocity.x = -Ball->Velocity.x;
          GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);
          if(dir == LEFT)
            Ball->Position.x += penetration;
          else
            Ball->Position.x -= penetration;
        } else
        {
          Ball->Velocity.y = -Ball->Velocity.y;
          GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
          if(dir == UP)
            Ball->Position.y -= penetration;
          else
            Ball->Position.y += penetration;
        }
      }
    }
  }
}

GLboolean CheckCollision(GameObject &one, GameObject &two)
{
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    return collisionX && collisionY;
}

Collision CheckCollision(BallObject &one, GameObject &two)
{
    glm::vec2 center(one.Position + one.Radius);
    glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
    glm::vec2 aabb_center(
        two.Position.x + aabb_half_extents.x,
        two.Position.y + aabb_half_extents.y
    );
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    glm::vec2 closest = aabb_center + clamped;
    difference = closest - center;

    if(glm::length(difference) <= one.Radius)
      return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
    else
      return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}

Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };
    GLfloat max = 0.0f;
    GLuint best_match = -1;
    for (GLuint i = 0; i < 4; i++)
    {
        GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}
