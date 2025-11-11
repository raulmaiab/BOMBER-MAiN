#ifndef MENU_H
#define MENU_H

typedef enum {
    ESCOLHA_BATTLE = 1, // Corresponde a "Battle"
    ESCOLHA_STORY,      // Corresponde a "Story"
    ESCOLHA_SHOP,       // Corresponde a "Shop"
    ESCOLHA_OTHER,      // Corresponde a "Other" (seu antigo "Opções")
    ESCOLHA_SAIR,       // Adicionei um "Sair" para ter uma opção de saída
    ESCOLHA_NENHUMA_OU_FECHOU = 0 
} OpcaoMenu;

OpcaoMenu ExecutarTelaMenu(void);

#endif // MENU_H