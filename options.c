// options.c
#include "options.h"
#include "menu.h" // Para as constantes de cor
#include <math.h> 
#include <string.h>

// --- Função de Desenho de Texto (Glow) - Copiada dos menus principais ---
// Assumindo que você não refatorou para um menu_ui.c, repetimos aqui.
static void DrawTextGlow(const char* text, Vector2 pos, float size, Color base, Color glow) {
    const int o = 2; // Offset do glow
    DrawText(text, pos.x - o, pos.y, size, glow);
    DrawText(text, pos.x + o, pos.y, size, glow);
    DrawText(text, pos.x, pos.y - o, size, glow);
    DrawText(text, pos.x, pos.y + o, size, glow);
    DrawText(text, pos.x, pos.y, size, base);
}

// --- Estrutura e Dados do Menu Options ---
typedef struct {
    const char* texto;
    OptionsAction idOpcao;
} OptionsItem;

OptionsAction ExecutarMenuOptions(void)
{
    OptionsItem opcoes[] = {
        {"RESUME", OPTIONS_ACAO_RESUME},
        {"RESTART", OPTIONS_ACAO_RESTART},
        {"MAIN MENU", OPTIONS_ACAO_MAIN_MENU},
    };
    int totalOpcoes = sizeof(opcoes) / sizeof(opcoes[0]);
    int indiceOpcaoAtual = 0; 
    
    // --- Configurações Visuais do Popup ---
    const int sw = GetScreenWidth();
    const int sh = GetScreenHeight();
    const int popupWidth = 600;
    const int popupHeight = 450;
    const int popupX = (sw - popupWidth) / 2;
    const int popupY = (sh - popupHeight) / 2;
    const float titleSize = 50;
    const float fontSizeOpcao = 35;
    const float espacamento = 50;

    // A partida já deve estar renderizada (desenhada) antes de chamar esta função, 
    // mas o loop de desenho precisa ser reaberto.
    // Lembrete: O BeginDrawing/EndDrawing precisa englobar toda a lógica do menu.
    
    while (!WindowShouldClose())
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
        
        // ESC age como um "Resume" rápido, mas só se RESUME for o item selecionado
        // ou se for a primeira opção (para conveniência).
        if (IsKeyPressed(KEY_ESCAPE)) {
            return OPTIONS_ACAO_RESUME;
        }
        
        // 2. Desenhar
        BeginDrawing();
        
            // 2a. Fundo: Desenha um overlay cinza escuro sobre a partida já desenhada.
            // O mapa e jogadores permanecem visíveis, mas escurecidos/pausados.
            DrawRectangle(0, 0, sw, sh, Fade(BLACK, 0.7f)); 
            
            // 2b. Popup: Caixa de fundo
            DrawRectangle(popupX, popupY, popupWidth, popupHeight, COLOR_UI_BACKGROUND);
            DrawRectangleLinesEx((Rectangle){popupX, popupY, popupWidth, popupHeight}, 5, COLOR_BLUE_HIGHLIGHT);
            
            // Título "OPTIONS"
            const char* titleText = "OPTIONS";
            float titleX = popupX + (popupWidth - MeasureText(titleText, titleSize)) / 2;
            DrawTextGlow(titleText, (Vector2){ titleX, popupY + 40 }, titleSize, 
                         RAYWHITE, COLOR_BLUE_HIGHLIGHT);

            // Opções do Menu
            float menuY_inicial = popupY + 130;

            for (int i = 0; i < totalOpcoes; i++)
            {
                const char* texto = opcoes[i].texto;
                float posX = popupX + (popupWidth - MeasureText(texto, fontSizeOpcao)) / 2;
                float posY = menuY_inicial + (i * espacamento);
                
                if (i == indiceOpcaoAtual)
                {
                    bool piscar = fmod(GetTime(), 0.2) > 0.1;
                    Color corBase = piscar ? COLOR_YELLOW_HIGHLIGHT : WHITE;
                    Color corGlow = piscar ? (Color){200, 160, 0, 150} : COLOR_BLUE_HIGHLIGHT;
                    
                    DrawTextGlow(texto, (Vector2){ posX, posY }, fontSizeOpcao, corBase, corGlow);
                    
                    // Desenha setas ao lado do item selecionado
                    float setaSize = 15;
                    DrawText(">", posX - setaSize*2.0f, posY + (fontSizeOpcao - setaSize)/2.0f + 5, setaSize, corBase);
                    DrawText("<", posX + MeasureText(texto, fontSizeOpcao) + setaSize, posY + (fontSizeOpcao - setaSize)/2.0f + 5, setaSize, corBase);
                }
                else
                {
                    DrawTextGlow(texto, (Vector2){ posX, posY }, fontSizeOpcao, 
                                 COLOR_GRAY_OPTION, (Color){50,50,50,100});
                }
            }
            
            // Ajuda inferior
            DrawText("Press ESC or RESUME to continue game", 
                     popupX + 20, popupY + popupHeight - 30, 18, RAYWHITE);

        EndDrawing();
    }
    
    return OPTIONS_ACAO_NENHUMA;
}