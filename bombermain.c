#include "raylib.h"
#include "inicio.h"
#include "menu.h"
#include "menu_battle.h"
#include "menu_story.h" 
#include "menu_prox.h"  
#include "options.h"    
#include "mapa.h"       
#include "jogador.h"    
#include "bomba.h"      
#include "explosao.h"   
#include "derrota.h"    
#include "vitoria.h"
#include "vitoria_battle.h" 
#include "extras.h"     
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>    

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;

// Assinaturas
OptionsAction ExecutarJogoBattle(BattleSettings settings);
OptionsAction ExecutarJogoStory(StorySettings settings); 
OptionsAction ExecutarNivelStory(StorySettings settings, const char* nomeMapa, int nivel, bool* nivelVitoria); 
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
                        
                        OptionsAction acao;
                        
                        // Loop para RESTART no MODO BATTLE
                        do {
                            acao = ExecutarJogoBattle(lastBattleSettings); 
                            
                            if (acao == OPTIONS_ACAO_EDIT_GAME) {
                                escolha = ESCOLHA_BATTLE; 
                                break; 
                            }
                            
                        } while (acao == OPTIONS_ACAO_RESTART);
                        
                        if (acao == OPTIONS_ACAO_EDIT_GAME) {
                            continue;
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
                        
                        OptionsAction acao;
                        
                        // Loop para RESTART no MODO STORY
                        do {
                            acao = ExecutarJogoStory(lastStorySettings); 
                            
                            if (acao == OPTIONS_ACAO_EDIT_GAME) {
                                escolha = ESCOLHA_STORY; 
                                break; 
                            }
                            
                        } while (acao == OPTIONS_ACAO_RESTART);
                        
                        if (acao == OPTIONS_ACAO_EDIT_GAME) {
                            continue;
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

// -----------------------------------------------------------------------------------
// --- MODO BATTLE (Atualizado: Usa spriteName e permite PvP após bots) ---
// -----------------------------------------------------------------------------------
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
    
    // Jogadores
    // Usando spriteName (e não spritePath)
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
        
        if (IsKeyPressed(KEY_P)) { 
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

            AtualizarBombas(&gBombas, deltaTime, &gExplosoes, todosJogadores, numJogadores);
            AtualizarExplosoes(&gExplosoes, deltaTime);

            // Lógica de Vitória/Derrota
            if (settings.numPlayers == 1) {
                // Modo 1P: J1 (Branco) vs. Bots (Vermelho, Azul, Preto)
                if (!j1.vivo) { 
                    ExecutarTelaDerrota(); acaoRetorno = OPTIONS_ACAO_MAIN_MENU; break; 
                }
                if (j1.vivo && !j2.vivo && !j3.vivo && !j4.vivo) { 
                    // J1 VENCEU
                    ExecutarTelaVitoriaBattle(j1.spriteName); // <<< Usando spriteName
                    acaoRetorno = OPTIONS_ACAO_MAIN_MENU; break; 
                }
            } else { // 2 Players
                // Modo 2P: J1 (Branco) e J4 (Preto) vs. Bots (Vermelho, Azul)
                bool p1_vivo = j1.vivo;
                bool p4_vivo = j4.vivo; 
                
                bool bots_mortos = !j2.vivo && !j3.vivo;

                if (!p1_vivo && !p4_vivo) { 
                    // Ambos humanos perderam
                    ExecutarTelaDerrota(); acaoRetorno = OPTIONS_ACAO_MAIN_MENU; break; 
                } 
                else if (p1_vivo && bots_mortos && !p4_vivo) {
                    // J1 vivo, Bots mortos, J4 (P2) morto -> J1 VENCEU
                    ExecutarTelaVitoriaBattle(j1.spriteName); // <<< Usando spriteName
                    acaoRetorno = OPTIONS_ACAO_MAIN_MENU; break;
                }
                else if (p4_vivo && bots_mortos && !p1_vivo) {
                    // J4 vivo, Bots mortos, J1 (P1) morto -> J4 VENCEU
                    ExecutarTelaVitoriaBattle(j4.spriteName); // <<< Usando spriteName
                    acaoRetorno = OPTIONS_ACAO_MAIN_MENU; break;
                }
                // *** Se (p1_vivo && p4_vivo && bots_mortos), o jogo CONTINUA
                // para o combate entre J1 e J4 (PvP). Nenhuma ação é tomada aqui.
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

        EndDrawing();
    }
    
    DescarregarMapa(); 
    DestruirJogador(&j1); DestruirJogador(&j2);
    DestruirJogador(&j3); DestruirJogador(&j4);
    UnloadBombas(&gBombas);
    UnloadExplosoes(&gExplosoes);
    
    return acaoRetorno;
}

// -----------------------------------------------------------------------------------
// --- MODO STORY (Controlador de Níveis) ---
// -----------------------------------------------------------------------------------
OptionsAction ExecutarJogoStory(StorySettings settings)
{
    // Define a ordem dos mapas: Default -> Cave -> Pirate Boat
    const char* mapasStory[] = {"Default", "Cave", "PirateBoat"};
    int totalNiveis = 3;
    OptionsAction acaoRetorno = OPTIONS_ACAO_MAIN_MENU;
    
    SetExtrasHabilitados(settings.extras);
    
    bool nivelVitoria = false;

    for (int nivel = 0; nivel < totalNiveis; nivel++)
    {
        const char* mapaAtual = mapasStory[nivel];
        nivelVitoria = false; 
        
        OptionsAction acaoNivel;
        
        // --- LOOP PARA RESTART DENTRO DO NÍVEL ---
        do {
            acaoNivel = ExecutarNivelStory(settings, mapaAtual, nivel, &nivelVitoria);
            
            if (acaoNivel != OPTIONS_ACAO_MAIN_MENU && acaoNivel != OPTIONS_ACAO_RESTART)
            {
                return acaoNivel;
            }
        } while (acaoNivel == OPTIONS_ACAO_RESTART);
        // ------------------------------------------

        if (!nivelVitoria)
        {
            return OPTIONS_ACAO_MAIN_MENU;
        }

        // Se for o último nível, o loop acaba e vamos para a tela de vitória final.
        if (nivel == totalNiveis - 1) break; 
        
        // Configura info para o Menu de Transição
        NivelInfo info = {
            .nivelAtual = nivel,
            .proximoMapa = mapasStory[nivel + 1]
        };
        
        OptionsAction acaoMenu = ExecutarMenuProximoNivel(info);
        
        if (acaoMenu == OPTIONS_ACAO_MAIN_MENU || acaoMenu == OPTIONS_ACAO_NENHUMA)
        {
            return OPTIONS_ACAO_MAIN_MENU;
        }
    }

    // Se completou todos os níveis
    if (nivelVitoria) {
        ExecutarTelaVitoria(); // Vitória final do modo história (genérica)
    }
    
    return OPTIONS_ACAO_MAIN_MENU;
}


// --- LÓGICA DE JOGO POR NÍVEL (Com 1 ou 2 Players e Bots Dinâmicos) ---
OptionsAction ExecutarNivelStory(StorySettings settings, const char* nomeMapa, int nivel, bool* nivelVitoria)
{
    // Inicialização do Mapa
    InicializarMapa(nomeMapa); 
    ResetarExtras();

    // VARIÁVEIS DE CONTROLE
    bool doisJogadores = (settings.numPlayers == 2);
    
    // POSIÇÕES DOS JOGADORES
    Jogador j1 = CriarJogador(GetPlayerStartPosition(0), "SpriteBranco", false); 
    
    Jogador j2;       
    Jogador j_botA;   
    Jogador j_botB;   
    Jogador j_botC;   

    // -----------------------------------------------------------------------------------
    // LÓGICA DE CONFIGURAÇÃO DE JOGADORES (Total sempre 4 jogadores ativos)
    // -----------------------------------------------------------------------------------

    if (doisJogadores) {
        // MODO: 2 PLAYERS (J1, J2, BotA, BotB)
        j2 = CriarJogador(GetPlayerStartPosition(3), "SpritePreto", false); 
        j_botA = CriarJogador(GetPlayerStartPosition(1), "SpriteVermelho", true); 
        j_botB = CriarJogador(GetPlayerStartPosition(2), "SpriteAzul", true); 
        j_botC.vivo = false; 
    } else {
        // MODO: 1 PLAYER (J1, BotA, BotB, BotC)
        j2.vivo = false; 
        j_botA = CriarJogador(GetPlayerStartPosition(1), "SpriteVermelho", true); 
        j_botB = CriarJogador(GetPlayerStartPosition(2), "SpriteAzul", true); 
        j_botC = CriarJogador(GetPlayerStartPosition(3), "SpritePreto", true); 
    }
    
    // Configuração de listas GLOBAIS
    Jogador* todosJogadores[4];
    todosJogadores[0] = &j1; 
    
    if (doisJogadores) {
        // 2P: [J1(B), J2(P), BotA(V), BotB(A)]
        todosJogadores[1] = &j2; 
        todosJogadores[2] = &j_botA; 
        todosJogadores[3] = &j_botB; 
    } else {
        // 1P: [J1(B), BotA(V), BotB(A), BotC(P)]
        todosJogadores[1] = &j_botA; 
        todosJogadores[2] = &j_botB; 
        todosJogadores[3] = &j_botC; 
    }
    int numJogadoresTotal = 4;
    
    // Lista de jogadores humanos vivos (para checar derrota)
    Jogador* jogadoresHumanos[] = {&j1, doisJogadores ? &j2 : NULL};
    int numHumanos = doisJogadores ? 2 : 1;
    
    // Lista de bots vivos (para checar vitória)
    Jogador* jogadoresBots[3];
    int numBots;
    
    if (doisJogadores) {
        jogadoresBots[0] = &j_botA; 
        jogadoresBots[1] = &j_botB;
        numBots = 2;
    } else {
        jogadoresBots[0] = &j_botA; 
        jogadoresBots[1] = &j_botB;
        jogadoresBots[2] = &j_botC;
        numBots = 3;
    }

    NodeBombas gBombas = CriarNodeBombas();
    NodeExplosoes gExplosoes = CriarNodeExplosoes();
    
    OptionsAction acaoRetorno = OPTIONS_ACAO_MAIN_MENU;
    bool isPaused = false;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_P)) { 
            isPaused = true;
        }

        if (!isPaused) {
            float deltaTime = GetFrameTime(); 
            
            // --- UPDATE JOGADORES HUMANOS ---
            if (j1.vivo) { AtualizarJogador(&j1, KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, &gBombas, deltaTime, NULL, NULL); VerificarColetaExtras(&j1); }
            if (doisJogadores && j2.vivo) { AtualizarJogador(&j2, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_J, &gBombas, deltaTime, NULL, NULL); VerificarColetaExtras(&j2); }
            
            // --- UPDATE JOGADORES BOTS (IA) ---
            if (j_botA.vivo) { AtualizarJogador(&j_botA, 0, 0, 0, 0, 0, &gBombas, deltaTime, NULL, NULL); VerificarColetaExtras(&j_botA); }
            if (j_botB.vivo) { AtualizarJogador(&j_botB, 0, 0, 0, 0, 0, &gBombas, deltaTime, NULL, NULL); VerificarColetaExtras(&j_botB); }
            if (!doisJogadores && j_botC.vivo) { AtualizarJogador(&j_botC, 0, 0, 0, 0, 0, &gBombas, deltaTime, NULL, NULL); VerificarColetaExtras(&j_botC); }


            AtualizarBombas(&gBombas, deltaTime, &gExplosoes, todosJogadores, numJogadoresTotal);
            AtualizarExplosoes(&gExplosoes, deltaTime);
            
            // --- LÓGICA DE DERROTA ---
            bool todosHumanosMortos = true;
            for (int i = 0; i < numHumanos; i++) { 
                if (jogadoresHumanos[i] != NULL && jogadoresHumanos[i]->vivo) { 
                    todosHumanosMortos = false; 
                    break; 
                } 
            }
            if (todosHumanosMortos) { ExecutarTelaDerrota(); acaoRetorno = OPTIONS_ACAO_MAIN_MENU; break; }

            // --- LÓGICA DE VITÓRIA ---
            bool todosBotsMortos = true;
            for (int i = 0; i < numBots; i++) { 
                if (jogadoresBots[i]->vivo) { 
                    todosBotsMortos = false; 
                    break; 
                } 
            }
            if (todosBotsMortos) { 
                *nivelVitoria = true; 
                acaoRetorno = OPTIONS_ACAO_MAIN_MENU; 
                break; 
            }
        } 
        
        // --- DESENHO ---
        BeginDrawing(); 
            ClearBackground(BLACK);
            DesenharMapa();
            
            DrawText(TextFormat("MAPA: %s", nomeMapa), 20, 20, 20, RAYWHITE); 
            DrawText(TextFormat("NÍVEL: %d / 3", nivel + 1), 20, 40, 20, RAYWHITE); 

            DesenharExtras();
            
            if (j1.vivo) DesenharJogador(&j1);
            if (doisJogadores && j2.vivo) DesenharJogador(&j2);
            if (j_botA.vivo) DesenharJogador(&j_botA);
            if (j_botB.vivo) DesenharJogador(&j_botB);
            if (!doisJogadores && j_botC.vivo) DesenharJogador(&j_botC); 

            DesenharBombas(&gBombas);
            DesenharExplosoes(&gExplosoes);
            
            if (isPaused) {
                OptionsAction acao = ExecutarMenuOptions();
                if (acao == OPTIONS_ACAO_RESUME) { isPaused = false; } 
                else { acaoRetorno = acao; EndDrawing(); break; } 
            }

        EndDrawing();
    }
    
    // Descarregamento
    DescarregarMapa(); 
    DestruirJogador(&j1);
    if (doisJogadores) DestruirJogador(&j2);
    DestruirJogador(&j_botA); 
    DestruirJogador(&j_botB);
    if (!doisJogadores) DestruirJogador(&j_botC); 
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