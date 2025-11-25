#include "bomba.h"
#include "raylib.h"
#include "mapa.h"       
#include "explosao.h"   
#include "jogador.h"    // Necessário para acessar a struct Jogador completa
#include <stddef.h>     // Necessário para NULL

#define VELOCIDADE_PISCAR_BOMBA 0.2f 
#define TEMPO_INICIO_PISCAR_BOMBA 1.0f 

NodeBombas CriarNodeBombas(void) {
    NodeBombas g;
    g.quantidade = 0;
    g.texNormal = LoadTexture("bombapng/bomba1.png");
    g.texAviso = LoadTexture("bombapng/bomba2.png");
    
    if (g.texNormal.id == 0) {
        TraceLog(LOG_WARNING, "Falha ao carregar bombapng/bomba1.png");
    }
    if (g.texAviso.id == 0) {
        TraceLog(LOG_WARNING, "Falha ao carregar bombapng/bomba2.png");
    }
    
    for (int i = 0; i < MAX_BOMBAS_ATIVAS; i++) {
        g.bombas[i].ativa = false;
        g.bombas[i].dono = NULL;
    }
    return g;
}

// Função corrigida para aceitar ponteiro para Jogador
void PlantarBomba(NodeBombas *g, Vector2 posBomba, int range, Jogador *dono) 
{
    if (g->quantidade >= MAX_BOMBAS_ATIVAS) {
        return;
    }
    
    for (int i = 0; i < g->quantidade; i++) {
        // Verifica se já existe uma bomba ativa na mesma posição
        if (g->bombas[i].ativa != false && g->bombas[i].posicao.x == posBomba.x && g->bombas[i].posicao.y == posBomba.y) {
            return; 
        }
    }
    
    Bomba *b = &g->bombas[g->quantidade];
    b->posicao = posBomba;
    b->tempoExplosao = 3.0f;   
    b->raioExplosao = range; 
    b->ativa = true;           
    b->frameAtual = 0; 
    b->temporizadorFrame = 0.0f;
    b->dono = dono; // Atribui o dono corretamente
    
    // Incrementa contador de bombas do jogador
    if (dono != NULL) {
        dono->bombasAtivas++;
    }

    g->quantidade++;
}

bool AtualizarBombas(NodeBombas *g, float deltaTime, NodeExplosoes *gExplosoes, struct Jogador* jogadores[], int numJogadores) 
{
    bool algumaExplodiu = false;

    for (int i = 0; i < g->quantidade; i++) {
        Bomba *b = &g->bombas[i];
        if (b->ativa == false) {
            continue; 
        }

        b->tempoExplosao -= deltaTime;

        if (b->tempoExplosao <= TEMPO_INICIO_PISCAR_BOMBA) {
            b->temporizadorFrame += deltaTime;
            if (b->temporizadorFrame >= VELOCIDADE_PISCAR_BOMBA) {
                b->temporizadorFrame = 0.0f;
                if (b->frameAtual == 0) {
                    b->frameAtual = 1;
                } else {
                    b->frameAtual = 0;
                }
            }
        } else {
            b->frameAtual = 0; 
        }

        if (b->tempoExplosao <= 0) {
            
            CriarExplosao(gExplosoes, b->posicao, b->raioExplosao, jogadores, numJogadores);
            
            if (b->dono != NULL) 
            {
                b->dono->bombasAtivas--;
                if (b->dono->bombasAtivas < 0) {
                    b->dono->bombasAtivas = 0;
                }
            }

            algumaExplodiu = true;
            b->ativa = false; 
            b->dono = NULL; // Limpa o dono

            // Remove a bomba da lista, trocando com a última
            g->bombas[i] = g->bombas[g->quantidade - 1];
            g->bombas[g->quantidade - 1].ativa = false; 
            g->bombas[g->quantidade - 1].dono = NULL;
            g->quantidade--;
            i--; // Volta um índice para reavaliar a bomba trocada para a posição atual
        }
    }
    return algumaExplodiu;
}

void DesenharBombas(const NodeBombas *g) {
    Vector2 origem = { 0, 0 };
    for (int i = 0; i < g->quantidade; i++) {
        const Bomba *b = &g->bombas[i];
        if (b->ativa == false) {
            continue;
        }
            
        Texture2D texDesenhar;
        if (b->frameAtual == 0) {
            texDesenhar = g->texNormal;
        } else {
            texDesenhar = g->texAviso;
        }
        
        if (texDesenhar.id > 0) {
             Rectangle recFonte = { 0.0f, 0.0f, (float)texDesenhar.width, (float)texDesenhar.height };
             Rectangle recDestino = { b->posicao.x, b->posicao.y, TAMANHO_TILE, TAMANHO_TILE };
             
             DrawTexturePro(texDesenhar, recFonte, recDestino, origem, 0.0f, WHITE);
        } else {
             DrawCircle(b->posicao.x + TAMANHO_TILE/2, b->posicao.y + TAMANHO_TILE/2, 20, BLACK);
        }
    }
}

void UnloadBombas(NodeBombas *g) {
    UnloadTexture(g->texNormal);
    UnloadTexture(g->texAviso);
}