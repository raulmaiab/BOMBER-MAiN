#include "historico.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

#define NOME_ARQUIVO "historico.txt"

//obter data e hora
static void ObterDataHoraAtual(char *buffer, size_t tamanhoBuffer) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(buffer, tamanhoBuffer, "%Y-%m-%d %H:%M:%S", tm);
}

void AdicionarRegistroHistorico(RegistroBatalha registro) {
    //Abre o arquivo, permite leitura
    FILE *arquivo = fopen(NOME_ARQUIVO, "a+");

    if (arquivo == NULL) {
        printf("ERRO: Nao foi possivel abrir/criar o arquivo %s\n", NOME_ARQUIVO);
        return;
    } else {
        //Obtém a data e hora atual
        char dataHora[25]; 
        ObterDataHoraAtual(dataHora, sizeof(dataHora));
        
        //Escreve o novo registro no formato "Data \ Modo \ Vencedor"
        fprintf(arquivo, "%s | %s | %s\n", dataHora, registro.modo, registro.vencedor);

        //Fecha o arquivo
        fclose(arquivo);
        printf("Registro de batalha adicionado ao %s\n", NOME_ARQUIVO);
    }
}