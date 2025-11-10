#include "raylib.h"
#include "inicio.h" // Inclui o nosso próprio header
#include <stdbool.h> // Necessário para o 'bool'

// Implementação da função prometida no .h
bool ExecutarTelaInicio(void)
{
    // 1. Carregar recursos
    // 
    Texture2D capa = LoadTexture("inicio.png"); 
    
    // Verificação de segurança
    if (capa.id == 0) {
        TraceLog(LOG_WARNING, "Não foi possível carregar a textura: inicio.png");
        return true; // Pula para o próximo estado (menu)
    }

    // Pega as dimensões da tela (que agora são as mesmas da imagem)
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Loop da tela de início
    while (!WindowShouldClose()) // Continua rodando enquanto o usuário NÃO fechar a janela
    {
        // 2. Atualizar lógica (input)
        if (IsKeyPressed(KEY_ENTER))
        {
            break; // Sai do loop da tela de início e continua para o menu/jogo
        }

        // 3. Desenhar
        BeginDrawing();

            ClearBackground(BLACK); // Limpa o fundo para preto
            
            // CORREÇÃO:
            // Desenha a textura na posição (0, 0).
            // Como a janela tem o mesmo tamanho da imagem,
            // ela vai se encaixar perfeitamente.
            DrawTexture(capa, 0, 0, WHITE);
            
            // Opcional: Adicionar um texto "Pressione Enter"
            const char* texto = "Pressione ENTER para comecar";
            int textoLargura = MeasureText(texto, 20);
            DrawText(texto, (screenWidth - textoLargura) / 2, screenHeight - 40, 20, RAYWHITE);

        EndDrawing();
    }

    // 4. Descarregar recursos
    UnloadTexture(capa);

    // Retorna 'false' se o loop parou porque o usuário fechou a janela
    // Retorna 'true' se o loop parou por outro motivo (Enter)
    return !WindowShouldClose();
}