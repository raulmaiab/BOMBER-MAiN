#include "bomba.h"
#include "raylib.h"
#include "mapa.h"       
#include "explosao.h"   
<<<<<<< HEAD
#include "jogador.h"    
=======
#include "jogador.h"  
>>>>>>> 9319279 (...)

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
    }
    return g;
}

void PlantarBomba(NodeBombas *g, Vector2 posBomba, int range) 
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
    g->quantidade++;
}

<<<<<<< HEAD
=======

// --- 1. NOVA FUNÇÃO: Verificar Dano ---
/**
 * @brief Verifica se algum jogador está na coordenada (gridX, gridY) e o "mata".
 */
static void VerificarDanoJogadores(int gridX, int gridY, struct Jogador* jogadores[], int numJogadores)
{
    for (int i = 0; i < numJogadores; i++)
    {
        Jogador *j = jogadores[i];
        if (!j->vivo) continue; // Já está morto

        // Pega o centro do jogador para colisão (mais justo)
        float centerX = j->pos.x + (TILE_SIZE / 2.0f);
        float centerY = j->pos.y + (TILE_SIZE / 2.0f);

        // Converte o centro do jogador para a grelha
        int pGridX = (int)(centerX / TILE_SIZE);
        int pGridY = (int)(centerY / TILE_SIZE);

        // Se a posição da grelha do jogador é a mesma da explosão
        if (pGridX == gridX && pGridY == gridY)
        {
            j->vivo = false; // MORREU!
        }
    }
}

static void PropagarDirecao(NodeExplosoes *gExplosoes, int startX, int startY, int dx, int dy, int raio, struct Jogador* jogadores[], int numJogadores)
{
    for (int i = 1; i <= raio; i++)
    {
        int checkX = startX + (dx * i);
        int checkY = startY + (dy * i);
        
        VerificarDanoJogadores(checkX, checkY, jogadores, numJogadores);
        
        TileType tipo = GetTileTipo(checkX, checkY);
        Vector2 posPixel = { (float)checkX * TILE_SIZE, (float)checkY * TILE_SIZE };
        
        if (tipo == TILE_INDESTRUCTIBLE) {
            break; 
        }
        
        if (tipo == TILE_DESTRUCTIBLE) {
            SetTileTipo(checkX, checkY, TILE_EMPTY); 
            AtivarExplosao(gExplosoes, posPixel);    
            break; 
        }
        
        if (tipo == TILE_EMPTY) {
            AtivarExplosao(gExplosoes, posPixel); 
        }
    }
}


>>>>>>> 9319279 (...)
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
            
<<<<<<< HEAD
            // --- AQUI ESTÁ A CORREÇÃO ESTRUTURAL ---
            // Em vez de processar a explosão aqui, chamamos explosao.c
            // Isso garante que SpawnarExtra seja chamado corretamente.
            CriarExplosao(gExplosoes, b->posicao, b->raioExplosao, jogadores, numJogadores);
            // ---------------------------------------
=======
            Vector2 gridPos = GetGridPosFromPixels(b->posicao);
            int gridX = (int)gridPos.x;
            int gridY = (int)gridPos.y;
            int raio = b->raioExplosao;

            // 1. Ativa explosão e verifica dano no CENTRO
            AtivarExplosao(gExplosoes, b->posicao);
            VerificarDanoJogadores(gridX, gridY, jogadores, numJogadores); 
            if (GetTileTipo(gridX, gridY) == TILE_DESTRUCTIBLE) {
                SetTileTipo(gridX, gridY, TILE_EMPTY);
            }
            
            // 2. Propaga (passando a lista de jogadores)
            PropagarDirecao(gExplosoes, gridX, gridY,  1,  0, raio, jogadores, numJogadores); // Direita
            PropagarDirecao(gExplosoes, gridX, gridY, -1,  0, raio, jogadores, numJogadores); // Esquerda
            PropagarDirecao(gExplosoes, gridX, gridY,  0,  1, raio, jogadores, numJogadores); // Baixo
            PropagarDirecao(gExplosoes, gridX, gridY,  0, -1, raio, jogadores, numJogadores); // Cima
>>>>>>> 9319279 (...)
            
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

<<<<<<< HEAD
=======
// DesenharBombas 
>>>>>>> 9319279 (...)
void DesenharBombas(const NodeBombas *g) {
    Vector2 origin = { 0, 0 };
    for (int i = 0; i < g->quantidade; i++) {
        const Bomba *b = &g->bombas[i];
        if (!b->ativa) continue;
            
        Texture2D texToDraw = (b->currentFrame == 0) ? g->texNormal : g->texAviso;
        
        if (texToDraw.id > 0) {
             // CORREÇÃO DE TAMANHO (GIGANTE -> NORMAL)
             Rectangle sourceRec = { 0.0f, 0.0f, (float)texToDraw.width, (float)texToDraw.height };
             Rectangle destRec = { b->posicao.x, b->posicao.y, TILE_SIZE, TILE_SIZE }; // Força TILE_SIZE
             
             DrawTexturePro(texToDraw, sourceRec, destRec, origin, 0.0f, WHITE);
        } else {
             DrawCircle(b->posicao.x + TILE_SIZE/2, b->posicao.y + TILE_SIZE/2, 20, BLACK);
        }
    }
}

<<<<<<< HEAD
=======
// UnloadBombas 
>>>>>>> 9319279 (...)
void UnloadBombas(NodeBombas *g) {
    UnloadTexture(g->texNormal);
    UnloadTexture(g->texAviso);
}