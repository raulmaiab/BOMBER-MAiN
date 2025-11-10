#include "raylib.h"
#include "inicio.h" // Inclui o header da tela de início
#include <stdbool.h> // Necessário para o 'bool'

// --- ATENCAO AQUI ---
// Ajuste estes valores para que sejam EXATAMENTE
// a largura e altura da sua imagem "inicio.png"
// (Exemplo: se a imagem for 1280x720)
const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;
// (Se sua imagem for 800x600, use 800 e 600)

int main(void)
{
    // 1. Inicialização
    // A janela agora será criada com o tamanho exato da sua imagem.
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BomberMain");

    ToggleFullscreen();

    SetTargetFPS(60);

    bool deveContinuar = ExecutarTelaInicio();

    // 3. Se o usuário não fechou a janela, prossiga
    if (deveContinuar)
    {
        // Loop principal do jogo (placeholder)
        while (!WindowShouldClose())
        {
            // --- Lógica do Jogo (placeholder) ---
            
            // --- Desenho (placeholder) ---
            BeginDrawing();
            ClearBackground(DARKGRAY);
            DrawText("JOGO PRINCIPAL - (Pressione ESC para sair)", 190, 200, 20, LIGHTGRAY);
            EndDrawing();
        }
    }

    // 4. Finalização
    CloseWindow(); // Fecha a janela e o contexto OpenGL

    return 0;
}