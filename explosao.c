#include "explosao.h"
#include "mapa.h" // Precisamos do TILE_SIZE para desenhar
#include "raylib.h"

#define EXPLOSAO_FRAME_SPEED 0.15f // 0.15 segundos por frame

NodeExplosoes CriarNodeExplosoes(void)
{
    NodeExplosoes g;
    g.quantidade = 0;
    
    g.texExplo1 = LoadTexture("bombapng/explo1.png");
    g.texExplo2 = LoadTexture("bombapng/explo2.png");

    if (g.texExplo1.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar bombapng/explo1.png");
    if (g.texExplo2.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar bombapng/explo2.png");

    for (int i = 0; i < MAX_EXPLOSOES; i++) {
        g.explosoes[i].ativa = false;
    }
    return g;
}

void AtivarExplosao(NodeExplosoes *g, Vector2 pos)
{
    if (g->quantidade >= MAX_EXPLOSOES) return; 

    Explosao *e = &g->explosoes[g->quantidade];
    e->pos = pos;
    e->ativa = true;
    e->currentFrame = 0; 
    e->frameTimer = 0.0f;
    
    g->quantidade++;
}

void AtualizarExplosoes(NodeExplosoes *g, float deltaTime)
{
    for (int i = 0; i < g->quantidade; i++) {
        Explosao *e = &g->explosoes[i];
        if (!e->ativa) continue;

        e->frameTimer += deltaTime;
        
        if (e->frameTimer >= EXPLOSAO_FRAME_SPEED) {
            e->frameTimer = 0.0f;
            e->currentFrame++; 
            
            if (e->currentFrame >= 2) {
                e->ativa = false; 
                
                g->explosoes[i] = g->explosoes[g->quantidade - 1];
                g->explosoes[g->quantidade - 1].ativa = false;
                g->quantidade--;
                i--;
            }
        }
    }
}

void DesenharExplosoes(const NodeExplosoes *g)
{
    Vector2 origin = { 0, 0 };
    
    for (int i = 0; i < g->quantidade; i++) {
        const Explosao *e = &g->explosoes[i];
        if (!e->ativa) continue;

        // --- CORREÇÃO (Linha 76) ---
        Texture2D texToDraw = (e->currentFrame == 0) ? g->texExplo1 : g->texExplo2;
        // --- FIM DA CORREÇÃO ---
        
        Rectangle sourceRec = { 0, 0, (float)texToDraw.width, (float)texToDraw.height };
        Rectangle destRec = { e->pos.x, e->pos.y, TILE_SIZE, TILE_SIZE };
        
        DrawTexturePro(texToDraw, sourceRec, destRec, origin, 0.0f, WHITE);
    }
}

void UnloadExplosoes(NodeExplosoes *g)
{
    UnloadTexture(g->texExplo1);
    
    // --- CORREÇÃO (Linha 91) ---
    UnloadTexture(g->texExplo2);
    // --- FIM DA CORREÇÃO ---
}