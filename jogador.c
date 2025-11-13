#include "jogador.h"
#include "raylib.h" 
#include "mapa.h"   
#include "bomba.h" 

// --- Funções Ajudantes (Sem alteração) ---
static void AlinharEPlantarBomba(Jogador* j, NodeBombas *gBombas)
{
    float centerX = j->pos.x + (TILE_SIZE / 2.0f);
    float centerY = j->pos.y + (TILE_SIZE / 2.0f);
    int gridX = (int)(centerX / TILE_SIZE);
    int gridY = (int)(centerY / TILE_SIZE);
    Vector2 posBombaAlinhada = { (float)gridX * TILE_SIZE, (float)gridY * TILE_SIZE };
    PlantarBomba(gBombas, posBombaAlinhada);
}

static void MoverJogadorX(Jogador *j, float delta)
{
    if (delta == 0) return;
    Vector2 posTentativa = j->pos;
    posTentativa.x += delta;
    float collisionRectX = (delta > 0) ? posTentativa.x + TILE_SIZE - 1 : posTentativa.x;
    float collisionRectY = j->pos.y;
    Vector2 gridPos1 = GetGridPosFromPixels((Vector2){collisionRectX, collisionRectY});
    Vector2 gridPos2 = GetGridPosFromPixels((Vector2){collisionRectX, collisionRectY + TILE_SIZE - 1});
    if (GetTileTipo((int)gridPos1.x, (int)gridPos1.y) == TILE_EMPTY &&
        GetTileTipo((int)gridPos2.x, (int)gridPos2.y) == TILE_EMPTY)
    {
        j->pos.x = posTentativa.x; 
    }
}

static void MoverJogadorY(Jogador *j, float delta)
{
    if (delta == 0) return;
    Vector2 posTentativa = j->pos;
    posTentativa.y += delta;
    float collisionRectX_Y = j->pos.x; 
    float collisionRectY_Y = (delta > 0) ? posTentativa.y + TILE_SIZE - 1 : posTentativa.y;
    Vector2 gridPos1 = GetGridPosFromPixels((Vector2){collisionRectX_Y, collisionRectY_Y});
    Vector2 gridPos2 = GetGridPosFromPixels((Vector2){collisionRectX_Y + TILE_SIZE - 1, collisionRectY_Y});
    if (GetTileTipo((int)gridPos1.x, (int)gridPos1.y) == TILE_EMPTY &&
        GetTileTipo((int)gridPos2.x, (int)gridPos2.y) == TILE_EMPTY)
    {
        j->pos.y = posTentativa.y;
    }
}
// --- Fim Funções Ajudantes ---


// --- ATUALIZADO: CriarJogador ---
Jogador CriarJogador(Vector2 posInicial, const char* imgSprite, bool ehBot)
{
    Jogador j;
    j.pos = posInicial;
    j.velocidade = 2.5f; 
    j.textura = LoadTexture(imgSprite);
    if (j.textura.id == 0) {
        TraceLog(LOG_WARNING, "Falha ao carregar textura do jogador: %s", imgSprite);
    }
    j.vivo = true;
    
    // --- Inicializa o Bot ---
    j.ehBot = ehBot;
    j.botState = BOT_STATE_WANDERING; // Começa passeando
    j.botStateTimer = (float)GetRandomValue(10, 30) / 10.0f; // Próxima decisão em 1-3s
    j.botMoveDirecao = GetRandomValue(0, 3); // Direção aleatória
    
    return j;
}

// --- ATUALIZADO: AtualizarJogador (IA v2) ---
void AtualizarJogador(Jogador* j, int keyUp, int keyDown, int keyLeft, int keyRight, int keyBomb, NodeBombas *gBombas, float deltaTime)
{
    if (!j->vivo) return;

    // --- LÓGICA DO BOT (IA v2) ---
    if (j->ehBot)
    {
        j->botStateTimer -= deltaTime; // Contagem regressiva do timer

        // Guarda a posição ANTES de tentar mover
        Vector2 posAntes = j->pos;

        // --- Máquina de Estados ---
        switch (j->botState)
        {
            case BOT_STATE_WANDERING:
            {
                // Timer de decisão (mudar direção, plantar bomba)
                if (j->botStateTimer <= 0.0f)
                {
                    j->botStateTimer = (float)GetRandomValue(10, 30) / 10.0f; // Próxima decisão em 1-3s
                    j->botMoveDirecao = GetRandomValue(0, 3);                 // Nova direção
                    
                    // Chance de 10% de plantar uma bomba
                    if (GetRandomValue(0, 100) > 90)
                    {
                        AlinharEPlantarBomba(j, gBombas);
                        
                        // --- TRANSIÇÃO: Mudar para FUGIR ---
                        j->botState = BOT_STATE_FLEEING;
                        j->botStateTimer = 2.0f; // Foge por 2 segundos
                        j->botMoveDirecao = GetRandomValue(0, 3); // Escolhe uma direção de fuga
                    }
                }
                break; // Fim do case WANDERING
            }
            
            case BOT_STATE_FLEEING:
            {
                // Timer de fuga
                if (j->botStateTimer <= 0.0f)
                {
                    // --- TRANSIÇÃO: Voltar a passear ---
                    j->botState = BOT_STATE_WANDERING;
                    j->botStateTimer = (float)GetRandomValue(10, 30) / 10.0f; // Próxima decisão
                }
                break; // Fim do case FLEEING
            }
        }

        // --- Lógica de Movimento (Comum a ambos os estados) ---
        switch (j->botMoveDirecao)
        {
            case 0: MoverJogadorY(j, -j->velocidade); break; // Cima
            case 1: MoverJogadorY(j, j->velocidade);  break; // Baixo
            case 2: MoverJogadorX(j, -j->velocidade); break; // Esquerda
            case 3: MoverJogadorX(j, j->velocidade);  break; // Direita
        }

        // --- Detecção de "Preso" ---
        // Se a posição não mudou, o bot bateu numa parede.
        if (posAntes.x == j->pos.x && posAntes.y == j->pos.y)
        {
            // Se estava a tentar fugir, escolhe IMEDIATAMENTE outra direção
            if (j->botState == BOT_STATE_FLEEING) {
                j->botMoveDirecao = GetRandomValue(0, 3);
            } 
            // Se estava a passear, antecipa a próxima decisão
            else {
                j->botStateTimer = 0; // Força uma nova decisão no próximo frame
            }
        }
    }
    // --- LÓGICA HUMANA (Sem alteração) ---
    else
    {
        float dx = 0.0f, dy = 0.0f;
        if (IsKeyDown(keyUp)) dy = -j->velocidade;
        if (IsKeyDown(keyDown)) dy = j->velocidade;
        if (IsKeyDown(keyLeft)) dx = -j->velocidade;
        if (IsKeyDown(keyRight)) dx = j->velocidade;
        
        MoverJogadorX(j, dx);
        MoverJogadorY(j, dy);
        
        if (IsKeyPressed(keyBomb))
        {
            AlinharEPlantarBomba(j, gBombas);
        }
    }
}


void DesenharJogador(const Jogador* j)
{
    if (!j->vivo) return; // Se está morto, não desenha
    
    Rectangle destRec = { j->pos.x, j->pos.y, TILE_SIZE, TILE_SIZE };
    Rectangle sourceRec = { 0.0f, 0.0f, (float)j->textura.width, (float)j->textura.height };
    Vector2 origin = { 0, 0 };

    DrawTexturePro(j->textura, sourceRec, destRec, origin, 0.0f, WHITE);
}

void DestruirJogador(Jogador* j)
{
    UnloadTexture(j->textura);
}