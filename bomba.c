#include "bomba.h"
#include "raylib.h"
#include "mapa.h"       
#include "explosao.h"   
#include "jogador.h"    // Necessário para acessar a struct Jogador completa
#include <stddef.h>     // Necessário para NULL

#define BOMBA_BLINK_SPEED 0.2f 
#define BOMBA_BLINK_START_TIME 1.0f 

NodeBombas CriarNodeBombas(void) {
    NodeBombas g;
    g.quantidade = 0;
    g.texNormal = LoadTexture("bombapng/bomba1.png");
    g.texAviso = LoadTexture("bombapng/bomba2.png");
    
    if (g.texNormal.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar bombapng/bomba1.png");
    if (g.texAviso.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar bombapng/bomba2.png");
    
    for (int i = 0; i < MAX_BOMBAS_ATIVAS; i++) {
        g.bombas[i].ativa = false;
        g.bombas[i].dono = NULL;
    }
    return g;
}

// Função corrigida para aceitar ponteiro para Jogador
void PlantarBomba(NodeBombas *g, Vector2 posBomba, int range, Jogador *dono) 
{
    if (g->quantidade >= MAX_BOMBAS_ATIVAS) return;
    
    for (int i = 0; i < g->quantidade; i++) {
        if (g->bombas[i].ativa && g->bombas[i].posicao.x == posBomba.x && g->bombas[i].posicao.y == posBomba.y) {
            return; 
        }
    }
    
    Bomba *b = &g->bombas[g->quantidade];
    b->posicao = posBomba;
    b->tempoExplosao = 3.0f;   
    b->raioExplosao = range; 
    b->ativa = true;           
    b->currentFrame = 0; 
    b->frameTimer = 0.0f;
    b->dono = dono; // Atribui o dono corretamente
    
    // Incrementa contador de bombas do jogador
    if (dono) {
        dono->bombasAtivas++;
    }

    g->quantidade++;
}

bool AtualizarBombas(NodeBombas *g, float deltaTime, NodeExplosoes *gExplosoes, struct Jogador* jogadores[], int numJogadores) 
{
    bool algumaExplodiu = false;

    for (int i = 0; i < g->quantidade; i++) {
        Bomba *b = &g->bombas[i];
        if (!b->ativa) continue; 

        b->tempoExplosao -= deltaTime;

        if (b->tempoExplosao <= BOMBA_BLINK_START_TIME) {
            b->frameTimer += deltaTime;
            if (b->frameTimer >= BOMBA_BLINK_SPEED) {
                b->frameTimer = 0.0f;
                b->currentFrame = (b->currentFrame == 0) ? 1 : 0; 
            }
        } else {
            b->currentFrame = 0; 
        }

        if (b->tempoExplosao <= 0) {
            
            CriarExplosao(gExplosoes, b->posicao, b->raioExplosao, jogadores, numJogadores);
            
            if (b->dono) 
            {
                b->dono->bombasAtivas--;
                if (b->dono->bombasAtivas < 0) b->dono->bombasAtivas = 0;
            }

            algumaExplodiu = true;
            b->ativa = false; 
            b->dono = NULL; // Limpa o dono

            // Remove a bomba da lista
            g->bombas[i] = g->bombas[g->quantidade - 1];
            g->bombas[g->quantidade - 1].ativa = false; 
            g->bombas[g->quantidade - 1].dono = NULL;
            g->quantidade--;
            i--; 
        }
    }
    return algumaExplodiu;
}

void DesenharBombas(const NodeBombas *g) {
    Vector2 origin = { 0, 0 };
    for (int i = 0; i < g->quantidade; i++) {
        const Bomba *b = &g->bombas[i];
        if (!b->ativa) continue;
            
        Texture2D texToDraw = (b->currentFrame == 0) ? g->texNormal : g->texAviso;
        
        if (texToDraw.id > 0) {
             Rectangle sourceRec = { 0.0f, 0.0f, (float)texToDraw.width, (float)texToDraw.height };
             Rectangle destRec = { b->posicao.x, b->posicao.y, TILE_SIZE, TILE_SIZE };
             
             DrawTexturePro(texToDraw, sourceRec, destRec, origin, 0.0f, WHITE);
        } else {
             DrawCircle(b->posicao.x + TILE_SIZE/2, b->posicao.y + TILE_SIZE/2, 20, BLACK);
        }
    }
}

void UnloadBombas(NodeBombas *g) {
    UnloadTexture(g->texNormal);
    UnloadTexture(g->texAviso);
}