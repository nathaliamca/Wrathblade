// menu.h
#ifndef MENU_H
#define MENU_H

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
void InputName();
void Jogo();
void BossMap(Player* player, float tempoJogo);
void adicionarRecorde(Recorde lista[], int *quantidade, const char *nome, float tempo);
void ordenarListaPorTempo(Recorde lista[], int quantidade);
void salvarRankingEmArquivo(Recorde lista[], int quantidade);

#endif
