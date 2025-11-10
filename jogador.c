#include "jogador.h"

Jogador CriarJogador(Vector2 posInicial, const char* imgSprite)
{
    Jogador j;
    j.pos = posInicial;
    j.velocidade = 2.5f;
    j.cor = LoadTexture(imgSprite);
    j.vivo = true;
    return j;
}

void AtualizarJogador(Jogador* j)
{
    if (IsKeyDown(KEY_RIGHT)) j->pos.x += j->velocidade;
    if (IsKeyDown(KEY_LEFT))  j->pos.x -= j->velocidade;
    if (IsKeyDown(KEY_UP))    j->pos.y -= j->velocidade;
    if (IsKeyDown(KEY_DOWN))  j->pos.y += j->velocidade;
}

void DesenharJogador(const Jogador* j)
{
    DrawTexture(j->textura, j->pos.x, j->pos.y, WHITE);
}


void DestruirJogador(Jogador* j)
{
    UnloadTexture(j->textura);
}
