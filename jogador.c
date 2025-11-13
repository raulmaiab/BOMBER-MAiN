#include "jogador.h"
#include "raylib.h" 
#include "mapa.h"   
#include "bomba.h" // <-- 1. INCLUÍDO (precisa de PlantarBomba)

Jogador CriarJogador(Vector2 posInicial, const char* imgSprite)
{
    Jogador j;
    j.pos = posInicial;
    j.velocidade = 2.5f; 
    j.textura = LoadTexture(imgSprite);
    if (j.textura.id == 0) {
        TraceLog(LOG_WARNING, "Falha ao carregar textura do jogador: %s", imgSprite);
    }
    j.vivo = true;
    
    return j;
}

// --- 2. FUNÇÃO ATUALIZADA ---
void AtualizarJogador(Jogador* j, int keyUp, int keyDown, int keyLeft, int keyRight, int keyBomb, NodeBombas *gBombas)
{
    if (!j->vivo) return;

    Vector2 posTentativa = j->pos;
    Vector2 posFinal = j->pos; 

    // --- Movimento X (Sem alteração) ---
    posTentativa.x += IsKeyDown(keyRight) ? j->velocidade : (IsKeyDown(keyLeft) ? -j->velocidade : 0);
    float collisionRectX = (IsKeyDown(keyRight)) ? posTentativa.x + TILE_SIZE - 1 : posTentativa.x;
    float collisionRectY = j->pos.y;
    Vector2 gridPos1 = GetGridPosFromPixels((Vector2){collisionRectX, collisionRectY});
    Vector2 gridPos2 = GetGridPosFromPixels((Vector2){collisionRectX, collisionRectY + TILE_SIZE - 1});

    if (GetTileTipo((int)gridPos1.x, (int)gridPos1.y) == TILE_EMPTY &&
        GetTileTipo((int)gridPos2.x, (int)gridPos2.y) == TILE_EMPTY)
    {
        posFinal.x = posTentativa.x; 
    }
    
    // --- Movimento Y (Sem alteração) ---
    posTentativa.y += IsKeyDown(keyDown) ? j->velocidade : (IsKeyDown(keyUp) ? -j->velocidade : 0);
    float collisionRectX_Y = posFinal.x; 
    float collisionRectY_Y = (IsKeyDown(keyDown)) ? posTentativa.y + TILE_SIZE - 1 : posTentativa.y;
    gridPos1 = GetGridPosFromPixels((Vector2){collisionRectX_Y, collisionRectY_Y});
    gridPos2 = GetGridPosFromPixels((Vector2){collisionRectX_Y + TILE_SIZE - 1, collisionRectY_Y});

    if (GetTileTipo((int)gridPos1.x, (int)gridPos1.y) == TILE_EMPTY &&
        GetTileTipo((int)gridPos2.x, (int)gridPos2.y) == TILE_EMPTY)
    {
        posFinal.y = posTentativa.y; 
    }

    j->pos = posFinal;
    
    // --- 3. LÓGICA DE PLANTAR BOMBA ---
    if (IsKeyPressed(keyBomb))
    {
        // Precisamos "alinhar" (snap) a bomba à grelha.
        // Se o jogador está em (92, 95), a bomba deve ir para (90, 90).
        
        // Pega o centro do jogador
        float centerX = j->pos.x + (TILE_SIZE / 2.0f);
        float centerY = j->pos.y + (TILE_SIZE / 2.0f);

        // Converte para a coordenada da grelha (ex: 1, 1)
        int gridX = (int)(centerX / TILE_SIZE);
        int gridY = (int)(centerY / TILE_SIZE);

        // Converte de volta para pixels (ex: 90.0f, 90.0f)
        Vector2 posBombaAlinhada = { (float)gridX * TILE_SIZE, (float)gridY * TILE_SIZE };
        
        PlantarBomba(gBombas, posBombaAlinhada);
    }
    // --- FIM DA LÓGICA ---
}
// --- FIM DA ATUALIZAÇÃO ---


void DesenharJogador(const Jogador* j)
{
    if (!j->vivo) return;
    
    Rectangle destRec = { j->pos.x, j->pos.y, TILE_SIZE, TILE_SIZE };
    Rectangle sourceRec = { 0.0f, 0.0f, (float)j->textura.width, (float)j->textura.height };
    Vector2 origin = { 0, 0 };

    DrawTexturePro(
        j->textura,  
        sourceRec, 
        destRec,     
        origin,      
        0.0f,        
        WHITE        
    );
}

void DestruirJogador(Jogador* j)
{
    UnloadTexture(j->textura);
}