class Game {
  public:
    GameState state;
    GLboolean keys[1024];
    GLuint width, height;

    Game(GLuint width, GLuint height);
    ~Game();

    void Init();
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();
};
