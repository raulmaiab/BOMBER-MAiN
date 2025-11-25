// historico.h

#ifndef ESTRUTURA_HISTORICO_H
#define ESTRUTURA_HISTORICO_H

#include <stddef.h> // Para size_t

// Definição da estrutura para armazenar um registro de batalha
typedef struct {
    char data[25];        // Aumentado para 25 para acomodar o formato YYYY-MM-DD HH:MM:SS
    char modo[20];        // Ex: "Story Nivel 1" ou "Battle"
    char vencedor[50];    // Ex: "Player 1 (Branco)" ou "Equipe Humana"
} RegistroBatalha;

// Assinaturas das funções

/**
 * @brief Adiciona um novo registro ao arquivo de histórico (historico.txt).
 * O arquivo é criado se não existir.
 * @param registro A estrutura contendo os dados da batalha.
 */
void AdicionarRegistroHistorico(RegistroBatalha registro);

#endif // ESTRUTURA_HISTORICO_H