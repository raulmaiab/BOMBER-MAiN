#ifndef BOMBA_H
#define BOMBA_H

#include "raylib.h"
#include <stdbool.h>
#include "explosao.h" // <-- Inclui o novo sistema

#define MAX_BOMBAS_ATIVAS 20

typedef struct Bomba 
{
    Vector2 posicao;          
    float tempoExplosao;      
    int raioExplosao;         
    bool ativa;               
    
    // Para Animação de piscar
    int currentFrame;         // Frame atual (0 = bomba1.png, 1 = bomba2.png)
    float frameTimer;         
    
} Bomba;

typedef struct {
    Bomba bombas[MAX_BOMBAS_ATIVAS]; 
    int quantidade;                  
    Texture2D texNormal; // Textura para bomba1.png
    Texture2D texAviso;  // Textura para bomba2.png
} NodeBombas;

/**
 * @brief Carrega as texturas da bomba (bomba1.png, bomba2.png)
 */
NodeBombas CriarNodeBombas(void); // Removemos o parâmetro, os caminhos estão no .c

/**
 * @brief Adiciona uma bomba ao mundo.
 */
void PlantarBomba(NodeBombas *g, Vector2 posBomba);

/**
 * @brief Atualiza as bombas. QUANDO EXPLODIR, chama AtivarExplosao.
 * @param gExplosoes O ponteiro para o gestor de explosões.
 */
bool AtualizarBombas(NodeBombas *g, float deltaTime, NodeExplosoes *gExplosoes);

/**
 * @brief Desenha as bombas ativas (piscando).
 */
void DesenharBombas(const NodeBombas *g);

/**
 * @brief Descarrega as texturas da bomba.
 */
void UnloadBombas(NodeBombas *g);

#endif // BOMBA_H