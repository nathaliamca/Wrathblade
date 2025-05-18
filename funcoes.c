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
} Player;

void Jogo(void) {

    Texture2D playerTex = LoadTexture("assets/player.png");
    Texture2D groundTex = LoadTexture("assets/ground.png");
    Texture2D backgroundTex = LoadTexture("assets/jogobg.png");

    Player player = {
        .position = {100, 600},
        .speed = 5.0,
        .jumpForce = 15.0,
        .velocityY = 0.0,
        .isJumping = false
    };

    const float scale = 4;
    const float gravity = 0.6;
    float groundY = GetScreenHeight() - groundTex.height * scale; // topo do chão real
    float playerheight = playerTex.height * scale;

    Camera2D camera = { 0 };
    camera.target = player.position;       // ponto que a câmera vai seguir
    camera.offset = (Vector2){ GetScreenWidth()/2.0f, GetScreenHeight()/2.0f }; // mantém o player no centro da tela
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;                    // pode aumentar para dar "zoom in"


    while (!WindowShouldClose()) {
        // Movimento lateral
        camera.target = (Vector2) { 
            player.position.x + (playerTex.width * scale)/2, 
            player.position.y + (playerTex.height * scale)/2 };

        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) player.position.x -= player.speed;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) player.position.x += player.speed;

        // Pulo (pressiona espaço e está no chão)
        if ((IsKeyPressed(KEY_SPACE) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && !player.isJumping) {
            player.velocityY = -player.jumpForce;
            player.isJumping = true;
        }

        // Aplica gravidade
        player.velocityY += gravity;
        player.position.y += player.velocityY;

        // Verifica se atingiu o chão
        if (player.position.y + playerheight >= groundY) {
            player.position.y = groundY - playerheight;
            player.velocityY = 0;
            player.isJumping = false;
        }

        // Desenho
       
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    BeginMode2D(camera);

    for (int i = -5; i < 25; i++) {
        Vector2 position = { i * backgroundTex.width * scale, -1.45*groundY };
        DrawTextureEx(backgroundTex, position, 0.0f, scale, WHITE);
    }
    for (int i = -5; i < 100; i++) {
        Vector2 position = { i * groundTex.width * scale, groundY };
        DrawTextureEx(groundTex, position, 0.0f, scale, WHITE);
    }
    DrawTextureEx(playerTex, player.position, 0.0f, scale, WHITE);  // Player 3x
    
    EndMode2D();

    EndDrawing();
    }

UnloadTexture(playerTex);
UnloadTexture(groundTex);
UnloadTexture(backgroundTex);

}