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
#include "extras.h"     // <-- NOVO: Inclui extras.h
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
    
    // --- NOVO: Inicializa o sistema de extras ---
    InicializarExtras(); 
    // --------------------------------------------

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
    
    // --- NOVO: Limpa o sistema de extras ---
    DescarregarExtras();
    // ---------------------------------------
    
    CloseWindow(); 
    return 0;
}


void ExecutarJogoBattle(BattleSettings settings) {
    
    // Seleção de Mapa
    if (settings.mapIndex == 1) {
        InicializarMapa("Cave");
    } 
    else if (settings.mapIndex == 2) {
        InicializarMapa("PirateBoat"); 
    } 
    else {
        InicializarMapa("Default");
    }
    
    // --- NOVO: Resetar extras ao iniciar uma partida ---
    // (Recomendado, embora InicializarExtras já limpe, garante que 
    // se começar nova partida, não tenha extras antigos no mapa)
    // Você pode criar uma função `ResetarExtras()` em extras.c se quiser,
    // ou apenas chamar InicializarExtras() de novo (se ela recarregar texturas, cuidado).
    // Por enquanto, vamos assumir que o mapa começa limpo.
    
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

        // Atualização dos Jogadores
        AtualizarJogador(&j1, KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, &gBombas, deltaTime, p1_target, p2_target); 
        AtualizarJogador(&j2, 0, 0, 0, 0, 0, &gBombas, deltaTime, p1_target, p2_target);
        AtualizarJogador(&j3, 0, 0, 0, 0, 0, &gBombas, deltaTime, p1_target, p2_target); 
        
        if (j4_ehHumano) { 
            AtualizarJogador(&j4, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_J, &gBombas, deltaTime, p1_target, p2_target);
        } else {
            AtualizarJogador(&j4, 0, 0, 0, 0, 0, &gBombas, deltaTime, p1_target, p2_target);
        }
        
        // --- NOVO: Verificar Coleta de Extras ---
        VerificarColetaExtras(&j1);
        VerificarColetaExtras(&j2);
        VerificarColetaExtras(&j3);
        VerificarColetaExtras(&j4);
        // ----------------------------------------

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
            
            // --- NOVO: Desenhar Extras ---
            DesenharExtras(); 
            // -----------------------------
            
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
    
    // Nota: Não descarregamos extras aqui, pois eles são carregados globalmente no main().
    // Se quiser limpar o array de extras ao sair da batalha, seria bom ter uma função `ResetarExtras()`.
}


void ExecutarJogoStory(void)
{
    InicializarMapa("Default"); 
    
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
        
        // --- NOVO: Verificar Coleta ---
        VerificarColetaExtras(&j1);
        // ------------------------------

        AtualizarBombas(&gBombas, deltaTime, &gExplosoes, todosJogadores, numJogadores);
        AtualizarExplosoes(&gExplosoes, deltaTime);
        if (!j1.vivo) {
            ExecutarTelaDerrota();
            break;
        }
        BeginDrawing(); 
            ClearBackground(BLACK);
            DesenharMapa();
            
            // --- NOVO: Desenhar Extras ---
            DesenharExtras(); 
            // -----------------------------
            
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