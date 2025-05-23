#include <stdbool.h>
#include "raylib.h"
#include "funcoes.h"
#include <math.h>
#include <stdio.h>   // NOVO: Para arquivos
#include <time.h>    // NOVO: Para medir tempo (opcional)
#include <stdlib.h>

char nome[50] = {0};
void SalvarRecorde(const char *nome, float tempoTotal);


typedef struct Projetil {
    Vector2 position;
    Vector2 velocity;
    Rectangle hitbox;
    struct Projetil *next;
    Texture2D texture;
} Projetil;

void AdicionarProjetil(Projetil **lista, Vector2 pos, Vector2 vel, Texture2D texture);
void RemoverProjetil(Projetil **lista, Projetil *proj);

int MostrarMenu() {
    int menuOption = 0;  // 0 = Iniciar Jogo, 1 = Sair

    // Carrega a imagem do título
    Texture2D menuBackground = LoadTexture("assets/cenario/menubg.png");

    while (true) {
        if (WindowShouldClose()) return 1; // sair direto se fechar janela

        // Controle das opções de menu com as teclas UP e DOWN
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) menuOption = (menuOption + 1) % 3;  // Vai para a próxima opção
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) menuOption = (menuOption + 3 - 1) % 3;  // Vai para a opção anterior

        // Ação ao pressionar ENTER
        if (IsKeyPressed(KEY_ENTER)) {

            return menuOption; // 0 = Iniciar, 1 = Recordes, 2 = Sair
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Obter a largura e altura da janela
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        // Centro da tela
        int centerX = screenWidth/2;
        int centerY = screenHeight/2;

        // Desenhando a imagem em um retângulo com a largura e altura da tela
        Rectangle destRect = {0, 0, (float)screenWidth, (float)screenHeight};
        DrawTexturePro(menuBackground, (Rectangle){0, 0, menuBackground.width, menuBackground.height}, destRect, (Vector2){0, 0}, 0.0f, WHITE);

        // Desenha a opção "Iniciar Jogo" 
        if (menuOption == 0) {
            DrawText("> Iniciar Jogo", centerX - 150, centerY - 150, 40, WHITE);  // Destacar a opção selecionada
        } else {
            DrawText("  Iniciar Jogo", centerX - 150, centerY - 150, 40, WHITE);
        }

        if (menuOption == 1) {
            DrawText("> Recordes", centerX - 150, centerY - 110, 40, WHITE);  // Destacar a opção selecionada
        } else {
            DrawText("  Recordes", centerX - 150, centerY - 110, 40, WHITE);
        }
        // Desenha a opção "Sair"
        if (menuOption == 2) {
            DrawText("> Sair", centerX - 150, centerY - 70, 40, WHITE);  // Destacar a opção selecionada
        } else {
            DrawText("  Sair", centerX - 150, centerY - 70, 40, WHITE);
        }

        EndDrawing();
    }

    // Libera a textura do título ao sair
    UnloadTexture(menuBackground);
}

void InputName() {
    Texture2D menuBackground = LoadTexture("assets/cenario/menubg.png");
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    int length = 0;

    while (!WindowShouldClose()) {
        // Captura o texto digitado
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (length < 49)) {
                nome[length] = (char)key;
                length++;
                nome[length] = '\0';
            }
            key = GetCharPressed();
        }

        // Apagar caractere
        if (IsKeyPressed(KEY_BACKSPACE) && length > 0) {
            length--;
            nome[length] = '\0';
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        Rectangle destRect = {0, 0, (float)screenWidth, (float)screenHeight};
        DrawTexturePro(menuBackground, (Rectangle){0, 0, menuBackground.width, menuBackground.height}, destRect, (Vector2){0, 0}, 0.0f, WHITE);

        DrawText("Digite seu nome:", 300, 300, 30, WHITE);
        DrawText(nome, 300, 340, 40, WHITE);
        DrawText("Pressione ENTER para continuar", 300, 400, 30, WHITE);
        EndDrawing();

        // Confirmar nome com ENTER
        if (IsKeyPressed(KEY_ENTER) && length > 0) {
            break;  // sai do loop e retorna para main
        }
    }

    // Aqui você pode salvar o nome em uma variável global ou passar para o jogo
}

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

void BossMap(Player* player);
void Jogo() {
    // Carrega as texturas
    Texture2D portalTex = LoadTexture("assets/cenario/portal.png");

    // slime texture
    Texture2D pinkSlime = LoadTexture("assets/slime/pinkslime.png");
    Texture2D backpinkSlime = LoadTexture("assets/slime/backpinkslime.png");

    // player texture
    Texture2D knightIdle = LoadTexture("assets/player/idle.png");
    
    Texture2D knightWalk = LoadTexture("assets/player/walk.png");
    Texture2D knightBackwalk = LoadTexture("assets/player/backwalk.png");
    
    Texture2D knightAttack = LoadTexture("assets/player/atack.png");
    Texture2D knightBackAttack = LoadTexture("assets/player/backatack.png");
    
    // cenario texture
    Texture2D groundTex = LoadTexture("assets/cenario/ground.png");
    Texture2D backgroundTex = LoadTexture("assets/cenario/jogobg.png");
    
    // heart texture
    Texture2D coracaoCheio = LoadTexture("assets/heart/full.png");
    Texture2D coracaoMeio  = LoadTexture("assets/heart/half.png");
    Texture2D coracaoVazio = LoadTexture("assets/heart/empty.png");
    
    // gameover texture
    Texture2D gameOverTexture = LoadTexture("assets/cenario/gameover.png");

    // fonte do hp
    Font fonte = LoadFont("assets/fonte.ttf");

    float tempoJogo = 0.0f;  // NOVO: Tempo acumulado desde o início da partida

    Portal portal = {
        .position = (Vector2){3700, 290},  // posição próxima ao fim do mapa
        .texture = portalTex,
        .frameRec = (Rectangle){0, 0, 32, 32},  // ajuste ao tamanho real da textura
        .hitbox = (Rectangle){3750, 315, 32, 32},

        .currentFrame = 0,
        .framesCounter = 0,
        .framesSpeed = 8,
        
        .ativo = false
    };

    Player player = {
        .position = {600, 600},
        .speed = 5.0,
        .jumpForce = 15.0,
        .velocityY = 0.0,
        .isJumping = false,
        .movingRight = true, // Começa virado para a direita
        .vida = 10,
        .frameRec = (Rectangle){0, 0, 32, 32}
    };

    int quantidade_slimes = 4;

    Slime slimes[quantidade_slimes];

    for (int i = 0; i < quantidade_slimes; i++) {
        slimes[i].position = (Vector2){1600 + i * 700, 315};
        slimes[i].speed = 1.5;
        slimes[i].frameRec = (Rectangle){0, 0, 32, 32};
        slimes[i].currentFrame = 0;
        slimes[i].framesCounter = 0;
        slimes[i].framesSpeed = 7;
        slimes[i].movingRight = true;
        slimes[i].vida = 5;
        slimes[i].alive = true;
    }

    bool slimesmortas = false;
    bool isGameOver = false; // gameover check
    float slimeDanoCooldown = 0;
    float playerDanoCooldown = 0;  // Tempo de espera entre danos
    const float tempoEntreDanoPlayer = 1;  // em segundos
    const float tempoEntreDanoSlime = 0.5;  // em segundos
    const int vidaMaxima = 10; // vida do player
    bool isAttacking = false; // verifica se esta atacando
    int attackFrame = 0; // frame inicial da animação de atacar
    int attackCounter = 0; // contador do frame
    const int attackFramesSpeed = 12; // velocidade da animação
    Rectangle attackRec = { 0, 0, 32, 32 };  // tamanho de cada frame

    const float scale = 4; // escala do jogo
    const float gravity = 0.6; // gravidade
    float groundY = GetScreenHeight() - groundTex.height * scale; // altura do chao
    float playerHeight = 32 * scale; // Altura do frame (32px * scale)

    // Configuração da animação
    int currentFrame = 0; // primeiro frame da animação do player
    int framesCounter = 0; // contador do frame
    const int framesSpeed = 8; // velocidade da animação do player
    bool isMoving = false; // Para controlar quando animar

    // tamanho do mapa
    const float mapStart = 300; // inicio do mapa
    const float mapEnd = 9000; // fim do mapa

    Camera2D camera = { 0 };
    camera.target = (Vector2){ player.position.x + (32 * scale)/2, 
                              player.position.y + (32 * scale)/2 };
    camera.offset = (Vector2){ GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
    SetTargetFPS(60); 
    while (!WindowShouldClose()) {
         tempoJogo += GetFrameTime();  // NOVO: Soma o tempo de cada frame
        // Reset movimento
        isMoving = false;
        playerDanoCooldown -= GetFrameTime();           // diminui o tempo de cooldown a cada frame
        if (playerDanoCooldown < 0) playerDanoCooldown = 0;
        slimeDanoCooldown -= GetFrameTime();
        if (slimeDanoCooldown < 0) slimeDanoCooldown = 0;                                        
        
// player

        // movimento player pra esquerda
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            player.position.x -= player.speed;
            player.movingRight = false;
            isMoving = true;
        }
        // movimento player pra direita
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            player.position.x += player.speed;
            player.movingRight = true;
            isMoving = true;
        }

        // limitar movimento do player de acordo com tamanho do mapa
        if (player.position.x < mapStart) player.position.x = mapStart;
        if (player.position.x > mapEnd)  player.position.x = mapEnd;

        // animação de ataque
        if (IsKeyPressed(KEY_SPACE) && !isAttacking) {
            isAttacking = true;
            attackFrame = 0;
            attackCounter = 0;
            attackRec.x = 0;
        }

        // pulo
        if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && !player.isJumping) {
            player.velocityY = -player.jumpForce;
            player.isJumping = true;
        }

        // gravidade
        player.velocityY += gravity;
        player.position.y += player.velocityY;

        // colisão com o chão
        if (player.position.y + playerHeight >= groundY) {
            player.position.y = groundY - playerHeight;
            player.velocityY = 0;
            player.isJumping = false;
        }

        // Atualiza animação apenas se estiver se movendo
        if (isMoving && !player.isJumping) {
            framesCounter++;
            if (framesCounter >= (60 / framesSpeed)) {
                framesCounter = 0;
                currentFrame++;
                if (currentFrame > 3) currentFrame = 0;
                player.frameRec.x = currentFrame * 32;
            }

        } 

        // Reset para frame parado quando não está se movendo
        else if (!player.isJumping){
           currentFrame = 0;
           player.frameRec.x = 0;
        }
        
        // contador da animação de ataque
        if (isAttacking) {
        attackCounter++;
            if (attackCounter >= (60 / attackFramesSpeed)) {
                attackCounter = 0;
                attackFrame++;
                if (attackFrame > 3) {
                    attackFrame = 0;
                    isAttacking = false;
                }
            attackRec.x = attackFrame * 32;
            }
        }
        
        for (int i = 0; i < quantidade_slimes; i++) {
            if (!slimes[i].alive) continue;

            // Animação
            slimes[i].framesCounter++;
            if (slimes[i].framesCounter >= (60 / slimes[i].framesSpeed)) {
                slimes[i].framesCounter = 0;
                slimes[i].currentFrame++;
                if (slimes[i].currentFrame > 7) slimes[i].currentFrame = 0;
                slimes[i].frameRec.x = slimes[i].currentFrame * slimes[i].frameRec.width;
            }

            // Movimento em direção ao player
            if (slimes[i].alive && fabs(slimes[i].position.x - player.position.x) <= 800) {
                if (slimes[i].position.x < player.position.x) {
                    slimes[i].position.x += slimes[i].speed;
                    slimes[i].movingRight = true;
                } else {
                    slimes[i].position.x -= slimes[i].speed;
                    slimes[i].movingRight = false;
                }
            }
            // Hitboxes
            Rectangle slimeRect = {
                slimes[i].position.x,
                slimes[i].position.y,
                slimes[i].frameRec.width * 2,
                slimes[i].frameRec.height * 2
            };

            Rectangle playerRect = {
                player.position.x,
                player.position.y,
                32 * 3,
                32 * 2
            };

            Rectangle playerAtackRect = {
                player.position.x,
                player.position.y,
                32 * 5,
                32 * 3
            };

            if (CheckCollisionRecs(slimeRect, playerRect) && playerDanoCooldown <= 0) {
                player.vida -= 1;
                if (player.vida < 0) player.vida = 0;
                playerDanoCooldown = tempoEntreDanoPlayer;
            }

            if (CheckCollisionRecs(slimeRect, playerAtackRect) && isAttacking && slimeDanoCooldown <= 0) {
                slimes[i].vida -= 1;
                slimeDanoCooldown = tempoEntreDanoSlime;
            }

            if (slimes[i].vida <= 0) {
                slimes[i].alive = false;
            }
        }

        for (int i = 0; i < quantidade_slimes; i++) {
            if (slimes[i].alive) {
                slimesmortas = false;
                break;
            }
            else slimesmortas = true;
        }

        if (slimesmortas) {
            portal.ativo = true;
        }

        // se a vida do Player chegar a 0 gameover
        if (player.vida <= 0) {
            isGameOver = true;
        }

        if (portal.ativo) {
            Rectangle playerRect = {
                player.position.x,
                player.position.y,
                32 * 3,
                32 * 3
            };

            portal.framesCounter++;
            if (portal.framesCounter >= (60 / portal.framesSpeed)) {
                portal.framesCounter = 0;
                portal.currentFrame++;
                if (portal.currentFrame > 5) portal.currentFrame = 0;
            }
            portal.frameRec.x = portal.currentFrame * 32;

            if (CheckCollisionRecs(playerRect, portal.hitbox)) {
                BossMap(&player);
            }
        }

        // Atualiza câmera
        camera.target.x = player.position.x + (32 * scale)/2;
        camera.target.y = player.position.y + (32 * scale)/2 - 100;

        if (isGameOver) {
            BeginDrawing();
            ClearBackground(BLACK);

            // Centraliza a imagem de game over na tela
            DrawTexture(gameOverTexture, 
            (GetScreenWidth() - gameOverTexture.width) / 2, 
            (GetScreenHeight() - gameOverTexture.height) / 2, 
            WHITE
            );

            DrawText("Pressione ENTER para reiniciar", 
                GetScreenWidth()/2 - 200, 
                GetScreenHeight() - 100, 
                30, 
                WHITE
            );

            EndDrawing();

            if (IsKeyPressed(KEY_ENTER)) {
                SalvarRecorde(nome, tempoJogo);  // NOVO: Salva nome e tempo no arquivo
                tempoJogo = 0.0f;                // Reseta tempo para próxima partida

                // Reinicia o jogo
                player.vida = vidaMaxima;

                player.position = (Vector2){600, 600};

                for (int i = 0; i < quantidade_slimes; i++) {
                    slimes[i].position = (Vector2){1400 + i * 1200, 315}; // Pode variar a posição para não ficarem sobrepostos
                    slimes[i].vida = 5; // Se tiver vida máxima específica para slimes
                    slimes[i].alive = true;
                    slimes[i].movingRight = false;
                    // Reinicie outros campos necessários
                }

                isGameOver = false;
            }

            continue; // Pula o resto do loop e volta
        }

        // Desenho
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            BeginMode2D(camera);
            // Desenha fundo
            for (int i = -5; i < 25; i++) {
                Vector2 position = { i * backgroundTex.width * scale, -1.45*groundY };
                DrawTextureEx(backgroundTex, position, 0, scale, WHITE);
            }
            
            // Desenha chão
            for (int i = -5; i < 150; i++) {
                Vector2 position = { i * groundTex.width * scale, groundY };
                DrawTextureEx(groundTex, position, 0, scale, WHITE);
            }

            for (int i = 0; i < quantidade_slimes; i++) {
                if (!slimes[i].alive) continue;

                Rectangle slimeDestRec = {
                    slimes[i].position.x,
                    slimes[i].position.y,
                    slimes[i].frameRec.width * scale,
                    slimes[i].frameRec.height * scale
                };

                DrawTextEx(fonte, TextFormat("HP: %d", slimes[i].vida),
                    (Vector2){slimes[i].position.x + 15, slimes[i].position.y - 20},
                    30, 2, RED);

                if (!slimes[i].movingRight) {
                    DrawTexturePro(pinkSlime, slimes[i].frameRec, slimeDestRec, (Vector2){0, 0}, 0, WHITE);
                } else {
                    DrawTexturePro(backpinkSlime, slimes[i].frameRec, slimeDestRec, (Vector2){0, 0}, 0, WHITE);
                }
            }

            Rectangle destRec = {
                player.position.x,
                player.position.y,
                32 * scale,
                32 * scale
            };
            
            
            if (isAttacking && player.movingRight) {
            DrawTexturePro(knightAttack, attackRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
            }

            else if (isAttacking && !player.movingRight) {
                DrawTexturePro(knightBackAttack, attackRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
            }
            else if (!isMoving && !player.isJumping){
                DrawTexturePro(knightIdle, player.frameRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
            }
            else {
                if (player.movingRight){
                    DrawTexturePro(knightWalk, player.frameRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
                } else {
                    DrawTexturePro(knightBackwalk, player.frameRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
                }
            }

            if (portal.ativo) {
                Rectangle destRec = {
                    portal.position.x,
                    portal.position.y,
                    portal.frameRec.width * scale,
                    portal.frameRec.height * scale
                };
                DrawTexturePro(portal.texture, portal.frameRec, destRec, (Vector2){0, 0}, 0, WHITE);
            }

            EndMode2D();
               // HUD de vida
                int spacing = coracaoCheio.width * scale - 55; // espaçamento entre corações

                                                                       
                for (int i = 0; i < 5; i++) {
                    float x = 20 + i * spacing;
                    float y = 20;

                    if (player.vida >= (i + 1) * 2) {
                        DrawTextureEx(coracaoCheio, (Vector2){x, y}, 0, scale, WHITE);
                    } else if (player.vida == (i * 2) + 1) {
                        DrawTextureEx(coracaoMeio, (Vector2){x, y}, 0, scale, WHITE);
                    } else {
                        DrawTextureEx(coracaoVazio, (Vector2){x, y}, 0, scale, WHITE);
                    }
                }

        EndDrawing();

    }
    // Libera texturas
    UnloadTexture(groundTex);
    UnloadTexture(backgroundTex);
    UnloadTexture(knightWalk);
    UnloadTexture(knightBackwalk);
    UnloadTexture(coracaoCheio);
    UnloadTexture(coracaoMeio);
    UnloadTexture(coracaoVazio);
    UnloadTexture(pinkSlime);
    UnloadTexture(knightAttack);
    UnloadTexture(knightBackAttack);
    UnloadTexture(gameOverTexture);
    UnloadTexture(portalTex);
}

typedef struct {
    Vector2 position;
    Texture2D texture;
    Rectangle frameRec;
    int currentFrame;
    int framesCounter;
    int framesSpeed;

    int vida;
    bool alive;
} Boss;



void BossMap(Player* player) {

    // boss
    Texture2D bossTexture = LoadTexture("assets/boss/boss.png");
    Texture2D texturaProjetil = LoadTexture("assets/boss/shoot.png");

    // cenario 
    Texture2D backgroundTex = LoadTexture("assets/cenario/bossbg.png");
    Texture2D groundTex = LoadTexture("assets/cenario/groundboss.png");

    // player texture
    Texture2D knightIdle = LoadTexture("assets/player/idle.png");

    Texture2D knightWalk = LoadTexture("assets/player/walk.png");
    Texture2D knightBackwalk = LoadTexture("assets/player/backwalk.png");
    
    Texture2D knightAttack = LoadTexture("assets/player/atack.png");
    Texture2D knightBackAttack = LoadTexture("assets/player/backatack.png");

    // heart texture
    Texture2D coracaoCheio = LoadTexture("assets/heart/full.png");
    Texture2D coracaoMeio  = LoadTexture("assets/heart/half.png");
    Texture2D coracaoVazio = LoadTexture("assets/heart/empty.png");
    
    // gameover texture
    Texture2D gameOverTexture = LoadTexture("assets/cenario/gameover.png");

    // fonte do hp
    Font fonte = LoadFont("assets/fonte.ttf");

    Projetil *listaProjetil = NULL;
    float tempoDisparo = 0.0f;
    const float intervaloDisparo = 2.0f;


    bool isGameOver = false; // gameover check
    float playerDanoCooldown = 0;  // Tempo de espera entre danos
    const float tempoEntreDanoPlayer = 1;  // em segundos
    const int vidaMaxima = 10; // vida do player
    bool isAttacking = false; // verifica se esta atacando
    int attackFrame = 0; // frame inicial da animação de atacar
    int attackCounter = 0; // contador do frame
    const int attackFramesSpeed = 12; // velocidade da animação
    Rectangle attackRec = { 0, 0, 32, 32 };  // tamanho de cada frame
    
    float bossDanoCooldown = 0;
    const float tempoEntreDanoBoss = 0.5;  // meio segundo, por exemplo

    const float scale = 4; // escala do jogo
    const float gravity = 0.6; // gravidade
    float groundY = GetScreenHeight() - groundTex.height * scale - 10; // altura do chao
    float playerHeight = 32 * scale; // Altura do frame (32px * scale)

    player->position = (Vector2){600, groundY - playerHeight};
    player->velocityY = 0;
    player->isJumping = false;

    // Configuração da animação
    int currentFrame = 0; // primeiro frame da animação do player
    int framesCounter = 0; // contador do frame
    const int framesSpeed = 8; // velocidade da animação do player
    bool isMoving = false; // Para controlar quando animar

    // tamanho do mapa
    const float mapStart = - 500; // inicio do mapa
    const float mapEnd = 1050; // fim do mapa
    
    Boss boss = {
        .position = (Vector2){1000, groundY - 64 * scale},  // ajuste a posição
        .texture = bossTexture,
        .frameRec = (Rectangle){0, 0, 64, 64},  // ajuste pro tamanho correto do frame
        .currentFrame = 0,
        .framesCounter = 0,
        .framesSpeed = 8,
        .vida = 30,
        .alive = true
    };
    
    Camera2D camera = { 0 };
    camera.target = (Vector2){ player->position.x + (32 * scale)/2, 
                              player->position.y + (32 * scale)/2 };
    camera.offset = (Vector2){ GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
    SetTargetFPS(60); 
    while (true) {
    if (WindowShouldClose()) {
        CloseWindow();
        }
        // Reset movimento
        isMoving = false;
        playerDanoCooldown -= GetFrameTime();           // diminui o tempo de cooldown a cada frame
        if (playerDanoCooldown < 0) playerDanoCooldown = 0;
        
// player

        // movimento player pra esquerda
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            player->position.x -= player->speed;
            player->movingRight = false;
            isMoving = true;
        }
        // movimento player pra direita
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            player->position.x += player->speed;
            player->movingRight = true;
            isMoving = true;
        }

        // limitar movimento do player de acordo com tamanho do mapa
        if (player->position.x < mapStart) player->position.x = mapStart;
        if (player->position.x > mapEnd)  player->position.x = mapEnd;

        // animação de ataque
        if (IsKeyPressed(KEY_SPACE) && !isAttacking) {
            isAttacking = true;
            attackFrame = 0;
            attackCounter = 0;
            attackRec.x = 0;
        }

        // pulo
        if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && !player->isJumping) {
            player->velocityY = -player->jumpForce;
            player->isJumping = true;
        }

        // gravidade
        player->velocityY += gravity;
        player->position.y += player->velocityY;

        // colisão com o chão
        if (player->position.y + playerHeight >= groundY) {
            player->position.y = groundY - playerHeight;
            player->velocityY = 0;
            player->isJumping = false;
        }

        // Atualiza animação apenas se estiver se movendo
        if (isMoving && !player->isJumping) {
            framesCounter++;
            if (framesCounter >= (60 / framesSpeed)) {
                framesCounter = 0;
                currentFrame++;
                if (currentFrame > 3) currentFrame = 0;
                player->frameRec.x = currentFrame * 32;
            }

        } 

        // Reset para frame parado quando não está se movendo
        else if (!player->isJumping){
           currentFrame = 0;
           player->frameRec.x = 0;
        }
        
        // contador da animação de ataque
        if (isAttacking) {
        attackCounter++;
            if (attackCounter >= (60 / attackFramesSpeed)) {
                attackCounter = 0;
                attackFrame++;
                if (attackFrame > 3) {
                    attackFrame = 0;
                    isAttacking = false;
                }
            attackRec.x = attackFrame * 32;
            }
        }

        if (boss.alive) {
            
            boss.framesCounter++;
            bossDanoCooldown -= GetFrameTime();

            if (bossDanoCooldown < 0) bossDanoCooldown = 0;

            if (boss.framesCounter >= (60 / boss.framesSpeed)) {
                boss.framesCounter = 0;
                boss.currentFrame++;
                if (boss.currentFrame > 5) boss.currentFrame = 0;
                boss.frameRec.x = boss.currentFrame * boss.frameRec.width;
            }

            tempoDisparo += GetFrameTime();
                if (tempoDisparo >= intervaloDisparo) {
                    Vector2 origem = { boss.position.x, boss.position.y + 128 };
                    Vector2 velocidade = { -5, 0 };
                    AdicionarProjetil(&listaProjetil, origem, velocidade, texturaProjetil);
                    tempoDisparo = 0.0f;
                }
        }
        
        Rectangle playerRect = {
            player->position.x,
            player->position.y,
            32 * 3,
            32 * 2
        };

        Rectangle playerAtackRect = {
            player->position.x,
            player->position.y,
            32 * 6,
            32 * 3
        };
        
        Rectangle bossRect = {
            boss.position.x,
            boss.position.y,
            64 * 1,
            64 * 4
        };

        // colisao dano do boss
        if (CheckCollisionRecs(playerRect, bossRect) && playerDanoCooldown <= 0) {
            player->vida -= 2;
            if (player->vida < 0) player->vida = 0;
            playerDanoCooldown = tempoEntreDanoPlayer;
        }

        // colisao dano do player
        if (CheckCollisionRecs(playerAtackRect, bossRect) && isAttacking && bossDanoCooldown <= 0) {
            boss.vida -= 1;
            bossDanoCooldown = tempoEntreDanoBoss;

            if (boss.vida <= 0) boss.alive = false;
        }

        Projetil *atual = listaProjetil;            
        while (atual != NULL) {
            atual->position.x += atual->velocity.x;
            atual->hitbox.x = atual->position.x;
            atual->hitbox.y = atual->position.y;

            if (atual->position.x < 0 || atual->position.x > GetScreenWidth()) {
                Projetil *remover = atual;
                atual = atual->next;
                RemoverProjetil(&listaProjetil, remover);
                continue;
            }

            if (CheckCollisionRecs(atual->hitbox, playerRect) && playerDanoCooldown <= 0) {
                player->vida -= 2;
                if (player->vida < 0) player->vida = 0;
                playerDanoCooldown = tempoEntreDanoPlayer;

                Projetil *remover = atual;
                atual = atual->next;
                RemoverProjetil(&listaProjetil, remover);
                continue;
            }

            atual = atual->next;
        }

        // se a vida do Player chegar a 0 gameover
        if (player->vida <= 0) {
            isGameOver = true;
        }

        // Atualiza câmera
        camera.target.x = player->position.x + (32 * scale)/2;
        camera.target.y = player->position.y + (32 * scale)/2 - 150;

        if (isGameOver) {
            BeginDrawing();
            ClearBackground(BLACK);

            // Centraliza a imagem de game over na tela
            DrawTexture(gameOverTexture, 
            (GetScreenWidth() - gameOverTexture.width) / 2, 
            (GetScreenHeight() - gameOverTexture.height) / 2, 
            WHITE
            );

            DrawText("Pressione ENTER para reiniciar", 
                GetScreenWidth()/2 - 200, 
                GetScreenHeight() - 100, 
                30, 
                WHITE
            );

            EndDrawing();

            if (IsKeyPressed(KEY_ENTER)) {
                Jogo();

                isGameOver = false;
            }

            continue; // Pula o resto do loop e volta
        }


        // Desenho
        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode2D(camera);
            // Desenha fundo
            for (int i = -5; i < 25; i++) {
                Vector2 position = { i * backgroundTex.width * scale, -1.45*groundY };
                DrawTextureEx(backgroundTex, position, 0, scale, WHITE);
            }
            
            // Desenha chão
            for (int i = -25; i < 150; i++) {
                Vector2 position = { i * groundTex.width * scale, groundY };
                DrawTextureEx(groundTex, position, 0, scale, WHITE);
            }


            Rectangle destRec = {
                player->position.x,
                player->position.y,
                32 * scale,
                32 * scale
            };
            
            
            if (isAttacking && player->movingRight) {
            DrawTexturePro(knightAttack, attackRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
            }

            else if (isAttacking && !player->movingRight) {
                DrawTexturePro(knightBackAttack, attackRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
            }
            else if (!isMoving && !player->isJumping){
                DrawTexturePro(knightIdle, player->frameRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
            }
            else {
                if (player->movingRight){
                    DrawTexturePro(knightWalk, player->frameRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
                } else {
                    DrawTexturePro(knightBackwalk, player->frameRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
                }
            }

            if (boss.alive) {
                Rectangle destRec = {
                    boss.position.x - 50, boss.position.y - 170,
                    boss.frameRec.width * 7.5,
                    boss.frameRec.height * 7.5
                };
                DrawTexturePro(boss.texture, boss.frameRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
            
                DrawTextEx(fonte, TextFormat("BOSS HP: %d", boss.vida),(Vector2){player->position.x, player->position.y -500}, 50, 2, RED);
            }   

            for (Projetil *p = listaProjetil; p != NULL; p = p->next) {
                DrawTextureEx(p->texture, p->position, 0, 1, WHITE);
            }


            EndMode2D();
               // HUD de vida
                int spacing = coracaoCheio.width * scale - 55; // espaçamento entre corações

                                                                       
                for (int i = 0; i < 5; i++) {
                    float x = 20 + i * spacing;
                    float y = 20;

                    if (player->vida >= (i + 1) * 2) {
                        DrawTextureEx(coracaoCheio, (Vector2){x, y}, 0, scale, WHITE);
                    } else if (player->vida == (i * 2) + 1) {
                        DrawTextureEx(coracaoMeio, (Vector2){x, y}, 0, scale, WHITE);
                    } else {
                        DrawTextureEx(coracaoVazio, (Vector2){x, y}, 0, scale, WHITE);
                    }
                }

        EndDrawing();
    }
}

void SalvarRecorde(const char *nome, float tempoTotal) {
    FILE *arquivo = fopen("recordes.txt", "a");
    if (arquivo != NULL) {
        fprintf(arquivo, "%s %.2f\n", nome, tempoTotal);
        fclose(arquivo);
    } else {
        printf("Erro ao abrir o arquivo de recordes.\n");
    }
}

void AdicionarProjetil(Projetil **lista, Vector2 pos, Vector2 vel, Texture2D texture) {
    Projetil *novo = malloc(sizeof(Projetil));
    if (novo == NULL) return;

    novo->position = pos;
    novo->velocity = vel;
    novo->hitbox = (Rectangle){ pos.x, pos.y, 16, 16 };
    novo->texture = texture;
    novo->next = *lista;
    *lista = novo;
}


void RemoverProjetil(Projetil **lista, Projetil *proj) {
    if (*lista == NULL || proj == NULL) return;

    if (*lista == proj) {
        *lista = proj->next;
        free(proj);
        return;
    }

    Projetil *anterior = *lista;
    while (anterior->next && anterior->next != proj)
        anterior = anterior->next;

    if (anterior->next == proj) {
        anterior->next = proj->next;
        free(proj);
    }
}
