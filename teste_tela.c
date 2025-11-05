#include <stdio.h>    // Para printf
#include <unistd.h>   // Para usleep (dar uma pausa)

// Inclui os cabeçalhos da cli-lib
#include "screen.h"
#include "keyboard.h" // Vamos usar o keyboardInit/Destroy para 
                      // preparar o terminal corretamente

int main() {
    
    // --- 1. PREPARAÇÃO ---
    
    // Prepara o terminal (importante para as funções de tela funcionarem bem)
    keyboardInit(); 
    
    // Limpa a tela inteira
    screenClear();
    
    // Esconde o cursor piscante (deixa mais profissional)
    screenHideCursor();

    // --- 2. DESENHANDO ---

    // Define a cor: Texto VERMELHO, Fundo PRETO
    screenSetColor(RED, BLACK);
    
    // Move o cursor para Coluna 10, Linha 5
    screenGotoxy(10, 5); 
    
    // Desenha o texto nessa posição
    printf("Olá, mundo!");

    
    // Define a cor: Texto CIANO, Fundo PRETO
    screenSetColor(CYAN, BLACK);
    
    // Move o cursor para Coluna 2, Linha 8
    screenGotoxy(2, 8);
    
    // Desenha uma "caixa" simples
    printf("+----------+");
    screenGotoxy(2, 9);
    printf("|          |");
    screenGotoxy(2, 10);
    printf("+----------+");

    // Move o cursor para o "jogador"
    screenSetColor(YELLOW, BLACK);
    screenGotoxy(5, 9);
    printf("B"); // 'B' de Bomberman

    
    // --- 3. ESPERA E FINALIZAÇÃO ---
    
    // Deixa o desenho na tela por 3 segundos
    // (Em um jogo real, isso seria o seu "game loop")
    usleep(3000000); // 3.000.000 microssegundos = 3 segundos

    // Limpa a tela antes de sair
    screenClear();
    
    // Mostra o cursor novamente
    screenShowCursor();
    
    // Restaura as configurações do terminal
    keyboardDestroy();
    
    // Volta a cor para o padrão
    screenSetColor(WHITE, BLACK);
    printf("Exemplo de tela finalizado!\n");
    
    return 0;
}