#include "bomba.h"
#include "raylib.h"
#include "mapa.h"       
#include "explosao.h"   
#include "jogador.h"    
#include <stddef.h>     
#include <stdlib.h>     // Necessário para malloc e free

#define VELOCIDADE_PISCAR_BOMBA 0.2f 
#define TEMPO_INICIO_PISCAR_BOMBA 1.0f 

// Função que cria o cabeçalho da lista encadeada
NodeBombas CriarNodeBombas(void) {
    NodeBombas g;
    // O cabeçalho da lista aponta para NULL (lista vazia)
    g.head = NULL; 
    g.quantidade = 0;
    
    // Carregamento de Texturas
    g.texNormal = LoadTexture("bombapng/bomba1.png");
    g.texAviso = LoadTexture("bombapng/bomba2.png");
    
    if (g.texNormal.id == 0) {
        TraceLog(LOG_WARNING, "Falha ao carregar bombapng/bomba1.png");
    }
    if (g.texAviso.id == 0) {
        TraceLog(LOG_WARNING, "Falha ao carregar bombapng/bomba2.png");
    }
    
    return g;
}

// Função para ADICIONAR uma bomba (novo nó) na lista encadeada.
void PlantarBomba(NodeBombas *g, Vector2 posBomba, int range, struct Jogador *dono) 
{
    NodeBomba *current = g->head;
    while (current != NULL) {
        if (current->ativa != false && current->posicao.x == posBomba.x && current->posicao.y == posBomba.y) {
            return; 
        }
        current = current->next;
    }
    
    //Alocação dinâmica pra o nó
    NodeBomba *novaBomba = (NodeBomba*)malloc(sizeof(NodeBomba));
    if (novaBomba == NULL) {
        TraceLog(LOG_ERROR, "FALHA DE ALOCAÇÃO DE MEMÓRIA para nova bomba.");
        return;
    }
    
    // 3. Inicialização dos dados da bomba
    novaBomba->posicao = posBomba;
    novaBomba->tempoExplosao = 3.0f;   
    novaBomba->raioExplosao = range; 
    novaBomba->ativa = true;           
    novaBomba->frameAtual = 0; 
    novaBomba->temporizadorFrame = 0.0f;
    novaBomba->dono = dono; 
    
    // 4. Inserção no início da lista (ajustando ponteiros)
    novaBomba->next = g->head; // O novo nó aponta para a antiga HEAD
    g->head = novaBomba;       // A HEAD agora é o novo nó
    
    // 5. Incrementa contador
    if (dono != NULL) {
        dono->bombasAtivas++;
    }

    g->quantidade++;
}

// Função que itera, atualiza o tempo e REMOVE nós que explodiram.
bool AtualizarBombas(NodeBombas *g, float deltaTime, NodeExplosoes *gExplosoes, struct Jogador* jogadores[], int numJogadores) 
{
    bool algumaExplodiu = false;

    // Ponteiros para iteração e remoção segura
    NodeBomba *current = g->head;
    NodeBomba *previous = NULL;

    while (current != NULL) {
        // A bomba deve estar ativa (embora todos os nós na lista devam ser ativos)
        if (current->ativa == false) {
             // Caso a lógica exija que 'ativa' seja checada
             previous = current;
             current = current->next;
             continue;
        }

        current->tempoExplosao -= deltaTime;

        // Lógica de piscar (mantida)
        if (current->tempoExplosao <= TEMPO_INICIO_PISCAR_BOMBA) {
            current->temporizadorFrame += deltaTime;
            if (current->temporizadorFrame >= VELOCIDADE_PISCAR_BOMBA) {
                current->temporizadorFrame = 0.0f;
                current->frameAtual = (current->frameAtual == 0) ? 1 : 0;
            }
        } else {
            current->frameAtual = 0; 
        }

        // --- LÓGICA DE DETONAÇÃO E REMOÇÃO ---
        if (current->tempoExplosao <= 0) {
            
            // 1. Lógica do Jogo (cria explosão, atualiza dono)
            CriarExplosao(gExplosoes, current->posicao, current->raioExplosao, jogadores, numJogadores);
            
            if (current->dono != NULL) 
            {
                current->dono->bombasAtivas--;
                if (current->dono->bombasAtivas < 0) {
                    current->dono->bombasAtivas = 0;
                }
            }

            algumaExplodiu = true;
            // Não precisamos mais setar 'ativa=false', pois o nó será removido.
            
            // 2. Remoção do nó:
            NodeBomba *to_remove = current;
            
            if (previous == NULL) {
                // Caso 1: O nó a remover é o HEAD
                g->head = current->next;
            } else {
                // Caso 2: O nó a remover está no meio ou fim.
                // O anterior "salta" o nó atual.
                previous->next = current->next;
            }
            
            // 3. Avança 'current' para o próximo nó antes de liberar a memória
            current = current->next; 
            
            // 4. Liberação de memória!
            free(to_remove);
            g->quantidade--;
            
        } else {
            // A bomba não explodiu, avança os ponteiros
            previous = current;
            current = current->next;
        }
    }
    return algumaExplodiu;
}

// Função para DESENHAR iterando sobre a lista encadeada
void DesenharBombas(const NodeBombas *g) {
    Vector2 origem = { 0, 0 };
    NodeBomba *current = g->head;

    while (current != NULL) {
        // A lógica de 'ativa == false' não é mais necessária, pois só nós ativos estão na lista.
        
        Texture2D texDesenhar;
        if (current->frameAtual == 0) {
            texDesenhar = g->texNormal;
        } else {
            texDesenhar = g->texAviso;
        }
        
        if (texDesenhar.id > 0) {
             Rectangle recFonte = { 0.0f, 0.0f, (float)texDesenhar.width, (float)texDesenhar.height };
             Rectangle recDestino = { current->posicao.x, current->posicao.y, TAMANHO_TILE, TAMANHO_TILE };
             
             DrawTexturePro(texDesenhar, recFonte, recDestino, origem, 0.0f, WHITE);
        } else {
             DrawCircle(current->posicao.x + TAMANHO_TILE/2, current->posicao.y + TAMANHO_TILE/2, 20, BLACK);
        }

        current = current->next; // Avança para o próximo nó
    }
}

// Função para LIBERAR a memória de todos os nós da lista
void UnloadBombas(NodeBombas *g) {
    NodeBomba *current = g->head;
    NodeBomba *next = NULL;

    while (current != NULL) {
        next = current->next; // Guarda o próximo nó
        free(current);        // Libera a memória do nó atual
        current = next;       // Avança para o próximo nó
    }
    
    // Limpa o cabeçalho após liberar tudo
    g->head = NULL; 
    g->quantidade = 0;
    
    // Descarrega texturas
    UnloadTexture(g->texNormal);
    UnloadTexture(g->texAviso);
}