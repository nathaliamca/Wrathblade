#ifndef FUNCOES_H
#define FUNCOES_H

typedef struct lista_recorde {
    char nome[50];
    float time;
    struct lista_recorde *next;
} Recorde;

typedef struct {
    Vector2 position;
    Texture2D texture;
    Rectangle frameRec;

    int currentFrame;
    int framesCounter;
    int framesSpeed;

    float speed;
    float jumpForce;
    float velocityY;
    bool isJumping;
    bool movingRight;
    int vida;
} Player;

typedef struct {
    Vector2 position;
    float speed;
    Texture2D texture;
    Rectangle frameRec;
    int currentFrame;
    int framesCounter;
    int framesSpeed;
    bool movingRight;
    int vida;
    bool alive;
} Slime;

typedef struct {
    Vector2 position;
    Texture2D texture;
    Rectangle frameRec;
    Rectangle hitbox;
   
    int currentFrame;
    int framesCounter;
    int framesSpeed;
    
    bool ativo;
    
} Portal;

typedef struct Projetil {
    Vector2 position;
    Vector2 velocity;
    Rectangle hitbox;
    struct Projetil *next;
    Texture2D texture;
} Projetil;


int MostrarMenu();  // declara a função para ser usada em outros arquivos
int InputName();
void Jogo();
void MostrarRecordes();
void SalvarRecorde(const char *nome, float tempoTotal);
void AdicionarProjetil(Projetil **lista, Vector2 pos, Vector2 vel, Texture2D texture);
void RemoverProjetil(Projetil **lista, Projetil *proj);
void BossMap(Player* player, float tempoJogo);

// void Recordes();

#endif