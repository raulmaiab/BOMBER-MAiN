#include "extras.h"
#include "raylib.h"
#include "raymath.h" 
#include "mapa.h"    
#include <stdlib.h> 

static Texture2D texRange;
static Texture2D texDefense;
static Texture2D texSpeed;

static ExtraItem extras[MAX_EXTRAS]; 
static int quantidadeExtras = 0;

// --- NOVO: Variável de Controle ---
static bool extrasPermitidos = true; // Padrão ligado

void SetExtrasHabilitados(bool habilitado) {
    extrasPermitidos = habilitado;
}
// ----------------------------------

void ResetarExtras(void)
{
    for (int i = 0; i < MAX_EXTRAS; i++) {
        extras[i].ativo = false;
    }
    quantidadeExtras = 0; 
}

void InicializarExtras(void)
{
    texRange = LoadTexture("extras/range.png");
    texDefense = LoadTexture("extras/defense.png");
    texSpeed = LoadTexture("extras/speed.png");

    if (texRange.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar extras/range.png");
    if (texDefense.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar extras/defense.png");
    if (texSpeed.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar extras/speed.png");

    ResetarExtras();
    extrasPermitidos = true; // Reseta para true por segurança
}

void DescarregarExtras(void)
{
    UnloadTexture(texRange);
    UnloadTexture(texDefense);
    UnloadTexture(texSpeed);
}

void SpawnarExtra(Vector2 pos)
{
    // --- NOVO: Verifica se está habilitado ---
    if (!extrasPermitidos) return; 
    // ----------------------------------------

    // Chance de 30% de spawnar um extra
    if (GetRandomValue(0, 99) >= 30) return; 

    if (quantidadeExtras >= MAX_EXTRAS) return;

    int slot = -1;
    for (int i = 0; i < MAX_EXTRAS; i++) {
        if (!extras[i].ativo) {
            slot = i;
            break;
        }
    }

    if (slot == -1) return; 

    extras[slot].pos = pos;
    extras[slot].ativo = true;
    extras[slot].type = GetRandomValue(0, EXTRA_MAX - 1); 
    quantidadeExtras++; 
}

bool GetExtraMaisProximo(Vector2 posJogador, float raioBusca, Vector2* posOut)
{
    // Se extras estiverem desligados, o bot não deve achar nada
    if (!extrasPermitidos) return false;

    int melhorIndice = -1;
    float menorDistancia = raioBusca; 

    Vector2 centerJ = { posJogador.x + TILE_SIZE/2.0f, posJogador.y + TILE_SIZE/2.0f };

    for (int i = 0; i < MAX_EXTRAS; i++) 
    {
        if (extras[i].ativo) {
            Vector2 centerE = { extras[i].pos.x + TILE_SIZE/2.0f, extras[i].pos.y + TILE_SIZE/2.0f };
            float dist = Vector2Distance(centerJ, centerE);
            
            if (dist < menorDistancia) {
                menorDistancia = dist;
                melhorIndice = i;
            }
        }
    }

    if (melhorIndice != -1) {
        *posOut = extras[melhorIndice].pos;
        return true;
    }
    return false;
}

void VerificarColetaExtras(Jogador* j)
{
    if (!j || !j->vivo) return;
    if (!extrasPermitidos) return; // Otimização

    Vector2 playerCenter = { j->pos.x + TILE_SIZE / 2.0f, j->pos.y + TILE_SIZE / 2.0f };

    for (int i = 0; i < MAX_EXTRAS; i++) {
        if (!extras[i].ativo) continue;

        Vector2 extraCenter = { extras[i].pos.x + TILE_SIZE / 2.0f, extras[i].pos.y + TILE_SIZE / 2.0f };
        float distancia = Vector2Distance(playerCenter, extraCenter);

        if (distancia < TILE_SIZE / 2.0f) { 
            switch (extras[i].type) {
                case EXTRA_RANGE:
                    j->bombRange++; 
                    break;
                case EXTRA_DEFENSE:
                    j->temDefesa = true;
                    j->timerDefesa = 10.0f; 
                    break;
                case EXTRA_SPEED:
                    j->temVelocidade = true;
                    j->timerVelocidade = 10.0f; 
                    break;
                default:
                    break;
            }
            
            extras[i].ativo = false; 
            quantidadeExtras--;      
        }
    }
}

void DesenharExtras(void)
{
    if (!extrasPermitidos) return; // Não desenha nada se estiver desligado

    Vector2 origin = { 0, 0 };
    for (int i = 0; i < MAX_EXTRAS; i++) {
        if (!extras[i].ativo) continue;

        Texture2D *t = NULL; 
        Color c = WHITE;     

        switch (extras[i].type) {
            case EXTRA_RANGE:    t = &texRange; c = RED; break;
            case EXTRA_DEFENSE:  t = &texDefense; c = BLUE; break;
            case EXTRA_SPEED:    t = &texSpeed; c = GREEN; break;
            default: break;
        }

        if (t && t->id > 0) { 
            Rectangle sourceRec = { 0.0f, 0.0f, (float)t->width, (float)t->height };
            Rectangle destRec = { extras[i].pos.x, extras[i].pos.y, TILE_SIZE, TILE_SIZE };
            DrawTexturePro(*t, sourceRec, destRec, origin, 0.0f, WHITE);
        } else { 
            DrawCircle(extras[i].pos.x + TILE_SIZE / 2, extras[i].pos.y + TILE_SIZE / 2, TILE_SIZE / 3, c);
        }
    }
}