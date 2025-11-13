#include "bomba.h"
#include "raylib.h"
#include "mapa.h"       // Precisamos do TILE_SIZE
#include "explosao.h"   // Precisamos chamar a explosão

// Constantes da Animação
#define BOMBA_BLINK_SPEED 0.2f 
#define BOMBA_BLINK_START_TIME 1.0f 

NodeBombas CriarNodeBombas(void) { // Parâmetro removido
    NodeBombas g;
    g.quantidade = 0;
    
    g.texNormal = LoadTexture("bombapng/bomba1.png");
    g.texAviso = LoadTexture("bombapng/bomba2.png");
    
    if (g.texNormal.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar bombapng/bomba1.png");
    if (g.texAviso.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar bombapng/bomba2.png");
    
    for (int i = 0; i < MAX_BOMBAS_ATIVAS; i++) {
        g.bombas[i].ativa = false;
    }
    return g;
}

void PlantarBomba(NodeBombas *g, Vector2 posBomba) 
{
    if (g->quantidade >= MAX_BOMBAS_ATIVAS) return;
    
    // Opcional: Verifica se já existe uma bomba nesse local
    for (int i = 0; i < g->quantidade; i++) {
        
        // --- CORREÇÃO (Linha 33) ---
        // g.bombas foi trocado para g->bombas (3 vezes)
        if (g->bombas[i].ativa && g->bombas[i].posicao.x == posBomba.x && g->bombas[i].posicao.y == posBomba.y) {
        // --- FIM DA CORREÇÃO ---
            return; 
        }
    }
    
    Bomba *b = &g->bombas[g->quantidade];

    b->posicao = posBomba;
    b->tempoExplosao = 2.5f;   
    b->raioExplosao = 2;       
    b->ativa = true;           
    b->currentFrame = 0; 
    b->frameTimer = 0.0f;

    g->quantidade++;
}

// Atualizado (Sem erros aqui, mas incluído para completude)
bool AtualizarBombas(NodeBombas *g, float deltaTime, NodeExplosoes *gExplosoes) {
    bool algumaExplodiu = false;

    for (int i = 0; i < g->quantidade; i++) {
        Bomba *b = &g->bombas[i];
        if (!b->ativa) continue; 

        b->tempoExplosao -= deltaTime;

        // Lógica de piscar
        if (b->tempoExplosao <= BOMBA_BLINK_START_TIME) {
            b->frameTimer += deltaTime;
            if (b->frameTimer >= BOMBA_BLINK_SPEED) {
                b->frameTimer = 0.0f;
                b->currentFrame = (b->currentFrame == 0) ? 1 : 0; 
            }
        } else {
            b->currentFrame = 0; 
        }

        // Explodiu
        if (b->tempoExplosao <= 0) {
            
            AtivarExplosao(gExplosoes, b->posicao);
            
            algumaExplodiu = true;
            b->ativa = false; 

            g->bombas[i] = g->bombas[g->quantidade - 1];
            g->bombas[g->quantidade - 1].ativa = false; 
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
        Rectangle sourceRec = { 0, 0, (float)texToDraw.width, (float)texToDraw.height };
        Rectangle destRec = { b->posicao.x, b->posicao.y, TILE_SIZE, TILE_SIZE };

        DrawTexturePro(texToDraw, sourceRec, destRec, origin, 0.0f, WHITE);
    }
}

void UnloadBombas(NodeBombas *g) {
    UnloadTexture(g->texNormal);
    UnloadTexture(g->texAviso);
}