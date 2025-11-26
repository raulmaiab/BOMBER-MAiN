#include "explosao.h"
#include "mapa.h"
#include "extras.h"  
#include "jogador.h"  
#include "raylib.h"

#define VELOCIDADE_FRAME_EXPLOSAO 0.15f 

NodeExplosoes CriarNodeExplosoes(void)
{
    NodeExplosoes g;
    g.quantidade = 0;
    
    g.texExplo1 = LoadTexture("bombapng/explo1.png");
    g.texExplo2 = LoadTexture("bombapng/explo2.png");

    if (g.texExplo1.id == 0) {
        TraceLog(LOG_WARNING, "Falha ao carregar bombapng/explo1.png");
    }
    if (g.texExplo2.id == 0) {
        TraceLog(LOG_WARNING, "Falha ao carregar bombapng/explo2.png");
    }

    for (int i = 0; i < MAX_EXPLOSOES; i++) {
        g.explosoes[i].ativa = false;
    }
    return g;
}

void AtivarExplosao(NodeExplosoes *g, Vector2 pos)
{
    if (g->quantidade >= MAX_EXPLOSOES) {
        return; 
    }

    int slot = -1;
    for(int i=0; i<MAX_EXPLOSOES; i++) {
        if (g->explosoes[i].ativa == false) {
            slot = i;
            break;
        }
    }
    if (slot == -1 && g->quantidade < MAX_EXPLOSOES) {
        slot = g->quantidade;
        g->quantidade++;
    }
    
    if (slot != -1) {
        Explosao *e = &g->explosoes[slot];
        e->pos = pos;
        e->ativa = true;
        e->frameAtual = 0; 
        e->temporizadorFrame = 0.0f;
    }
}

//Lógica de dano
static void VerificarDanoJogadores(int gradeX, int gradeY, Jogador* jogadores[], int numJogadores)
{
    for (int i = 0; i < numJogadores; i++)
    {
        Jogador *j = jogadores[i];
        if (j->vivo == false) {
            continue;
        }

        //Centro do jogador
        float centroX = j->pos.x + (TAMANHO_TILE / 2.0f);
        float centroY = j->pos.y + (TAMANHO_TILE / 2.0f);
        int pGradeX = (int)(centroX / TAMANHO_TILE);
        int pGradeY = (int)(centroY / TAMANHO_TILE);

        if (pGradeX == gradeX && pGradeY == gradeY)
        {
            if (j->temDefesa == true) {
                continue; 
            }
            
            j->vivo = false; 
        }
    }
}

static void PropagarDirecao(NodeExplosoes *g, int inicioX, int inicioY, int dx, int dy, int alcance, Jogador* jogadores[], int numJogadores)
{
    for (int i = 1; i <= alcance; i++)
    {
        int verificarX = inicioX + (dx * i);
        int verificarY = inicioY + (dy * i);
        
        //Verifica Jogadores
        VerificarDanoJogadores(verificarX, verificarY, jogadores, numJogadores);
        
        //Verifica Paredes
        TipoTile tipo = ObterTipoTile(verificarX, verificarY);
        Vector2 posPixel = { (float)verificarX * TAMANHO_TILE, (float)verificarY * TAMANHO_TILE };
        if (tipo == TILE_INDESTRUTIVEL) {
            break; //Para a explosão
        }
        
        if (tipo == TILE_DESTRUTIVEL) {
            // Quebra o bloco
            DefinirTipoTile(verificarX, verificarY, TILE_VAZIO); 
            
            //Visual da explosão no bloco
            AtivarExplosao(g, posPixel);
            
            //Spawner extra
            SpawnarExtra(posPixel); 
            
            break; 
        }
        
        if (tipo == TILE_VAZIO) {
            AtivarExplosao(g, posPixel); 
        }
    }
}

void CriarExplosao(NodeExplosoes *g, Vector2 centro, int alcance, Jogador* jogadores[], int numJogadores)
{
    Vector2 posGrade = ObterPosGradeDePixels(centro);
    int gradeX = (int)posGrade.x;
    int gradeY = (int)posGrade.y;

    AtivarExplosao(g, centro);
    VerificarDanoJogadores(gradeX, gradeY, jogadores, numJogadores);
    
    if (ObterTipoTile(gradeX, gradeY) == TILE_DESTRUTIVEL) {
        DefinirTipoTile(gradeX, gradeY, TILE_VAZIO);
        SpawnarExtra(centro);
    }

    //explode pra os 4 lados
    PropagarDirecao(g, gradeX, gradeY,  1,  0, alcance, jogadores, numJogadores); // Direita
    PropagarDirecao(g, gradeX, gradeY, -1,  0, alcance, jogadores, numJogadores); // Esquerda
    PropagarDirecao(g, gradeX, gradeY,  0,  1, alcance, jogadores, numJogadores); // Baixo
    PropagarDirecao(g, gradeX, gradeY,  0, -1, alcance, jogadores, numJogadores); // Cima
}
//Desenhar
void AtualizarExplosoes(NodeExplosoes *g, float deltaTime)
{
    for (int i = 0; i < MAX_EXPLOSOES; i++) { 
        Explosao *e = &g->explosoes[i];
        if (e->ativa == false) {
            continue;
        }

        e->temporizadorFrame += deltaTime;
        
        if (e->temporizadorFrame >= VELOCIDADE_FRAME_EXPLOSAO) {
            e->temporizadorFrame = 0.0f;
            e->frameAtual++; 
            
            if (e->frameAtual >= 2) {
                e->ativa = false; 
            }
        }
    }
}

void DesenharExplosoes(const NodeExplosoes *g)
{
    Vector2 origem = { 0, 0 };
    
    for (int i = 0; i < MAX_EXPLOSOES; i++) {
        const Explosao *e = &g->explosoes[i];
        if (e->ativa == false) {
            continue;
        }

        Texture2D texDesenhar;
        if (e->frameAtual == 0) {
            texDesenhar = g->texExplo1;
        } else {
            texDesenhar = g->texExplo2;
        }
        
        Rectangle recFonte = { 0, 0, (float)texDesenhar.width, (float)texDesenhar.height };
        Rectangle recDestino = { e->pos.x, e->pos.y, TAMANHO_TILE, TAMANHO_TILE };
        
        DrawTexturePro(texDesenhar, recFonte, recDestino, origem, 0.0f, WHITE);
    }
}

void DescarregarExplosoes(NodeExplosoes *g)
{
    UnloadTexture(g->texExplo1);
    UnloadTexture(g->texExplo2);
}