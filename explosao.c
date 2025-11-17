#include "explosao.h"
#include "mapa.h"
#include "extras.h"   // <--- Necessário para SpawnarExtra
#include "jogador.h"  // <--- Necessário para verificar vida dos jogadores
#include "raylib.h"

#define EXPLOSAO_FRAME_SPEED 0.15f 

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

// Ativa apenas o efeito visual (frame único)
void AtivarExplosao(NodeExplosoes *g, Vector2 pos)
{
    if (g->quantidade >= MAX_EXPLOSOES) return; 

    // Procura slot inativo para reciclar
    int slot = -1;
    for(int i=0; i<MAX_EXPLOSOES; i++) {
        if(!g->explosoes[i].ativa) {
            slot = i;
            break;
        }
    }
    // Se não achou vazio e ainda tem espaço no fim
    if (slot == -1 && g->quantidade < MAX_EXPLOSOES) {
        slot = g->quantidade;
        g->quantidade++;
    }
    
    if (slot != -1) {
        Explosao *e = &g->explosoes[slot];
        e->pos = pos;
        e->ativa = true;
        e->currentFrame = 0; 
        e->frameTimer = 0.0f;
    }
}

// --- Lógica de Dano (Copiada e adaptada para cá) ---
static void VerificarDanoJogadores(int gridX, int gridY, Jogador* jogadores[], int numJogadores)
{
    for (int i = 0; i < numJogadores; i++)
    {
        Jogador *j = jogadores[i];
        if (!j->vivo) continue;

        // Centro do jogador
        float centerX = j->pos.x + (TILE_SIZE / 2.0f);
        float centerY = j->pos.y + (TILE_SIZE / 2.0f);
        int pGridX = (int)(centerX / TILE_SIZE);
        int pGridY = (int)(centerY / TILE_SIZE);

        if (pGridX == gridX && pGridY == gridY)
        {
            // --- Verifica POWER-UP de DEFESA ---
            if (j->temDefesa) {
                // O jogador sobrevive! (Opcional: j->temDefesa = false; para gastar o escudo)
                continue; 
            }
            
            j->vivo = false; // Mata o jogador
        }
    }
}

// --- Lógica de Propagação e Drop de Extras ---
static void PropagarDirecao(NodeExplosoes *g, int startX, int startY, int dx, int dy, int range, Jogador* jogadores[], int numJogadores)
{
    for (int i = 1; i <= range; i++)
    {
        int checkX = startX + (dx * i);
        int checkY = startY + (dy * i);
        
        // 1. Verifica Jogadores
        VerificarDanoJogadores(checkX, checkY, jogadores, numJogadores);
        
        // 2. Verifica Paredes
        TileType tipo = GetTileTipo(checkX, checkY);
        Vector2 posPixel = { (float)checkX * TILE_SIZE, (float)checkY * TILE_SIZE };
        
        if (tipo == TILE_INDESTRUCTIBLE) {
            break; // Para a explosão
        }
        
        if (tipo == TILE_DESTRUCTIBLE) {
            // Quebra o bloco
            SetTileTipo(checkX, checkY, TILE_EMPTY); 
            
            // Visual da explosão no bloco
            AtivarExplosao(g, posPixel);
            
            // --- AQUI: Tenta spawnar um Extra ---
            SpawnarExtra(posPixel); 
            // ------------------------------------
            
            break; // Para a explosão ao quebrar o bloco
        }
        
        if (tipo == TILE_EMPTY) {
            AtivarExplosao(g, posPixel); 
        }
    }
}

// --- Função Principal chamada pelo bomba.c ---
void CriarExplosao(NodeExplosoes *g, Vector2 centro, int range, Jogador* jogadores[], int numJogadores)
{
    Vector2 gridPos = GetGridPosFromPixels(centro);
    int gridX = (int)gridPos.x;
    int gridY = (int)gridPos.y;

    // 1. Explode o centro
    AtivarExplosao(g, centro);
    VerificarDanoJogadores(gridX, gridY, jogadores, numJogadores);
    
    // Caso raro: bomba em cima de bloco quebrável
    if (GetTileTipo(gridX, gridY) == TILE_DESTRUCTIBLE) {
        SetTileTipo(gridX, gridY, TILE_EMPTY);
        SpawnarExtra(centro);
    }

    // 2. Propaga para as 4 direções
    PropagarDirecao(g, gridX, gridY,  1,  0, range, jogadores, numJogadores); // Direita
    PropagarDirecao(g, gridX, gridY, -1,  0, range, jogadores, numJogadores); // Esquerda
    PropagarDirecao(g, gridX, gridY,  0,  1, range, jogadores, numJogadores); // Baixo
    PropagarDirecao(g, gridX, gridY,  0, -1, range, jogadores, numJogadores); // Cima
}

// --- Funções Padrão (Atualizar/Desenhar) ---

void AtualizarExplosoes(NodeExplosoes *g, float deltaTime)
{
    for (int i = 0; i < MAX_EXPLOSOES; i++) { // Percorre array fixo para evitar problemas de índice
        Explosao *e = &g->explosoes[i];
        if (!e->ativa) continue;

        e->frameTimer += deltaTime;
        
        if (e->frameTimer >= EXPLOSAO_FRAME_SPEED) {
            e->frameTimer = 0.0f;
            e->currentFrame++; 
            
            if (e->currentFrame >= 2) {
                e->ativa = false; 
                // Não precisamos reordenar o array se usarmos slots fixos, 
                // mas se usarmos o método de "pilha" (g->quantidade), mantemos:
                /*
                g->explosoes[i] = g->explosoes[g->quantidade - 1];
                g->explosoes[g->quantidade - 1].ativa = false;
                g->quantidade--;
                i--;
                */
                // Nota: Com MAX_EXPLOSOES alto, apenas desativar é mais seguro e simples.
            }
        }
    }
}

void DesenharExplosoes(const NodeExplosoes *g)
{
    Vector2 origin = { 0, 0 };
    
    for (int i = 0; i < MAX_EXPLOSOES; i++) {
        const Explosao *e = &g->explosoes[i];
        if (!e->ativa) continue;

        Texture2D texToDraw = (e->currentFrame == 0) ? g->texExplo1 : g->texExplo2;
        
        Rectangle sourceRec = { 0, 0, (float)texToDraw.width, (float)texToDraw.height };
        Rectangle destRec = { e->pos.x, e->pos.y, TILE_SIZE, TILE_SIZE };
        
        DrawTexturePro(texToDraw, sourceRec, destRec, origin, 0.0f, WHITE);
    }
}

void UnloadExplosoes(NodeExplosoes *g)
{
    UnloadTexture(g->texExplo1);
    UnloadTexture(g->texExplo2);
}