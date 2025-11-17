#include "raylib.h"
#include "menu.h"
#include <stdbool.h>
#include <string.h> 
#include <math.h> 

// --- Variáveis Globais (Apenas para Efeitos) ---
static RainDrop rain[MAX_RAIN_DROPS];
static Pulse pulses[MAX_PULSES];
static bool effectsInitialized = false;

// --- Forward Declarations ---
static void InitMenuEffects(void);
static void UpdateMenuEffects(void);
static void DrawMenuEffects(void);


// --- Função de Desenho de Texto (Glow) ---
static void DrawTextGlow(const char* text, Vector2 pos, float size, Color base, Color glow) {
    const int o = 2; // Offset do glow
    DrawText(text, pos.x - o, pos.y, size, glow);
    DrawText(text, pos.x + o, pos.y, size, glow);
    DrawText(text, pos.x, pos.y - o, size, glow);
    DrawText(text, pos.x, pos.y + o, size, glow);
    DrawText(text, pos.x, pos.y, size, base);
}

OpcaoMenu ExecutarTelaMenu(void)
{
    if (!effectsInitialized) {
        InitMenuEffects();
    }

    MenuItem opcoes[] = {
        {"BATTLE", ESCOLHA_BATTLE},
        {"STORY", ESCOLHA_STORY},
        {"OTHER", ESCOLHA_OTHER},
        {"SAIR", ESCOLHA_SAIR} 
    };
    int totalOpcoes = sizeof(opcoes) / sizeof(opcoes[0]);
    int indiceOpcaoAtual = 0; 

    while (!WindowShouldClose())
    {
        // 1. Atualizar (Lógica / Input)
        if (IsKeyPressed(KEY_DOWN)) {
            indiceOpcaoAtual = (indiceOpcaoAtual + 1) % totalOpcoes; 
        }
        if (IsKeyPressed(KEY_UP)) {
            indiceOpcaoAtual = (indiceOpcaoAtual - 1 + totalOpcoes) % totalOpcoes;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            return opcoes[indiceOpcaoAtual].idOpcao; 
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            return ESCOLHA_SAIR;
        }
        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }

        UpdateMenuEffects();

        // 2. Desenhar
        BeginDrawing();
        
            int sw = GetScreenWidth();
            int sh = GetScreenHeight();

            ClearBackground(COLOR_UI_BACKGROUND); 
            DrawMenuEffects();
            
            // Título "MAIN MENU" com glow
            float titleSize = 80;
            float titleX = (sw - MeasureText("MAIN MENU", titleSize)) / 2;
            DrawTextGlow("MAIN MENU", (Vector2){ titleX, sh * 0.15f }, titleSize, 
                         RAYWHITE, COLOR_BLUE_HIGHLIGHT);

            // Opções do Menu (Centralizadas)
            float fontSizeOpcao = 50;
            float espacamento = 70;
            float menuY_inicial = sh * 0.4f;

            for (int i = 0; i < totalOpcoes; i++)
            {
                const char* texto = opcoes[i].texto;
                float posX = (sw - MeasureText(texto, fontSizeOpcao)) / 2;
                float posY = menuY_inicial + (i * espacamento);
                
                if (i == indiceOpcaoAtual)
                {
                    bool piscar = fmod(GetTime(), 0.2) > 0.1;
                    Color corBase = piscar ? COLOR_YELLOW_HIGHLIGHT : WHITE;
                    Color corGlow = piscar ? (Color){200, 160, 0, 150} : COLOR_BLUE_HIGHLIGHT;
                    
                    DrawTextGlow(texto, (Vector2){ posX, posY }, fontSizeOpcao, corBase, corGlow);
                    
                    float setaSize = 20;
                    DrawText(">", posX - setaSize*2.5f, posY + (fontSizeOpcao - setaSize)/2.0f, setaSize, corBase);
                    DrawText("<", posX + MeasureText(texto, fontSizeOpcao) + setaSize*1.5f, posY + (fontSizeOpcao - setaSize)/2.0f, setaSize, corBase);
                }
                else
                {
                    DrawTextGlow(texto, (Vector2){ posX, posY }, fontSizeOpcao, 
                                 COLOR_GRAY_OPTION, (Color){50,50,50,100});
                }
            }
            
            // Texto de ajuda inferior (Descrição da opção)
            const char* helpText = "Use UP/DOWN e ENTER"; 
            if (opcoes[indiceOpcaoAtual].idOpcao == ESCOLHA_STORY) { helpText = "Play Story Mode"; }
            else if (opcoes[indiceOpcaoAtual].idOpcao == ESCOLHA_BATTLE) { helpText = "Start a Battle"; }
            else if (opcoes[indiceOpcaoAtual].idOpcao == ESCOLHA_OTHER) { helpText = "Game Settings"; }
            else if (opcoes[indiceOpcaoAtual].idOpcao == ESCOLHA_SAIR) { helpText = "Exit to Desktop"; }

            int helpTextFontSize = 20;
            int helpTextWidth = MeasureText(helpText, helpTextFontSize);
            DrawText(helpText, (sw - helpTextWidth) / 2, sh - 40, helpTextFontSize, RAYWHITE);

        EndDrawing();
    }
    
    return ESCOLHA_NENHUMA_OU_FECHOU;
}


// --- Implementação das Funções de Efeito ---

static void InitMenuEffects(void)
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    for (int i = 0; i < MAX_RAIN_DROPS; i++) {
        rain[i].pos.x = (float)GetRandomValue(0, screenWidth);
        rain[i].pos.y = (float)GetRandomValue(-screenHeight, 0); 
        rain[i].speed = (float)GetRandomValue(200, 600); 
    }
    
    for (int i = 0; i < MAX_PULSES; i++) {
        pulses[i].alpha = 0.0f; 
    }
    effectsInitialized = true;
}

static void UpdateMenuEffects(void)
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    float frameTime = GetFrameTime(); 

    // --- CORREÇÃO AQUI ---
    for (int i = 0; i < MAX_RAIN_DROPS; i++) {
    // --- FIM DA CORREÇÃO ---
        rain[i].pos.y += rain[i].speed * frameTime; 
        if (rain[i].pos.y > screenHeight) {
            rain[i].pos.x = (float)GetRandomValue(0, screenWidth);
            rain[i].pos.y = (float)GetRandomValue(-40, -20); 
            rain[i].speed = (float)GetRandomValue(200, 600);
        }
    }

    for (int i = 0; i < MAX_PULSES; i++) {
        if (pulses[i].alpha > 0.0f) {
            pulses[i].radius += 150.0f * frameTime; 
            pulses[i].alpha -= 0.7f * frameTime;  
            if (pulses[i].alpha < 0.0f) pulses[i].alpha = 0.0f; 
        } else {
            if (GetRandomValue(0, 1000) > 995) {
                pulses[i].center.x = (float)GetRandomValue(0, screenWidth);
                pulses[i].center.y = (float)GetRandomValue(0, screenHeight);
                pulses[i].radius = 0.0f;
                pulses[i].alpha = 1.0f; 
            }
        }
    }
}

static void DrawMenuEffects(void)
{
    for (int i = 0; i < MAX_RAIN_DROPS; i++) {
        DrawRectangle(rain[i].pos.x, rain[i].pos.y, 2, 12, Fade(COLOR_BLUE_HIGHLIGHT, 0.3f));
    }
    for (int i = 0; i < MAX_PULSES; i++) {
        if (pulses[i].alpha > 0.0f) {
            DrawCircleLines(pulses[i].center.x, pulses[i].center.y, pulses[i].radius, Fade(COLOR_YELLOW_HIGHLIGHT, pulses[i].alpha));
        }
    }
}