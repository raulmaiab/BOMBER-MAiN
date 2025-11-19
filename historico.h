// historico.h

#ifndef HISTORICO_H
#define HISTORICO_H

// Definição da estrutura para armazenar um registro de batalha
typedef struct {
    char data[20];        // Ex: "2025-11-19 09:42"
    char modo[20];        // Ex: "Story Nivel 1" ou "Battle"
    char vencedor[50];    // Ex: "Player 1 (Branco)" ou "Equipe Humana"
} RegistroBatalha;

// Assinaturas das funções

/**
 * @brief Adiciona um novo registro ao arquivo historico.txt.
 * O arquivo é criado se não existir.
 * * @param registro A estrutura contendo os dados da batalha.
 */
void AdicionarRegistroHistorico(RegistroBatalha registro);

#endif // HISTORICO_H