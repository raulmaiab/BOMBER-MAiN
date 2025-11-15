#include "jogador.h"
#include "raylib.h" 
#include "raymath.h" // Para Vector2Distance
#include "mapa.h"   
#include "bomba.h" 
#include <stdio.h> 
#include <float.h> // Para FLT_MAX
#include <stdlib.h> // Para abs()

#define COLLISION_MARGIN 4.0f 
#define BOMBA_COOLDOWN_TEMPO 2.5f 

// --- Funções Ajudantes (Sem alteração) ---
static void AlinharEPlantarBomba(Jogador* j, NodeBombas *gBombas) {
    float centerX = j->pos.x + (TILE_SIZE / 2.0f);
    float centerY = j->pos.y + (TILE_SIZE / 2.0f);
    int gridX = (int)(centerX / TILE_SIZE);
    int gridY = (int)(centerY / TILE_SIZE);
    Vector2 posBombaAlinhada = { (float)gridX * TILE_SIZE, (float)gridY * TILE_SIZE };
    PlantarBomba(gBombas, posBombaAlinhada);
}
static bool IsBombAt(NodeBombas *gBombas, int gridX, int gridY) {
    for (int i = 0; i < gBombas->quantidade; i++) {
        if (!gBombas->bombas[i].ativa) continue;
        Vector2 bombGridPos = GetGridPosFromPixels(gBombas->bombas[i].posicao);
        if ((int)bombGridPos.x == gridX && (int)bombGridPos.y == gridY) { return true; }
    }
    return false; 
}
static void MoverJogadorX(Jogador *j, float delta, NodeBombas *gBombas) {
    if (delta == 0) return;
    Vector2 playerCenter = { j->pos.x + TILE_SIZE/2.0f, j->pos.y + TILE_SIZE/2.0f };
    Vector2 playerGridPos = GetGridPosFromPixels(playerCenter);
    Vector2 posTentativa = j->pos;
    posTentativa.x += delta;
    float cX = (delta > 0) ? (posTentativa.x+TILE_SIZE-1-COLLISION_MARGIN) : (posTentativa.x+COLLISION_MARGIN);
    float cY1 = j->pos.y + COLLISION_MARGIN; 
    float cY2 = j->pos.y + TILE_SIZE - 1 - COLLISION_MARGIN; 
    Vector2 gPos1 = GetGridPosFromPixels((Vector2){cX, cY1});
    Vector2 gPos2 = GetGridPosFromPixels((Vector2){cX, cY2});
    bool b1 = IsBombAt(gBombas, (int)gPos1.x, (int)gPos1.y);
    bool p1 = ((int)gPos1.x == (int)playerGridPos.x && (int)gPos1.y == (int)playerGridPos.y);
    bool t1 = (GetTileTipo((int)gPos1.x, (int)gPos1.y) == TILE_EMPTY) && (!b1 || p1);
    bool b2 = IsBombAt(gBombas, (int)gPos2.x, (int)gPos2.y);
    bool p2 = ((int)gPos2.x == (int)playerGridPos.x && (int)gPos2.y == (int)playerGridPos.y);
    bool t2 = (GetTileTipo((int)gPos2.x, (int)gPos2.y) == TILE_EMPTY) && (!b2 || p2);
    if (t1 && t2) { j->pos.x = posTentativa.x; }
}
static void MoverJogadorY(Jogador *j, float delta, NodeBombas *gBombas) {
    if (delta == 0) return;
    Vector2 playerCenter = { j->pos.x + TILE_SIZE/2.0f, j->pos.y + TILE_SIZE/2.0f };
    Vector2 playerGridPos = GetGridPosFromPixels(playerCenter);
    Vector2 posTentativa = j->pos;
    posTentativa.y += delta;
    float cX1 = j->pos.x + COLLISION_MARGIN; 
    float cX2 = j->pos.x + TILE_SIZE - 1 - COLLISION_MARGIN; 
    float cY = (delta > 0) ? (posTentativa.y+TILE_SIZE-1-COLLISION_MARGIN) : (posTentativa.y+COLLISION_MARGIN);
    Vector2 gPos1 = GetGridPosFromPixels((Vector2){cX1, cY});
    Vector2 gPos2 = GetGridPosFromPixels((Vector2){cX2, cY});
    bool b1 = IsBombAt(gBombas, (int)gPos1.x, (int)gPos1.y);
    bool p1 = ((int)gPos1.x == (int)playerGridPos.x && (int)gPos1.y == (int)playerGridPos.y);
    bool t1 = (GetTileTipo((int)gPos1.x, (int)gPos1.y) == TILE_EMPTY) && (!b1 || p1);
    bool b2 = IsBombAt(gBombas, (int)gPos2.x, (int)gPos2.y);
    bool p2 = ((int)gPos2.x == (int)playerGridPos.x && (int)gPos2.y == (int)playerGridPos.y);
    bool t2 = (GetTileTipo((int)gPos2.x, (int)gPos2.y) == TILE_EMPTY) && (!b2 || p2);
    if (t1 && t2) { j->pos.y = posTentativa.y; }
}
// --- Fim Funções Ajudantes ---


// --- CriarJogador (Sem alteração) ---
Jogador CriarJogador(Vector2 posInicial, const char* pastaSprites, bool ehBot)
{
    Jogador j;
    j.pos = posInicial; j.velocidade = 2.5f; j.vivo = true;
    char pathBuffer[256]; 
    sprintf(pathBuffer, "%s/andando.png", pastaSprites); j.texParado = LoadTexture(pathBuffer);
    sprintf(pathBuffer, "%s/costas1.png", pastaSprites); j.texCima[0] = LoadTexture(pathBuffer);
    sprintf(pathBuffer, "%s/costas2.png", pastaSprites); j.texCima[1] = LoadTexture(pathBuffer);
    sprintf(pathBuffer, "%s/costas3.png", pastaSprites); j.texCima[2] = LoadTexture(pathBuffer);
    j.texBaixo[0] = j.texParado; j.texBaixo[1] = j.texParado; j.texBaixo[2] = j.texParado; 
    sprintf(pathBuffer, "%s/esquerda1.png", pastaSprites); j.texEsquerda[0] = LoadTexture(pathBuffer);
    sprintf(pathBuffer, "%s/esquerda2.png", pastaSprites); j.texEsquerda[1] = LoadTexture(pathBuffer);
    sprintf(pathBuffer, "%s/esquerda3.png", pastaSprites); j.texEsquerda[2] = LoadTexture(pathBuffer);
    sprintf(pathBuffer, "%s/direita1.png", pastaSprites); j.texDireita[0] = LoadTexture(pathBuffer);
    sprintf(pathBuffer, "%s/direita2.png", pastaSprites); j.texDireita[1] = LoadTexture(pathBuffer);
    sprintf(pathBuffer, "%s/direita3.png", pastaSprites); j.texDireita[2] = LoadTexture(pathBuffer);
    j.currentFrame = 0; j.frameTimer = 0.0f; j.currentDir = DIR_BAIXO; 
    j.ehBot = ehBot;
    if (j.ehBot) {
        j.botState = BOT_STATE_WANDERING; 
        j.botStateTimer = (float)GetRandomValue(5, 20) / 10.0f; 
        j.botMoveDirecao = GetRandomValue(0, 4); 
    } else {
        j.botState = BOT_STATE_WANDERING; 
        j.botStateTimer = 1.0f; 
        j.botMoveDirecao = 4; 
    }
    j.botLastBombPos = (Vector2){0,0};
    j.bombaCooldown = 0.0f; 
    return j;
}

// --- ATUALIZADO: AtualizarJogador (IA v14) ---
void AtualizarJogador(Jogador* j, int keyUp, int keyDown, int keyLeft, int keyRight, int keyBomb, 
                      NodeBombas *gBombas, float deltaTime, 
                      Jogador* targetHuman1, Jogador* targetHuman2)
{
    if (!j->vivo) return;
    if (j->bombaCooldown > 0.0f) { j->bombaCooldown -= deltaTime; }
    j->botStateTimer -= deltaTime; 

    if (j->ehBot)
    {
        Vector2 posAntes = j->pos; 
        Jogador* target = NULL;
        float dist1 = FLT_MAX; float dist2 = FLT_MAX;
        if (targetHuman1 && targetHuman1->vivo) dist1 = Vector2Distance(j->pos, targetHuman1->pos);
        if (targetHuman2 && targetHuman2->vivo) dist2 = Vector2Distance(j->pos, targetHuman2->pos);
        if (dist1 < dist2) target = targetHuman1;
        else if (dist2 < FLT_MAX) target = targetHuman2;
        
        switch (j->botState)
        {
            // --- ATUALIZADO: Estado WANDERING ---
            case BOT_STATE_WANDERING:
            {
                if (j->bombaCooldown <= 0.0f) 
                {
                    Vector2 myGridPos = GetGridPosFromPixels(j->pos);
                    int gridX = (int)myGridPos.x;
                    int gridY = (int)myGridPos.y;

                    // --- 1. Constraints (Verificações de Segurança) ---

                    // C1: Rota de Fuga
                    int safeDir = -1;
                    if (GetTileTipo(gridX, gridY - 1) == TILE_EMPTY) safeDir = 0; 
                    else if (GetTileTipo(gridX, gridY + 1) == TILE_EMPTY) safeDir = 1; 
                    else if (GetTileTipo(gridX - 1, gridY) == TILE_EMPTY) safeDir = 2; 
                    else if (GetTileTipo(gridX + 1, gridY) == TILE_EMPTY) safeDir = 3; 

                    // C2: Vértices
                    bool isAtExtremity = false;
                    if ((gridX == 1 && gridY == 1) || (gridX == 1 && gridY == 2) || (gridX == 2 && gridY == 1)) { isAtExtremity = true; }
                    else if ((gridX == MAP_GRID_WIDTH - 2 && gridY == 1) || (gridX == MAP_GRID_WIDTH - 3 && gridY == 1) || (gridX == MAP_GRID_WIDTH - 2 && gridY == 2)) { isAtExtremity = true; }
                    else if ((gridX == 1 && gridY == MAP_GRID_HEIGHT - 2) || (gridX == 1 && gridY == MAP_GRID_HEIGHT - 3) || (gridX == 2 && gridY == MAP_GRID_HEIGHT - 2)) { isAtExtremity = true; }
                    else if ((gridX == MAP_GRID_WIDTH - 2 && gridY == MAP_GRID_HEIGHT - 2) || (gridX == MAP_GRID_WIDTH - 3 && gridY == MAP_GRID_HEIGHT - 2) || (gridX == MAP_GRID_WIDTH - 2 && gridY == MAP_GRID_HEIGHT - 3)) { isAtExtremity = true; }
                    
                    // C3 (NOVO): Perto de wallb.png
                    bool isNearDestructible = false;
                    if (GetTileTipo(gridX, gridY - 1) == TILE_DESTRUCTIBLE) isNearDestructible = true;
                    else if (GetTileTipo(gridX, gridY + 1) == TILE_DESTRUCTIBLE) isNearDestructible = true;
                    else if (GetTileTipo(gridX - 1, gridY) == TILE_DESTRUCTIBLE) isNearDestructible = true;
                    else if (GetTileTipo(gridX + 1, gridY) == TILE_DESTRUCTIBLE) isNearDestructible = true;

                    // --- 2. Decisão de Ação ---
                    // Se TODOS os constraints de segurança forem verdadeiros
                    if (safeDir != -1 && !isAtExtremity && isNearDestructible)
                    {
                        // O bot está seguro E tem um motivo (quebrar o bloco).
                        // Ele pode plantar. (A lógica de caçar o jogador torna-se
                        // secundária, pois quebrar blocos é a prioridade).
                        
                        AlinharEPlantarBomba(j, gBombas); 
                        j->bombaCooldown = BOMBA_COOLDOWN_TEMPO;
                        j->botState = BOT_STATE_FLEEING;
                        j->botStateTimer = BOMBA_COOLDOWN_TEMPO; 
                        j->botMoveDirecao = safeDir; 
                        j->botLastBombPos = (Vector2){ (float)gridX * TILE_SIZE, (float)gridY * TILE_SIZE };
                        break; // Sai do switch (pula o movimento)
                    }

                    // (Opcional: Se quiser que ele cace MESMO que não haja blocos,
                    // pode adicionar a lógica de "caça" aqui fora do if 'isNearDestructible')
                    
                } // Fim do check de cooldown

                // --- 3. Movimento Padrão (Vaguear) ---
                if (j->botStateTimer <= 0.0f)
                {
                    j->botStateTimer = (float)GetRandomValue(5, 20) / 10.0f;
                    j->botMoveDirecao = GetRandomValue(0, 4); 
                }
                break; 
            }
            // --- FIM DA ATUALIZAÇÃO ---

            // --- ATUALIZADO: Estado FLEEING (Fugindo) ---
            case BOT_STATE_FLEEING:
            {
                Vector2 myGridPos = GetGridPosFromPixels(j->pos);
                Vector2 bombGridPos = GetGridPosFromPixels(j->botLastBombPos);
                int dist = abs((int)myGridPos.x - (int)bombGridPos.x) + abs((int)myGridPos.y - (int)bombGridPos.y);
                
                // --- CORREÇÃO AQUI ---
                int bombRange = 1; // (Assume que o range é 1)
                int safeDist = bombRange + 3; // (1 + 3 = 4)
                // --- FIM DA CORREÇÃO ---

                // Se a distância for 4 ou mais, estamos seguros.
                if (dist >= safeDist)
                {
                    j->botState = BOT_STATE_HOLDING; 
                    j->botStateTimer = BOMBA_COOLDOWN_TEMPO - 0.5f; 
                    j->botMoveDirecao = 4; // PARAR
                }
                
                break;
            }
            // --- FIM DA ATUALIZAÇÃO ---

            // HOLDING (Sem alteração)
            case BOT_STATE_HOLDING:
            {
                j->botMoveDirecao = 4; 
                if (j->botStateTimer <= 0.0f)
                {
                    j->botState = BOT_STATE_WANDERING; 
                    j->botStateTimer = 1.0f; 
                    j->botMoveDirecao = GetRandomValue(0, 4); 
                }
                break;
            }
        }

        // 3. Execução do Movimento (Sem alteração)
        j->currentDir = DIR_PARADO; 
        switch (j->botMoveDirecao)
        {
            case 0: MoverJogadorY(j, -j->velocidade, gBombas); j->currentDir = DIR_CIMA; break; 
            case 1: MoverJogadorY(j, j->velocidade, gBombas);  j->currentDir = DIR_BAIXO; break; 
            case 2: MoverJogadorX(j, -j->velocidade, gBombas); j->currentDir = DIR_ESQUERDA; break; 
            case 3: MoverJogadorX(j, j->velocidade, gBombas);  j->currentDir = DIR_DIREITA; break; 
            case 4: j->currentFrame = 0; break; 
        }

        if (posAntes.x == j->pos.x && posAntes.y == j->pos.y && j->botMoveDirecao != 4)
        {
            if (j->botState == BOT_STATE_FLEEING) {
                j->botMoveDirecao = GetRandomValue(0, 3);
            }
            else if (j->botState == BOT_STATE_WANDERING) {
                j->botStateTimer = 0.0f; 
            }
        }
    }
    // --- LÓGICA HUMANA (Sem alteração) ---
    else
    {
        float dx = 0.0f, dy = 0.0f;
        if (IsKeyDown(keyUp)) { dy = -j->velocidade; j->currentDir = DIR_CIMA; }
        else if (IsKeyDown(keyDown)) { dy = j->velocidade; j->currentDir = DIR_BAIXO; }
        else if (IsKeyDown(keyLeft)) { dx = -j->velocidade; j->currentDir = DIR_ESQUERDA; }
        else if (IsKeyDown(keyRight)) { dx = j->velocidade; j->currentDir = DIR_DIREITA; }
        else {
            j->currentDir = DIR_PARADO;
            j->currentFrame = 0; 
        }
        MoverJogadorX(j, dx, gBombas);
        MoverJogadorY(j, dy, gBombas);
        if (IsKeyPressed(keyBomb) && j->bombaCooldown <= 0.0f)
        {
            AlinharEPlantarBomba(j, gBombas);
            j->bombaCooldown = BOMBA_COOLDOWN_TEMPO; 
        }
    }
    
    // Lógica de Animação (Sem alteração)
    if (j->currentDir != DIR_PARADO) {
        j->frameTimer += deltaTime;
        if (j->frameTimer >= ANIM_FRAME_SPEED) {
            j->frameTimer = 0.0f;
            j->currentFrame = (j->currentFrame + 1) % NUM_FRAMES_ANDAR; 
        }
    } else {
        j->currentFrame = 0;
    }
}

// --- DesenharJogador (Sem alteração) ---
void DesenharJogador(const Jogador* j)
{
    if (!j->vivo) return; 
    Rectangle destRec = { j->pos.x, j->pos.y, TILE_SIZE, TILE_SIZE };
    Vector2 origin = { 0, 0 };
    Texture2D texToDraw;
    switch (j->currentDir) {
        case DIR_CIMA:    texToDraw = j->texCima[j->currentFrame]; break;
        case DIR_BAIXO:   texToDraw = j->texBaixo[j->currentFrame]; break;
        case DIR_ESQUERDA:texToDraw = j->texEsquerda[j->currentFrame]; break;
        case DIR_DIREITA: texToDraw = j->texDireita[j->currentFrame]; break;
        case DIR_PARADO:  texToDraw = j->texParado; break; 
        default:          texToDraw = j->texParado; break;
    }
    if (texToDraw.id == 0) { texToDraw = j->texParado; }
    Rectangle sourceRec = { 0.0f, 0.0f, (float)texToDraw.width, (float)texToDraw.height };
    DrawTexturePro(texToDraw, sourceRec, destRec, origin, 0.0f, WHITE);
}

// --- DestruirJogador (Sem alteração) ---
void DestruirJogador(Jogador* j)
{
    UnloadTexture(j->texParado);
    for (int i = 0; i < NUM_FRAMES_ANDAR; i++) {
        if (j->texCima[i].id > 0 && j->texCima[i].id != j->texParado.id) UnloadTexture(j->texCima[i]);
        if (j->texBaixo[i].id > 0 && j->texBaixo[i].id != j->texParado.id) UnloadTexture(j->texBaixo[i]);
        if (j->texEsquerda[i].id > 0 && j->texEsquerda[i].id != j->texParado.id) UnloadTexture(j->texEsquerda[i]);
        if (j->texDireita[i].id > 0 && j->texDireita[i].id != j->texParado.id) UnloadTexture(j->texDireita[i]);
    }
}