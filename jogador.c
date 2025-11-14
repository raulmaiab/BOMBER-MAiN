#include "jogador.h"
#include "raylib.h" 
#include "mapa.h"   
#include "bomba.h" 
#include <stdio.h> 

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
static void MoverJogadorX(Jogador *j, float delta) {
    if (delta == 0) return;
    Vector2 posTentativa = j->pos;
    posTentativa.x += delta;
    float collisionRectX = (delta > 0) ? (posTentativa.x + TILE_SIZE - 1 - COLLISION_MARGIN) : (posTentativa.x + COLLISION_MARGIN);
    float collisionRectY = j->pos.y + COLLISION_MARGIN; 
    float collisionRectY_End = j->pos.y + TILE_SIZE - 1 - COLLISION_MARGIN; 
    Vector2 gridPos1 = GetGridPosFromPixels((Vector2){collisionRectX, collisionRectY});
    Vector2 gridPos2 = GetGridPosFromPixels((Vector2){collisionRectX, collisionRectY_End});
    if (GetTileTipo((int)gridPos1.x, (int)gridPos1.y) == TILE_EMPTY &&
        GetTileTipo((int)gridPos2.x, (int)gridPos2.y) == TILE_EMPTY)
    { j->pos.x = posTentativa.x; }
}
static void MoverJogadorY(Jogador *j, float delta) {
    if (delta == 0) return;
    Vector2 posTentativa = j->pos;
    posTentativa.y += delta;
    float collisionRectX_Y = j->pos.x + COLLISION_MARGIN; 
    float collisionRectX_Y_End = j->pos.x + TILE_SIZE - 1 - COLLISION_MARGIN; 
    float collisionRectY_Y = (delta > 0) ? (posTentativa.y + TILE_SIZE - 1 - COLLISION_MARGIN) : (posTentativa.y + COLLISION_MARGIN);
    Vector2 gridPos1 = GetGridPosFromPixels((Vector2){collisionRectX_Y, collisionRectY_Y});
    Vector2 gridPos2 = GetGridPosFromPixels((Vector2){collisionRectX_Y_End, collisionRectY_Y});
    if (GetTileTipo((int)gridPos1.x, (int)gridPos1.y) == TILE_EMPTY &&
        GetTileTipo((int)gridPos2.x, (int)gridPos2.y) == TILE_EMPTY)
    { j->pos.y = posTentativa.y; }
}
// --- Fim Funções Ajudantes ---


// --- CriarJogador (Sem alteração) ---
Jogador CriarJogador(Vector2 posInicial, const char* pastaSprites, bool ehBot)
{
    Jogador j;
    j.pos = posInicial;
    j.velocidade = 2.5f; 
    j.vivo = true;
    
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

    j.currentFrame = 0;
    j.frameTimer = 0.0f;
    j.currentDir = DIR_BAIXO; 
    j.ehBot = ehBot;
    j.botState = BOT_STATE_WANDERING; 
    j.botStateTimer = 1.0f; 
    j.botMoveDirecao = 4; // Começa parado
    j.botLastBombPos = (Vector2){0,0};
    j.bombaCooldown = 0.0f; 
    
    return j;
}

// --- ATUALIZADO: AtualizarJogador (IA v5) ---
void AtualizarJogador(Jogador* j, int keyUp, int keyDown, int keyLeft, int keyRight, int keyBomb, NodeBombas *gBombas, float deltaTime)
{
    if (!j->vivo) return;

    if (j->bombaCooldown > 0.0f) {
        j->bombaCooldown -= deltaTime; 
    }
    
    j->botStateTimer -= deltaTime; 

    // --- LÓGICA DO BOT (IA v5) ---
    if (j->ehBot)
    {
        Vector2 posAntes = j->pos; 

        // --- Máquina de Estados ---
        switch (j->botState)
        {
            // --- 1. PASSEANDO ---
            case BOT_STATE_WANDERING:
            {
                if (j->botStateTimer <= 0.0f)
                {
                    j->botStateTimer = (float)GetRandomValue(5, 20) / 10.0f; // 0.5-2.0s
                    j->botMoveDirecao = GetRandomValue(0, 4); // 0-3=Move, 4=Parado

                    if (j->bombaCooldown <= 0.0f && GetRandomValue(0, 100) > 80)
                    {
                        Vector2 myGridPos = GetGridPosFromPixels(j->pos);
                        int gridX = (int)myGridPos.x;
                        int gridY = (int)myGridPos.y;
                        
                        int safeDir = -1;
                        if (GetTileTipo(gridX, gridY - 1) == TILE_EMPTY) safeDir = 0; // Cima
                        else if (GetTileTipo(gridX, gridY + 1) == TILE_EMPTY) safeDir = 1; // Baixo
                        else if (GetTileTipo(gridX - 1, gridY) == TILE_EMPTY) safeDir = 2; // Esquerda
                        else if (GetTileTipo(gridX + 1, gridY) == TILE_EMPTY) safeDir = 3; // Direita

                        if (safeDir != -1) 
                        {
                            float centerX = j->pos.x + (TILE_SIZE / 2.0f);
                            float centerY = j->pos.y + (TILE_SIZE / 2.0f);
                            Vector2 posBombaAlinhada = { (float)((int)(centerX / TILE_SIZE)) * TILE_SIZE, (float)((int)(centerY / TILE_SIZE)) * TILE_SIZE };
                            
                            PlantarBomba(gBombas, posBombaAlinhada);
                            
                            j->bombaCooldown = BOMBA_COOLDOWN_TEMPO;
                            j->botState = BOT_STATE_FLEEING;
                            j->botStateTimer = 0.5f; // Tempo curto para sair do tile
                            j->botMoveDirecao = safeDir; // Foge na direção segura
                        }
                    }
                }
                break; 
            }
            
            // --- 2. FUGINDO (Para sair do tile da bomba) ---
            case BOT_STATE_FLEEING:
            {
                // Se o tempo de "sair do tile" (0.5s) acabou
                if (j->botStateTimer <= 0.0f)
                {
                    // --- CORREÇÃO: Entra em HOLDING e PARA ---
                    j->botState = BOT_STATE_HOLDING; 
                    j->botStateTimer = 2.0f; // (BOMBA_COOLDOWN_TEMPO - 0.5f)
                    j->botMoveDirecao = 4; // PARAR
                    // --- FIM CORREÇÃO ---
                }
                // (O movimento continua na 'safeDir' até o timer acabar)
                break;
            }
            
            // --- 3. ESPERANDO (Parado) ---
            case BOT_STATE_HOLDING:
            {
                j->botMoveDirecao = 4; // Garante que está parado
                
                if (j->botStateTimer <= 0.0f)
                {
                    j->botState = BOT_STATE_WANDERING; 
                    j->botStateTimer = 1.0f; 
                    j->botMoveDirecao = GetRandomValue(0, 4); // Escolhe a próxima ação
                }
                break;
            }
        }

        // --- Execução do Movimento ---
        j->currentDir = DIR_PARADO; 
        
        switch (j->botMoveDirecao)
        {
            case 0: MoverJogadorY(j, -j->velocidade); j->currentDir = DIR_CIMA; break; 
            case 1: MoverJogadorY(j, j->velocidade);  j->currentDir = DIR_BAIXO; break; 
            case 2: MoverJogadorX(j, -j->velocidade); j->currentDir = DIR_ESQUERDA; break; 
            case 3: MoverJogadorX(j, j->velocidade);  j->currentDir = DIR_DIREITA; break; 
            case 4: j->currentFrame = 0; break; // Parado
        }

        // --- Detecção de Colisão ---
        // Se bateu na parede ENQUANTO FUGIA, escolhe nova direção IMEDIATAMENTE
        if (posAntes.x == j->pos.x && posAntes.y == j->pos.y && j->botMoveDirecao != 4)
        {
            if (j->botState == BOT_STATE_FLEEING) {
                 // Tenta outra direção de fuga (pode ser suicida, mas é melhor que ficar parado)
                j->botMoveDirecao = GetRandomValue(0, 3);
            }
            else if (j->botState == BOT_STATE_WANDERING) {
                j->botStateTimer = 0.0f; // Força re-decisão
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
        
        MoverJogadorX(j, dx);
        MoverJogadorY(j, dy);
        
        if (IsKeyPressed(keyBomb) && j->bombaCooldown <= 0.0f)
        {
            AlinharEPlantarBomba(j, gBombas);
            j->bombaCooldown = BOMBA_COOLDOWN_TEMPO; 
        }
    }
    
    // Lógica de Animação
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
    
    if (texToDraw.id == 0) {
        texToDraw = j->texParado; 
    }
    
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