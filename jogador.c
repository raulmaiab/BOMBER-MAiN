#include "jogador.h"
#include "raylib.h" 
#include "mapa.h"   // <-- ADICIONADO: Precisamos do TILE_SIZE

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

    // --- NOVO: Definir o frame inicial ---
    // Isto é um "chute" olhando para a sua spritesheet (SpriteAzul.png)
    // O primeiro frame da animação "Walk" parece estar nestas coordenadas:
    // x: 8, y: 24, largura: 16, altura: 24
    // Você talvez precise ajustar estes valores!
    j.frameRec = (Rectangle){ 8.0f, 24.0f, 16.0f, 24.0f };
    
    return j;
}

// ATUALIZADO: Com lógica de colisão (sem alteração da última vez)
void AtualizarJogador(Jogador* j, int keyUp, int keyDown, int keyLeft, int keyRight)
{
    if (!j->vivo) return;

    Vector2 posTentativa = j->pos;
    Vector2 posFinal = j->pos; 

    // --- Movimento X ---
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
    
    // --- Movimento Y ---
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
}


// --- ATUALIZADO: Usando DrawTexturePro ---
void DesenharJogador(const Jogador* j)
{
    if (!j->vivo) return;
    
    // Onde o jogador será desenhado (destino)
    // Ele será desenhado na posição X/Y do jogador,
    // e com o tamanho de um TILE_SIZE (40x40)
    Rectangle destRec = { j->pos.x, j->pos.y, TILE_SIZE, TILE_SIZE };
    
    // A origem (para rotação/escala, 0,0 é o canto)
    Vector2 origin = { 0, 0 };

    // Desenha a parte da textura (j->frameRec) no destino (destRec)
    DrawTexturePro(
        j->textura,  // A spritesheet inteira (ex: SpriteAzul.png)
        j->frameRec, // O retângulo FONTE (ex: {8, 24, 16, 24})
        destRec,     // O retângulo DESTINO (ex: {100, 100, 40, 40})
        origin,      // Origem da rotação
        0.0f,        // Rotação
        WHITE        // Cor (sem filtro)
    );
}


void DestruirJogador(Jogador* j)
{
    UnloadTexture(j->textura);
}