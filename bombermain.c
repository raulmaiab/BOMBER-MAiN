#include "raylib.h"
#include "inicio.h"
#include "menu.h"
#include "menu_battle.h"
#include "menu_story.h" 
#include "options.h"    
#include "mapa.h"       
#include "jogador.h"    
#include "bomba.h"      
#include "explosao.h"   
#include "derrota.h"    
#include "vitoria.h"
#include "extras.h"     
#include <stdbool.h>
#include <stddef.h>   

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;

<<<<<<< HEAD
// Assinaturas
OptionsAction ExecutarJogoBattle(BattleSettings settings);
OptionsAction ExecutarJogoStory(StorySettings settings); 
=======
// Declarações 
void ExecutarJogoBattle(BattleSettings settings);
void ExecutarJogoStory(void);
>>>>>>> 9319279 (...)
void ExecutarShop(void);
void ExecutarOther(void);

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BomberMain");
    ToggleFullscreen();
    SetTargetFPS(60);
    
    InicializarExtras(); 

    bool deveContinuar = ExecutarTelaInicio();
    
    BattleSettings lastBattleSettings = {0};
    StorySettings lastStorySettings = {0};
    OpcaoMenu ultimoModo = ESCOLHA_NENHUMA_OU_FECHOU;
    
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
                        lastBattleSettings = settings;
                        ultimoModo = ESCOLHA_BATTLE;
                        
                        OptionsAction acao = ExecutarJogoBattle(settings);
                        
                        if (acao == OPTIONS_ACAO_EDIT_GAME) {
                            escolha = ESCOLHA_BATTLE;
                            continue;
                        }
                        if (acao == OPTIONS_ACAO_RESTART) {
                            ExecutarJogoBattle(lastBattleSettings);
                        }
                    }
                }
                break;
            
            case ESCOLHA_STORY: 
                {
                    StorySettings sSettings;
                    bool iniciar = ExecutarMenuStory(&sSettings);
                    if (iniciar)
                    {
                        lastStorySettings = sSettings;
                        ultimoModo = ESCOLHA_STORY;
                        
                        OptionsAction acao = ExecutarJogoStory(sSettings);
                        
                        if (acao == OPTIONS_ACAO_EDIT_GAME) {
                            escolha = ESCOLHA_STORY;
                            continue;
                        }
                        if (acao == OPTIONS_ACAO_RESTART) {
                            ExecutarJogoStory(lastStorySettings);
                        }
                    }
                }
                break;

            case ESCOLHA_SHOP: ExecutarShop(); break;
            case ESCOLHA_OTHER: ExecutarOther(); break;
            case ESCOLHA_SAIR:
            case ESCOLHA_NENHUMA_OU_FECHOU:
                deveContinuar = false;
                break;
        }
    }
    
    DescarregarExtras();
    CloseWindow(); 
    return 0;
}

// --- MODO BATTLE ---
OptionsAction ExecutarJogoBattle(BattleSettings settings) {
    
    SetExtrasHabilitados(settings.extras);

    if (settings.mapIndex == 1) {
        InicializarMapa("Cave");
    } 
    else if (settings.mapIndex == 2) {
        InicializarMapa("PirateBoat"); 
    } 
    else {
        InicializarMapa("Default");
    }
    
    ResetarExtras();
    
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
    
    OptionsAction acaoRetorno = OPTIONS_ACAO_MAIN_MENU;
    
    bool isPaused = false;

    while (!WindowShouldClose()) {
        
        // --- DETECÇÃO DE PAUSA AGORA COM 'P' ---
        if (IsKeyPressed(KEY_P)) { // <-- ALTERADO PARA KEY_P
            isPaused = true;
        }
        
        if (!isPaused) {
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
            
            // Verificar Coleta de Extras
            VerificarColetaExtras(&j1);
            VerificarColetaExtras(&j2);
            VerificarColetaExtras(&j3);
            VerificarColetaExtras(&j4);

<<<<<<< HEAD
            AtualizarBombas(&gBombas, deltaTime, &gExplosoes, todosJogadores, numJogadores);
            AtualizarExplosoes(&gExplosoes, deltaTime);
=======
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
>>>>>>> 9319279 (...)

            // Lógica de Vitória/Derrota
            if (settings.numPlayers == 1) {
                if (!j1.vivo) { ExecutarTelaDerrota(); acaoRetorno = OPTIONS_ACAO_MAIN_MENU; break; }
                if (j1.vivo && !j2.vivo && !j3.vivo && !j4.vivo) { ExecutarTelaVitoria(); acaoRetorno = OPTIONS_ACAO_MAIN_MENU; break; }
            } else { // 2 Players
                bool p1_vivo = j1.vivo;
                bool p2_vivo = j4.vivo; 
                if (!p1_vivo && !p2_vivo) { ExecutarTelaDerrota(); acaoRetorno = OPTIONS_ACAO_MAIN_MENU; break; }
                else if ( (p1_vivo && !p2_vivo) || (!p1_vivo && p2_vivo) ) { ExecutarTelaVitoria(); acaoRetorno = OPTIONS_ACAO_MAIN_MENU; break; }
            }
        } 
        
        // --- DESENHO ---
        BeginDrawing(); 
            ClearBackground(BLACK);
            DesenharMapa();
            
            DesenharExtras(); 
            
            DesenharJogador(&j1); DesenharJogador(&j2);
            DesenharJogador(&j3); DesenharJogador(&j4);
            DesenharBombas(&gBombas);
            DesenharExplosoes(&gExplosoes);

            // --- LÓGICA DO MENU OPTIONS ---
            if (isPaused) {
                OptionsAction acao = ExecutarMenuOptions();
                
                if (acao == OPTIONS_ACAO_RESUME) {
                    isPaused = false; 
                } else {
                    acaoRetorno = acao;
                    EndDrawing(); 
                    break; 
                }
            }
            // ------------------------------------

        EndDrawing();
    }
    
    DescarregarMapa(); 
    DestruirJogador(&j1); DestruirJogador(&j2);
    DestruirJogador(&j3); DestruirJogador(&j4);
    UnloadBombas(&gBombas);
    UnloadExplosoes(&gExplosoes);
    
    return acaoRetorno;
}

// --- MODO STORY ---
OptionsAction ExecutarJogoStory(StorySettings settings)
{
    SetExtrasHabilitados(settings.extras);
    InicializarMapa("Default"); 
    ResetarExtras();
    
    Jogador j1 = CriarJogador(GetPlayerStartPosition(0), "SpriteBranco", false); 

    Jogador j2;
    bool doisJogadores = (settings.numPlayers == 2);
    
    if (doisJogadores) {
        j2 = CriarJogador(GetPlayerStartPosition(3), "SpritePreto", false); 
    } else {
        j2.vivo = false; 
        j2.ehBot = false; 
    }
    
    Jogador* todosJogadores[2];
    todosJogadores[0] = &j1;
    todosJogadores[1] = &j2;
    int numJogadores = doisJogadores ? 2 : 1;

    NodeBombas gBombas = CriarNodeBombas();
    NodeExplosoes gExplosoes = CriarNodeExplosoes();
    
    OptionsAction acaoRetorno = OPTIONS_ACAO_MAIN_MENU;
    bool isPaused = false;

    while (!WindowShouldClose())
    {
        // --- DETECÇÃO DE PAUSA AGORA COM 'P' ---
        if (IsKeyPressed(KEY_P)) { // <-- ALTERADO PARA KEY_P
            isPaused = true;
        }

        if (!isPaused) {
            float deltaTime = GetFrameTime(); 
            
            // Update J1 (WASD)
            if (j1.vivo) {
                AtualizarJogador(&j1, KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, &gBombas, deltaTime, NULL, NULL);
                VerificarColetaExtras(&j1);
            }

            // Update J2 (Setas) - Se existir
            if (doisJogadores && j2.vivo) {
                AtualizarJogador(&j2, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_J, &gBombas, deltaTime, NULL, NULL);
                VerificarColetaExtras(&j2);
            }

            AtualizarBombas(&gBombas, deltaTime, &gExplosoes, todosJogadores, numJogadores);
            AtualizarExplosoes(&gExplosoes, deltaTime);
            
            // Condição de Derrota: Se todos os humanos morrerem
            bool p1Morto = !j1.vivo;
            bool p2Morto = (doisJogadores && !j2.vivo) || (!doisJogadores); 
            
            if (p1Morto && p2Morto) {
                ExecutarTelaDerrota();
                acaoRetorno = OPTIONS_ACAO_MAIN_MENU;
                break;
            }
        } 
        
        // --- DESENHO ---
        BeginDrawing(); 
            ClearBackground(BLACK);
            DesenharMapa();
            
            DesenharExtras();
            
            if (j1.vivo) DesenharJogador(&j1);
            if (doisJogadores && j2.vivo) DesenharJogador(&j2);
            
            DesenharBombas(&gBombas);
            DesenharExplosoes(&gExplosoes);
            
            // --- LÓGICA DO MENU OPTIONS ---
            if (isPaused) {
                OptionsAction acao = ExecutarMenuOptions();
                
                if (acao == OPTIONS_ACAO_RESUME) {
                    isPaused = false; 
                } else {
                    acaoRetorno = acao;
                    EndDrawing(); 
                    break; 
                }
            }
            // ------------------------------------
        EndDrawing();
    }
    
    DescarregarMapa(); 
    DestruirJogador(&j1);
    if (doisJogadores) DestruirJogador(&j2);
    UnloadBombas(&gBombas);
    UnloadExplosoes(&gExplosoes);
    
    return acaoRetorno;
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