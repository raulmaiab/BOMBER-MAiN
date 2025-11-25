#include "extras.h"
#include "raylib.h"
#include "raymath.h" 
#include "mapa.h"    
#include <stdlib.h> 

static Texture2D texAlcance;
static Texture2D texDefesa;
static Texture2D texVelocidade;
static Texture2D texLimite;

static ItemExtra extras[MAX_EXTRAS]; 
static int quantidadeExtras = 0;
static bool extrasPermitidos = true; 

void DefinirExtrasHabilitados(bool habilitado) {
    extrasPermitidos = habilitado;
}

void ResetarExtras(void) {
    for (int i = 0; i < MAX_EXTRAS; i++) {
        extras[i].ativo = false;
    }
    quantidadeExtras = 0; 
}

void InicializarExtras(void) {
    texAlcance = LoadTexture("extras/range.png");
    texDefesa = LoadTexture("extras/defense.png");
    texVelocidade = LoadTexture("extras/speed.png");
    texLimite = LoadTexture("extras/plus.png"); 

    if (texAlcance.id == 0) {
        TraceLog(LOG_WARNING, "Falha ao carregar extras/range.png");
    }
    if (texDefesa.id == 0) {
        TraceLog(LOG_WARNING, "Falha ao carregar extras/defense.png");
    }
    if (texVelocidade.id == 0) {
        TraceLog(LOG_WARNING, "Falha ao carregar extras/speed.png");
    }
    if (texLimite.id == 0) {
        TraceLog(LOG_WARNING, "Falha ao carregar extras/plus.png");
    }

    ResetarExtras();
    extrasPermitidos = true; 
}

void DescarregarExtras(void) {
    UnloadTexture(texAlcance);
    UnloadTexture(texDefesa);
    UnloadTexture(texVelocidade);
    UnloadTexture(texLimite);
}

void SpawnarExtra(Vector2 pos) {
    if (extrasPermitidos == false) {
        return; 
    }

    if (GetRandomValue(0, 99) >= 30) {
        return; 
    }
    if (quantidadeExtras >= MAX_EXTRAS) {
        return;
    }

    int slot = -1;
    for (int i = 0; i < MAX_EXTRAS; i++) {
        if (extras[i].ativo == false) {
            slot = i;
            break;
        }
    }
    if (slot == -1) {
        return; 
    }

    extras[slot].pos = pos;
    extras[slot].ativo = true;
    
    // Sorteia de 0 a 3 (inclui EXTRA_LIMITE_BOMBA)
    extras[slot].tipo = GetRandomValue(0, EXTRA_MAX - 1); 
    
    quantidadeExtras++; 
}

bool ObterExtraMaisProximo(Vector2 posJogador, float raioBusca, Vector2* posOut) {
    if (extrasPermitidos == false) {
        return false;
    }
    int melhorIndice = -1;
    float menorDistancia = raioBusca; 
    Vector2 centroJ = { posJogador.x + TAMANHO_TILE/2.0f, posJogador.y + TAMANHO_TILE/2.0f };

    for (int i = 0; i < MAX_EXTRAS; i++) {
        if (extras[i].ativo == true) {
            Vector2 centroE = { extras[i].pos.x + TAMANHO_TILE/2.0f, extras[i].pos.y + TAMANHO_TILE/2.0f };
            float dist = Vector2Distance(centroJ, centroE);
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

void VerificarColetaExtras(Jogador* j) {
    if (j == NULL || j->vivo == false) {
        return;
    }
    if (extrasPermitidos == false) {
        return;
    }

    Vector2 centroJogador = { j->pos.x + TAMANHO_TILE / 2.0f, j->pos.y + TAMANHO_TILE / 2.0f };

    for (int i = 0; i < MAX_EXTRAS; i++) {
        if (extras[i].ativo == false) {
            continue;
        }

        Vector2 centroExtra = { extras[i].pos.x + TAMANHO_TILE / 2.0f, extras[i].pos.y + TAMANHO_TILE / 2.0f };
        float distancia = Vector2Distance(centroJogador, centroExtra);

        if (distancia < TAMANHO_TILE / 2.0f) { 
            switch (extras[i].tipo) {
                case EXTRA_ALCANCE:
                    j->alcanceBomba++; 
                    TraceLog(LOG_INFO, "Jogador pegou Alcance. Novo: %d", j->alcanceBomba);
                    break;
                case EXTRA_DEFESA:
                    j->temDefesa = true;
                    j->temporizadorDefesa = 10.0f; 
                    break;
                case EXTRA_VELOCIDADE:
                    j->temVelocidade = true;
                    j->temporizadorVelocidade = 10.0f; 
                    break;
                case EXTRA_LIMITE_BOMBA:
                    j->limiteBombas++;
                    TraceLog(LOG_INFO, "Jogador pegou Bomba Extra. Limite agora: %d", j->limiteBombas);
                    break;
                default: break;
            }
            extras[i].ativo = false; 
            quantidadeExtras--;      
        }
    }
}

void DesenharExtras(void) {
    if (extrasPermitidos == false) {
        return; 
    }
    Vector2 origem = { 0, 0 };
    for (int i = 0; i < MAX_EXTRAS; i++) {
        if (extras[i].ativo == false) {
            continue;
        }

        Texture2D *t = NULL; 
        Color c = WHITE;     

        switch (extras[i].tipo) {
            case EXTRA_ALCANCE:      t = &texAlcance; c = RED; break;
            case EXTRA_DEFESA:    t = &texDefesa; c = BLUE; break;
            case EXTRA_VELOCIDADE:      t = &texVelocidade; c = GREEN; break;
            case EXTRA_LIMITE_BOMBA: t = &texLimite; c = YELLOW; break; 
            default: break;
        }

        if (t != NULL && t->id > 0) { 
            Rectangle recFonte = { 0.0f, 0.0f, (float)t->width, (float)t->height };
            Rectangle recDestino = { extras[i].pos.x, extras[i].pos.y, TAMANHO_TILE, TAMANHO_TILE };
            DrawTexturePro(*t, recFonte, recDestino, origem, 0.0f, WHITE);
        } else { 
            DrawCircle(extras[i].pos.x + TAMANHO_TILE / 2, extras[i].pos.y + TAMANHO_TILE / 2, TAMANHO_TILE / 3, c);
        }
    }
}