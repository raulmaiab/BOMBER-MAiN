// options.h
#ifndef OPTIONS_H
#define OPTIONS_H

#include "raylib.h"
#include <stdbool.h>

// --- Ações que o menu de Options pode retornar ---
typedef enum {
    OPTIONS_ACAO_RESUME,    // Voltar para a partida atual (Resume)
    OPTIONS_ACAO_RESTART,   // Reiniciar a partida atual (Restart)
    OPTIONS_ACAO_EDIT_GAME, // Voltar ao Menu Battle/Story para reconfigurar
    OPTIONS_ACAO_MAIN_MENU, // Voltar ao Menu Principal
    OPTIONS_ACAO_NENHUMA    // Não houve escolha (ex: se o usuário fechar a janela)
} OptionsAction;

/**
 * @brief Exibe o menu de opções (Options) durante a partida (popup).
 *
 * Durante a execução desta função, o jogo estará pausado. O fundo
 * da partida será escurecido para dar destaque ao popup.
 *
 * @return Retorna a ação escolhida pelo jogador.
 */
OptionsAction ExecutarMenuOptions(void);

#endif // OPTIONS_H