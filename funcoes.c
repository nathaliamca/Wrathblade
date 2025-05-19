#include <stdbool.h>
#include "raylib.h"
#include "funcoes.h"

int MostrarMenu(void) {
    int menuOption = 0;  // 0 = Iniciar Jogo, 1 = Sair

    // Carrega a imagem do título
    Texture2D menuBackground = LoadTexture("assets/menubg.png");

    while (true) {
        if (WindowShouldClose()) return 1; // sair direto se fechar janela

        // Controle das opções de menu com as teclas UP e DOWN
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) menuOption = (menuOption + 1) % 2;  // Vai para a próxima opção
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) menuOption = (menuOption + 1) % 2;  // Vai para a opção anterior

        // Ação ao pressionar ENTER
        if (IsKeyPressed(KEY_ENTER)) {
            return menuOption; // 0 = Iniciar, 1 = Sair
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Obter a largura e altura da janela
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        // Centro da tela
        int centerX = screenWidth/2;
        int centerY = screenHeight/2;

        // Desenha a imagem redimensionada para cobrir toda a tela
        // Desenhando a imagem em um retângulo com a largura e altura da tela
        Rectangle destRect = {0, 0, (float)screenWidth, (float)screenHeight};
        DrawTexturePro(menuBackground, (Rectangle){0, 0, menuBackground.width, menuBackground.height}, destRect, (Vector2){0, 0}, 0.0f, WHITE);

        // Desenha a opção "Iniciar Jogo"
        if (menuOption == 0) {
            DrawText("> Iniciar Jogo", centerX - 100, centerY - 150, 40, WHITE);  // Destacar a opção selecionada
        } else {
            DrawText("  Iniciar Jogo", centerX - 100, centerY - 150, 40, WHITE);
        }

        // Desenha a opção "Sair"
        if (menuOption == 1) {
            DrawText("> Sair", centerX - 100, centerY - 110, 40, WHITE);  // Destacar a opção selecionada
        } else {
            DrawText("  Sair", centerX - 100, centerY - 110, 40, WHITE);
        }

        EndDrawing();
    }

    // Libera a textura do título ao sair
    UnloadTexture(menuBackground);
}

typedef struct {
    Vector2 position;
    float speed;
    float jumpForce;
    float velocityY;
    bool isJumping;
    bool facingRight; // Adicionado para controlar a direção do sprite
    bool idle;
} Player;

void Jogo(void) {
    // Carrega as texturas
    Texture2D knightIdle = LoadTexture("assets/playeridle.png");
    Texture2D groundTex = LoadTexture("assets/ground.png");
    Texture2D backgroundTex = LoadTexture("assets/jogobg.png");
    Texture2D knightWalk = LoadTexture("assets/playerwalk.png");
    Texture2D knightBackwalk = LoadTexture("assets/backwalk.png");

    Player player = {
        .position = {100, 600},
        .speed = 5.0,
        .jumpForce = 15.0,
        .velocityY = 0.0,
        .isJumping = false,
        .facingRight = true // Começa virado para a direita
    };

    const float scale = 4;
    const float gravity = 0.6;
    float groundY = GetScreenHeight() - groundTex.height * scale;
    float playerHeight = 32 * scale; // Altura do frame (32px * scale)

    // Configuração da animação
    const int frameWidth = 32;
    const int frameHeight = 32;
    Rectangle frameRec = { 0, 0, frameWidth, frameHeight };
    int currentFrame = 0;
    int framesCounter = 0;
    const int framesSpeed = 8;
    bool isMoving = false; // Para controlar quando animar

    Camera2D camera = { 0 };
    camera.target = (Vector2){ player.position.x + (frameWidth * scale)/2, 
                              player.position.y + (frameHeight * scale)/2 };
    camera.offset = (Vector2){ GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    while (!WindowShouldClose()) {
        // Reset movimento
        isMoving = false;

        // Movimento lateral
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            player.position.x -= player.speed;
            player.facingRight = false;
            isMoving = true;
            player.idle = false;
        }
        else{
            player.idle = true;
        }

        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            player.position.x += player.speed;
            player.facingRight = true;
            isMoving = true;
            player.idle = false;
        }
        else{
            player.idle = true;
        }

        // Pulo
        if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && !player.isJumping) {
            player.velocityY = -player.jumpForce;
            player.isJumping = true;
        }

        // Gravidade
        player.velocityY += gravity;
        player.position.y += player.velocityY;

        // Verifica colisão com o chão
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
                frameRec.x = (float)currentFrame * frameWidth;
            }
        } else {
            // Reset para frame parado quando não está se movendo
            currentFrame = 0;
            frameRec.x = 0;
        }

        // Atualiza câmera
        camera.target.x = player.position.x + (frameWidth * scale)/2;
        camera.target.y = player.position.y + (frameHeight * scale)/2;

        // Desenho
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            BeginMode2D(camera);
                // Desenha fundo
                for (int i = -5; i < 25; i++) {
                    Vector2 position = { i * backgroundTex.width * scale, -1.45*groundY };
                    DrawTextureEx(backgroundTex, position, 0.0f, scale, WHITE);
                }
                
                // Desenha chão
                for (int i = -5; i < 100; i++) {
                    Vector2 position = { i * groundTex.width * scale, groundY };
                    DrawTextureEx(groundTex, position, 0.0f, scale, WHITE);
                }
                
                // Desenha personagem (com flip horizontal se necessário)
                Rectangle destRec = {
                    player.position.x,
                    player.position.y,
                    frameWidth * scale,
                    frameHeight * scale
                };
                
                if (!isMoving && !player.isJumping){
                    DrawTexturePro(knightIdle, frameRec, destRec, 
                                 (Vector2){0, 0}, 0.0f, WHITE);
                }
                else {
                    if (player.facingRight == true){
                        DrawTexturePro(knightWalk, frameRec, destRec, 
                                    (Vector2){0, 0}, 0.0f, WHITE);
                        }
                    else if(player.facingRight == false){
                        DrawTexturePro(knightBackwalk, frameRec, destRec, 
                                    (Vector2){0, 0}, 0.0f, WHITE);
                    }
                }
            EndMode2D();
        EndDrawing();
    }

    // Libera texturas
    UnloadTexture(groundTex);
    UnloadTexture(backgroundTex);
    UnloadTexture(knightWalk);
    UnloadTexture(knightBackwalk);
}