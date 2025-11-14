#include "raylib.h"
#include "inicio.h"
#include "menu.h"
#include "mapa.h"       
#include "jogador.h"    
#include "bomba.h"      
#include "explosao.h"   
#include "derrota.h"    
#include "vitoria.h"    // <-- 1. INCLUÍDO

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


void ExecutarJogoBattle(void) {
    
    InicializarMapa(); 
    
    Jogador j1 = CriarJogador(GetPlayerStartPosition(0), "SpriteBranco", false); // Humano
    Jogador j2 = CriarJogador(GetPlayerStartPosition(1), "SpriteVermelho", true);  // Bot
    Jogador j3 = CriarJogador(GetPlayerStartPosition(2), "SpriteAzul", true);      // Bot
    Jogador j4 = CriarJogador(GetPlayerStartPosition(3), "SpritePreto", true);       // Bot
    
    Jogador* todosJogadores[] = {&j1, &j2, &j3, &j4};
    int numJogadores = 4;

    NodeBombas gBombas = CriarNodeBombas();
    NodeExplosoes gExplosoes = CriarNodeExplosoes();

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        
        float deltaTime = GetFrameTime(); 

        AtualizarJogador(&j1, KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, &gBombas, deltaTime); 
        AtualizarJogador(&j2, 0, 0, 0, 0, 0, &gBombas, deltaTime);
        AtualizarJogador(&j3, 0, 0, 0, 0, 0, &gBombas, deltaTime);
        AtualizarJogador(&j4, 0, 0, 0, 0, 0, &gBombas, deltaTime);

        AtualizarBombas(&gBombas, deltaTime, &gExplosoes, todosJogadores, numJogadores);
        AtualizarExplosoes(&gExplosoes, deltaTime);

        // --- VERIFICAÇÃO DE DERROTA ---
        if (!j1.vivo) 
        {
            ExecutarTelaDerrota(); 
            break; 
        }
        
        // --- 2. VERIFICAÇÃO DE VITÓRIA ---
        // Se o J1 (Humano) está vivo E todos os bots estão mortos
        if (j1.vivo && !j2.vivo && !j3.vivo && !j4.vivo)
        {
            ExecutarTelaVitoria(); // Mostra a tela de Vitória
            break; // Sai do loop da batalha e volta ao menu
        }
        // --- FIM DA VERIFICAÇÃO ---

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


void ExecutarJogoStory(void)
{
    // (Sem alteração aqui, pois a condição de vitória
    // era específica para o modo Battle com os 3 bots)
    
    InicializarMapa(); 
    Jogador j1 = CriarJogador(GetPlayerStartPosition(0), "SpriteBranco", false); 
    Jogador* todosJogadores[] = {&j1};
    int numJogadores = 1;
    NodeBombas gBombas = CriarNodeBombas();
    NodeExplosoes gExplosoes = CriarNodeExplosoes();
    
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        
        float deltaTime = GetFrameTime(); 
        AtualizarJogador(&j1, KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, &gBombas, deltaTime);
        AtualizarBombas(&gBombas, deltaTime, &gExplosoes, todosJogadores, numJogadores);
        AtualizarExplosoes(&gExplosoes, deltaTime);

        if (!j1.vivo)
        {
            ExecutarTelaDerrota();
            break;
        }

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
void ExecutarShop(void) { /* ... */ }
void ExecutarOther(void) { /* ... */ }