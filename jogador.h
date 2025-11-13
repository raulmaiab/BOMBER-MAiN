#ifndef JOGADOR_H
#define JOGADOR_H

#include "raylib.h"
#include <stdbool.h>
#include "bomba.h" // <-- 1. INCLUÍDO (precisa saber o que é NodeBombas)

typedef struct Jogador
{
    Vector2 pos;       
    float velocidade;  
    Texture2D textura; 
    bool vivo;         
} Jogador;

Jogador CriarJogador(Vector2 posInicial, const char* imgSprite);

// --- 2. ASSINATURA ATUALIZADA ---
void AtualizarJogador(Jogador* j, int keyUp, int keyDown, int keyLeft, int keyRight, int keyBomb, NodeBombas *gBombas);
// --- FIM DA ATUALIZAÇÃO ---

void DesenharJogador(const Jogador* j);

void DestruirJogador(Jogador* j);

#endif