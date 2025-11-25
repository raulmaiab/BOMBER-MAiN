#include "raylib.h"
#include "menu.h"
#include <stdbool.h>
#include <string.h> 
#include <math.h> 

static RainDrop gotas_chuva[MAX_GOTAS_CHUVA];
static Pulse pulsos_circulares[MAX_PULSOS_CIRCULARES];
static bool efeitos_inicializados = false;

//defs das funções dos detalhes
static void InicializarEfeitosMenu(void);
static void AtualizarEfeitosMenu(void);
static void DesenharEfeitosMenu(void);


//Função de Desenho de Texto (Brilho)
static void DesenharTextoBrilhante(const char* texto, Vector2 posicao, float tamanho, Color cor_base, Color cor_brilho) {
    const int deslocamento_brilho = 2; // Offset do glow
    
    //Desenha o brilho/sombra
    DrawText(texto, posicao.x - deslocamento_brilho, posicao.y, tamanho, cor_brilho);
    DrawText(texto, posicao.x + deslocamento_brilho, posicao.y, tamanho, cor_brilho);
    DrawText(texto, posicao.x, posicao.y - deslocamento_brilho, tamanho, cor_brilho);
    DrawText(texto, posicao.x, posicao.y + deslocamento_brilho, tamanho, cor_brilho);
    
    //Desenha o texto principal por cima
    DrawText(texto, posicao.x, posicao.y, tamanho, cor_base);
}

//Def principal

OpcaoMenu ExecutarTelaMenu(void)
{
    //Verifica se os efeitos precisam ser inicializados
    if (efeitos_inicializados == false) {
        InicializarEfeitosMenu();
    }

    MenuItem opcoes_menu[] = {
        {"BATALHA", ESCOLHA_BATTLE},
        {"HISTÓRIA", ESCOLHA_STORY},
        {"SAIR", ESCOLHA_SAIR} 
    };
    int total_opcoes = sizeof(opcoes_menu) / sizeof(opcoes_menu[0]);
    int indice_opcao_atual = 0; 

    //Loop de Menu 
    while (WindowShouldClose() == 0)
    {
    //Atualizar ações
        if (IsKeyPressed(KEY_DOWN)) {
            indice_opcao_atual = (indice_opcao_atual + 1) % total_opcoes; 
        }
        
        if (IsKeyPressed(KEY_UP)) {
            //Garante que o índice não se torne negativo
            indice_opcao_atual = (indice_opcao_atual - 1 + total_opcoes) % total_opcoes;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            return opcoes_menu[indice_opcao_atual].idOpcao; 
        }
        
        if (IsKeyPressed(KEY_ESCAPE)) {
            return ESCOLHA_SAIR;
        }
        
        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }

        AtualizarEfeitosMenu();

        //Desenhar
        BeginDrawing();
        {
            int largura_tela = GetScreenWidth();
            int altura_tela = GetScreenHeight();

            ClearBackground(COLOR_UI_BACKGROUND); 
            DesenharEfeitosMenu();
            
            //Título "MAIN MENU" com brilho
            float tamanho_titulo = 80;
            float posicao_titulo_x = (largura_tela - MeasureText("MAIN MENU", tamanho_titulo)) / 2;
            DesenharTextoBrilhante("MAIN MENU", (Vector2){ posicao_titulo_x, altura_tela * 0.15f }, tamanho_titulo, 
                                 RAYWHITE, COLOR_BLUE_HIGHLIGHT);

            //Opções do Menu (Centralizadas)
            float tamanho_fonte_opcao = 50;
            float espacamento_vertical = 70;
            float menu_posicao_y_inicial = altura_tela * 0.4f;

            for (int indice = 0; indice < total_opcoes; indice++)
            {
                const char* texto_opcao = opcoes_menu[indice].texto;
                float posicao_x = (largura_tela - MeasureText(texto_opcao, tamanho_fonte_opcao)) / 2;
                float posicao_y = menu_posicao_y_inicial + (indice * espacamento_vertical);
                
                if (indice == indice_opcao_atual)
                {
                    //Efeito de piscar da opção selecionada
                    bool deve_piscar = fmod(GetTime(), 0.2) > 0.1;
                    Color cor_base = deve_piscar ? COLOR_YELLOW_HIGHLIGHT : WHITE;
                    Color cor_brilho = deve_piscar ? (Color){200, 160, 0, 150} : COLOR_BLUE_HIGHLIGHT;
                    
                    DesenharTextoBrilhante(texto_opcao, (Vector2){ posicao_x, posicao_y }, tamanho_fonte_opcao, cor_base, cor_brilho);
                    
                    //Setas de indicação da opção
                    float tamanho_seta = 20;
                    DrawText(">", posicao_x - tamanho_seta*2.5f, posicao_y + (tamanho_fonte_opcao - tamanho_seta)/2.0f, tamanho_seta, cor_base);
                    DrawText("<", posicao_x + MeasureText(texto_opcao, tamanho_fonte_opcao) + tamanho_seta*1.5f, posicao_y + (tamanho_fonte_opcao - tamanho_seta)/2.0f, tamanho_seta, cor_base);
                }
                else
                {
                    DesenharTextoBrilhante(texto_opcao, (Vector2){ posicao_x, posicao_y }, tamanho_fonte_opcao, COLOR_GRAY_OPTION, (Color){50,50,50,100});
                }
            }
            
            //Texto de dica
            const char* texto_ajuda = "Use CIMA/BAIXO e ENTER"; 
            if (opcoes_menu[indice_opcao_atual].idOpcao == ESCOLHA_STORY) { 
                texto_ajuda = "Jogar o Modo História (Avançar nos níveis)"; 
            }
            else if (opcoes_menu[indice_opcao_atual].idOpcao == ESCOLHA_BATTLE) { 
                texto_ajuda = "Iniciar uma Batalha Customizada ou Rápida"; 
            }
            else if (opcoes_menu[indice_opcao_atual].idOpcao == ESCOLHA_SAIR) { 
                texto_ajuda = "Sair do jogo para a Área de Trabalho"; 
            }

            int tamanho_fonte_ajuda = 20;
            int largura_texto_ajuda = MeasureText(texto_ajuda, tamanho_fonte_ajuda);
            DrawText(texto_ajuda, (largura_tela - largura_texto_ajuda) / 2, altura_tela - 40, tamanho_fonte_ajuda, RAYWHITE);

        }
        EndDrawing();
    }
    
    return ESCOLHA_NENHUMA_OU_FECHOU;
}

//Efeitos de fundo
static void InicializarEfeitosMenu(void)
{
    int largura_tela = GetScreenWidth();
    int altura_tela = GetScreenHeight();
    
    //Inicialização das Gotas de Chuva (Rain Drops)
    for (int indice = 0; indice < MAX_GOTAS_CHUVA; indice++) {
        gotas_chuva[indice].posicao.x = (float)GetRandomValue(0, largura_tela);
        gotas_chuva[indice].posicao.y = (float)GetRandomValue(-altura_tela, 0); //Começam fora da tela, acima
        gotas_chuva[indice].velocidade = (float)GetRandomValue(200, 600); 
    }
    
    //Inicialização dos Pulsos Circulares
    for (int indice = 0; indice < MAX_PULSOS_CIRCULARES; indice++) {
        pulsos_circulares[indice].alpha = 0.0f; //Todos invisíveis inicialmente
    }
    
    //Marcar inicialização como concluída
    efeitos_inicializados = true;
}

static void AtualizarEfeitosMenu(void)
{
    int largura_tela = GetScreenWidth();
    int altura_tela = GetScreenHeight();
    float tempo_quadro = GetFrameTime(); 

    //Atualização das Gotas de Chuva
    for (int indice = 0; indice < MAX_GOTAS_CHUVA; indice++) {
        gotas_chuva[indice].posicao.y += gotas_chuva[indice].velocidade * tempo_quadro; 
        
        //Reinicia a gota quando ela sair da tela
        if (gotas_chuva[indice].posicao.y > altura_tela) {
            gotas_chuva[indice].posicao.x = (float)GetRandomValue(0, largura_tela);
            gotas_chuva[indice].posicao.y = (float)GetRandomValue(-40, -20); 
            gotas_chuva[indice].velocidade = (float)GetRandomValue(200, 600);
        }
    }

    //Atualização dos Pulsos Circulares (Efeito de Ondas)
    for (int indice = 0; indice < MAX_PULSOS_CIRCULARES; indice++) {
        if (pulsos_circulares[indice].alpha > 0.0f) {
            //Pulso ativo: Aumenta o raio e diminui a transparência (fade out)
            pulsos_circulares[indice].raio += 150.0f * tempo_quadro; 
            pulsos_circulares[indice].alpha -= 0.7f * tempo_quadro;  
            
            if (pulsos_circulares[indice].alpha < 0.0f) {
                pulsos_circulares[indice].alpha = 0.0f; //Garante que não fique negativo
            }
        } else {
            //Pulso inativo: Tenta iniciar um novo pulso aleatoriamente
            if (GetRandomValue(0, 1000) > 995) {
                pulsos_circulares[indice].centro.x = (float)GetRandomValue(0, largura_tela);
                pulsos_circulares[indice].centro.y = (float)GetRandomValue(0, altura_tela);
                pulsos_circulares[indice].raio = 0.0f;
                pulsos_circulares[indice].alpha = 1.0f; //Torna o pulso visível
            }
        }
    }
}

static void DesenharEfeitosMenu(void)
{
    //Desenha Gotas de Chuva
    for (int indice = 0; indice < MAX_GOTAS_CHUVA; indice++) {
        DrawRectangle(gotas_chuva[indice].posicao.x, gotas_chuva[indice].posicao.y, 2, 12, Fade(COLOR_BLUE_HIGHLIGHT, 0.3f));
    }
    
    //Desenha Pulsos Circulares
    for (int indice = 0; indice < MAX_PULSOS_CIRCULARES; indice++) {
        //Verifica se o pulso está visível
        if (pulsos_circulares[indice].alpha > 0.0f) {
            DrawCircleLines(pulsos_circulares[indice].centro.x, pulsos_circulares[indice].centro.y, pulsos_circulares[indice].raio, Fade(COLOR_YELLOW_HIGHLIGHT, pulsos_circulares[indice].alpha));
        }
    }
}