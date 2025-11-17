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
#include <stddef.h>   

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;

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
    
    // --- ATUALIZADO: Passa o nome da pasta ---
    // Futuramente, você pode usar:
    // if (settings.mapIndex == 0) InicializarMapa("Default");
    // else if (settings.mapIndex == 1) InicializarMapa("Gelo");
    InicializarMapa("Default"); 
    // --- FIM DA ATUALIZAÇÃO ---
    
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
    
    Jogador* p1_target = &j1;
    Jogador* p2_target = (j4_ehHumano) ? &j4 : NULL; 

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        
        float deltaTime = GetFrameTime(); 

        AtualizarJogador(&j1, KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, &gBombas, deltaTime, p1_target, p2_target); 
        AtualizarJogador(&j2, 0, 0, 0, 0, 0, &gBombas, deltaTime, p1_target, p2_target);
        AtualizarJogador(&j3, 0, 0, 0, 0, 0, &gBombas, deltaTime, p1_target, p2_target); 
        
        if (j4_ehHumano) { 
            AtualizarJogador(&j4, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_J, &gBombas, deltaTime, p1_target, p2_target);
        } else {
            AtualizarJogador(&j4, 0, 0, 0, 0, 0, &gBombas, deltaTime, p1_target, p2_target);
        }

        AtualizarBombas(&gBombas, deltaTime, &gExplosoes, todosJogadores, numJogadores);
        AtualizarExplosoes(&gExplosoes, deltaTime);

        // Lógica de Vitória/Derrota
        if (settings.numPlayers == 1) {
            if (!j1.vivo) { ExecutarTelaDerrota(); break; }
            if (j1.vivo && !j2.vivo && !j3.vivo && !j4.vivo) { ExecutarTelaVitoria(); break; }
        } else { // 2 Players
            bool p1_vivo = j1.vivo;
            bool p2_vivo = j4.vivo; 
            if (!p1_vivo && !p2_vivo) { ExecutarTelaDerrota(); break; }
            else if ( (p1_vivo && !p2_vivo) || (!p1_vivo && p2_vivo) ) { ExecutarTelaVitoria(); break; }
        }

        BeginDrawing(); 
            ClearBackground(BLACK);
            DesenharMapa();
            DesenharJogador(&j1); DesenharJogador(&j2);
            DesenharJogador(&j3); DesenharJogador(&j4);
            DesenharBombas(&gBombas);
            DesenharExplosoes(&gExplosoes);
        EndDrawing();
    }
    
    DescarregarMapa(); 
    DestruirJogador(&j1); DestruirJogador(&j2);
    DestruirJogador(&j3); DestruirJogador(&j4);
    UnloadBombas(&gBombas);
    UnloadExplosoes(&gExplosoes);
}


void ExecutarJogoStory(void)
{
    // --- ATUALIZADO: Passa o nome da pasta ---
    InicializarMapa("Default"); 
    // --- FIM DA ATUALIZAÇÃO ---
    
    Jogador j1 = CriarJogador(GetPlayerStartPosition(0), "SpriteBranco", false); 
    Jogador* todosJogadores[] = {&j1};
    int numJogadores = 1;
    NodeBombas gBombas = CriarNodeBombas();
    NodeExplosoes gExplosoes = CriarNodeExplosoes();
    
    Jogador* p1_target = &j1;
    Jogador* p2_target = NULL; 
    
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        float deltaTime = GetFrameTime(); 
        
        AtualizarJogador(&j1, KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, &gBombas, deltaTime, p1_target, p2_target);

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