#include "raylib.h"
#include "inicio.h"
#include "menu.h"
#include "menu_battle.h" // <-- 1. INCLUÍDO
#include "mapa.h"       
#include "jogador.h"    
#include "bomba.h"      
#include "explosao.h"   
#include "derrota.h"    
#include "vitoria.h"    
#include <stdbool.h>

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;

// --- ATUALIZADO: Declaração antecipada ---
void ExecutarJogoBattle(BattleSettings settings);
// --- FIM DA ATUALIZAÇÃO ---

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
            // --- ATUALIZADO: Lógica do Menu Battle ---
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
            // --- FIM DA ATUALIZAÇÃO ---
            
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

// --- ATUALIZADO: ExecutarJogoBattle (aceita 'settings') ---
void ExecutarJogoBattle(BattleSettings settings) {
    
    InicializarMapa(); // (Futuramente, pode usar settings.mapIndex aqui)
    
    // --- Lógica de Criação de Jogadores baseada nas Configurações ---
    // (settings.extras ainda não está a ser usado)
    
    // Jogador 1 (Sempre Humano)
    Jogador j1 = CriarJogador(GetPlayerStartPosition(0), "SpriteBranco", false); 

    // Jogador 2 (Humano ou Bot)
    bool j2_ehBot = (settings.numPlayers == 1);
    Jogador j2 = CriarJogador(GetPlayerStartPosition(1), "SpriteVermelho", j2_ehBot);
    
    // Jogador 3 (Sempre Bot por agora)
    Jogador j3 = CriarJogador(GetPlayerStartPosition(2), "SpriteAzul", true);
    
    // Jogador 4 (Sempre Bot por agora)
    Jogador j4 = CriarJogador(GetPlayerStartPosition(3), "SpritePreto", true);
    // --- FIM DA LÓGICA DE CRIAÇÃO ---
    
    
    Jogador* todosJogadores[] = {&j1, &j2, &j3, &j4};
    int numJogadores = 4;

    NodeBombas gBombas = CriarNodeBombas();
    NodeExplosoes gExplosoes = CriarNodeExplosoes();

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        
        float deltaTime = GetFrameTime(); 

        // --- ATUALIZADO: Controles do Jogador 2 ---
        AtualizarJogador(&j1, KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, &gBombas, deltaTime); 
        
        if (j2_ehBot) {
            // J2 é Bot
            AtualizarJogador(&j2, 0, 0, 0, 0, 0, &gBombas, deltaTime);
        } else {
            // J2 é Humano
            AtualizarJogador(&j2, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_RIGHT_CONTROL, &gBombas, deltaTime);
        }
        
        AtualizarJogador(&j3, 0, 0, 0, 0, 0, &gBombas, deltaTime); // Bot
        AtualizarJogador(&j4, 0, 0, 0, 0, 0, &gBombas, deltaTime); // Bot
        // --- FIM DA ATUALIZAÇÃO ---

        AtualizarBombas(&gBombas, deltaTime, &gExplosoes, todosJogadores, numJogadores);
        AtualizarExplosoes(&gExplosoes, deltaTime);

        // --- Lógica de Vitória/Derrota ---
        // Se o J1 morreu, ou se o J1 e J2 (humanos) morreram
        bool humanosVivos = j1.vivo;
        if (!j2_ehBot) humanosVivos = humanosVivos || j2.vivo; // Se J2 é humano, verifica se ele está vivo

        if (!humanosVivos) {
            ExecutarTelaDerrota(); 
            break; 
        }
        
        // Se J1 (e J2, se humano) estão vivos, mas todos os bots morreram
        bool botsVivos = j3.vivo || j4.vivo;
        if (j2_ehBot) botsVivos = botsVivos || j2.vivo; // Se J2 é bot, inclui-o na verificação

        if (humanosVivos && !botsVivos) {
            ExecutarTelaVitoria();
            break;
        }
        // --- FIM DA LÓGICA ---

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
    // (Sem alteração)
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