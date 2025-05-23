#include "raylib.h"
#include "funcoes.h"

int main(void) {
    InitWindow(1280, 960, "WRATHBLADE");
    SetTargetFPS(60);

    int escolha = MostrarMenu();  // chama o menu

    if (escolha == 0) {
        InputName();
        Jogo(); // começa o jogo se escolher "Iniciar"
    }

   else if (escolha == 1) {
    MostrarRecordes();
}

    CloseWindow();
    return 0;
}
