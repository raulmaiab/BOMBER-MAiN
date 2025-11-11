#ifndef JOGADOR_H
#define JOGADOR_H

#include "raylib.h"
#include <stdbool.h>

typedef struct Jogador
{
    Vector2 pos;       // posição do jogador (x, y)
    float velocidade;  // velocidade de movimento
    Texture2D textura; // sprite do jogador
    bool vivo;         // status vivo/morto
    Rectangle frameRec; // <-- NOVO: O retângulo fonte da spritesheet
} Jogador;

// Inicializa (apenas cria) o jogador com renderização e na posição inicial
Jogador CriarJogador(Vector2 posInicial, const char* imgSprite);

// Atualiza o jogador (movimento, colisão, etc.)
void AtualizarJogador(Jogador* j, int keyUp, int keyDown, int keyLeft, int keyRight);

// Desenha o jogador na tela durante o jogo
void DesenharJogador(const Jogador* j);

// Libera memória (textura)
void DestruirJogador(Jogador* j); // (Corrigido para bater com o .c)

#endif