#ifndef INICIO_H
#define INICIO_H

// Incluímos bool para podermos retornar true/false
#include <stdbool.h> 

/**
 * @brief Executa a tela de início (splash screen).
 * @return Retorna 'true' se o jogo deve continuar (ex: pressionou Enter),
 * ou 'false' se o usuário fechou a janela.
 */
bool ExecutarTelaInicio(void);

#endif // INICIO_H