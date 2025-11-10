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

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    int posX = (screenWidth - capa.width) / 2;
    int posY = (screenHeight - capa.height) / 2;


    while (!WindowShouldClose()) 
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            break; 
        }

        BeginDrawing();

            ClearBackground(BLACK); 
            
            DrawTexture(capa, posX, posY, WHITE);
            
            const char* texto = "Pressione ENTER para comecar";
            int textoLargura = MeasureText(texto, 20);
            DrawText(texto, (screenWidth - textoLargura) / 2, screenHeight - 40, 20, RAYWHITE);

        EndDrawing();
    }

    UnloadTexture(capa);

    return !WindowShouldClose();
}