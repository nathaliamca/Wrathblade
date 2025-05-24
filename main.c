#include "raylib.h"
#include "funcoes.h"

int main(void) {
    InitWindow(1280, 960, "WRATHBLADE");
    SetTargetFPS(60);

    SetExitKey(0);
    
    while (!WindowShouldClose()) {
        int escolha = MostrarMenu();  // mostra o menu e retorna a escolha

        if (escolha == 0) {
            int resultado = InputName();
            if (resultado == 0) {
                Jogo();  // prossegue
            } else {
                // voltou ao menu, reexibe o menu
                escolha = MostrarMenu();
            }
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
