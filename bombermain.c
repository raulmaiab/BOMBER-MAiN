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
    while (deveContinuar && !WindowShouldClose()) {
        OpcaoMenu escolha = ExecutarTelaMenu();
        switch (escolha) {
            case ESCOLHA_BATTLE: ExecutarJogoBattle(); break;
            case ESCOLHA_STORY:  ExecutarJogoStory();  break;
            case ESCOLHA_SHOP:   ExecutarShop();    break;
            case ESCOLHA_OTHER:  ExecutarOther();   break;
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
    
    Jogador j1 = CriarJogador(GetPlayerStartPosition(0), "SpriteBranco.png", false); // Humano
    Jogador j2 = CriarJogador(GetPlayerStartPosition(1), "SpriteVermelho.png", true);  // Bot
    Jogador j3 = CriarJogador(GetPlayerStartPosition(2), "SpriteAzul.png", true);      // Bot
    Jogador j4 = CriarJogador(GetPlayerStartPosition(3), "SpritePreto.png", true);       // Bot
    
    // --- NOVO: Array de Jogadores ---
    Jogador* todosJogadores[] = {&j1, &j2, &j3, &j4};
    int numJogadores = 4;
    // --- FIM NOVO ---

    NodeBombas gBombas = CriarNodeBombas();
    NodeExplosoes gExplosoes = CriarNodeExplosoes();

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        
        float deltaTime = GetFrameTime(); 

        // Atualiza Jogadores (sem alteração)
        AtualizarJogador(&j1, KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, &gBombas, deltaTime); 
        AtualizarJogador(&j2, 0, 0, 0, 0, 0, &gBombas, deltaTime);
        AtualizarJogador(&j3, 0, 0, 0, 0, 0, &gBombas, deltaTime);
        AtualizarJogador(&j4, 0, 0, 0, 0, 0, &gBombas, deltaTime);

        // --- ATUALIZADO: Passa a lista de jogadores ---
        AtualizarBombas(&gBombas, deltaTime, &gExplosoes, todosJogadores, numJogadores);
        // --- FIM DA ATUALIZAÇÃO ---
        
        AtualizarExplosoes(&gExplosoes, deltaTime);

        BeginDrawing(); 
            ClearBackground(BLACK);
            DesenharMapa();
            
            // Desenha os jogadores (já têm a lógica !j->vivo)
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
    Jogador j1 = CriarJogador(GetPlayerStartPosition(0), "SpriteBranco.png", false); // Humano
    
    // --- NOVO: Array de Jogadores ---
    Jogador* todosJogadores[] = {&j1};
    int numJogadores = 1;
    // --- FIM NOVO ---
    
    NodeBombas gBombas = CriarNodeBombas();
    NodeExplosoes gExplosoes = CriarNodeExplosoes();
    
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        
        float deltaTime = GetFrameTime(); 
        AtualizarJogador(&j1, KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, &gBombas, deltaTime);

        // --- ATUALIZADO: Passa a lista de jogadores ---
        AtualizarBombas(&gBombas, deltaTime, &gExplosoes, todosJogadores, numJogadores);
        // --- FIM DA ATUALIZAÇÃO ---
        
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