// options.c
#include "options.h"
#include "menu.h" // Para as constantes de cor
#include <math.h> 
#include <string.h>

// Definições de Cores (Assumindo que estão definidas em menu.h)
// #define COLOR_UI_BACKGROUND (Color){ 30, 30, 50, 255 } 
// #define COLOR_BLUE_HIGHLIGHT (Color){ 0, 150, 255, 255 }
// #define COLOR_YELLOW_HIGHLIGHT (Color){ 255, 200, 0, 255 }
// #define COLOR_GRAY_OPTION (Color){ 180, 180, 180, 255 }

// --- Função de Desenho de Texto (Brilho) - Traduzida ---
static void DesenharTextoBrilho(const char* texto, Vector2 pos, float tamanho, Color base, Color brilho) {
    const int o = 2; // Offset do brilho
    DrawText(texto, (int)(pos.x - o), (int)pos.y, (int)tamanho, brilho);
    DrawText(texto, (int)(pos.x + o), (int)pos.y, (int)tamanho, brilho);
    DrawText(texto, (int)pos.x, (int)(pos.y - o), (int)tamanho, brilho);
    DrawText(texto, (int)pos.x, (int)(pos.y + o), (int)tamanho, brilho);
    DrawText(texto, (int)pos.x, (int)pos.y, (int)tamanho, base);
}

// --- Estrutura e Dados do Menu Opções ---
typedef struct {
    const char* texto;
    AcaoOpcoes idOpcao;
} ItemOpcoes;

AcaoOpcoes ExecutarMenuOpcoes(void)
{
    ItemOpcoes opcoes[] = {
        {"RETOMAR", OPCOES_ACAO_RESUMO},
        {"REINICIAR", OPCOES_ACAO_REINICIAR},
        {"EDITAR JOGO", OPCOES_ACAO_EDITAR_JOGO}, // Adicionada opção que existe no .h
        {"MENU PRINCIPAL", OPCOES_ACAO_MENU_PRINCIPAL},
    };
    int totalOpcoes = sizeof(opcoes) / sizeof(opcoes[0]);
    int indiceOpcaoAtual = 0; 
    
    // --- Configurações Visuais do Popup ---
    const int larguraTela = GetScreenWidth();
    const int alturaTela = GetScreenHeight();
    const int larguraPopup = 600;
    const int alturaPopup = 450;
    const int xPopup = (larguraTela - larguraPopup) / 2;
    const int yPopup = (alturaTela - alturaPopup) / 2;
    const float tamanhoTitulo = 50;
    const float tamanhoFonteOpcao = 35;
    const float espacamento = 50;
    
    // CORREÇÃO: !WindowShouldClose() -> WindowShouldClose() == 0
    while (WindowShouldClose() == 0)
    {
        // 1. Atualizar (Lógica / Input)
        if (IsKeyPressed(KEY_DOWN)) {
            indiceOpcaoAtual = (indiceOpcaoAtual + 1) % totalOpcoes; 
        }
        if (IsKeyPressed(KEY_UP)) {
            indiceOpcaoAtual = (indiceOpcaoAtual - 1 + totalOpcoes) % totalOpcoes;
        }

        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            return opcoes[indiceOpcaoAtual].idOpcao; 
        }
        
        // ESC age como um "Resume" rápido, retornando a ação de Resumo.
        if (IsKeyPressed(KEY_ESCAPE)) {
            return OPCOES_ACAO_RESUMO;
        }
        
        // 2. Desenhar
        BeginDrawing();
        {
            // 2a. Fundo: Desenha um overlay cinza escuro sobre a partida já desenhada.
            DrawRectangle(0, 0, larguraTela, alturaTela, Fade(BLACK, 0.7f)); 
            
            // 2b. Popup: Caixa de fundo
            // Assumindo COLOR_UI_BACKGROUND e COLOR_BLUE_HIGHLIGHT definidas em menu.h
            DrawRectangle(xPopup, yPopup, larguraPopup, alturaPopup, COLOR_UI_BACKGROUND);
            DrawRectangleLinesEx((Rectangle){(float)xPopup, (float)yPopup, (float)larguraPopup, (float)alturaPopup}, 5, COLOR_BLUE_HIGHLIGHT);
            
            // Título "OPÇÕES"
            const char* textoTitulo = "OPÇÕES";
            float xTitulo = xPopup + (larguraPopup - MeasureText(textoTitulo, tamanhoTitulo)) / 2;
            DesenharTextoBrilho(textoTitulo, (Vector2){ xTitulo, (float)yPopup + 40 }, tamanhoTitulo, 
                                 RAYWHITE, COLOR_BLUE_HIGHLIGHT);

            // Opções do Menu
            float yMenu_inicial = (float)yPopup + 130;

            for (int i = 0; i < totalOpcoes; i++)
            {
                const char* texto = opcoes[i].texto;
                float xPos = xPopup + (larguraPopup - MeasureText(texto, tamanhoFonteOpcao)) / 2;
                float yPos = yMenu_inicial + (i * espacamento);
                
                if (i == indiceOpcaoAtual)
                {
                    bool piscar;
                    // CORREÇÃO: fmod(GetTime(), 0.2) > 0.1 ? true : false
                    if (fmod(GetTime(), 0.2) > 0.1) {
                        piscar = true;
                    } else {
                        piscar = false;
                    }
                    
                    // Assumindo COLOR_YELLOW_HIGHLIGHT e COLOR_BLUE_HIGHLIGHT definidas
                    Color corBase;
                    Color corGlow;

                    if (piscar) {
                        corBase = COLOR_YELLOW_HIGHLIGHT;
                        corGlow = (Color){200, 160, 0, 150};
                    } else {
                        corBase = WHITE;
                        corGlow = COLOR_BLUE_HIGHLIGHT;
                    }

                    
                    DesenharTextoBrilho(texto, (Vector2){ xPos, yPos }, tamanhoFonteOpcao, corBase, corGlow);
                    
                    // Desenha setas ao lado do item selecionado
                    float tamanhoSeta = 15;
                    DrawText(">", (int)(xPos - tamanhoSeta*2.0f), (int)(yPos + (tamanhoFonteOpcao - tamanhoSeta)/2.0f + 5), (int)tamanhoSeta, corBase);
                    DrawText("<", (int)(xPos + MeasureText(texto, tamanhoFonteOpcao) + tamanhoSeta), (int)(yPos + (tamanhoFonteOpcao - tamanhoSeta)/2.0f + 5), (int)tamanhoSeta, corBase);
                }
                else
                {
                    // Assumindo COLOR_GRAY_OPTION definida
                    DesenharTextoBrilho(texto, (Vector2){ xPos, yPos }, tamanhoFonteOpcao, 
                                 COLOR_GRAY_OPTION, (Color){50,50,50,100});
                }
            }
            
            // Ajuda inferior
            DrawText("Pressione ESC ou RETOMAR para continuar o jogo", 
                     xPopup + 20, yPopup + alturaPopup - 30, 18, RAYWHITE);
        }
        EndDrawing();
    }
    
    return OPCOES_ACAO_NENHUMA;
}