#include "raylib.h"
#include "menu_battle.h"
#include "menu.h"
#include <stdbool.h>
#include <math.h> 
#include <stdio.h> 

//Variáveis das animações
static RainDrop gotas_chuva[MAX_GOTAS_CHUVA];
static Pulse pulsos_circulares[MAX_PULSOS_CIRCULARES];
static bool efeitos_inicializados = false;

//Funções de efeito
static void InicializarEfeitosMenu(void);
static void AtualizarEfeitosMenu(void);
static void DesenharEfeitosMenu(void);
static void DesenharTextoBrilhante(const char* texto, Vector2 posicao, float tamanho, Color cor_base, Color cor_brilho);

//Def principal
bool ExecutarMenuBattle(BattleSettings *configuracoes)
{
    if (efeitos_inicializados == false) {
        InicializarEfeitosMenu();
    }

    //variaveis do menu
    static int opcao_atual = 0; 
    static int numero_jogadores = 1;
    static bool extras_habilitados = false;
    static int indice_mapa = 0;
    
    //Reseta mouse ao entrar
    opcao_atual = 0; 
    
    //Opções: Jogadores, Extras, Mapa, start, back
    const int numero_opcoes = 5; 

    while (WindowShouldClose() == 0)
    {
        //atualizar ação
        
        if (IsKeyPressed(KEY_DOWN)) {
            opcao_atual = (opcao_atual + 1) % numero_opcoes; 
        }
        
        if (IsKeyPressed(KEY_UP)) {
            opcao_atual = (opcao_atual - 1 + numero_opcoes) % numero_opcoes;
        }

        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
            bool pressionou_direita = IsKeyPressed(KEY_RIGHT);
            
            switch (opcao_atual)
            {
                case 0: //Nº de jogadores
                {
                    if (numero_jogadores == 1) {
                        numero_jogadores = 2;
                    } else {
                        numero_jogadores = 1;
                    }
                    break;
                }
                case 1: //Power ups
                {
                    if (extras_habilitados == true) {
                        extras_habilitados = false;
                    } else {
                        extras_habilitados = true;
                    }
                    break;
                }
                case 2: //Mapas
                {
                    if (pressionou_direita) {
                        indice_mapa = (indice_mapa + 1) % 3;
                    } else {
                        indice_mapa = (indice_mapa - 1 + 3) % 3;
                    }
                    break;
                }
            }
        }
        
        //Confirmação (enter)
        if (IsKeyReleased(KEY_ENTER)) { 
            if (opcao_atual == 3) { //start
                configuracoes->numero_jogadores = numero_jogadores;
                configuracoes->extras_habilitados = extras_habilitados;
                configuracoes->indice_mapa = indice_mapa;
                return true; //Inicia o jogo
            }
            if (opcao_atual == 4) { //back
                return false; //Volta ao menu principal
            }
        }
        
        if (IsKeyPressed(KEY_ESCAPE)) {
            return false; 
        }
        
        AtualizarEfeitosMenu();

        //DESENHAR
        BeginDrawing();
        {
            int largura_tela = GetScreenWidth();
            int altura_tela = GetScreenHeight();
            ClearBackground(COLOR_UI_BACKGROUND); 
            DesenharEfeitosMenu();
            
            //Título
            float tamanho_titulo = 70;
            float posicao_titulo_x = (largura_tela - MeasureText("CONFIGURAR BATALHA", tamanho_titulo)) / 2;
            DesenharTextoBrilhante("CONFIGURAR BATALHA", (Vector2){ posicao_titulo_x, altura_tela * 0.1f }, tamanho_titulo,RAYWHITE, COLOR_BLUE_HIGHLIGHT);
                         
            //Variáveis do desenho das frases
            float tamanho_fonte_opcao = 40;
            float espacamento_vertical = 65;
            float menu_posicao_y_inicial = altura_tela * 0.35f;
            float coluna_rotulo_x = largura_tela * 0.3f; 
            float coluna_valor_x = largura_tela * 0.55f; 

            //Formatação das preferências
            char texto_jogadores[4]; sprintf(texto_jogadores, "%d", numero_jogadores);
            const char* texto_extras = (extras_habilitados) ? "Ligado" : "Desligado";
            
            char texto_mapa[16];
            switch(indice_mapa) {
                case 0: 
                {
                    sprintf(texto_mapa, "Padrão"); 
                    break;
                }
                case 1:
                {
                    sprintf(texto_mapa, "Caverna"); 
                    break;
                }
                case 2:
                {
                    sprintf(texto_mapa, "Barco Pirata"); 
                    break;
                }
                default: 
                {
                    sprintf(texto_mapa, "Padrão");
                    break;
                }
            }

            const char* rotulos[] = {"Jogadores:", "Extras:", "Mapa:"};
            const char* valores[] = {texto_jogadores, texto_extras, texto_mapa};
            
            //Desenha as opções configuráveis (0, 1, 2)
            for (int indice = 0; indice < 3; indice++) 
            {
                Color cor_base = (indice == opcao_atual) ? COLOR_YELLOW_HIGHLIGHT : COLOR_GRAY_OPTION;
                Color cor_brilho = (indice == opcao_atual) ? (Color){200, 160, 0, 150} : (Color){50,50,50,100};
                
                //Desenha o Rótulo
                DesenharTextoBrilhante(rotulos[indice], (Vector2){ coluna_rotulo_x, menu_posicao_y_inicial + (indice * espacamento_vertical) }, tamanho_fonte_opcao, cor_base, cor_brilho);
                             
                //Desenha a Seta Esquerda (<)
                DesenharTextoBrilhante("<", (Vector2){ coluna_valor_x - 30, menu_posicao_y_inicial + (indice * espacamento_vertical) }, tamanho_fonte_opcao, cor_base, cor_brilho);
                
                //Desenha o VALOR
                DesenharTextoBrilhante(valores[indice], (Vector2){ coluna_valor_x, menu_posicao_y_inicial + (indice * espacamento_vertical) }, tamanho_fonte_opcao, cor_base, cor_brilho);
                
                //Desenha a Seta Direita (>)
                DesenharTextoBrilhante(">", (Vector2){ coluna_valor_x + MeasureText(valores[indice], tamanho_fonte_opcao) + 30, menu_posicao_y_inicial + (indice * espacamento_vertical) }, tamanho_fonte_opcao, cor_base, cor_brilho);
            }
            
            //Botão Iniciar
            float iniciar_y = menu_posicao_y_inicial + (3 * espacamento_vertical) + 20;
            float iniciar_x = (largura_tela - MeasureText("INICIAR", tamanho_fonte_opcao)) / 2;
            
            if (opcao_atual == 3) {
                bool deve_piscar = fmod(GetTime(), 0.2) > 0.1;
                Color cor_base = deve_piscar ? COLOR_YELLOW_HIGHLIGHT : WHITE;
                Color cor_brilho = deve_piscar ? (Color){200, 160, 0, 150} : COLOR_BLUE_HIGHLIGHT;
                DesenharTextoBrilhante("INICIAR", (Vector2){ iniciar_x, iniciar_y }, tamanho_fonte_opcao, cor_base, cor_brilho);
            } else {
                DesenharTextoBrilhante("INICIAR", (Vector2){ iniciar_x, iniciar_y }, tamanho_fonte_opcao, COLOR_GRAY_OPTION, (Color){50,50,50,100});
            }

            //Botão Voltar
            float voltar_y = iniciar_y + espacamento_vertical; 
            float voltar_x = (largura_tela - MeasureText("VOLTAR", tamanho_fonte_opcao)) / 2;
            
            if (opcao_atual == 4) {
                Color cor_base = COLOR_YELLOW_HIGHLIGHT; 
                Color cor_brilho = (Color){200, 160, 0, 150};
                DesenharTextoBrilhante("VOLTAR", (Vector2){ voltar_x, voltar_y }, tamanho_fonte_opcao, cor_base, cor_brilho);
            } else {
                DesenharTextoBrilhante("VOLTAR", (Vector2){ voltar_x, voltar_y }, tamanho_fonte_opcao, COLOR_GRAY_OPTION, (Color){50,50,50,100});
            }
            
            //Dica
            const char* texto_dica = "Use UP/DOWN to navigate, LEFT/RIGHT to switch, ENTER to select";
            int largura_texto_dica = MeasureText(texto_dica, 20);
            DrawText(texto_dica, (largura_tela - largura_texto_dica) / 2, altura_tela - 40, 20, RAYWHITE);
        
        }
        EndDrawing();
    }
    
    return false; 
}


static void InicializarEfeitosMenu(void) {
    int largura_tela = GetScreenWidth();
    int altura_tela = GetScreenHeight();
    
    //Gotas de Chuva
    for (int indice = 0; indice < MAX_GOTAS_CHUVA; indice++) {
        gotas_chuva[indice].posicao.x = (float)GetRandomValue(0, largura_tela);
        gotas_chuva[indice].posicao.y = (float)GetRandomValue(-altura_tela, 0); 
        gotas_chuva[indice].velocidade = (float)GetRandomValue(200, 600); 
    }
    
    //Pulsos Circulares
    for (int indice = 0; indice < MAX_PULSOS_CIRCULARES; indice++) { 
        pulsos_circulares[indice].alpha = 0.0f; 
    }
    
    efeitos_inicializados = true;
}

static void AtualizarEfeitosMenu(void) {
    int largura_tela = GetScreenWidth();
    int altura_tela = GetScreenHeight();
    float tempo_quadro = GetFrameTime(); 
    
    //Gotas de Chuva
    for (int indice = 0; indice < MAX_GOTAS_CHUVA; indice++) {
        gotas_chuva[indice].posicao.y += gotas_chuva[indice].velocidade * tempo_quadro; 
        
        if (gotas_chuva[indice].posicao.y > altura_tela) {
            gotas_chuva[indice].posicao.x = (float)GetRandomValue(0, largura_tela);
            gotas_chuva[indice].posicao.y = (float)GetRandomValue(-40, -20); 
            gotas_chuva[indice].velocidade = (float)GetRandomValue(200, 600);
        }
    }
    
    //Pulsos Circulares
    for (int indice = 0; indice < MAX_PULSOS_CIRCULARES; indice++) {
        if (pulsos_circulares[indice].alpha > 0.0f) {
            pulsos_circulares[indice].raio += 150.0f * tempo_quadro; 
            pulsos_circulares[indice].alpha -= 0.7f * tempo_quadro;  
            
            if (pulsos_circulares[indice].alpha < 0.0f) {
                pulsos_circulares[indice].alpha = 0.0f;
            }
        } else {
            //Chance de iniciar um novo pulso
            if (GetRandomValue(0, 1000) > 995) {
                pulsos_circulares[indice].centro.x = (float)GetRandomValue(0, largura_tela);
                pulsos_circulares[indice].centro.y = (float)GetRandomValue(0, altura_tela);
                pulsos_circulares[indice].raio = 0.0f;
                pulsos_circulares[indice].alpha = 1.0f; 
            }
        }
    }
}

static void DesenharEfeitosMenu(void) {
    //Gotas de Chuva
    for (int indice = 0; indice < MAX_GOTAS_CHUVA; indice++) {
        DrawRectangle(gotas_chuva[indice].posicao.x, gotas_chuva[indice].posicao.y, 2, 12, Fade(COLOR_BLUE_HIGHLIGHT, 0.3f));
    }
    
    //Pulsos Circulares
    for (int indice = 0; indice < MAX_PULSOS_CIRCULARES; indice++) {
        if (pulsos_circulares[indice].alpha > 0.0f) {
            DrawCircleLines(pulsos_circulares[indice].centro.x, pulsos_circulares[indice].centro.y, pulsos_circulares[indice].raio, Fade(COLOR_YELLOW_HIGHLIGHT, pulsos_circulares[indice].alpha));
        }
    }
}

static void DesenharTextoBrilhante(const char* texto, Vector2 posicao, float tamanho, Color cor_base, Color cor_brilho) {
    const int deslocamento_brilho = 2;
    //Brilho
    DrawText(texto, posicao.x - deslocamento_brilho, posicao.y, tamanho, cor_brilho);
    DrawText(texto, posicao.x + deslocamento_brilho, posicao.y, tamanho, cor_brilho);
    DrawText(texto, posicao.x, posicao.y - deslocamento_brilho, tamanho, cor_brilho);
    DrawText(texto, posicao.x, posicao.y + deslocamento_brilho, tamanho, cor_brilho);
    //Texto principal
    DrawText(texto, posicao.x, posicao.y, tamanho, cor_base);
}