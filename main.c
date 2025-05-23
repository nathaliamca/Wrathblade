#include "raylib.h"
#include "funcoes.h"

int main(void) {
    InitWindow(1280, 960, "WRATHBLADE");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        int escolha = MostrarMenu();  // mostra o menu e retorna a escolha

        if (escolha == 0) {
            InputName();
            Jogo(); // inicia o jogo
        }
        else if (escolha == 1) {
            MostrarRecordes(); // mostra os recordes
        }
        else if (escolha == 2) {
            break; // sair do loop
        }
    }

    CloseWindow();
    return 0;
}
