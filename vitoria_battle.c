#include "vitoria_battle.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

const int FONT_SIZE = 70;

// --- FUNÇÃO AUXILIAR PARA MAPEAR O NOME DO SPRITE PARA A COR ---
static const char* ObterNomeCor(const char* spriteName) {
    if (strcmp(spriteName, "SpriteBranco") == 0) {
        return "BRANCO";
    } else if (strcmp(spriteName, "SpriteVermelho") == 0) {
        return "VERMELHO";
    } else if (strcmp(spriteName, "SpriteAzul") == 0) {
        return "AZUL";
    } else if (strcmp(spriteName, "SpritePreto") == 0) {
        return "PRETO";
    }
    // Caso não encontre, retorna o nome original (em maiúsculas para manter o padrão)
    return TextToUpper(spriteName); 
}
// ------------------------------------------------------------------


void ExecutarTelaVitoriaBattle(const char* winnerSpriteName)
{
    // Obtém o nome da cor em português
    const char* winnerColor = ObterNomeCor(winnerSpriteName);

    char dynamicTitle[256];
    
    // Altera a linha para usar o nome da cor em português
    sprintf(dynamicTitle, "%s VENCEU!", winnerColor); 
    
    // Calcula o centro da tela para o texto
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    Vector2 titleSize = MeasureTextEx(GetFontDefault(), dynamicTitle, FONT_SIZE, 2);
    int xPos = (screenWidth - titleSize.x) / 2;
    int yPos = screenHeight / 2 - FONT_SIZE;
    
    // Define a cor de exibição (opcionalmente, para dar um toque especial)
    Color displayColor = WHITE;
    if (strcmp(winnerColor, "BRANCO") == 0) displayColor = LIGHTGRAY;
    else if (strcmp(winnerColor, "VERMELHO") == 0) displayColor = RED;
    else if (strcmp(winnerColor, "AZUL") == 0) displayColor = SKYBLUE;
    else if (strcmp(winnerColor, "PRETO") == 0) displayColor = DARKGRAY;


    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ESCAPE)) break;

        BeginDrawing();
            ClearBackground(BLACK);
            
            // Desenha o título dinâmico (ex: BRANCO VENCEU!)
            DrawTextEx(GetFontDefault(), dynamicTitle, (Vector2){xPos, yPos}, FONT_SIZE, 2, displayColor);
            
            // Desenha a instrução de retorno
            DrawText("Pressione ESPACO para voltar ao Menu Principal...", 
                     (screenWidth - MeasureText("Pressione ESPACO para voltar ao Menu Principal...", 20)) / 2, 
                     screenHeight - 50, 20, RAYWHITE);
        EndDrawing();
    }
}