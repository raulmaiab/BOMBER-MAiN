#include "raylib.h"
#include "inicio.h" 
#include <stdbool.h> 

bool ExecutarTelaInicio(void)
{
    // 1. Carregar recursos
    Texture2D capa = LoadTexture("inicio.png"); 
    
    if (capa.id == 0) {
        TraceLog(LOG_WARNING, "Não foi possível carregar a textura: inicio.png");
        return true; 
    }

    // Loop da tela de início
    while (!WindowShouldClose()) 
    {
        // 2. Atualizar lógica (input)
        if (IsKeyPressed(KEY_ENTER))
        {
            break; // Sai do loop da tela de início
        }

        // (Bônus: Permite ao usuário alternar a tela cheia)
        if (IsKeyPressed(KEY_F11))
        {
            ToggleFullscreen();
        }

        // --- CORREÇÃO IMPORTANTE PARA TELA CHEIA ---
        
        // Pega as dimensões ATUAIS da tela (que podem ser 1920x1080, etc.)
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        // Define a "fonte" (nossa imagem inteira)
        Rectangle sourceRec = { 0.0f, 0.0f, (float)capa.width, (float)capa.height };
        
        // Define o "destino" (a tela inteira)
        Rectangle destRec = { 0.0f, 0.0f, (float)screenWidth, (float)screenHeight };
        
        // Define a origem (para rotação/escala, 0,0 é o canto)
        Vector2 origin = { 0.0f, 0.0f };

        // --- FIM DA CORREÇÃO ---

        // 3. Desenhar
        BeginDrawing();

            ClearBackground(BLACK); 
            
            // Usa DrawTexturePro para "esticar" a imagem de 'sourceRec'
            // para caber exatamente em 'destRec' (a tela inteira).
            DrawTexturePro(capa, sourceRec, destRec, origin, 0.0f, WHITE);
            
            // Texto "Pressione Enter"
            const char* texto = "Pressione ENTER para comecar";
            int textoLargura = MeasureText(texto, 20);
            DrawText(texto, (screenWidth - textoLargura) / 2, screenHeight - 40, 20, RAYWHITE);

        EndDrawing();
    }

    // 4. Descarregar recursos
    UnloadTexture(capa);

    return !WindowShouldClose();
}