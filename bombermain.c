#include "raylib.h"
#include "inicio.h"
#include "menu.h"
#include "mapa.h"       
#include "jogador.h"    
#include "bomba.h"      
#include "explosao.h"   
#include <stdbool.h>

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;

// Declarações (sem alteração)
void ExecutarJogoBattle(void);
void ExecutarJogoStory(void);
void ExecutarShop(void);
void ExecutarOther(void);


int main(void)
{
    // (Sem alteração no main)
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


// --- ATUALIZADO: ExecutarJogoBattle ---
void ExecutarJogoBattle(void) {
    
    InicializarMapa(); 
    
    Jogador j1 = CriarJogador(GetPlayerStartPosition(0), "SpriteBranco.png");
    Jogador j2 = CriarJogador(GetPlayerStartPosition(1), "SpriteVermelho.png");
    Jogador j3 = CriarJogador(GetPlayerStartPosition(2), "SpriteAzul.png");
    Jogador j4 = CriarJogador(GetPlayerStartPosition(3), "SpritePreto.png");

    NodeBombas gBombas = CriarNodeBombas();
    NodeExplosoes gExplosoes = CriarNodeExplosoes();

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        
        // --- ATUALIZADO: Chamadas de AtualizarJogador ---
        // Passa a tecla de bomba (KEY_SPACE) e o gestor (&gBombas)
        AtualizarJogador(&j1, KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, &gBombas); 
        AtualizarJogador(&j2, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_RIGHT_CONTROL, &gBombas); // J2 usa Right-Ctrl
        // --- FIM DA ATUALIZAÇÃO ---

        float deltaTime = GetFrameTime();
        AtualizarBombas(&gBombas, deltaTime, &gExplosoes);
        AtualizarExplosoes(&gExplosoes, deltaTime);

        BeginDrawing(); 
            ClearBackground(BLACK);
            
            DesenharMapa();
                
            DesenharJogador(&j1);
            DesenharJogador(&j2);
            DesenharJogador(&j3);
            DesenharJogador(&j4);

            DesenharBombas(&gBombas);
            DesenharExplosoes(&gExplosoes);

        EndDrawing();
    }
    
    DescarregarMapa(); 
    DestruirJogador(&j1);
    DestruirJogador(&j2);
    DestruirJogador(&j3);
    DestruirJogador(&j4);
    UnloadBombas(&gBombas);
    UnloadExplosoes(&gExplosoes);
}

// --- ATUALIZADO: ExecutarJogoStory ---
void ExecutarJogoStory(void)
{
    InicializarMapa(); 
    Jogador j1 = CriarJogador(GetPlayerStartPosition(0), "SpriteBranco.png");
    
    NodeBombas gBombas = CriarNodeBombas();
    NodeExplosoes gExplosoes = CriarNodeExplosoes();
    
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        
        // --- ATUALIZADO: Chamada de AtualizarJogador ---
        AtualizarJogador(&j1, KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, &gBombas);
        // --- FIM DA ATUALIZAÇÃO ---

        float deltaTime = GetFrameTime();
        AtualizarBombas(&gBombas, deltaTime, &gExplosoes);
        AtualizarExplosoes(&gExplosoes, deltaTime);

        BeginDrawing(); 
            ClearBackground(BLACK);
            
            DesenharMapa();
            DesenharJogador(&j1);
                
            DesenharBombas(&gBombas);
            DesenharExplosoes(&gExplosoes);
            
        EndDrawing();
    }
    
    DescarregarMapa(); 
    DestruirJogador(&j1);
    UnloadBombas(&gBombas);
    UnloadExplosoes(&gExplosoes);
}

// --- Funções Placeholder (sem alteração) ---
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