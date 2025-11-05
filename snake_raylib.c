#include "raylib.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

const int screenWidth = 800;
const int screenHeight = 450;
const int cellSize = 20;
const int gameWidth = screenWidth / cellSize;
const int gameHeight = screenHeight / cellSize;

const float updateTime = 0.1f; // Tempo em segundos para a cobra se mover (velocidade)

// --- Estruturas de Dados ---

// Representa um segmento da cobra
typedef struct SnakeSegment {
    Vector2 position; // Posição (x, y) no grid
} SnakeSegment;

// --- Variáveis Globais do Jogo ---
static SnakeSegment snake[100]; // Array para a cobra (máx 100 segmentos)
static int snakeLength = 0;
static Vector2 food = { 0, 0 }; // Posição da comida
static Vector2 direction = { 1, 0 }; // Direção inicial: direita (x=1, y=0)
static float timer = 0.0f; // Usado para controlar a velocidade da cobra
static bool gameOver = false;
static int score = 0;

// --- Funções do Jogo ---

// Inicializa a cobra, comida e variáveis do jogo
static void InitGame(void) {
    snakeLength = 1;
    snake[0].position = (Vector2){ gameWidth / 2, gameHeight / 2 }; // Cabeça no centro

    direction = (Vector2){ 1, 0 }; // Direção inicial para a direita
    gameOver = false;
    score = 0;
    timer = 0.0f;

    // Gera a primeira comida
    food.x = GetRandomValue(0, gameWidth - 1);
    food.y = GetRandomValue(0, gameHeight - 1);
}

// Atualiza a lógica do jogo (movimento, colisões, etc.)
static void UpdateGame(void) {
    if (gameOver) {
        // Se o jogo acabou, permite reiniciar
        if (IsKeyPressed(KEY_ENTER)) {
            InitGame();
        }
        return;
    }

    // --- Input (Teclado) ---
    if (IsKeyPressed(KEY_W) && direction.y != 1) { // Cima
        direction = (Vector2){ 0, -1 };
    } else if (IsKeyPressed(KEY_S) && direction.y != -1) { // Baixo
        direction = (Vector2){ 0, 1 };
    } else if (IsKeyPressed(KEY_A) && direction.x != 1) { // Esquerda
        direction = (Vector2){ -1, 0 };
    } else if (IsKeyPressed(KEY_D) && direction.x != -1) { // Direita
        direction = (Vector2){ 1, 0 };
    }

    // --- Movimento da Cobra (Baseado no tempo) ---
    timer += GetFrameTime(); // Acumula o tempo que passou desde o último frame

    if (timer >= updateTime) { // Se passou o tempo para a cobra se mover...
        timer = 0.0f; // Reseta o timer

        // 1. Move o corpo da cobra (cada segmento segue o anterior)
        for (int i = snakeLength - 1; i > 0; i--) {
            snake[i].position = snake[i - 1].position;
        }

        // 2. Move a cabeça da cobra na direção atual
        snake[0].position.x += direction.x;
        snake[0].position.y += direction.y;

        // --- Colisões ---
        
        // Colisão com paredes
        if (snake[0].position.x < 0 || snake[0].position.x >= gameWidth ||
            snake[0].position.y < 0 || snake[0].position.y >= gameHeight) {
            gameOver = true;
        }

        // Colisão com o próprio corpo
        for (int i = 1; i < snakeLength; i++) {
            if ((snake[0].position.x == snake[i].position.x) &&
                (snake[0].position.y == snake[i].position.y)) {
                gameOver = true;
            }
        }

        // Colisão com a comida
        if ((snake[0].position.x == food.x) && (snake[0].position.y == food.y)) {
            score++;
            snakeLength++; // A cobra cresce
            // Garante que não ultrapasse o tamanho máximo do array
            if (snakeLength >= 100) snakeLength = 100;

            // Gera nova comida (evitando que apareça em cima da cobra)
            bool foodOnSnake = false;
            do {
                food.x = GetRandomValue(0, gameWidth - 1);
                food.y = GetRandomValue(0, gameHeight - 1);
                foodOnSnake = false;
                for (int i = 0; i < snakeLength; i++) {
                    if ((food.x == snake[i].position.x) && (food.y == snake[i].position.y)) {
                        foodOnSnake = true;
                        break;
                    }
                }
            } while (foodOnSnake);
        }
    }
}

// Desenha todos os elementos do jogo
static void DrawGame(void) {
    BeginDrawing();

        ClearBackground(DARKGRAY); // Fundo do jogo

        // Desenha a cobra
        for (int i = 0; i < snakeLength; i++) {
            DrawRectangle(snake[i].position.x * cellSize, 
                          snake[i].position.y * cellSize, 
                          cellSize, cellSize, 
                          (i == 0) ? GREEN : LIME); // Cabeça verde, corpo lima
        }

        // Desenha a comida
        DrawRectangle(food.x * cellSize, food.y * cellSize, cellSize, cellSize, RED);

        // Desenha a pontuação
        DrawText(TextFormat("Pontuação: %i", score), 10, 10, 20, RAYWHITE);

        // Se GAME OVER
        if (gameOver) {
            DrawText("GAME OVER!", screenWidth/2 - MeasureText("GAME OVER!", 40)/2, screenHeight/2 - 40, 40, RAYWHITE);
            DrawText("Pressione ENTER para reiniciar", screenWidth/2 - MeasureText("Pressione ENTER para reiniciar", 20)/2, screenHeight/2 + 10, 20, RAYWHITE);
        }

    EndDrawing();
}

// --- Função Principal ---
int main(void) {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Inicializa a janela da Raylib
    InitWindow(screenWidth, screenHeight, "Snake Raylib!");
    SetTargetFPS(60); // Limita o FPS

    InitGame(); // Prepara o jogo pela primeira vez

    // Game Loop principal da Raylib
    while (!WindowShouldClose()) {
        UpdateGame(); // Atualiza a lógica do jogo
        DrawGame();   // Desenha os elementos na tela
    }

    // Finaliza a Raylib
    CloseWindow();

    return 0;
}