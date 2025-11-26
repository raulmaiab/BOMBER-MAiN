#include "raylib.h"
#include "menu_story.h"
#include "menu.h" 
#include <stdbool.h>
#include <math.h> 
#include <stdio.h> 

//Variáveis de efeito
static RainDrop gotas_chuva[MAX_GOTAS_CHUVA];
static Pulse pulsos_circulares[MAX_PULSOS_CIRCULARES];
static bool efeitos_inicializados = false;

//Funções de efeito
static void InicializarEfeitosMenu(void);
static void AtualizarEfeitosMenu(void);
static void DesenharEfeitosMenu(void);
static void DesenharTextoBrilhante(const char* texto, Vector2 posicao, float tamanho, Color cor_base, Color cor_brilho);

//Def principal
bool ExecutarMenuStory(StorySettings *configuracoes)
{
    if (efeitos_inicializados == false) {
        InicializarEfeitosMenu();
    }

    //Variáveis de Estado
    static int opcao_atual = 0; 
    
    static int numero_jogadores = 1;
    static bool extras_habilitados = true; 
    
    opcao_atual = 0; 
    
    //Total de 4 (Jogadores, Extras, iniciar, voltar)
    const int numero_opcoes = 4; 

    while (WindowShouldClose() == 0)
    {
        //Atualizar ações
        if (IsKeyPressed(KEY_DOWN)) {
            opcao_atual = (opcao_atual + 1) % numero_opcoes; 
        }
        
        if (IsKeyPressed(KEY_UP)) {
            opcao_atual = (opcao_atual - 1 + numero_opcoes) % numero_opcoes;
        }

        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
            
            switch (opcao_atual)
            {
                case 0: //Jogadores
                {
                    //Alternando entre 1 e 2 jogadores
                    if (numero_jogadores == 1) {
                        numero_jogadores = 2;
                    } else {
                        numero_jogadores = 1;
                    }
                    break;
                }
                case 1: //Extras
                {
                    //Alternando o estado dos extras
                    if (extras_habilitados == true) {
                        extras_habilitados = false;
                    } else {
                        extras_habilitados = true;
                    } 
                    break;
                }
            }
        }
        
        //Confirmação da ação
        if (IsKeyReleased(KEY_ENTER)) { 
            if (opcao_atual == 2) { //iniciar
                configuracoes->numero_jogadores = numero_jogadores;
                configuracoes->extras_habilitados = extras_habilitados;
                return true; 
            }
            if (opcao_atual == 3) { //voltar
                return false; 
            }
        }
        
        if (IsKeyPressed(KEY_ESCAPE)) {
            return false; 
        }
        
        AtualizarEfeitosMenu();

        //Desenhar
        BeginDrawing();
        {
            int largura_tela = GetScreenWidth();
            int altura_tela = GetScreenHeight();
            ClearBackground(COLOR_UI_BACKGROUND); 
            DesenharEfeitosMenu();
            
            float tamanho_titulo = 70;
            const char* texto_titulo = "MODO HISTÓRIA";
            float posicao_titulo_x = (largura_tela - MeasureText(texto_titulo, tamanho_titulo)) / 2;
            DesenharTextoBrilhante(texto_titulo, (Vector2){ posicao_titulo_x, altura_tela * 0.1f }, tamanho_titulo, RAYWHITE, COLOR_BLUE_HIGHLIGHT);
                         
            float tamanho_fonte_opcao = 40;
            float espacamento_vertical = 65;
            float menu_posicao_y_inicial = altura_tela * 0.35f;
            float coluna_rotulo_x = largura_tela * 0.3f; 
            float coluna_valor_x = largura_tela * 0.55f; 

            char pText[4]; sprintf(pText, "%d", numero_jogadores);
            const char* eText = (extras_habilitados) ? "Ligado" : "Desligado";
            
            const char* rotulos[] = {"Jogadores:", "Extras:"}; 
            const char* valores[] = {pText, eText}; 
            
            //Opção
            for (int indice = 0; indice < 2; indice++) 
            {
                Color cor_base;
                Color cor_brilho;

                if (indice == opcao_atual) {
                    cor_base = COLOR_YELLOW_HIGHLIGHT;
                } else {
                    cor_base = COLOR_GRAY_OPTION;
                }
                if (indice == opcao_atual) {
                    cor_brilho = (Color){200, 160, 0, 150};
                } else {
                    cor_brilho = (Color){50, 50, 50, 100};
                }
                
                DesenharTextoBrilhante(rotulos[indice], (Vector2){ coluna_rotulo_x, menu_posicao_y_inicial + (indice * espacamento_vertical) }, tamanho_fonte_opcao, cor_base, cor_brilho);
                             
                DesenharTextoBrilhante("<", (Vector2){ coluna_valor_x - 30, menu_posicao_y_inicial + (indice * espacamento_vertical) }, tamanho_fonte_opcao, cor_base, cor_brilho);
                
                DesenharTextoBrilhante(valores[indice], (Vector2){ coluna_valor_x, menu_posicao_y_inicial + (indice * espacamento_vertical) }, tamanho_fonte_opcao, cor_base, cor_brilho);
                
                DesenharTextoBrilhante(">", (Vector2){ coluna_valor_x + MeasureText(valores[indice], tamanho_fonte_opcao) + 30, menu_posicao_y_inicial + (indice * espacamento_vertical) }, tamanho_fonte_opcao, cor_base, cor_brilho);
            }
            
            //Iniciar (opção 2)
            float iniciar_y = menu_posicao_y_inicial + (2 * espacamento_vertical) + 20; 
            float iniciar_x = (largura_tela - MeasureText("INICIAR JOGO", tamanho_fonte_opcao)) / 2;
            
            if (opcao_atual == 2) {
                Color cor_base;
                Color cor_brilho;

                bool deve_piscar = fmod(GetTime(), 0.2) > 0.1;
                if (deve_piscar == true) {
                    cor_base = COLOR_YELLOW_HIGHLIGHT;
                } else {
                    cor_base = WHITE;
                }
                if (deve_piscar == true) {
                    cor_brilho = (Color){200, 160, 0, 150};
                } else {
                    cor_brilho = COLOR_BLUE_HIGHLIGHT;
                }
                DesenharTextoBrilhante("INICIAR JOGO", (Vector2){ iniciar_x, iniciar_y }, tamanho_fonte_opcao, cor_base, cor_brilho);
            } else {
                DesenharTextoBrilhante("INICIAR JOGO", (Vector2){ iniciar_x, iniciar_y }, tamanho_fonte_opcao, COLOR_GRAY_OPTION, (Color){50,50,50,100});
            }
            
            //Voltar (opção 3)
            float voltar_y = iniciar_y + espacamento_vertical; 
            float voltar_x = (largura_tela - MeasureText("VOLTAR", tamanho_fonte_opcao)) / 2;
            
            if (opcao_atual == 3) {
                Color cor_base = COLOR_YELLOW_HIGHLIGHT;
                Color cor_brilho = (Color){200, 160, 0, 150};
                DesenharTextoBrilhante("VOLTAR", (Vector2){ voltar_x, voltar_y }, tamanho_fonte_opcao, cor_base, cor_brilho);
            } else {
                DesenharTextoBrilhante("VOLTAR", (Vector2){ voltar_x, voltar_y }, tamanho_fonte_opcao, COLOR_GRAY_OPTION, (Color){50,50,50,100});
            }
            
            const char* texto_dica = "Use CIMA/BAIXO para navegar, ESQUERDA/DIREITA para alterar, ENTER para selecionar";
            int largura_texto_dica = MeasureText(texto_dica, 20);
            DrawText(texto_dica, (largura_tela - largura_texto_dica) / 2, altura_tela - 40, 20, RAYWHITE);
        }
        EndDrawing();
    }
    
    return false; 
}

//Funções de Efeito
static void InicializarEfeitosMenu(void) {
    int largura_tela = GetScreenWidth();
    int altura_tela = GetScreenHeight();
    
    for (int i = 0; i < MAX_GOTAS_CHUVA; i++) {
        gotas_chuva[i].posicao.x = (float)GetRandomValue(0, largura_tela);
        gotas_chuva[i].posicao.y = (float)GetRandomValue(-altura_tela, 0); 
        gotas_chuva[i].velocidade = (float)GetRandomValue(200, 600); 
    }
    
    for (int i = 0; i < MAX_PULSOS_CIRCULARES; i++) { 
        pulsos_circulares[i].alpha = 0.0f; 
    }
    efeitos_inicializados = true;
}

static void AtualizarEfeitosMenu(void) {
    int largura_tela = GetScreenWidth();
    int altura_tela = GetScreenHeight();
    float tempo_quadro = GetFrameTime(); 
    
    //Gotas de Chuva
    for (int i = 0; i < MAX_GOTAS_CHUVA; i++) {
        gotas_chuva[i].posicao.y += gotas_chuva[i].velocidade * tempo_quadro; 
        
        if (gotas_chuva[i].posicao.y > altura_tela) {
            gotas_chuva[i].posicao.x = (float)GetRandomValue(0, largura_tela);
            gotas_chuva[i].posicao.y = (float)GetRandomValue(-40, -20); 
            gotas_chuva[i].velocidade = (float)GetRandomValue(200, 600);
        }
    }
    
    //Pulsos Circulares
    for (int i = 0; i < MAX_PULSOS_CIRCULARES; i++) {
        if (pulsos_circulares[i].alpha > 0.0f) {
            pulsos_circulares[i].raio += 150.0f * tempo_quadro; 
            pulsos_circulares[i].alpha -= 0.7f * tempo_quadro;  
            
            if (pulsos_circulares[i].alpha < 0.0f) {
                pulsos_circulares[i].alpha = 0.0f; 
            }
        } else {
            if (GetRandomValue(0, 1000) > 995) {
                pulsos_circulares[i].centro.x = (float)GetRandomValue(0, largura_tela);
                pulsos_circulares[i].centro.y = (float)GetRandomValue(0, altura_tela);
                pulsos_circulares[i].raio = 0.0f;
                pulsos_circulares[i].alpha = 1.0f; 
            }
        }
    }
}

static void DesenharEfeitosMenu(void) {
    for (int i = 0; i < MAX_GOTAS_CHUVA; i++) {
        DrawRectangle(gotas_chuva[i].posicao.x, gotas_chuva[i].posicao.y, 2, 12, Fade(COLOR_BLUE_HIGHLIGHT, 0.3f));
    }
    
    for (int i = 0; i < MAX_PULSOS_CIRCULARES; i++) {
        if (pulsos_circulares[i].alpha > 0.0f) {
            DrawCircleLines(pulsos_circulares[i].centro.x, pulsos_circulares[i].centro.y, pulsos_circulares[i].raio, Fade(COLOR_YELLOW_HIGHLIGHT, pulsos_circulares[i].alpha));
        }
    }
}

static void DesenharTextoBrilhante(const char* texto, Vector2 posicao, float tamanho, Color cor_base, Color cor_brilho) {
    const int deslocamento_brilho = 2;
    DrawText(texto, posicao.x - deslocamento_brilho, posicao.y, tamanho, cor_brilho);
    DrawText(texto, posicao.x + deslocamento_brilho, posicao.y, tamanho, cor_brilho);
    DrawText(texto, posicao.x, posicao.y - deslocamento_brilho, tamanho, cor_brilho);
    DrawText(texto, posicao.x, posicao.y + deslocamento_brilho, tamanho, cor_brilho);
    DrawText(texto, posicao.x, posicao.y, tamanho, cor_base);
}