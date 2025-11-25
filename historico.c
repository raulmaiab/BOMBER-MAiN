// historico.c

#include "historico.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

#define NOME_ARQUIVO "historico.txt"

// Função utilitária para obter a data e hora atual no formato string
static void ObterDataHoraAtual(char *buffer, size_t tamanhoBuffer) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    // Formato: YYYY-MM-DD HH:MM:SS
    strftime(buffer, tamanhoBuffer, "%Y-%m-%d %H:%M:%S", tm);
}

void AdicionarRegistroHistorico(RegistroBatalha registro) {
    // Abre o arquivo no modo de append ("a+"). Se o arquivo não existir, ele é criado.
    FILE *arquivo = fopen(NOME_ARQUIVO, "a+");

    if (arquivo == NULL) {
        // Em um projeto real, você registraria esse erro.
        printf("ERRO: Nao foi possivel abrir/criar o arquivo %s\n", NOME_ARQUIVO);
        return;
    } else {
        // Obtém a data e hora atual
        char dataHora[25]; // Aumentado para acomodar o formato com segundos e terminador nulo
        ObterDataHoraAtual(dataHora, sizeof(dataHora));
        
        // Escreve o novo registro no formato "Data | Modo | Vencedor\n"
        fprintf(arquivo, "%s | %s | %s\n", 
                dataHora, 
                registro.modo, 
                registro.vencedor);

        // Fecha o arquivo
        fclose(arquivo);
        printf("Registro de batalha adicionado ao %s\n", NOME_ARQUIVO);
    }
}