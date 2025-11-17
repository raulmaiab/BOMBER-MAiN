#include "raylib.h"
#include "menu_battle.h"
#include "menu.h" // Inclui o menu.h para usar as CORES (COLOR_UI_BACKGROUND, etc)
#include <stdbool.h>
#include <math.h> 
#include <stdio.h> // Para sprintf

// --- Variáveis Globais (Efeitos de Fundo) ---
// (Copiadas do menu.c)
static RainDrop rain[MAX_RAIN_DROPS];
static Pulse pulses[MAX_PULSES];
static bool effectsInitialized = false;

// --- Declarações Antecipadas (Aprendemos com o último erro) ---
static void InitMenuEffects(void);
static void UpdateMenuEffects(void);
static void DrawMenuEffects(void);
static void DrawTextGlow(const char* text, Vector2 pos, float size, Color base, Color glow);

// --- Função Principal ---
bool ExecutarMenuBattle(BattleSettings *settings)
{
    if (!effectsInitialized) {
        InitMenuEffects();
    }

    // --- Variáveis de Estado (static) ---
    // (static faz com que o menu "lembre" da última escolha)
    static int currentOption = 0; // 0=Players, 1=Extras, 2=Map, 3=Start
    static int numPlayers = 1;
    static bool extras = false;
    static int mapIndex = 0;
    
    const int numOptions = 4; // Players, Extras, Map, START

    while (!WindowShouldClose())
    {
        // 1. ATUALIZAR (Input)
        
        // Mover entre opções (Cima/Baixo)
        if (IsKeyPressed(KEY_DOWN)) {
            currentOption = (currentOption + 1) % numOptions; 
        }
        if (IsKeyPressed(KEY_UP)) {
            currentOption = (currentOption - 1 + numOptions) % numOptions;
        }

        // Mudar valores (Esquerda/Direita)
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
            bool isRight = IsKeyPressed(KEY_RIGHT);
            
            switch (currentOption)
            {
                case 0: // Players
                    numPlayers = (numPlayers == 1) ? 2 : 1;
                    break;
                case 1: // Extras
                    extras = !extras;
                    break;
                case 2: // Map
                    // (Assumindo 3 mapas por agora)
                    if (isRight) mapIndex = (mapIndex + 1) % 3;
                    else mapIndex = (mapIndex - 1 + 3) % 3;
                    break;
            }
        }
        
        // Confirmar ou Cancelar
        if (IsKeyPressed(KEY_ENTER)) {
            // Se a opção for "START", preenche a struct e retorna true
            if (currentOption == 3) {
                settings->numPlayers = numPlayers;
                settings->extras = extras;
                settings->mapIndex = mapIndex;
                return true; 
            }
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            return false; // Cancela e volta ao menu principal
        }
        
        UpdateMenuEffects();

        // 2. DESENHAR
        BeginDrawing();
            int sw = GetScreenWidth();
            int sh = GetScreenHeight();
            ClearBackground(COLOR_UI_BACKGROUND); 
            DrawMenuEffects();
            
            // Título
            float titleSize = 70;
            float titleX = (sw - MeasureText("BATTLE SETUP", titleSize)) / 2;
            DrawTextGlow("BATTLE SETUP", (Vector2){ titleX, sh * 0.1f }, titleSize, 
                         RAYWHITE, COLOR_BLUE_HIGHLIGHT);
                         
            // --- Opções ---
            float fontSizeOpcao = 40;
            float espacamento = 65;
            float menuY_inicial = sh * 0.35f;
            float colLabelX = sw * 0.3f; // Coluna das "Etiquetas" (Players, Extras)
            float colValorX = sw * 0.55f; // Coluna dos "Valores" (< 1 >, < Off >)

            // Converte valores para texto
            char pText[4]; sprintf(pText, "%d", numPlayers);
            const char* eText = (extras) ? "On" : "Off";
            
            char mMapText[16];
            switch(mapIndex) {
                case 0: sprintf(mMapText, "Default"); break;
                case 1: sprintf(mMapText, "Cave"); break;
                case 2: sprintf(mMapText, "Ice Rink"); break;
            }

            // Textos das opções
            const char* labels[] = {"Players:", "Extras:", "Map:"};
            const char* values[] = {pText, eText, mMapText};
            
            for (int i = 0; i < 3; i++) // Desenha as 3 primeiras opções
            {
                Color base = (i == currentOption) ? COLOR_YELLOW_HIGHLIGHT : COLOR_GRAY_OPTION;
                Color glow = (i == currentOption) ? (Color){200, 160, 0, 150} : (Color){50,50,50,100};
                
                // Desenha a Etiqueta (ex: "Players:")
                DrawTextGlow(labels[i], (Vector2){ colLabelX, menuY_inicial + (i * espacamento) }, 
                             fontSizeOpcao, base, glow);
                             
                // Desenha o Valor (ex: "< 1 >")
                DrawTextGlow("<", (Vector2){ colValorX - 30, menuY_inicial + (i * espacamento) }, 
                             fontSizeOpcao, base, glow);
                DrawTextGlow(values[i], (Vector2){ colValorX, menuY_inicial + (i * espacamento) }, 
                             fontSizeOpcao, base, glow);
                DrawTextGlow(">", (Vector2){ colValorX + MeasureText(values[i], fontSizeOpcao) + 30, menuY_inicial + (i * espacamento) }, 
                             fontSizeOpcao, base, glow);
            }
            
            // --- Opção "START" ---
            float startY = menuY_inicial + (3 * espacamento) + 20;
            float startX = (sw - MeasureText("START", fontSizeOpcao)) / 2;
            if (currentOption == 3) {
                bool piscar = fmod(GetTime(), 0.2) > 0.1;
                Color corBase = piscar ? COLOR_YELLOW_HIGHLIGHT : WHITE;
                Color corGlow = piscar ? (Color){200, 160, 0, 150} : COLOR_BLUE_HIGHLIGHT;
                DrawTextGlow("START", (Vector2){ startX, startY }, fontSizeOpcao, corBase, corGlow);
            } else {
                DrawTextGlow("START", (Vector2){ startX, startY }, fontSizeOpcao, COLOR_GRAY_OPTION, (Color){50,50,50,100});
            }
            
            // Ajuda
            DrawText("Use UP/DOWN to navigate, LEFT/RIGHT to change, ENTER to start", 
                     (sw - MeasureText("Use UP/DOWN to navigate, LEFT/RIGHT to change, ENTER to start", 20)) / 2, 
                     sh - 40, 20, RAYWHITE);
        
        EndDrawing();
    }
    
    return false; // Se saiu pelo WindowShouldClose()
}


// --- Funções de Efeito (Copiadas do menu.c) ---

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