// menu_prox.c

#include "menu_prox.h"
#include "raylib.h"
#include <stdio.h>

// Definições de cores baseadas no tema do menu (ajustadas para Raylib)
#define COLOR_BUTTON_NORMAL CLITERAL(Color){ 150, 150, 150, 255 }  // Cinza claro para o texto normal
#define COLOR_BUTTON_HOVER CLITERAL(Color){ 255, 255, 255, 255 }  // Branco para o texto em hover
#define COLOR_SUCCESS GREEN
#define COLOR_TITLE RAYWHITE 
#define COLOR_BACKGROUND CLITERAL(Color){ 20, 20, 20, 255 } // Quase preto

// --- Lógica do Menu de Próximo Nível ---
OptionsAction ExecutarMenuProximoNivel(NivelInfo info)
{
    OptionsAction acao = OPTIONS_ACAO_NENHUMA;
    
    // Posição central do botão START
    int btnX = GetScreenWidth() / 2;
    int btnY = (GetScreenHeight() / 2) + 150; 
    
    // Texto do botão
    const char* btnText = "START NEXT LEVEL (ENTER)";
    int btnFontSize = 40; // Aumentei um pouco a fonte
    int btnWidth = MeasureText(btnText, btnFontSize);
    
    // Área de colisão do botão (usada para hover e clique)
    Rectangle btnBounds = { 
        btnX - btnWidth / 2, 
        btnY - btnFontSize / 2, 
        btnWidth, 
        btnFontSize 
    };
    
    // Textos informativos
    char nivelConcluido[64];
    sprintf(nivelConcluido, "LEVEL %d: COMPLETED", info.nivelAtual + 1); 
    
    char proximaFase[64];
    sprintf(proximaFase, "NEXT STAGE: %s", info.proximoMapa);
    
    while (!WindowShouldClose())
    {
        Vector2 mousePoint = GetMousePosition();
        bool mouseOverButton = CheckCollisionPointRec(mousePoint, btnBounds);

        // Verifica a entrada do usuário
        if (IsKeyPressed(KEY_ESCAPE)) {
            acao = OPTIONS_ACAO_MAIN_MENU;
            break;
        }

        // --- DETECÇÃO DE BOTÃO (MOUSE OU ENTER) ---
        if (IsKeyPressed(KEY_ENTER) || (mouseOverButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
        {
            acao = OPTIONS_ACAO_RESTART; 
            break;
        }
        // ------------------------------------------

        BeginDrawing();
            ClearBackground(COLOR_BACKGROUND);
            
            // Título de Sucesso
            DrawText("YOU WON", 
                     (GetScreenWidth() / 2) - MeasureText("YOU WON", 80) / 2, 
                     100, 
                     80, 
                     COLOR_SUCCESS);

            // Nível Concluído
            DrawText(nivelConcluido, 
                     (GetScreenWidth() / 2) - MeasureText(nivelConcluido, 35) / 2, 
                     280, 
                     35, 
                     COLOR_TITLE);

            // Próxima Fase
            DrawText(proximaFase, 
                     (GetScreenWidth() / 2) - MeasureText(proximaFase, 50) / 2, 
                     350, 
                     50, 
                     COLOR_TITLE);

            // Desenho do Botão START (agora apenas texto com mudança de cor)
            DrawText(btnText,
                     (btnX - btnWidth / 2),
                     (btnY - btnFontSize / 2),
                     btnFontSize,
                     mouseOverButton ? COLOR_BUTTON_HOVER : COLOR_BUTTON_NORMAL);

        EndDrawing();
    }
    
    return acao;
}