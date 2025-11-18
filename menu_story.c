#include "raylib.h"
#include "menu_story.h"
#include "menu.h" 
#include <stdbool.h>
#include <math.h> 
#include <stdio.h> 

// Note: MAX_RAIN_DROPS, MAX_PULSES, RainDrop, Pulse, e as cores
// são agora lidas de "menu.h" para evitar redefinição.

// --- Variáveis Globais (Efeitos) ---
static RainDrop rain[MAX_RAIN_DROPS];
static Pulse pulses[MAX_PULSES];
static bool effectsInitialized = false;

// --- Forward Declarations ---
static void InitMenuEffects(void);
static void UpdateMenuEffects(void);
static void DrawMenuEffects(void);
static void DrawTextGlow(const char* text, Vector2 pos, float size, Color base, Color glow);

// --- Função Principal ---
bool ExecutarMenuStory(StorySettings *settings)
{
    if (!effectsInitialized) {
        InitMenuEffects();
    }

    // Variáveis de Estado
    static int currentOption = 0; 
    
    static int numPlayers = 1;
    static bool extras = true; 
    
    currentOption = 0; 
    
    // Total de 4 (Players, Extras, START, BACK)
    const int numOptions = 4; 

    while (!WindowShouldClose())
    {
        // 1. ATUALIZAR (Input)
        if (IsKeyPressed(KEY_DOWN)) {
            currentOption = (currentOption + 1) % numOptions; 
        }
        if (IsKeyPressed(KEY_UP)) {
            currentOption = (currentOption - 1 + numOptions) % numOptions;
        }

        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
            
            switch (currentOption)
            {
                case 0: numPlayers = (numPlayers == 1) ? 2 : 1; break; // Players
                case 1: extras = !extras; break; // Extras
            }
        }
        
        // Confirmação
        if (IsKeyReleased(KEY_ENTER)) { 
            if (currentOption == 2) { // START
                settings->numPlayers = numPlayers;
                settings->extras = extras;
                // Dificuldade removida
                return true; 
            }
            if (currentOption == 3) { // BACK
                return false; // Volta ao menu principal
            }
        }
        
        if (IsKeyPressed(KEY_ESCAPE)) {
            return false; 
        }
        
        UpdateMenuEffects();

        // 2. DESENHAR
        BeginDrawing();
            int sw = GetScreenWidth();
            int sh = GetScreenHeight();
            ClearBackground(COLOR_UI_BACKGROUND); 
            DrawMenuEffects();
            
            float titleSize = 70;
            const char* titleText = "STORY MODE";
            float titleX = (sw - MeasureText(titleText, titleSize)) / 2;
            DrawTextGlow(titleText, (Vector2){ titleX, sh * 0.1f }, titleSize, 
                         RAYWHITE, COLOR_BLUE_HIGHLIGHT);
                         
            float fontSizeOpcao = 40;
            float espacamento = 65;
            float menuY_inicial = sh * 0.35f;
            float colLabelX = sw * 0.3f; 
            float colValorX = sw * 0.55f; 

            char pText[4]; sprintf(pText, "%d", numPlayers);
            const char* eText = (extras) ? "On" : "Off";
            
            const char* labels[] = {"Players:", "Extras:"}; 
            const char* values[] = {pText, eText}; 
            
            // Opções configuráveis (0, 1)
            for (int i = 0; i < 2; i++) 
            {
                Color base = (i == currentOption) ? COLOR_YELLOW_HIGHLIGHT : COLOR_GRAY_OPTION;
                Color glow = (i == currentOption) ? (Color){200, 160, 0, 150} : (Color){50,50,50,100};
                
                DrawTextGlow(labels[i], (Vector2){ colLabelX, menuY_inicial + (i * espacamento) }, 
                             fontSizeOpcao, base, glow);
                             
                DrawTextGlow("<", (Vector2){ colValorX - 30, menuY_inicial + (i * espacamento) }, 
                             fontSizeOpcao, base, glow);
                DrawTextGlow(values[i], (Vector2){ colValorX, menuY_inicial + (i * espacamento) }, 
                             fontSizeOpcao, base, glow);
                DrawTextGlow(">", (Vector2){ colValorX + MeasureText(values[i], fontSizeOpcao) + 30, menuY_inicial + (i * espacamento) }, 
                             fontSizeOpcao, base, glow);
            }
            
            // START (Índice 2)
            float startY = menuY_inicial + (2 * espacamento) + 20; 
            float startX = (sw - MeasureText("START GAME", fontSizeOpcao)) / 2;
            if (currentOption == 2) {
                bool piscar = fmod(GetTime(), 0.2) > 0.1;
                Color corBase = piscar ? COLOR_YELLOW_HIGHLIGHT : WHITE;
                Color corGlow = piscar ? (Color){200, 160, 0, 150} : COLOR_BLUE_HIGHLIGHT;
                DrawTextGlow("START GAME", (Vector2){ startX, startY }, fontSizeOpcao, corBase, corGlow);
            } else {
                DrawTextGlow("START GAME", (Vector2){ startX, startY }, fontSizeOpcao, COLOR_GRAY_OPTION, (Color){50,50,50,100});
            }
            
            // BACK (Índice 3)
            float backY = startY + espacamento; 
            float backX = (sw - MeasureText("BACK", fontSizeOpcao)) / 2;
            if (currentOption == 3) {
                Color corBase = COLOR_YELLOW_HIGHLIGHT;
                Color corGlow = (Color){200, 160, 0, 150};
                DrawTextGlow("BACK", (Vector2){ backX, backY }, fontSizeOpcao, corBase, corGlow);
            } else {
                DrawTextGlow("BACK", (Vector2){ backX, backY }, fontSizeOpcao, COLOR_GRAY_OPTION, (Color){50,50,50,100});
            }
            
            DrawText("Use UP/DOWN to navigate, LEFT/RIGHT to change, ENTER to select", 
                     (sw - MeasureText("Use UP/DOWN to navigate, LEFT/RIGHT to change, ENTER to select", 20)) / 2, 
                     sh - 40, 20, RAYWHITE);
        
        EndDrawing();
    }
    
    return false; 
}

// --- Funções de Efeito (Com lógica atualizada para usar os macros de menu.h) ---
static void InitMenuEffects(void) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    for (int i = 0; i < MAX_RAIN_DROPS; i++) {
        rain[i].pos.x = (float)GetRandomValue(0, screenWidth);
        rain[i].pos.y = (float)GetRandomValue(-screenHeight, 0); 
        rain[i].speed = (float)GetRandomValue(200, 600); 
    }
    for (int i = 0; i < MAX_PULSES; i++) { pulses[i].alpha = 0.0f; }
    effectsInitialized = true;
}

static void UpdateMenuEffects(void) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    float frameTime = GetFrameTime(); 
    for (int i = 0; i < MAX_RAIN_DROPS; i++) {
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

static void DrawMenuEffects(void) {
    for (int i = 0; i < MAX_RAIN_DROPS; i++) {
        DrawRectangle(rain[i].pos.x, rain[i].pos.y, 2, 12, Fade(COLOR_BLUE_HIGHLIGHT, 0.3f));
    }
    for (int i = 0; i < MAX_PULSES; i++) {
        if (pulses[i].alpha > 0.0f) {
            DrawCircleLines(pulses[i].center.x, pulses[i].center.y, pulses[i].radius, Fade(COLOR_YELLOW_HIGHLIGHT, pulses[i].alpha));
        }
    }
}

static void DrawTextGlow(const char* text, Vector2 pos, float size, Color base, Color glow) {
    const int o = 2;
    DrawText(text, pos.x - o, pos.y, size, glow);
    DrawText(text, pos.x + o, pos.y, size, glow);
    DrawText(text, pos.x, pos.y - o, size, glow);
    DrawText(text, pos.x, pos.y + o, size, glow);
    DrawText(text, pos.x, pos.y, size, base);
}