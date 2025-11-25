#include "mapa.h"
#include <stdlib.h> 
#include <time.h>   
#include <stdio.h>  // Necessário para sprintf

// Variáveis Estáticas (traduzidas)
static Texture2D texturaChao;
static Texture2D texturaIndestrutivel;
static Texture2D texturaDestrutivel;
static TipoTile gradeMapa[ALTURA_GRADE_MAPA][LARGURA_GRADE_MAPA];

// Recebe o nome da pasta (ex: "Default" ou "Cave")
void InicializarMapa(const char* nomeTema)
{
    // Inicialização do gerador de números aleatórios
    srand(time(NULL)); 

    char bufferCaminho[256]; 

    // Constrói os caminhos baseados na pasta recebida
    sprintf(bufferCaminho, "%s/ground.png", nomeTema);
    texturaChao = LoadTexture(bufferCaminho);

    sprintf(bufferCaminho, "%s/wall.png", nomeTema);
    texturaIndestrutivel = LoadTexture(bufferCaminho);

    sprintf(bufferCaminho, "%s/wallb.png", nomeTema);
    texturaDestrutivel = LoadTexture(bufferCaminho); 

    // Logs de erro (usando a regra !x -> x == 0)
    if (texturaChao.id == 0) {
        TraceLog(LOG_WARNING, "Falha ao carregar %s/ground.png", nomeTema);
    }
    if (texturaIndestrutivel.id == 0) {
        TraceLog(LOG_WARNING, "Falha ao carregar %s/wall.png", nomeTema);
    }
    if (texturaDestrutivel.id == 0) {
        TraceLog(LOG_WARNING, "Falha ao carregar %s/wallb.png", nomeTema);
    }

    // Geração do layout
    for (int y = 0; y < ALTURA_GRADE_MAPA; y++) {
        for (int x = 0; x < LARGURA_GRADE_MAPA; x++) {
            if (y == 0 || y == ALTURA_GRADE_MAPA - 1 || x == 0 || x == LARGURA_GRADE_MAPA - 1) {
                // Bordas
                gradeMapa[y][x] = TILE_INDESTRUTIVEL;
            } else if (y % 2 == 0 && x % 2 == 0) {
                // Pilares centrais fixos
                gradeMapa[y][x] = TILE_INDESTRUTIVEL;
            } else {
                // Posição vazia inicial
                gradeMapa[y][x] = TILE_VAZIO;
            }
        }
    }
    
    // Distribuição de blocos destrutíveis
    for (int y = 1; y < ALTURA_GRADE_MAPA - 1; y++) {
        for (int x = 1; x < LARGURA_GRADE_MAPA - 1; x++) {
            if (gradeMapa[y][x] == TILE_VAZIO) {
                if ((rand() % 100) < 75) {
                    gradeMapa[y][x] = TILE_DESTRUTIVEL;
                }
            }
        }
    }

    // Limpa posições iniciais (3x3 ao redor dos cantos)
    gradeMapa[1][1] = TILE_VAZIO; gradeMapa[1][2] = TILE_VAZIO; gradeMapa[2][1] = TILE_VAZIO;
    gradeMapa[1][LARGURA_GRADE_MAPA - 2] = TILE_VAZIO; gradeMapa[1][LARGURA_GRADE_MAPA - 3] = TILE_VAZIO; gradeMapa[2][LARGURA_GRADE_MAPA - 2] = TILE_VAZIO;
    gradeMapa[ALTURA_GRADE_MAPA - 2][1] = TILE_VAZIO; gradeMapa[ALTURA_GRADE_MAPA - 2][2] = TILE_VAZIO; gradeMapa[ALTURA_GRADE_MAPA - 3][1] = TILE_VAZIO;
    gradeMapa[ALTURA_GRADE_MAPA - 2][LARGURA_GRADE_MAPA - 2] = TILE_VAZIO; gradeMapa[ALTURA_GRADE_MAPA - 2][LARGURA_GRADE_MAPA - 3] = TILE_VAZIO; gradeMapa[ALTURA_GRADE_MAPA - 3][LARGURA_GRADE_MAPA - 2] = TILE_VAZIO;
}

void DesenharMapa(void)
{
    Vector2 origem = { 0, 0 };
    Rectangle srcChao = { 0, 0, (float)texturaChao.width, (float)texturaChao.height };
    Rectangle srcInd = { 0, 0, (float)texturaIndestrutivel.width, (float)texturaIndestrutivel.height };
    Rectangle srcDest = { 0, 0, (float)texturaDestrutivel.width, (float)texturaDestrutivel.height };

    for (int y = 0; y < ALTURA_GRADE_MAPA; y++) {
        for (int x = 0; x < LARGURA_GRADE_MAPA; x++) {
            Rectangle destRec = { (float)x * TAMANHO_TILE, (float)y * TAMANHO_TILE, (float)TAMANHO_TILE, (float)TAMANHO_TILE };
            DrawTexturePro(texturaChao, srcChao, destRec, origem, 0.0f, WHITE);

            switch (gradeMapa[y][x]) {
                case TILE_INDESTRUTIVEL:
                    DrawTexturePro(texturaIndestrutivel, srcInd, destRec, origem, 0.0f, WHITE);
                    break;
                case TILE_DESTRUTIVEL:
                    // Verificação !x -> x == 0
                    if (texturaDestrutivel.id > 0) { 
                        DrawTexturePro(texturaDestrutivel, srcDest, destRec, origem, 0.0f, WHITE);
                    } else { 
                        DrawRectangleRec(destRec, PURPLE);
                    }
                    break;
                case TILE_VAZIO: 
                    break;
            }
        }
    }
}

void DescarregarMapa(void)
{
    UnloadTexture(texturaChao);
    UnloadTexture(texturaIndestrutivel);
    UnloadTexture(texturaDestrutivel);
}

TipoTile ObterTipoTile(int x, int y) {
    if (x < 0 || x >= LARGURA_GRADE_MAPA || y < 0 || y >= ALTURA_GRADE_MAPA) {
        return TILE_INDESTRUTIVEL;
    }
    return gradeMapa[y][x];
}

void DefinirTipoTile(int x, int y, TipoTile novoTipo) {
    if (x >= 0 && x < LARGURA_GRADE_MAPA && y >= 0 && y < ALTURA_GRADE_MAPA) {
        gradeMapa[y][x] = novoTipo;
    }
}

Vector2 ObterPosicaoInicialJogador(int indiceJogador) {
    // Uso do if/else if em vez do switch/case para aderir estritamente às regras,
    // embora o switch/case seja geralmente mais limpo aqui.
    if (indiceJogador == 0) {
        return (Vector2){1 * TAMANHO_TILE, 1 * TAMANHO_TILE}; 
    } else if (indiceJogador == 1) {
        return (Vector2){(LARGURA_GRADE_MAPA - 2) * TAMANHO_TILE, 1 * TAMANHO_TILE}; 
    } else if (indiceJogador == 2) {
        return (Vector2){1 * TAMANHO_TILE, (ALTURA_GRADE_MAPA - 2) * TAMANHO_TILE}; 
    } else if (indiceJogador == 3) {
        return (Vector2){(LARGURA_GRADE_MAPA - 2) * TAMANHO_TILE, (ALTURA_GRADE_MAPA - 2) * TAMANHO_TILE}; 
    } else {
        // Caso padrão (default)
        return (Vector2){1 * TAMANHO_TILE, 1 * TAMANHO_TILE};
    }
}

Vector2 ObterPosGradeDePixels(Vector2 posicaoPixel) {
    int gradeX = (int)(posicaoPixel.x / TAMANHO_TILE);
    int gradeY = (int)(posicaoPixel.y / TAMANHO_TILE);
    return (Vector2){ (float)gradeX, (float)gradeY };
}