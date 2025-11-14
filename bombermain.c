#include "raylib.h"
#include "inicio.h"
#include "menu.h"
#include "menu_battle.h" 
#include "mapa.h"       
#include "jogador.h"    
#include "bomba.h"      
#include "explosao.h"   
#include "derrota.h"    
#include "vitoria.h"    
#include <stdbool.h>

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;

// Declarações (Sem alteração)
void ExecutarJogoBattle(BattleSettings settings);
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
                { 
                    BattleSettings settings; 
                    bool iniciar = ExecutarMenuBattle(&settings); 
                    if (iniciar)
                    {
                        ExecutarJogoBattle(settings); 
                    }
                }
                break;
            case ESCOLHA_STORY: ExecutarJogoStory(); break;
            case ESCOLHA_SHOP: ExecutarShop(); break;
            case ESCOLHA_OTHER: ExecutarOther(); break;
            case ESCOLHA_SAIR:
            case ESCOLHA_NENHUMA_OU_FECHOU:
                deveContinuar = false;
                break;
        }
    }
    CloseWindow(); 
    return 0;
}


void ExecutarJogoBattle(BattleSettings settings) {
    
    // --- CORREÇÃO AQUI (Linha 62) ---
    InicializarMapa(); 
    // --- FIM DA CORREÇÃO ---
    
    bool j1_ehHumano = true;
    bool j4_ehHumano = (settings.numPlayers == 2); 
    
    Jogador j1 = CriarJogador(GetPlayerStartPosition(0), "SpriteBranco", !j1_ehHumano); 
    Jogador j2 = CriarJogador(GetPlayerStartPosition(1), "SpriteVermelho", true);      
    Jogador j3 = CriarJogador(GetPlayerStartPosition(2), "SpriteAzul", true);      
    Jogador j4 = CriarJogador(GetPlayerStartPosition(3), "SpritePreto", !j4_ehHumano); 
    
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
        if (j4_ehHumano) { 
            AtualizarJogador(&j4, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_J, &gBombas, deltaTime);
        } else {
            AtualizarJogador(&j4, 0, 0, 0, 0, 0, &gBombas, deltaTime);
        }

        AtualizarBombas(&gBombas, deltaTime, &gExplosoes, todosJogadores, numJogadores);
        AtualizarExplosoes(&gExplosoes, deltaTime);

        bool humanosVivos = j1.vivo;
        if (j4_ehHumano) humanosVivos = humanosVivos || j4.vivo;
        if (!humanosVivos) {
            ExecutarTelaDerrota(); 
            break; 
        }
        bool botsVivos = j2.vivo || j3.vivo;
        if (!j4_ehHumano) botsVivos = botsVivos || j4.vivo;
        if (humanosVivos && !botsVivos) {
            ExecutarTelaVitoria();
            break;
        }

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
    // --- CORREÇÃO AQUI (Linha 156) ---
    InicializarMapa(); 
    // --- FIM DA CORREÇÃO ---
    
    Jogador j1 = CriarJogador(GetPlayerStartPosition(0), "SpriteBranco", false); 
    Jogador* todosJogadores[] = {&j1};
    int numJogadores = 1;
    NodeBombas gBombas = CriarNodeBombas();
    NodeExplosoes gExplosoes = CriarNodeExplosoes();
    
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        float deltaTime = GetFrameTime(); 
        
        // (Esta linha já estava correta da última vez, sem KEYD)
        AtualizarJogador(&j1, KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, &gBombas, deltaTime);

        AtualizarBombas(&gBombas, deltaTime, &gExplosoes, todosJogadores, numJogadores);
        AtualizarExplosoes(&gExplosoes, deltaTime);
        if (!j1.vivo) {
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