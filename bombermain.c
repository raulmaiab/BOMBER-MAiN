#include "raylib.h"
#include "inicio.h"
#include "menu.h"
#include <stdbool.h>

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;

// --- FUNÇÕES PLACEHOLDER MOVIDAS PARA O FIM ---
// (Declarações antecipadas, se necessário, mas como estão no mesmo
// ficheiro e antes de 'main', podemos apenas definir abaixo)
void ExecutarJogoBattle(void);
void ExecutarJogoStory(void);
void ExecutarShop(void);
void ExecutarOther(void);
// --------------------------------------------------


int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BomberMain");
    ToggleFullscreen();
    SetTargetFPS(60);

    bool deveContinuar = ExecutarTelaInicio();
    
    while (deveContinuar && !WindowShouldClose())
    {
        OpcaoMenu escolha = ExecutarTelaMenu();
        
        switch (escolha)
        {
            case ESCOLHA_BATTLE:
                ExecutarJogoBattle();
                break;
            
            case ESCOLHA_STORY:
                ExecutarJogoStory();
                break;
                
            case ESCOLHA_SHOP:
                ExecutarShop();
                break;

            case ESCOLHA_OTHER:
                ExecutarOther(); 
                break;

            case ESCOLHA_SAIR:
            case ESCOLHA_NENHUMA_OU_FECHOU:
                deveContinuar = false;
                break;
        }
    }

    CloseWindow(); 
    return 0;
}

// --- Implementações das funções (agora definidas apenas UMA VEZ) ---
void ExecutarJogoBattle(void) {
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        BeginDrawing(); ClearBackground(RED);
        DrawText("MODO BATTLE - Pressione ESC para voltar", 190, 200, 20, WHITE);
        EndDrawing();
    }
}

void ExecutarJogoStory(void)
{
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        BeginDrawing(); ClearBackground(GREEN);
        DrawText("MODO STORY - Pressione ESC para voltar", 190, 200, 20, WHITE);
        EndDrawing();
    }
}

void ExecutarShop(void) {
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        BeginDrawing(); ClearBackground(BROWN);
        DrawText("LOJA - Pressione ESC para voltar", 190, 200, 20, WHITE);
        EndDrawing();
    }
}
void ExecutarOther(void) {
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        BeginDrawing(); ClearBackground(DARKGRAY);
        DrawText("OUTROS/OPCOES - Pressione ESC para voltar", 190, 200, 20, WHITE);
        EndDrawing();
    }
}