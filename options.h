// options.h
#ifndef OPCOES_H
#define OPCOES_H

#include "raylib.h"
#include <stdbool.h>

// --- Ações que o menu de Opções pode retornar ---
typedef enum {
    OPCOES_ACAO_RESUMO,     // Voltar para a partida atual (Resume)
    OPCOES_ACAO_REINICIAR,  // Reiniciar a partida atual (Restart)
    OPCOES_ACAO_EDITAR_JOGO, // Voltar ao Menu Battle/Story para reconfigurar
    OPCOES_ACAO_MENU_PRINCIPAL, // Voltar ao Menu Principal
    OPCOES_ACAO_NENHUMA     // Não houve escolha (ex: se o usuário fechar a janela)
} AcaoOpcoes;

/**
 * @brief Exibe o menu de opções (Options) durante a partida (popup).
 *
 * Durante a execução desta função, o jogo estará pausado. O fundo
 * da partida será escurecido para dar destaque ao popup.
 *
 * @return Retorna a ação escolhida pelo jogador.
 */
AcaoOpcoes ExecutarMenuOpcoes(void);

#endif // OPCOES_H