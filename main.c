#include "raylib.h"
#include "funcoes.h"

int main(void) {
    InitWindow(1280, 960, "WRATHBLADE");
    SetTargetFPS(60);

    int escolha = MostrarMenu();  // chama o menu

    if (escolha == 0) {
        Jogo(); // começa o jogo se escolher "Iniciar"
    }

    /*if (escolha == 1) {
        Recordes();
    }*/

    CloseWindow();
    return 0;
}
