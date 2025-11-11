#include "raylib.h"
#include "inicio.h"
#include "menu.h"
#include "mapa.h"       
#include "jogador.h"    
#include <stdbool.h>

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;

// Declarações das funções
void ExecutarJogoBattle(void);
void ExecutarJogoStory(void);
void ExecutarShop(void);
void ExecutarOther(void);


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

// --- Implementações das funções ---

// --- ATUALIZADO: ExecutarJogoBattle ---
void ExecutarJogoBattle(void) {
    
    InicializarMapa(); 
    
    Jogador j1 = CriarJogador(GetPlayerStartPosition(0), "SpriteBranco.png");
    Jogador j2 = CriarJogador(GetPlayerStartPosition(1), "SpriteVermelho.png");
    Jogador j3 = CriarJogador(GetPlayerStartPosition(2), "SpriteAzul.png");
    Jogador j4 = CriarJogador(GetPlayerStartPosition(3), "SpritePreto.png");

    // --- REMOVIDO: Configuração da Câmera ---
    // Não precisamos mais da câmera, pois o mapa (16x10 * 90)
    // preenche a tela (1440x900) perfeitamente.
    // --- FIM DA REMOÇÃO ---

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        
        AtualizarJogador(&j1, KEY_W, KEY_S, KEY_A, KEY_D); 
        AtualizarJogador(&j2, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT); 

        BeginDrawing(); 
            ClearBackground(BLACK);
            
            // --- REMOVIDO: BeginMode2D() ---
            
                DesenharMapa(); // Desenha o mapa em (0,0)
                
                DesenharJogador(&j1);
                DesenharJogador(&j2);
                DesenharJogador(&j3);
                DesenharJogador(&j4);

            // --- REMOVIDO: EndMode2D() ---

        EndDrawing();
    }
    
    DescarregarMapa(); 
    DestruirJogador(&j1);
    DestruirJogador(&j2);
    DestruirJogador(&j3);
    DestruirJogador(&j4);
}

// --- ATUALIZADO: ExecutarJogoStory ---
void ExecutarJogoStory(void)
{
    InicializarMapa(); 
    Jogador j1 = CriarJogador(GetPlayerStartPosition(0), "SpriteBranco.png");
    
    // --- REMOVIDO: Configuração da Câmera ---
    
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        
        AtualizarJogador(&j1, KEY_W, KEY_S, KEY_A, KEY_D);

        BeginDrawing(); 
            ClearBackground(BLACK);
            
            // --- REMOVIDO: BeginMode2D() ---
            
                DesenharMapa();
                DesenharJogador(&j1);
                
            // --- REMOVIDO: EndMode2D() ---
            
        EndDrawing();
    }
    
    DescarregarMapa(); 
    DestruirJogador(&j1);
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