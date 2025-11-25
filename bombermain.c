#include "raylib.h"
#include "inicio.h"
#include "menu.h"
#include "menu_battle.h"
#include "menu_story.h" 
#include "menu_prox.h"  
#include "options.h"    // Agora AcaoOpcoes
#include "mapa.h"       
#include "jogador.h"    
#include "bomba.h"      
#include "explosao.h" 
#include "derrota.h"  
#include "vitoria.h"
#include "vitoria_battle.h" 
#include "extras.h"     
#include "historico.h"    
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>    
#include <string.h>       

// --- DEFINIÇÕES GLOBAIS ---
const int LARGURA_TELA = 1440;
const int ALTURA_TELA = 900;

// CORREÇÃO: Definição das constantes de menu que estavam faltando
// NOTA: Estas constantes DEVEM ser definidas em menu.h, mas são adicionadas
// aqui para resolver o erro de compilação imediato.
#define ESCOLHA_LOJA 10
#define ESCOLHA_OUTROS 11


// Assinaturas (Traduzidas)
AcaoOpcoes ExecutarJogoBatalha(BattleSettings configuracoes);
AcaoOpcoes ExecutarJogoHistoria(StorySettings configuracoes); 
AcaoOpcoes ExecutarNivelHistoria(StorySettings configuracoes, const char* nomeMapa, int nivel, bool* nivelVitoria); 
void ExecutarLoja(void);
void ExecutarOutros(void);

// -----------------------------------------------------------------------------------
// --- MAIN LOOP ---
// -----------------------------------------------------------------------------------
int main(void)
{
    InitWindow(LARGURA_TELA, ALTURA_TELA, "BomberMain");
    ToggleFullscreen();
    SetTargetFPS(60);
    
    InicializarExtras(); 

    bool deveContinuar = ExecutarTelaInicio();
    
    BattleSettings ultimasConfiguracoesBatalha = {0};
    StorySettings ultimasConfiguracoesHistoria = {0};
    OpcaoMenu ultimoModo = ESCOLHA_NENHUMA_OU_FECHOU;
    
    while (deveContinuar == true && WindowShouldClose() == 0)
    {
        OpcaoMenu escolha = ExecutarTelaMenu();
        
        // CORREÇÃO: Uso do do-while para criar um loop interno que permite o 'continue'
        // quando a ação EDITAR JOGO é acionada, permitindo refazer a escolha do menu.
        do {
            if (escolha == ESCOLHA_BATTLE) {
                
                BattleSettings configuracoes; 
                bool iniciar = ExecutarMenuBattle(&configuracoes); 
                
                if (iniciar == true)
                {
                    ultimasConfiguracoesBatalha = configuracoes;
                    ultimoModo = ESCOLHA_BATTLE;
                    
                    AcaoOpcoes acao;
                    
                    // Loop para RESTART no MODO BATTLE
                    do {
                        acao = ExecutarJogoBatalha(ultimasConfiguracoesBatalha); 
                        
                        if (acao == OPCOES_ACAO_EDITAR_JOGO) {
                            escolha = ESCOLHA_BATTLE; // Mantém a escolha para reexecutar o menu_battle
                            break; 
                        }
                        
                    } while (acao == OPCOES_ACAO_REINICIAR);
                    
                    if (acao == OPCOES_ACAO_EDITAR_JOGO) {
                        // O 'continue' agora aponta para o próximo loop 'do-while'
                        continue; 
                    }
                }

            } else if (escolha == ESCOLHA_STORY) {
                
                StorySettings configuracoes_story;
                bool iniciar = ExecutarMenuStory(&configuracoes_story);
                
                if (iniciar == true)
                {
                    ultimasConfiguracoesHistoria = configuracoes_story;
                    ultimoModo = ESCOLHA_STORY;
                    
                    AcaoOpcoes acao;
                    
                    // Loop para RESTART no MODO STORY
                    do {
                        acao = ExecutarJogoHistoria(ultimasConfiguracoesHistoria); 
                        
                        if (acao == OPCOES_ACAO_EDITAR_JOGO) {
                            escolha = ESCOLHA_STORY; // Mantém a escolha para reexecutar o menu_story
                            break; 
                        }
                        
                    } while (acao == OPCOES_ACAO_REINICIAR);
                    
                    if (acao == OPCOES_ACAO_EDITAR_JOGO) {
                        // O 'continue' agora aponta para o próximo loop 'do-while'
                        continue; 
                    }
                }

            } else if (escolha == ESCOLHA_LOJA) { 
                ExecutarLoja();
            } else if (escolha == ESCOLHA_OUTROS) { 
                ExecutarOutros();
            }
            
            // Sai do loop do-while para retornar ao ExecutarTelaMenu()
            break; 
            
        } while (true); // O loop interno é controlado pelos 'break' e 'continue'

        if (escolha == ESCOLHA_SAIR || escolha == ESCOLHA_NENHUMA_OU_FECHOU) {
             deveContinuar = false;
        }

    } // FIM DO LOOP EXTERNO (while (deveContinuar...))
    
    DescarregarExtras(); 
    CloseWindow(); 
    return 0;
}

// -----------------------------------------------------------------------------------
// --- MODO BATTLE (ExecutarJogoBatalha) ---
// -----------------------------------------------------------------------------------
AcaoOpcoes ExecutarJogoBatalha(BattleSettings configuracoes) {
    
    DefinirExtrasHabilitados(configuracoes.extras_habilitados); 

    if (configuracoes.indice_mapa == 1) {
        InicializarMapa("Cave");
    } 
    else if (configuracoes.indice_mapa == 2) {
        InicializarMapa("PirateBoat"); 
    } 
    else {
        InicializarMapa("Default");
    }
    
    ResetarExtras();
    
    bool j1_ehHumano = true;

    // Lógica ajustada para j4_ehHumano
    bool j4_ehHumano;
    if (configuracoes.numero_jogadores == 2) {
        j4_ehHumano = true; // 2P: J1 e J4 são humanos
    } else {
        j4_ehHumano = false; // 1P: J4 é bot
    }
    
    // Jogadores
    Jogador j1 = CriarJogador(ObterPosicaoInicialJogador(0), "SpriteBranco", j1_ehHumano == false); 
    Jogador j2 = CriarJogador(ObterPosicaoInicialJogador(1), "SpriteVermelho", true);      
    Jogador j3 = CriarJogador(ObterPosicaoInicialJogador(2), "SpriteAzul", true);      
    Jogador j4 = CriarJogador(ObterPosicaoInicialJogador(3), "SpritePreto", j4_ehHumano == false); 
    
    Jogador* todosJogadores[] = {&j1, &j2, &j3, &j4};
    int numJogadores = 4;
    NodeBombas gBombas = CriarNodeBombas();
    NodeExplosoes gExplosoes = CriarNodeExplosoes();
    
    // Alvos para a IA no modo 1P/2P
    Jogador* p1_target = &j1;
    Jogador* p2_target;
    if (j4_ehHumano == true) { 
        p2_target = &j4;
    } else {
        p2_target = NULL;
    } 
    
    AcaoOpcoes acaoRetorno = OPCOES_ACAO_MENU_PRINCIPAL;
    bool isPaused = false;

    // CORREÇÃO: !WindowShouldClose() -> WindowShouldClose() == 0
    while (WindowShouldClose() == 0) {
        
        if (IsKeyPressed(KEY_P)) { 
            isPaused = true;
        }
        
        if (isPaused == false) {
            float deltaTime = GetFrameTime(); 

            // Atualização dos Jogadores
            AtualizarJogador(&j1, KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, &gBombas, deltaTime, p1_target, p2_target); 
            AtualizarJogador(&j2, 0, 0, 0, 0, 0, &gBombas, deltaTime, p1_target, p2_target);
            AtualizarJogador(&j3, 0, 0, 0, 0, 0, &gBombas, deltaTime, p1_target, p2_target); 
            
            if (j4_ehHumano == true) { 
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
            if (configuracoes.numero_jogadores == 1) {
                // Modo 1P: J1 (Branco) vs. Bots (Vermelho, Azul, Preto)
                if (j1.vivo == false) { 
                    // --- REGISTRO DE DERROTA ---
                    RegistroBatalha reg;
                    strncpy(reg.modo, "Battle (1P)", sizeof(reg.modo) - 1);
                    reg.modo[sizeof(reg.modo) - 1] = '\0';
                    strncpy(reg.vencedor, "Bots (IA)", sizeof(reg.vencedor) - 1);
                    reg.vencedor[sizeof(reg.vencedor) - 1] = '\0';
                    AdicionarRegistroHistorico(reg);
                    // ---------------------------
                    ExecutarTelaDerrota(); 
                    acaoRetorno = OPCOES_ACAO_MENU_PRINCIPAL; 
                    break; 
                }
                if (j1.vivo == true && j2.vivo == false && j3.vivo == false && j4.vivo == false) { 
                    // J1 VENCEU
                    // --- REGISTRO DE VITORIA ---
                    RegistroBatalha reg;
                    strncpy(reg.modo, "Battle (1P)", sizeof(reg.modo) - 1);
                    reg.modo[sizeof(reg.modo) - 1] = '\0';
                    strncpy(reg.vencedor, "Player 1 (Branco)", sizeof(reg.vencedor) - 1);
                    reg.vencedor[sizeof(reg.vencedor) - 1] = '\0';
                    AdicionarRegistroHistorico(reg);
                    // ---------------------------
                    ExecutarTelaVitoriaBattle(j1.nomeSprite); 
                    acaoRetorno = OPCOES_ACAO_MENU_PRINCIPAL; 
                    break; 
                }
            } else { // 2 Players
                // Modo 2P: J1 (Branco) e J4 (Preto) vs. Bots (Vermelho, Azul)
                bool p1_vivo = j1.vivo;
                bool p4_vivo = j4.vivo; 
                
                bool bots_mortos = j2.vivo == false && j3.vivo == false;

                if (p1_vivo == false && p4_vivo == false) { 
                    // Ambos humanos perderam
                    // --- REGISTRO DE DERROTA ---
                    RegistroBatalha reg;
                    strncpy(reg.modo, "Battle (2P)", sizeof(reg.modo) - 1);
                    reg.modo[sizeof(reg.modo) - 1] = '\0';
                    strncpy(reg.vencedor, "Bots (IA)", sizeof(reg.vencedor) - 1);
                    reg.vencedor[sizeof(reg.vencedor) - 1] = '\0';
                    AdicionarRegistroHistorico(reg);
                    // ---------------------------
                    ExecutarTelaDerrota(); 
                    acaoRetorno = OPCOES_ACAO_MENU_PRINCIPAL; 
                    break; 
                } 
                else if (p1_vivo == true && bots_mortos == true && p4_vivo == false) {
                    // J1 vivo, Bots mortos, J4 (P2) morto -> J1 VENCEU
                    // --- REGISTRO DE VITORIA J1 ---
                    RegistroBatalha reg;
                    strncpy(reg.modo, "Battle (2P)", sizeof(reg.modo) - 1);
                    reg.modo[sizeof(reg.modo) - 1] = '\0';
                    strncpy(reg.vencedor, "Player 1 (Branco) - PvP Vitoria", sizeof(reg.vencedor) - 1);
                    reg.vencedor[sizeof(reg.vencedor) - 1] = '\0';
                    AdicionarRegistroHistorico(reg);
                    // ------------------------------
                    ExecutarTelaVitoriaBattle(j1.nomeSprite); 
                    acaoRetorno = OPCOES_ACAO_MENU_PRINCIPAL; 
                    break;
                }
                else if (p4_vivo == true && bots_mortos == true && p1_vivo == false) {
                    // J4 vivo, Bots mortos, J1 (P1) morto -> J4 VENCEU
                    // --- REGISTRO DE VITORIA J4 ---
                    RegistroBatalha reg;
                    strncpy(reg.modo, "Battle (2P)", sizeof(reg.modo) - 1);
                    reg.modo[sizeof(reg.modo) - 1] = '\0';
                    strncpy(reg.vencedor, "Player 2 (Preto) - PvP Vitoria", sizeof(reg.vencedor) - 1);
                    reg.vencedor[sizeof(reg.vencedor) - 1] = '\0';
                    AdicionarRegistroHistorico(reg);
                    // ------------------------------
                    ExecutarTelaVitoriaBattle(j4.nomeSprite); 
                    acaoRetorno = OPCOES_ACAO_MENU_PRINCIPAL; 
                    break;
                }
                // Se (p1_vivo && p4_vivo && bots_mortos), o jogo continua (PvP).
            }
        } 
        
        // --- DESENHO ---
        BeginDrawing(); 
        {
            ClearBackground(BLACK);
            DesenharMapa();
            
            DesenharExtras(); 
            
            DesenharJogador(&j1); DesenharJogador(&j2);
            DesenharJogador(&j3); DesenharJogador(&j4);
            DesenharBombas(&gBombas);
            DesenharExplosoes(&gExplosoes);

            // --- LÓGICA DO MENU OPTIONS ---
            if (isPaused == true) {
                AcaoOpcoes acao = ExecutarMenuOpcoes();
                
                if (acao == OPCOES_ACAO_RESUMO) {
                    isPaused = false; 
                } else {
                    acaoRetorno = acao;
                    EndDrawing(); 
                    break; 
                }
            }
        }
        EndDrawing();
    }
    
    DescarregarMapa(); 
    DestruirJogador(&j1); DestruirJogador(&j2);
    DestruirJogador(&j3); DestruirJogador(&j4);
    UnloadBombas(&gBombas);
    DescarregarExplosoes(&gExplosoes); 
    
    return acaoRetorno;
}

// -----------------------------------------------------------------------------------
// --- MODO STORY (ExecutarJogoHistoria - Controlador de Níveis) ---
// -----------------------------------------------------------------------------------
AcaoOpcoes ExecutarJogoHistoria(StorySettings configuracoes)
{
    // Define a ordem dos mapas: Default -> Cave -> Pirate Boat
    const char* mapasStory[] = {"Default", "Cave", "PirateBoat"};
    int totalNiveis = 3;
    
    DefinirExtrasHabilitados(configuracoes.extras_habilitados); 
    
    bool nivelVitoria = false;

    for (int nivel = 0; nivel < totalNiveis; nivel++)
    {
        const char* mapaAtual = mapasStory[nivel];
        nivelVitoria = false; 
        
        AcaoOpcoes acaoNivel;
        
        // --- LOOP PARA RESTART DENTRO DO NÍVEL ---
        do {
            acaoNivel = ExecutarNivelHistoria(configuracoes, mapaAtual, nivel, &nivelVitoria);
            
            if (acaoNivel != OPCOES_ACAO_MENU_PRINCIPAL && acaoNivel != OPCOES_ACAO_REINICIAR)
            {
                // Sai do loop do For para retornar a ação (RESUMO, EDITAR JOGO, etc.)
                return acaoNivel;
            }
        } while (acaoNivel == OPCOES_ACAO_REINICIAR);
        // ------------------------------------------

        if (nivelVitoria == false)
        {
            // O nível não foi concluído (o jogador perdeu ou saiu)
            return OPCOES_ACAO_MENU_PRINCIPAL;
        }

        // Se for o último nível, o loop acaba e vamos para a tela de vitória final.
        if (nivel == totalNiveis - 1) { 
            break; 
        }
        
        // Configura info para o Menu de Transição
        InfoNivel info = { // Usando a nova struct InfoNivel
            .nivelAtual = nivel,
            .proximoMapa = mapasStory[nivel + 1]
        };
        
        AcaoOpcoes acaoMenu = ExecutarMenuProximoNivel(info);
        
        if (acaoMenu == OPCOES_ACAO_MENU_PRINCIPAL || acaoMenu == OPCOES_ACAO_NENHUMA)
        {
            return OPCOES_ACAO_MENU_PRINCIPAL;
        }
    }

    // Se completou todos os níveis
    if (nivelVitoria == true) {
        ExecutarTelaVitoria(); // Vitória final do modo história (genérica)
    }
    
    return OPCOES_ACAO_MENU_PRINCIPAL;
}


// --- LÓGICA DE JOGO POR NÍVEL (ExecutarNivelHistoria) ---
AcaoOpcoes ExecutarNivelHistoria(StorySettings configuracoes, const char* nomeMapa, int nivel, bool* nivelVitoria)
{
    // Inicialização do Mapa
    InicializarMapa(nomeMapa); 
    ResetarExtras();

    // VARIÁVEIS DE CONTROLE
    bool doisJogadores;
    if (configuracoes.numero_jogadores == 2) {
        doisJogadores = true;
    } else {
        doisJogadores = false;
    }
    
    // POSIÇÕES DOS JOGADORES
    Jogador j1 = CriarJogador(ObterPosicaoInicialJogador(0), "SpriteBranco", false); 
    
    Jogador j2 = {0};       
    Jogador j_botA = {0};   
    Jogador j_botB = {0};   
    Jogador j_botC = {0};   

    // -----------------------------------------------------------------------------------
    // LÓGICA DE CONFIGURAÇÃO DE JOGADORES (Total sempre 4 jogadores ativos)
    // -----------------------------------------------------------------------------------

    if (doisJogadores == true) {
        // MODO: 2 PLAYERS (J1(B), J2(P), BotA(V), BotB(A))
        j2 = CriarJogador(ObterPosicaoInicialJogador(3), "SpritePreto", false); 
        j_botA = CriarJogador(ObterPosicaoInicialJogador(1), "SpriteVermelho", true); 
        j_botB = CriarJogador(ObterPosicaoInicialJogador(2), "SpriteAzul", true); 
        j_botC.vivo = false; 
    } else {
        // MODO: 1 PLAYER (J1(B), BotA(V), BotB(A), BotC(P))
        j2.vivo = false; 
        j_botA = CriarJogador(ObterPosicaoInicialJogador(1), "SpriteVermelho", true); 
        j_botB = CriarJogador(ObterPosicaoInicialJogador(2), "SpriteAzul", true); 
        j_botC = CriarJogador(ObterPosicaoInicialJogador(3), "SpritePreto", true); 
    }
    
    // Configuração de listas GLOBAIS
    Jogador* todosJogadores[4];
    todosJogadores[0] = &j1; 
    
    if (doisJogadores == true) {
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
    Jogador* jogadoresHumanos[2];
    jogadoresHumanos[0] = &j1;
    int numHumanos;
    
    if (doisJogadores == true) {
        numHumanos = 2;
        jogadoresHumanos[1] = &j2;
    } else {
        numHumanos = 1;
        jogadoresHumanos[1] = NULL;
    }
    
    // Lista de bots vivos (para checar vitória)
    Jogador* jogadoresBots[3];
    int numBots;
    
    if (doisJogadores == true) {
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
    
    AcaoOpcoes acaoRetorno = OPCOES_ACAO_MENU_PRINCIPAL;
    bool isPaused = false;

    // CORREÇÃO: !WindowShouldClose() -> WindowShouldClose() == 0
    while (WindowShouldClose() == 0)
    {
        if (IsKeyPressed(KEY_P)) { 
            isPaused = true;
        }

        if (isPaused == false) {
            float deltaTime = GetFrameTime(); 
            
            // --- UPDATE JOGADORES HUMANOS ---
            if (j1.vivo == true) { 
                AtualizarJogador(&j1, KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, &gBombas, deltaTime, NULL, NULL); 
                VerificarColetaExtras(&j1); 
            }
            if (doisJogadores == true && j2.vivo == true) { 
                AtualizarJogador(&j2, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_J, &gBombas, deltaTime, NULL, NULL); 
                VerificarColetaExtras(&j2); 
            }
            
            // --- UPDATE JOGADORES BOTS (IA) ---
            if (j_botA.vivo == true) { 
                AtualizarJogador(&j_botA, 0, 0, 0, 0, 0, &gBombas, deltaTime, NULL, NULL); 
                VerificarColetaExtras(&j_botA); 
            }
            if (j_botB.vivo == true) { 
                AtualizarJogador(&j_botB, 0, 0, 0, 0, 0, &gBombas, deltaTime, NULL, NULL); 
                VerificarColetaExtras(&j_botB); 
            }
            if (doisJogadores == false && j_botC.vivo == true) { 
                AtualizarJogador(&j_botC, 0, 0, 0, 0, 0, &gBombas, deltaTime, NULL, NULL); 
                VerificarColetaExtras(&j_botC); 
            }

            AtualizarBombas(&gBombas, deltaTime, &gExplosoes, todosJogadores, numJogadoresTotal);
            AtualizarExplosoes(&gExplosoes, deltaTime);
            
            // --- LÓGICA DE DERROTA ---
            bool todosHumanosMortos = true;
            for (int i = 0; i < numHumanos; i++) { 
                if (jogadoresHumanos[i] != NULL && jogadoresHumanos[i]->vivo == true) { 
                    todosHumanosMortos = false; 
                    break; 
                } 
            }
            if (todosHumanosMortos == true) { 
                // --- REGISTRO DE DERROTA STORY ---
                RegistroBatalha reg;
                
                // Formata a string do modo
                char modo_str[50];
                snprintf(modo_str, sizeof(modo_str), "Story Nivel %d (%dP)", nivel + 1, numHumanos);
                strncpy(reg.modo, modo_str, sizeof(reg.modo) - 1);
                reg.modo[sizeof(reg.modo) - 1] = '\0';
                
                strncpy(reg.vencedor, "Bots (IA)", sizeof(reg.vencedor) - 1);
                reg.vencedor[sizeof(reg.vencedor) - 1] = '\0';
                
                AdicionarRegistroHistorico(reg);
                // ---------------------------------
                ExecutarTelaDerrota(); 
                acaoRetorno = OPCOES_ACAO_MENU_PRINCIPAL; 
                break; 
            }

            // --- LÓGICA DE VITÓRIA ---
            bool todosBotsMortos = true;
            for (int i = 0; i < numBots; i++) { 
                if (jogadoresBots[i]->vivo == true) { 
                    todosBotsMortos = false; 
                    break; 
                } 
            }
            if (todosBotsMortos == true) { 
                *nivelVitoria = true; 
                
                // --- REGISTRO DE VITORIA STORY ---
                RegistroBatalha reg;
                const char* vencedor_str;
                if (numHumanos == 1) {
                    vencedor_str = "Player 1 (Branco)";
                } else {
                    vencedor_str = "Equipe Humana (Branco/Preto)";
                }
                
                // Formata a string do modo
                char modo_str[50];
                snprintf(modo_str, sizeof(modo_str), "Story Nivel %d (%dP)", nivel + 1, numHumanos);
                strncpy(reg.modo, modo_str, sizeof(reg.modo) - 1);
                reg.modo[sizeof(reg.modo) - 1] = '\0';
                
                strncpy(reg.vencedor, vencedor_str, sizeof(reg.vencedor) - 1);
                reg.vencedor[sizeof(reg.vencedor) - 1] = '\0';
                
                AdicionarRegistroHistorico(reg);
                // ---------------------------------
                acaoRetorno = OPCOES_ACAO_NENHUMA; // Retorna NENHUMA para sinalizar sucesso/continuação
                break; 
            }
        } 
        
        // --- DESENHO ---
        BeginDrawing(); 
        {
            ClearBackground(BLACK);
            DesenharMapa();
            
            DrawText(TextFormat("MAPA: %s", nomeMapa), 20, 20, 20, RAYWHITE); 
            DrawText(TextFormat("NÍVEL: %d / 3", nivel + 1), 20, 40, 20, RAYWHITE); 

            DesenharExtras();
            
            if (j1.vivo == true) { 
                DesenharJogador(&j1);
            }
            if (doisJogadores == true && j2.vivo == true) { 
                DesenharJogador(&j2);
            }
            if (j_botA.vivo == true) { 
                DesenharJogador(&j_botA);
            }
            if (j_botB.vivo == true) { 
                DesenharJogador(&j_botB);
            }
            if (doisJogadores == false && j_botC.vivo == true) { 
                DesenharJogador(&j_botC); 
            }

            DesenharBombas(&gBombas);
            DesenharExplosoes(&gExplosoes);
            
            if (isPaused == true) {
                AcaoOpcoes acao = ExecutarMenuOpcoes();
                if (acao == OPCOES_ACAO_RESUMO) { 
                    isPaused = false; 
                } 
                else { 
                    acaoRetorno = acao; 
                    EndDrawing(); 
                    break; 
                } 
            }
        }
        EndDrawing();
    }
    
    // Descarregamento
    DescarregarMapa(); 
    DestruirJogador(&j1);
    if (doisJogadores == true) {
        DestruirJogador(&j2);
    }
    DestruirJogador(&j_botA); 
    DestruirJogador(&j_botB);
    if (doisJogadores == false) {
        DestruirJogador(&j_botC); 
    }
    UnloadBombas(&gBombas);
    DescarregarExplosoes(&gExplosoes); 
    
    return acaoRetorno;
}

// -----------------------------------------------------------------------------------
// --- TELAS SIMPLES (ExecutarLoja/ExecutarOutros) ---
// -----------------------------------------------------------------------------------
void ExecutarLoja(void) {
    // CORREÇÃO: !WindowShouldClose() -> WindowShouldClose() == 0
    while (WindowShouldClose() == 0) {
        if (IsKeyPressed(KEY_ESCAPE)) { 
            break;
        }
        BeginDrawing(); 
        {
            ClearBackground(BROWN);
            DrawText("LOJA - Pressione ESC para voltar", 190, 200, 20, WHITE);
        }
        EndDrawing();
    }
}
void ExecutarOutros(void) {
    // CORREÇÃO: !WindowShouldClose() -> WindowShouldClose() == 0
    while (WindowShouldClose() == 0) {
        if (IsKeyPressed(KEY_ESCAPE)) { 
            break;
        }
        BeginDrawing(); 
        {
            ClearBackground(DARKGRAY);
            DrawText("OUTROS/OPCOES - Pressione ESC para voltar", 190, 200, 20, WHITE);
        }
        EndDrawing();
    }
}