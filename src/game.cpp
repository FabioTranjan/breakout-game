#include "game.hpp"
#include "resource_manager.hpp"
#include "sprite_renderer.hpp"

#include <iostream>

SpriteRenderer *Renderer;
GameObject *Player;

Game::Game(GLuint width, GLuint height) :
    State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
  delete Renderer;
  delete Player;
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
}

void Game::Update(GLfloat dt)
{

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
  }
}
