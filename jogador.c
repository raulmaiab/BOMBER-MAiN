#include "jogador.h"
#include "raylib.h" 
#include "raymath.h" 
#include "mapa.h"   
#include "bomba.h" 
#include "extras.h" 
#include <stdio.h> 
#include <float.h> 
#include <stdlib.h> 
#include <string.h> 

#define COLLISION_MARGIN 4.0f 
#define BOMBA_COOLDOWN_TEMPO 2.5f 

// --- Funções Ajudantes (Sem alteração) ---
// ... (SnapToGrid, AlinharEPlantarBomba, IsBombAt, IsInCorner, IsDirectionSafe, IsDirectionWalkable, MoverJogadorX, MoverJogadorY) ...

// Funções Ajudantes
static void SnapToGrid(Jogador* j, int direction) {
    Vector2 center = { j->pos.x + TILE_SIZE/2.0f, j->pos.y + TILE_SIZE/2.0f };
    Vector2 gridPos = GetGridPosFromPixels(center);
    
    if (direction == 0 || direction == 1) { 
        j->pos.x = gridPos.x * TILE_SIZE;
    }
    else if (direction == 2 || direction == 3) { 
        j->pos.y = gridPos.y * TILE_SIZE;
    }
}

static void AlinharEPlantarBomba(Jogador* j, NodeBombas *gBombas) {

    // checa limite do jogador antes de tentar plantar
    if (j->bombasAtivas >= j->bombLimit) return;

    float centerX = j->pos.x + (TILE_SIZE / 2.0f);
    float centerY = j->pos.y + (TILE_SIZE / 2.0f);
    int gridX = (int)(centerX / TILE_SIZE);
    int gridY = (int)(centerY / TILE_SIZE);
    Vector2 posBombaAlinhada = { (float)gridX * TILE_SIZE, (float)gridY * TILE_SIZE };
    PlantarBomba(gBombas, posBombaAlinhada, j->bombRange, j); 

    
}

static bool IsBombAt(NodeBombas *gBombas, int gridX, int gridY) {
    for (int i = 0; i < gBombas->quantidade; i++) {
        if (!gBombas->bombas[i].ativa) continue;
        Vector2 bombGridPos = GetGridPosFromPixels(gBombas->bombas[i].posicao);
        if ((int)bombGridPos.x == gridX && (int)bombGridPos.y == gridY) { return true; }
    }
    return false; 
}

static bool IsInCorner(int x, int y) {
    int W = MAP_GRID_WIDTH;
    int H = MAP_GRID_HEIGHT;
    if ((x == 1 && y == 1) || (x == 2 && y == 1) || (x == 1 && y == 2)) return true;
    if ((x == W-2 && y == 1) || (x == W-3 && y == 1) || (x == W-2 && y == 2)) return true;
    if ((x == 1 && y == H-2) || (x == 2 && y == H-2) || (x == 1 && y == H-3)) return true;
    if ((x == W-2 && y == H-2) || (x == W-3 && y == H-2) || (x == W-2 && y == H-3)) return true;
    return false;
}

static bool IsDirectionSafe(int startX, int startY, int dir, NodeBombas *gBombas) {
    int dx = 0, dy = 0;
    if (dir == 0) dy = -1; else if (dir == 1) dy = 1; else if (dir == 2) dx = -1; else if (dir == 3) dx = 1;  
    int checkX = startX + dx;
    int checkY = startY + dy;
    if (checkX < 0 || checkX >= MAP_GRID_WIDTH || checkY < 0 || checkY >= MAP_GRID_HEIGHT) return false;
    if (GetTileTipo(checkX, checkY) != TILE_EMPTY) return false;
    if (IsBombAt(gBombas, checkX, checkY)) return false;
    return true;
}

static bool IsDirectionWalkable(int startX, int startY, int dir) {
    int dx = 0, dy = 0;
    if (dir == 0) dy = -1; else if (dir == 1) dy = 1; else if (dir == 2) dx = -1; else if (dir == 3) dx = 1;  
    int checkX = startX + dx;
    int checkY = startY + dy;
    if (checkX < 0 || checkX >= MAP_GRID_WIDTH || checkY < 0 || checkY >= MAP_GRID_HEIGHT) return false;
    if (GetTileTipo(checkX, checkY) != TILE_EMPTY) return false; 
    return true;
}

static void MoverJogadorX(Jogador *j, float delta, NodeBombas *gBombas, bool ignoreBombs) {
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

    bool b1 = ignoreBombs ? false : IsBombAt(gBombas, (int)gPos1.x, (int)gPos1.y);
    bool p1 = ((int)gPos1.x == (int)playerGridPos.x && (int)gPos1.y == (int)playerGridPos.y);
    bool t1 = (GetTileTipo((int)gPos1.x, (int)gPos1.y) == TILE_EMPTY) && (!b1 || p1);
    
    bool b2 = ignoreBombs ? false : IsBombAt(gBombas, (int)gPos2.x, (int)gPos2.y);
    bool p2 = ((int)gPos2.x == (int)playerGridPos.x && (int)gPos2.y == (int)playerGridPos.y);
    bool t2 = (GetTileTipo((int)gPos2.x, (int)gPos2.y) == TILE_EMPTY) && (!b2 || p2);

    if (t1 && t2) { j->pos.x = posTentativa.x; }
}

static void MoverJogadorY(Jogador *j, float delta, NodeBombas *gBombas, bool ignoreBombs) {
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
    
    bool b1 = ignoreBombs ? false : IsBombAt(gBombas, (int)gPos1.x, (int)gPos1.y);
    bool p1 = ((int)gPos1.x == (int)playerGridPos.x && (int)gPos1.y == (int)playerGridPos.y);
    bool t1 = (GetTileTipo((int)gPos1.x, (int)gPos1.y) == TILE_EMPTY) && (!b1 || p1);
    
    bool b2 = ignoreBombs ? false : IsBombAt(gBombas, (int)gPos2.x, (int)gPos2.y);
    bool p2 = ((int)gPos2.x == (int)playerGridPos.x && (int)gPos2.y == (int)playerGridPos.y);
    bool t2 = (GetTileTipo((int)gPos2.x, (int)gPos2.y) == TILE_EMPTY) && (!b2 || p2);

    if (t1 && t2) { j->pos.y = posTentativa.y; }
}

// --- CriarJogador ---
Jogador CriarJogador(Vector2 posInicial, const char* pastaSprites, bool ehBot)
{
    Jogador j;
    j.pos = posInicial; j.velocidade = 2.5f; j.vivo = true;
    
    // Adicionar o nome do sprite/cor
    strncpy(j.spriteName, pastaSprites, MAX_SPRITE_NAME_LENGTH - 1); // <<< PREENCHIMENTO DO CAMPO
    j.spriteName[MAX_SPRITE_NAME_LENGTH - 1] = '\0';
    
    //--- Design e Movimentacao
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
    
    //--- Mecanica dos BOTs
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

    //--- Controle dos extras
    j.bombRange = 1;
    j.temDefesa = false; j.timerDefesa = 0.0f;
    j.temVelocidade = false; j.timerVelocidade = 0.0f;
    j.bombLimit = 1;
    j.bombasAtivas = 0;

    return j;
}

// --- AtualizarJogador 
void AtualizarJogador(Jogador* j, int keyUp, int keyDown, int keyLeft, int keyRight, int keyBomb, 
                      NodeBombas *gBombas, float deltaTime, 
                      Jogador* targetHuman1, Jogador* targetHuman2)
{
    if (!j->vivo) return;

    float speedMultiplier = 1.0f;
    if (j->temVelocidade) {
        j->timerVelocidade -= deltaTime;
        speedMultiplier = 1.5f; 
        if (j->timerVelocidade <= 0.0f) j->temVelocidade = false;
    }
    if (j->temDefesa) {
        j->timerDefesa -= deltaTime;
        if (j->timerDefesa <= 0.0f) j->temDefesa = false;
    }

    float currentSpeed = j->velocidade * speedMultiplier; 

    if (j->bombaCooldown > 0.0f) { j->bombaCooldown -= deltaTime; }
    j->botStateTimer -= deltaTime; 

    if (j->ehBot)
    {
        Vector2 posAntes = j->pos; 
        bool acabouDePlantar = false;

        switch (j->botState)
        {
            // --- WANDERING ---
            case BOT_STATE_WANDERING:
            {
                bool plantouBomba = false;

                if (j->bombaCooldown <= 0.0f) 
                {
                    Vector2 myGridPos = GetGridPosFromPixels(j->pos);
                    int gridX = (int)myGridPos.x;
                    int gridY = (int)myGridPos.y;

                    if (!IsInCorner(gridX, gridY))
                    {
                        bool triggerAcionado = false;
                        if (GetTileTipo(gridX, gridY - 1) == TILE_DESTRUCTIBLE || 
                            GetTileTipo(gridX, gridY + 1) == TILE_DESTRUCTIBLE ||
                            GetTileTipo(gridX - 1, gridY) == TILE_DESTRUCTIBLE || 
                            GetTileTipo(gridX + 1, gridY) == TILE_DESTRUCTIBLE) {
                            triggerAcionado = true;
                        }

                        if (triggerAcionado)
                        {
                            int escapeDir = -1;
                            int oposto = -1;
                            if (j->botMoveDirecao == 0) oposto = 1;      
                            else if (j->botMoveDirecao == 1) oposto = 0; 
                            else if (j->botMoveDirecao == 2) oposto = 3; 
                            else if (j->botMoveDirecao == 3) oposto = 2; 

                            // Usa IsDirectionSafe (sem  bomba) para PLANEJAR
                            if (oposto != -1 && IsDirectionSafe(gridX, gridY, oposto, gBombas)) {
                                escapeDir = oposto;
                            }
                            else {
                                if (IsDirectionSafe(gridX, gridY, j->botMoveDirecao, gBombas)) escapeDir = j->botMoveDirecao; 
                                else if (IsDirectionSafe(gridX, gridY, 0, gBombas)) escapeDir = 0;
                                else if (IsDirectionSafe(gridX, gridY, 1, gBombas)) escapeDir = 1;
                                else if (IsDirectionSafe(gridX, gridY, 2, gBombas)) escapeDir = 2;
                                else if (IsDirectionSafe(gridX, gridY, 3, gBombas)) escapeDir = 3;
                            }

                            if (escapeDir != -1) 
                            {
                                AlinharEPlantarBomba(j, gBombas); 
                                j->bombaCooldown = BOMBA_COOLDOWN_TEMPO;
                                j->botState = BOT_STATE_FLEEING;
                                j->botMoveDirecao = escapeDir; 
                                j->botLastBombPos = (Vector2){ (float)gridX * TILE_SIZE, (float)gridY * TILE_SIZE };
                                
                                SnapToGrid(j, escapeDir); 
                                plantouBomba = true;
                                acabouDePlantar = true; 
                            }
                        }
                    }
                }

                // Movimento Normal
                if (!plantouBomba && j->botStateTimer <= 0.0f)
                {
                    int moveDir = -1;
                    Vector2 myGridPos = GetGridPosFromPixels(j->pos);
                    int gx = (int)myGridPos.x;
                    int gy = (int)myGridPos.y;

                    Vector2 itemPos;
                    if (GetExtraMaisProximo(j->pos, TILE_SIZE * 6, &itemPos)) {
                        float dx = itemPos.x - j->pos.x;
                        float dy = itemPos.y - j->pos.y;
                        int pH = (dx > 0) ? 3 : 2;
                        int pV = (dy > 0) ? 1 : 0;
                        
                        if (fabs(dx) > fabs(dy)) {
                            if (IsDirectionSafe(gx, gy, pH, gBombas)) moveDir = pH;
                            else if (IsDirectionSafe(gx, gy, pV, gBombas)) moveDir = pV;
                        } else {
                            if (IsDirectionSafe(gx, gy, pV, gBombas)) moveDir = pV;
                            else if (IsDirectionSafe(gx, gy, pH, gBombas)) moveDir = pH;
                        }
                        j->botStateTimer = 0.2f;
                    }

                    if (moveDir == -1) {
                        int validDirs[4];
                        int count = 0;
                        if (IsDirectionSafe(gx, gy, 0, gBombas)) validDirs[count++] = 0;
                        if (IsDirectionSafe(gx, gy, 1, gBombas)) validDirs[count++] = 1;
                        if (IsDirectionSafe(gx, gy, 2, gBombas)) validDirs[count++] = 2;
                        if (IsDirectionSafe(gx, gy, 3, gBombas)) validDirs[count++] = 3;

                        if (count > 0) {
                            if (j->botMoveDirecao >= 0 && j->botMoveDirecao <= 3 && 
                                IsDirectionSafe(gx, gy, j->botMoveDirecao, gBombas) && 
                                GetRandomValue(0, 100) > 20) { 
                                moveDir = j->botMoveDirecao;
                            } else {
                                moveDir = validDirs[GetRandomValue(0, count - 1)];
                            }
                            j->botStateTimer = (float)GetRandomValue(5, 15) / 10.0f;
                        } else {
                            moveDir = 4; 
                            j->botStateTimer = 0.1f;
                        }
                    }

                    if (moveDir != -1) {
                        bool mudouEixo = (j->botMoveDirecao >= 2 && moveDir <= 1) || (j->botMoveDirecao <= 1 && moveDir >= 2);
                        if (mudouEixo) SnapToGrid(j, moveDir);
                        j->botMoveDirecao = moveDir;
                    }
                }
                break; 
            }
            
            // --- FLEEING (Fuga com Correção de Travamento) ---
            case BOT_STATE_FLEEING:
            {
                Vector2 myGridPos = GetGridPosFromPixels(j->pos);
                Vector2 bombGridPos = GetGridPosFromPixels(j->botLastBombPos);
                
                int targetDist = j->bombRange + 3; 
                int currentDist = abs((int)myGridPos.x - (int)bombGridPos.x) + abs((int)myGridPos.y - (int)bombGridPos.y);

                if (currentDist >= targetDist) {
                    j->botState = BOT_STATE_HOLDING;
                    j->botStateTimer = BOMBA_COOLDOWN_TEMPO - 0.5f; 
                    j->botMoveDirecao = 4; 
                }
                // Anti-stuck PÂNICO
                else if (!acabouDePlantar && Vector2Distance(posAntes, j->pos) < 0.05f) 
                {
                    // Se travou no meio da fuga, usa IsDirectionWalkable (IGNORA BOMBAS)
                    
                    int gx = (int)myGridPos.x;
                    int gy = (int)myGridPos.y;
                    int novaDir = -1;
                    
                    // Tenta direção perpendicular primeiro
                    if (j->botMoveDirecao <= 1) { // Vertical
                        if (IsDirectionWalkable(gx, gy, 2)) novaDir = 2;
                        else if (IsDirectionWalkable(gx, gy, 3)) novaDir = 3;
                    } else { // Horizontal
                        if (IsDirectionWalkable(gx, gy, 0)) novaDir = 0;
                        else if (IsDirectionWalkable(gx, gy, 1)) novaDir = 1;
                    }
                    
                    // Se não achou perpendicular, tenta qualquer uma que não seja voltar
                    if (novaDir == -1) {
                        if (IsDirectionWalkable(gx, gy, 0) && j->botMoveDirecao != 1) novaDir = 0;
                        else if (IsDirectionWalkable(gx, gy, 1) && j->botMoveDirecao != 0) novaDir = 1;
                        else if (IsDirectionWalkable(gx, gy, 2) && j->botMoveDirecao != 3) novaDir = 2;
                        else if (IsDirectionWalkable(gx, gy, 3) && j->botMoveDirecao != 2) novaDir = 3;
                    }

                    if (novaDir != -1) {
                        j->botMoveDirecao = novaDir;
                        SnapToGrid(j, novaDir);
                    }
                }
                break;
            }

            case BOT_STATE_HOLDING:
            {
                j->botMoveDirecao = 4; 
                if (j->botStateTimer <= 0.0f)
                {
                    j->botState = BOT_STATE_WANDERING; 
                    j->botStateTimer = 0.5f; 
                    j->botMoveDirecao = GetRandomValue(0, 4); 
                }
                break;
            }
        }

        // --- CONFIGURAÇÃO DE MOVIMENTO ---
        bool ignoreBombs = (j->botState == BOT_STATE_FLEEING);

        j->currentDir = DIR_PARADO; 
        switch (j->botMoveDirecao)
        {
            case 0: MoverJogadorY(j, -currentSpeed, gBombas, ignoreBombs); j->currentDir = DIR_CIMA; break; 
            case 1: MoverJogadorY(j, currentSpeed, gBombas, ignoreBombs);  j->currentDir = DIR_BAIXO; break; 
            case 2: MoverJogadorX(j, -currentSpeed, gBombas, ignoreBombs); j->currentDir = DIR_ESQUERDA; break; 
            case 3: MoverJogadorX(j, currentSpeed, gBombas, ignoreBombs);  j->currentDir = DIR_DIREITA; break; 
            case 4: j->currentFrame = 0; break; 
        }
    }
    // --- LÓGICA HUMANA ---
    else
    {
        float dx = 0.0f, dy = 0.0f;
        bool moveu = false;

        if (IsKeyDown(keyUp)) { dy -= currentSpeed; j->currentDir = DIR_CIMA; moveu = true; }
        if (IsKeyDown(keyDown)) { dy += currentSpeed; j->currentDir = DIR_BAIXO; moveu = true; }
        if (IsKeyDown(keyLeft)) { dx -= currentSpeed; j->currentDir = DIR_ESQUERDA; moveu = true; }
        if (IsKeyDown(keyRight)) { dx += currentSpeed; j->currentDir = DIR_DIREITA; moveu = true; }

        if (!moveu) {
            j->currentDir = DIR_PARADO;
            j->currentFrame = 0;
        }

        MoverJogadorX(j, dx, gBombas, false);
        MoverJogadorY(j, dy, gBombas, false);
        
        if (IsKeyPressed(keyBomb) && j->bombaCooldown <= 0.0f)
        {
            AlinharEPlantarBomba(j, gBombas);
            j->bombaCooldown = BOMBA_COOLDOWN_TEMPO; 
        }
    }
    
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


// --- DesenharJogador 
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
    
    if (texToDraw.id == 0) texToDraw = j->texParado;
    
    Rectangle sourceRec = { 0.0f, 0.0f, (float)texToDraw.width, (float)texToDraw.height };
    
    Color tint = WHITE;
    if (j->temDefesa) tint = SKYBLUE;        
    else if (j->temVelocidade) tint = LIME;  

    DrawTexturePro(texToDraw, sourceRec, destRec, origin, 0.0f, tint);
}

// --- DestruirJogador 
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