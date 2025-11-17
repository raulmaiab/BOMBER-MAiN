#include "extras.h"
#include "mapa.h"
#include <stdlib.h>

static Texture2D texRange;
static Texture2D texDefense;
static Texture2D texSpeed;

static ExtraItem extras[MAX_EXTRAS];

void InicializarExtras(void)
{
    // Carrega as imagens da pasta "extras"
    texRange = LoadTexture("extras/range.png");
    texDefense = LoadTexture("extras/defense.png");
    texSpeed = LoadTexture("extras/speed.png");

    // Limpa o array
    for (int i = 0; i < MAX_EXTRAS; i++) {
        extras[i].ativo = false;
    }
}

void SpawnarExtra(Vector2 pixelPos)
{
    // 1. Chance de Drop (ex: 25%)
    if (GetRandomValue(0, 100) > 25) return; 

    // 2. Encontrar slot vazio
    int slot = -1;
    for (int i = 0; i < MAX_EXTRAS; i++) {
        if (!extras[i].ativo) {
            slot = i;
            break;
        }
    }
    if (slot == -1) return; // Não há espaço

    // 3. Definir tipo aleatório
    int r = GetRandomValue(0, 2);
    ExtraType tipo = EXTRA_RANGE;
    if (r == 1) tipo = EXTRA_DEFENSE;
    if (r == 2) tipo = EXTRA_SPEED;

    // 4. Criar
    extras[slot].pos = pixelPos;
    extras[slot].type = tipo;
    extras[slot].ativo = true;
}

void DesenharExtras(void)
{
    for (int i = 0; i < MAX_EXTRAS; i++) {
        if (extras[i].ativo) {
            Texture2D* t = NULL;
            switch(extras[i].type) {
                case EXTRA_RANGE: t = &texRange; break;
                case EXTRA_DEFENSE: t = &texDefense; break;
                case EXTRA_SPEED: t = &texSpeed; break;
                default: break;
            }
            
            if (t && t->id > 0) {
                // Desenha centralizado no tile (assumindo texturas menores ou iguais ao tile)
                float offset = (TILE_SIZE - t->width) / 2.0f; 
                DrawTexture(*t, extras[i].pos.x + offset, extras[i].pos.y + offset, WHITE);
            } else {
                // Fallback se não tiver png: Círculos coloridos
                Color c = WHITE;
                if (extras[i].type == EXTRA_RANGE) c = RED;
                if (extras[i].type == EXTRA_DEFENSE) c = BLUE;
                if (extras[i].type == EXTRA_SPEED) c = GREEN;
                DrawCircle(extras[i].pos.x + TILE_SIZE/2, extras[i].pos.y + TILE_SIZE/2, 15, c);
            }
        }
    }
}

void VerificarColetaExtras(Jogador* j)
{
    if (!j->vivo) return;

    // Rectangle do jogador (um pouco menor para não pegar de raspão)
    Rectangle recJog = { j->pos.x + 10, j->pos.y + 10, TILE_SIZE - 20, TILE_SIZE - 20 };

    for (int i = 0; i < MAX_EXTRAS; i++) {
        if (extras[i].ativo) {
            Rectangle recItem = { extras[i].pos.x, extras[i].pos.y, TILE_SIZE, TILE_SIZE };
            
            if (CheckCollisionRecs(recJog, recItem)) {
                // --- APLICAR EFEITO ---
                switch(extras[i].type) {
                    case EXTRA_RANGE:
                        j->bombRange++; // Permanente na partida
                        break;
                    case EXTRA_DEFENSE:
                        j->temDefesa = true;
                        j->timerDefesa = 10.0f; // 10 segundos
                        break;
                    case EXTRA_SPEED:
                        j->temVelocidade = true;
                        j->timerVelocidade = 10.0f; // 10 segundos
                        break;
                    default: break;
                }
                // Consome o item
                extras[i].ativo = false;
            }
        }
    }
}

void DescarregarExtras(void)
{
    UnloadTexture(texRange);
    UnloadTexture(texDefense);
    UnloadTexture(texSpeed);
}