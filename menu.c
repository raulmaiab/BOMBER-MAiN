#include "raylib.h"
#include "menu.h"
#include <stdbool.h>
#include <string.h>

// --- Cores personalizadas ---
#define COLOR_UI_BACKGROUND CLITERAL(Color){ 20, 30, 40, 255 }   
#define COLOR_UI_TOP_BAR    CLITERAL(Color){ 30, 40, 50, 200 }   
#define COLOR_BLUE_HIGHLIGHT CLITERAL(Color){ 0, 150, 200, 255 } 
#define COLOR_GRAY_OPTION   CLITERAL(Color){ 100, 100, 100, 255 } 
#define COLOR_YELLOW_HIGHLIGHT CLITERAL(Color){ 255, 200, 0, 255 } 

typedef struct {
    const char* texto;
    OpcaoMenu idOpcao;
} MenuItem;

// --- Estruturas para os Efeitos de Fundo ---
#define MAX_RAIN_DROPS 100
#define MAX_PULSES 10
typedef struct { Vector2 pos; float speed; } RainDrop;
typedef struct { Vector2 center; float radius; float alpha; } Pulse;

static RainDrop rain[MAX_RAIN_DROPS];
static Pulse pulses[MAX_PULSES];
static bool effectsInitialized = false;

// Funções de carregamento/descarregamento
static void CarregarRecursosMenu(void);
static void DescarregarRecursosMenu(void);

// Funções para os Efeitos
static void InitMenuEffects(void);
static void UpdateMenuEffects(void);
static void DrawMenuEffects(void);

// Texturas
static Texture2D iconBattle;
static Texture2D iconStory;
static Texture2D iconShop;
static Texture2D iconOther;
static Texture2D iconSair;
static Texture2D buttonBackground; 

bool recursosCarregados = false; 

OpcaoMenu ExecutarTelaMenu(void)
{
    if (!recursosCarregados) {
        CarregarRecursosMenu();
        recursosCarregados = true;
    }
    if (!effectsInitialized) {
        InitMenuEffects();
    }

    MenuItem opcoes[] = {
        {"Battle", ESCOLHA_BATTLE},
        {"Story", ESCOLHA_STORY},
        {"Shop", ESCOLHA_SHOP},
        {"Other", ESCOLHA_OTHER},
        {"Sair", ESCOLHA_SAIR} 
    };
    int totalOpcoes = sizeof(opcoes) / sizeof(opcoes[0]);
    int indiceOpcaoAtual = 1; 

    while (!WindowShouldClose())
    {
        // 1. Atualizar (Lógica / Input)
        if (IsKeyPressed(KEY_DOWN))
        {
            indiceOpcaoAtual++;
            if (indiceOpcaoAtual >= totalOpcoes) indiceOpcaoAtual = 0; 
        }
        if (IsKeyPressed(KEY_UP))
        {
            indiceOpcaoAtual--;
            if (indiceOpcaoAtual < 0) indiceOpcaoAtual = totalOpcoes - 1; 
        }

        // --- CORREÇÃO DE INPUT ---
        // Esta tela espera por PRESSIONAR (o que está correto)
        // O bug é resolvido no inicio.c
        if (IsKeyPressed(KEY_ENTER))
        {
            return opcoes[indiceOpcaoAtual].idOpcao; 
        }
        // --- FIM DA CORREÇÃO ---
        
        if (IsKeyPressed(KEY_ESCAPE)) 
        {
            return ESCOLHA_SAIR;
        }
        if (IsKeyPressed(KEY_F11))
        {
            ToggleFullscreen();
        }

        UpdateMenuEffects();

        // 2. Desenhar
        BeginDrawing();
        
            int screenWidth = GetScreenWidth();
            int screenHeight = GetScreenHeight();

            ClearBackground(COLOR_UI_BACKGROUND); 
            DrawMenuEffects();
            
            DrawRectangle(0, 0, screenWidth, screenHeight / 6, COLOR_UI_TOP_BAR); 
            DrawText("MAIN MENU", screenWidth / 15, screenHeight / 25, screenHeight / 20, WHITE);
            DrawCircle(screenWidth - screenWidth / 8, screenHeight / 10, screenHeight / 25, WHITE);
            DrawText("?", screenWidth - screenWidth / 8 - (MeasureText("?", screenHeight/20)/2), screenHeight / 10 - (screenHeight/20/2), screenHeight / 20, COLOR_UI_BACKGROUND);
            DrawText("320", screenWidth - screenWidth / 15, screenHeight / 15, screenHeight / 25, WHITE); 
            
          
            float scale = 0.20f; // <-- MEXA NESTA LINHAßß

            // 2. Definir valores PADRÃO (fallback)
            float buttonWidth = screenWidth * 0.15f; 
            float buttonHeight = screenHeight * 0.1f;
            
            // 3. Se a textura carregou, usa as dimensões REAIS * a escala
            if (buttonBackground.id > 0) {
                buttonWidth = (float)buttonBackground.width * scale; 
                buttonHeight = (float)buttonBackground.height * scale; 
            }
            // --- FIM DA CORREÇÃO ---

            float buttonSpacing = screenHeight * 0.02f; 
            float menuX = (screenWidth - buttonWidth) / 1.15f; // Centraliza
            float menuY = screenHeight * 0.3f; 

            for (int i = 0; i < totalOpcoes; i++)
            {
                Rectangle opcaoRect = {
                    menuX,
                    menuY + (i * (buttonHeight + buttonSpacing)),
                    buttonWidth,
                    buttonHeight
                };

                Color corBarra = (i == indiceOpcaoAtual) ? COLOR_BLUE_HIGHLIGHT : COLOR_GRAY_OPTION;
                
                if (buttonBackground.id > 0) 
                {
                    DrawTexturePro(
                        buttonBackground, 
                        (Rectangle){ 0.0f, 0.0f, (float)buttonBackground.width, (float)buttonBackground.height }, 
                        opcaoRect, (Vector2){ 0, 0 }, 0.0f, corBarra);
                } else {
                    // Se a imagem falhou, desenha um retângulo simples
                    DrawRectangleRec(opcaoRect, corBarra);
                }
                
                if (i == indiceOpcaoAtual) {
                    DrawRectangleLinesEx(opcaoRect, 3, COLOR_YELLOW_HIGHLIGHT); 
                }

                // Posição do texto
                int tamanhoFonteOpcao = (int)(buttonHeight * 0.45f);
                float textoPaddingLeft = buttonHeight * 0.9f; 
                int textoX = opcaoRect.x + textoPaddingLeft; 
                int textoY = opcaoRect.y + (buttonHeight - tamanhoFonteOpcao) / 2; 
                DrawText(opcoes[i].texto, textoX, textoY, tamanhoFonteOpcao, WHITE);
                
                // Posição do ícone
                float iconSize = buttonHeight * 0.2f; 
                Rectangle iconRect = { 
                    opcaoRect.x + buttonHeight * 0.15f, 
                    opcaoRect.y + (buttonHeight - iconSize) / 2.0f, 
                    iconSize, 
                    iconSize 
                };

                Texture2D iconToDraw = iconBattle; 
                switch (opcoes[i].idOpcao) {
                    case ESCOLHA_BATTLE: iconToDraw = iconBattle; break;
                    case ESCOLHA_STORY:  iconToDraw = iconStory;  break;
                    case ESCOLHA_SHOP:   iconToDraw = iconShop;   break;
                    case ESCOLHA_OTHER:  iconToDraw = iconOther;  break;
                    case ESCOLHA_SAIR:   iconToDraw = iconSair;   break;
                    default: break;
                }
                if (iconToDraw.id > 0) 
                {
                    DrawTexturePro(
                        iconToDraw, 
                        (Rectangle){ 0.0f, 0.0f, (float)iconToDraw.width, (float)iconToDraw.height }, 
                        iconRect, (Vector2){ 0, 0 }, 0.0f, WHITE);
                }
            }
            
            // Texto de ajuda inferior
            const char* helpText = "Pressione ESC para voltar"; 
            if (opcoes[indiceOpcaoAtual].idOpcao == ESCOLHA_STORY) { helpText = "Play Story Mode"; }
            else if (opcoes[indiceOpcaoAtual].idOpcao == ESCOLHA_BATTLE) { helpText = "Start a Battle"; }
            else if (opcoes[indiceOpcaoAtual].idOpcao == ESCOLHA_SHOP) { helpText = "Visit the Shop"; }
            else if (opcoes[indiceOpcaoAtual].idOpcao == ESCOLHA_OTHER) { helpText = "Game Settings"; }
            else if (opcoes[indiceOpcaoAtual].idOpcao == ESCOLHA_SAIR) { helpText = "Exit to Desktop"; }

            int helpTextFontSize = screenHeight / 30;
            int helpTextWidth = MeasureText(helpText, helpTextFontSize);
            DrawText(helpText, (screenWidth - helpTextWidth) / 2, screenHeight - (screenHeight / 15), helpTextFontSize, RAYWHITE);

            // Setas de navegação (decorativas)
            int arrowSize = screenHeight / 25;
            DrawTriangle((Vector2){screenWidth/4.0f, screenHeight - (screenHeight/15.0f) + arrowSize/2.0f}, (Vector2){screenWidth/4.0f - arrowSize, screenHeight - (screenHeight/15.0f)}, (Vector2){screenWidth/4.0f - arrowSize, screenHeight - (screenHeight/15.0f) + arrowSize}, WHITE);
            DrawTriangle((Vector2){screenWidth*0.75f, screenHeight - (screenHeight/15.0f) + arrowSize/2.0f}, (Vector2){screenWidth*0.75f + arrowSize, screenHeight - (screenHeight/15.0f)}, (Vector2){screenWidth*0.75f + arrowSize, screenHeight - (screenHeight/15.0f) + arrowSize}, WHITE);


        EndDrawing();
    }

    DescarregarRecursosMenu();
    recursosCarregados = false;
    return ESCOLHA_NENHUMA_OU_FECHOU;
}

// --- Funções de Carregar/Descarregar Recursos ---
static void CarregarRecursosMenu(void)
{
    // Adicionei logs de aviso. Se as imagens não carregarem,
    // você verá um aviso no terminal, o que ajuda a depurar.
    iconBattle = LoadTexture("icon_battle.png");
    if (iconBattle.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar icon_battle.png");
    
    iconStory  = LoadTexture("icon_story.png");
    if (iconStory.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar icon_story.png");
    
    iconShop   = LoadTexture("icon_shop.png");
    if (iconShop.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar icon_shop.png");
    
    iconOther  = LoadTexture("icon_other.png");
    if (iconOther.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar icon_other.png");
    
    iconSair   = LoadTexture("icon_sair.png");
    if (iconSair.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar icon_sair.png");
    
    buttonBackground = LoadTexture("button_gray_template.png");
    if (buttonBackground.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar button_gray_template.png");
}

static void DescarregarRecursosMenu(void)
{
    UnloadTexture(iconBattle);
    UnloadTexture(iconStory);
    UnloadTexture(iconShop);
    UnloadTexture(iconOther);
    UnloadTexture(iconSair);
    UnloadTexture(buttonBackground);
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