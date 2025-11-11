#include "raylib.h"
#include "inicio.h" 
#include <stdbool.h> 

bool ExecutarTelaInicio(void)
{
    Texture2D capa = LoadTexture("inicio.png"); 
    
    if (capa.id == 0) {
        TraceLog(LOG_WARNING, "Não foi possível carregar a textura: inicio.png");
        return true; 
    }

    while (!WindowShouldClose()) 
    {
        // --- CORREÇÃO DE INPUT ---
        // Agora esperamos o usuário SOLTAR a tecla
        if (IsKeyReleased(KEY_ENTER))
        {
            break; // Sai do loop da tela de início
        }
        // --- FIM DA CORREÇÃO ---

        if (IsKeyPressed(KEY_F11))
        {
            ToggleFullscreen();
        }

        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        Rectangle sourceRec = { 0.0f, 0.0f, (float)capa.width, (float)capa.height };
        Rectangle destRec = { 0.0f, 0.0f, (float)screenWidth, (float)screenHeight };
        Vector2 origin = { 0.0f, 0.0f };

        BeginDrawing();

            ClearBackground(BLACK); 
            DrawTexturePro(capa, sourceRec, destRec, origin, 0.0f, WHITE);
            
            // Texto "Pressione Enter" REMOVIDO, pois já está na imagem

        EndDrawing();
    }

    UnloadTexture(capa);

    return !WindowShouldClose();
}