#include "options.h"
#include "menu.h"
#include <math.h> 
#include <string.h>

static void DesenharTextoBrilho(const char* texto, Vector2 pos, float tamanho, Color base, Color brilho) {
    const int o = 2; // Offset do brilho
    DrawText(texto, (int)(pos.x - o), (int)pos.y, (int)tamanho, brilho);
    DrawText(texto, (int)(pos.x + o), (int)pos.y, (int)tamanho, brilho);
    DrawText(texto, (int)pos.x, (int)(pos.y - o), (int)tamanho, brilho);
    DrawText(texto, (int)pos.x, (int)(pos.y + o), (int)tamanho, brilho);
    DrawText(texto, (int)pos.x, (int)pos.y, (int)tamanho, base);
}

//Estrutura do menu
typedef struct {
    const char* texto;
    AcaoOpcoes idOpcao;
} ItemOpcoes;

AcaoOpcoes ExecutarMenuOpcoes(void)
{
    ItemOpcoes opcoes[] = {
        {"RETOMAR", OPCOES_ACAO_RESUMO},
        {"REINICIAR", OPCOES_ACAO_REINICIAR},
        {"EDITAR JOGO", OPCOES_ACAO_EDITAR_JOGO}, 
        {"MENU PRINCIPAL", OPCOES_ACAO_MENU_PRINCIPAL},
    };
    int totalOpcoes = sizeof(opcoes) / sizeof(opcoes[0]);
    int indiceOpcaoAtual = 0; 
    
    //Visual do popup
    const int larguraTela = GetScreenWidth();
    const int alturaTela = GetScreenHeight();
    const int larguraPopup = 600;
    const int alturaPopup = 450;
    const int xPopup = (larguraTela - larguraPopup) / 2;
    const int yPopup = (alturaTela - alturaPopup) / 2;
    const float tamanhoTitulo = 50;
    const float tamanhoFonteOpcao = 35;
    const float espacamento = 50;
    
    while (WindowShouldClose() == 0)
    {
    //Atualizar açoes
        if (IsKeyPressed(KEY_DOWN)) {
            indiceOpcaoAtual = (indiceOpcaoAtual + 1) % totalOpcoes; 
        }
        if (IsKeyPressed(KEY_UP)) {
            indiceOpcaoAtual = (indiceOpcaoAtual - 1 + totalOpcoes) % totalOpcoes;
        }

        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            return opcoes[indiceOpcaoAtual].idOpcao; 
        }
        
        //ESC retorna a partida
        if (IsKeyPressed(KEY_ESCAPE)) {
            return OPCOES_ACAO_RESUMO;
        }
        
        //Desenhar
        BeginDrawing();
        {
            //Desenha um cinza escuro sobre a partida.
            DrawRectangle(0, 0, larguraTela, alturaTela, Fade(BLACK, 0.7f)); 
            
            //Popup: Caixa de fundo
            DrawRectangle(xPopup, yPopup, larguraPopup, alturaPopup, COLOR_UI_BACKGROUND);
            DrawRectangleLinesEx((Rectangle){(float)xPopup, (float)yPopup, (float)larguraPopup, (float)alturaPopup}, 5, COLOR_BLUE_HIGHLIGHT);
            
            //Título OPÇÕES
            const char* textoTitulo = "OPÇÕES";
            float xTitulo = xPopup + (larguraPopup - MeasureText(textoTitulo, tamanhoTitulo)) / 2;
            DesenharTextoBrilho(textoTitulo, (Vector2){ xTitulo, (float)yPopup + 40 }, tamanhoTitulo, RAYWHITE, COLOR_BLUE_HIGHLIGHT);

            //Opções do Menu
            float yMenu_inicial = (float)yPopup + 130;

            for (int i = 0; i < totalOpcoes; i++)
            {
                const char* texto = opcoes[i].texto;
                float xPos = xPopup + (larguraPopup - MeasureText(texto, tamanhoFonteOpcao)) / 2;
                float yPos = yMenu_inicial + (i * espacamento);
                
                if (i == indiceOpcaoAtual)
                {
                    bool piscar;
                    if (fmod(GetTime(), 0.2) > 0.1) {
                        piscar = true;
                    } else {
                        piscar = false;
                    }
                    
                    //Cor amarela e azul
                    Color corBase;
                    Color corGlow;

                    if (piscar) {
                        corBase = COLOR_YELLOW_HIGHLIGHT;
                        corGlow = (Color){200, 160, 0, 150};
                    } else {
                        corBase = WHITE;
                        corGlow = COLOR_BLUE_HIGHLIGHT;
                    }

                    DesenharTextoBrilho(texto, (Vector2){ xPos, yPos }, tamanhoFonteOpcao, corBase, corGlow);
                    
                    //Desenha setas ao lado do item selecionado
                    float tamanhoSeta = 15;
                    DrawText(">", (int)(xPos - tamanhoSeta*2.0f), (int)(yPos + (tamanhoFonteOpcao - tamanhoSeta)/2.0f + 5), (int)tamanhoSeta, corBase);
                    DrawText("<", (int)(xPos + MeasureText(texto, tamanhoFonteOpcao) + tamanhoSeta), (int)(yPos + (tamanhoFonteOpcao - tamanhoSeta)/2.0f + 5), (int)tamanhoSeta, corBase);
                }
                else
                {
                    //Cor
                    DesenharTextoBrilho(texto, (Vector2){ xPos, yPos }, tamanhoFonteOpcao, COLOR_GRAY_OPTION, (Color){50,50,50,100});
                }
            }
            
            //Dica
            DrawText("Pressione ESC ou RETOMAR para continuar o jogo", xPopup + 20, yPopup + alturaPopup - 30, 18, RAYWHITE);
        }
        EndDrawing();
    }
    
    return OPCOES_ACAO_NENHUMA;
}