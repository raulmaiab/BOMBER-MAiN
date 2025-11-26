#include "jogador.h"
#include "raylib.h" 
#include "raymath.h" 
#include "mapa.h"   
#include "bomba.h" 
#include "extras.h" 
#include <stdio.h> 
#include <float.h> 
#include <stdlib.h> 
#include <string.h> 
#include <math.h>

//Constantes
#define MARGEM_COLISAO 4.0f 
#define TEMPO_RECARGA_BOMBA 2.5f 

//Funções Ajudantes

static void AlinharNaGrade(Jogador* j, int direcao) {
    Vector2 centro = { j->pos.x + TAMANHO_TILE/2.0f, j->pos.y + TAMANHO_TILE/2.0f };
    Vector2 posGrade = ObterPosGradeDePixels(centro);
    
    //Vertical -> Alinha X
    if (direcao == 0 || direcao == 1) { 
        j->pos.x = posGrade.x * TAMANHO_TILE;
    }
    //Horizontal -> Alinha Y
    else if (direcao == 2 || direcao == 3) { 
        j->pos.y = posGrade.y * TAMANHO_TILE;
    }
}

static void AlinharEPlantarBomba(Jogador* j, NodeBombas *gBombas) {
    //Verifica limite de bombas do jogador
    //O limite de bombas do jogador deve ser implementado no lado do jogador
    if (j->bombasAtivas >= j->limiteBombas) {
        return;
    }

    float centroX = j->pos.x + (TAMANHO_TILE / 2.0f);
    float centroY = j->pos.y + (TAMANHO_TILE / 2.0f);
    int gradeX = (int)(centroX / TAMANHO_TILE);
    int gradeY = (int)(centroY / TAMANHO_TILE);
    Vector2 posBombaAlinhada = { (float)gradeX * TAMANHO_TILE, (float)gradeY * TAMANHO_TILE };
    
    //A checagem de posição duplicada é feita dentro da função PlantarBomba em bomba.c
    //Passa o próprio jogador 'j' como dono
    PlantarBomba(gBombas, posBombaAlinhada, j->alcanceBomba, j); 
}

static bool ExisteBombaEm(NodeBombas *gBombas, int gradeX, int gradeY) {
    // Utiliza o ponteiro head da lista encadeada
    NodeBomba *current = gBombas->head; 

    while (current != NULL) { //Itera sobre cada nó
        //A checagem de 'ativa' não é estritamente necessária se apenas
        //bombas ativas são inseridas na lista, mas é mantida por segurança.
        if (current->ativa != false) {
            Vector2 posGradeBomba = ObterPosGradeDePixels(current->posicao);
            if ((int)posGradeBomba.x == gradeX && (int)posGradeBomba.y == gradeY) { 
                return true; //encontrado
            }
        }
        current = current->next; //Próximo nó
    }
    return false; //Não encontrado
}

static bool EstaNoCanto(int x, int y) {
    int L = LARGURA_GRADE_MAPA;
    int A = ALTURA_GRADE_MAPA;
    if ((x == 1 && y == 1) || (x == 2 && y == 1) || (x == 1 && y == 2)) {
        return true;
    }
    if ((x == L-2 && y == 1) || (x == L-3 && y == 1) || (x == L-2 && y == 2)) {
        return true;
    }
    if ((x == 1 && y == A-2) || (x == 2 && y == A-2) || (x == 1 && y == A-3)) {
        return true;
    }
    if ((x == L-2 && y == A-2) || (x == L-3 && y == A-2) || (x == L-2 && y == A-3)) {
        return true;
    }
    return false;
}

//Verifica se é seguro
static bool DirecaoEhSegura(int inicioX, int inicioY, int dir, NodeBombas *gBombas) {
    int dx = 0, dy = 0;
    if (dir == 0) {
        dy = -1;
    } else if (dir == 1) {
        dy = 1;
    } else if (dir == 2) {
        dx = -1;
    } else if (dir == 3) {
        dx = 1;
    }
    
    int verificarX = inicioX + dx;
    int verificarY = inicioY + dy;
    
    if (verificarX < 0 || verificarX >= LARGURA_GRADE_MAPA || verificarY < 0 || verificarY >= ALTURA_GRADE_MAPA) {
        return false;
    }
    if (ObterTipoTile(verificarX, verificarY) != TILE_VAZIO) {
        return false;
    }
    if (ExisteBombaEm(gBombas, verificarX, verificarY)) {
        return false;
    }
    return true;
}
static bool DirecaoEhAndavel(int inicioX, int inicioY, int dir) {
    int dx = 0, dy = 0;
    if (dir == 0) {
        dy = -1;
    } else if (dir == 1) {
        dy = 1;
    } else if (dir == 2) {
        dx = -1;
    } else if (dir == 3) {
        dx = 1;
    }
    
    int verificarX = inicioX + dx;
    int verificarY = inicioY + dy;
    
    if (verificarX < 0 || verificarX >= LARGURA_GRADE_MAPA || verificarY < 0 || verificarY >= ALTURA_GRADE_MAPA) {
        return false;
    }
    if (ObterTipoTile(verificarX, verificarY) != TILE_VAZIO) {
        return false;
    } 
    return true;
}

static void MoverJogadorX(Jogador *j, float delta, NodeBombas *gBombas, bool ignorarBombas) {
    if (delta == 0) {
        return;
    }
    Vector2 centroJogador = { j->pos.x + TAMANHO_TILE/2.0f, j->pos.y + TAMANHO_TILE/2.0f };
    Vector2 posGradeJogador = ObterPosGradeDePixels(centroJogador);
    Vector2 posTentativa = j->pos;
    posTentativa.x += delta;
    
    float cX;
    if (delta > 0) {
        cX = posTentativa.x + TAMANHO_TILE - 1 - MARGEM_COLISAO;
    } else {
        cX = posTentativa.x + MARGEM_COLISAO;
    }
    float cY1 = j->pos.y + MARGEM_COLISAO; 
    float cY2 = j->pos.y + TAMANHO_TILE - 1 - MARGEM_COLISAO; 
    
    Vector2 gPos1 = ObterPosGradeDePixels((Vector2){cX, cY1});
    Vector2 gPos2 = ObterPosGradeDePixels((Vector2){cX, cY2});

    bool b1;
    if (ignorarBombas == false) {
        b1 = ExisteBombaEm(gBombas, (int)gPos1.x, (int)gPos1.y);
    } else {
        b1 = false;
    }
    bool p1 = ((int)gPos1.x == (int)posGradeJogador.x && (int)gPos1.y == (int)posGradeJogador.y);
    bool t1 = (ObterTipoTile((int)gPos1.x, (int)gPos1.y) == TILE_VAZIO) && (b1 == false || p1);
    
    bool b2;
    if (ignorarBombas == false) {
        b2 = ExisteBombaEm(gBombas, (int)gPos2.x, (int)gPos2.y);
    } else {
        b2 = false;
    }
    bool p2 = ((int)gPos2.x == (int)posGradeJogador.x && (int)gPos2.y == (int)posGradeJogador.y);
    bool t2 = (ObterTipoTile((int)gPos2.x, (int)gPos2.y) == TILE_VAZIO) && (b2 == false || p2);

    if (t1 && t2) { 
        j->pos.x = posTentativa.x; 
    }
}

static void MoverJogadorY(Jogador *j, float delta, NodeBombas *gBombas, bool ignorarBombas) {
    if (delta == 0) {
        return;
    }
    Vector2 centroJogador = { j->pos.x + TAMANHO_TILE/2.0f, j->pos.y + TAMANHO_TILE/2.0f };
    Vector2 posGradeJogador = ObterPosGradeDePixels(centroJogador);
    Vector2 posTentativa = j->pos;
    posTentativa.y += delta;
    
    float cX1 = j->pos.x + MARGEM_COLISAO; 
    float cX2 = j->pos.x + TAMANHO_TILE - 1 - MARGEM_COLISAO; 
    float cY;
    if (delta > 0) {
        cY = posTentativa.y + TAMANHO_TILE - 1 - MARGEM_COLISAO;
    } else {
        cY = posTentativa.y + MARGEM_COLISAO;
    }
    
    Vector2 gPos1 = ObterPosGradeDePixels((Vector2){cX1, cY});
    Vector2 gPos2 = ObterPosGradeDePixels((Vector2){cX2, cY});
    
    bool b1;
    if (ignorarBombas == false) {
        b1 = ExisteBombaEm(gBombas, (int)gPos1.x, (int)gPos1.y);
    } else {
        b1 = false;
    }
    bool p1 = ((int)gPos1.x == (int)posGradeJogador.x && (int)gPos1.y == (int)posGradeJogador.y);
    bool t1 = (ObterTipoTile((int)gPos1.x, (int)gPos1.y) == TILE_VAZIO) && (b1 == false || p1);
    
    bool b2;
    if (ignorarBombas == false) {
        b2 = ExisteBombaEm(gBombas, (int)gPos2.x, (int)gPos2.y);
    } else {
        b2 = false;
    }
    bool p2 = ((int)gPos2.x == (int)posGradeJogador.x && (int)gPos2.y == (int)posGradeJogador.y);
    bool t2 = (ObterTipoTile((int)gPos2.x, (int)gPos2.y) == TILE_VAZIO) && (b2 == false || p2);

    if (t1 && t2) { 
        j->pos.y = posTentativa.y; 
    }
}

//Criar jogador
Jogador CriarJogador(Vector2 posInicial, const char* pastaSprites, bool ehBot)
{
    Jogador j;
    j.pos = posInicial; j.velocidade = 2.5f; j.vivo = true;
    
    strncpy(j.nomeSprite, pastaSprites, COMPRIMENTO_MAX_NOME_SPRITE - 1); 
    j.nomeSprite[COMPRIMENTO_MAX_NOME_SPRITE - 1] = '\0';
    
    char bufferCaminho[256]; 
    sprintf(bufferCaminho, "%s/andando.png", pastaSprites); j.texParado = LoadTexture(bufferCaminho);
    sprintf(bufferCaminho, "%s/costas1.png", pastaSprites); j.texCima[0] = LoadTexture(bufferCaminho);
    sprintf(bufferCaminho, "%s/costas2.png", pastaSprites); j.texCima[1] = LoadTexture(bufferCaminho);
    sprintf(bufferCaminho, "%s/costas3.png", pastaSprites); j.texCima[2] = LoadTexture(bufferCaminho);
    j.texBaixo[0] = j.texParado; j.texBaixo[1] = j.texParado; j.texBaixo[2] = j.texParado; 
    sprintf(bufferCaminho, "%s/esquerda1.png", pastaSprites); j.texEsquerda[0] = LoadTexture(bufferCaminho);
    sprintf(bufferCaminho, "%s/esquerda2.png", pastaSprites); j.texEsquerda[1] = LoadTexture(bufferCaminho);
    sprintf(bufferCaminho, "%s/esquerda3.png", pastaSprites); j.texEsquerda[2] = LoadTexture(bufferCaminho);
    sprintf(bufferCaminho, "%s/direita1.png", pastaSprites); j.texDireita[0] = LoadTexture(bufferCaminho);
    sprintf(bufferCaminho, "%s/direita2.png", pastaSprites); j.texDireita[1] = LoadTexture(bufferCaminho);
    sprintf(bufferCaminho, "%s/direita3.png", pastaSprites); j.texDireita[2] = LoadTexture(bufferCaminho);
    j.frameAtual = 0; j.temporizadorFrame = 0.0f; j.direcaoAtual = DIR_BAIXO; 
    
    j.ehBot = ehBot;
    if (j.ehBot == true) {
        j.estadoBot = BOT_ESTADO_VAGANDO; 
        j.temporizadorEstadoBot = (float)GetRandomValue(5, 20) / 10.0f; 
        j.direcaoMovimentoBot = GetRandomValue(0, 4); 
    } else {
        j.estadoBot = BOT_ESTADO_VAGANDO; 
        j.temporizadorEstadoBot = 1.0f; 
        j.direcaoMovimentoBot = 4; 
    }
    j.ultimaPosicaoBombaBot = (Vector2){0,0};
    j.recargaBomba = 0.0f; 

    j.alcanceBomba = 1;
    j.temDefesa = false; j.temporizadorDefesa = 0.0f;
    j.temVelocidade = false; j.temporizadorVelocidade = 0.0f;
    j.limiteBombas = 1;
    j.bombasAtivas = 0;

    return j;
}

//Atualizar ações do jogador
void AtualizarJogador(Jogador* j, int teclaCima, int teclaBaixo, int teclaEsquerda, int teclaDireita, int teclaBomba, NodeBombas *gBombas, float deltaTime, Jogador* alvoHumano1, Jogador* alvoHumano2)
{
    if (j->vivo == false) {
        return;
    }

    float multiplicadorVelocidade = 1.0f;
    if (j->temVelocidade == true) {
        j->temporizadorVelocidade -= deltaTime;
        multiplicadorVelocidade = 1.5f; 
        if (j->temporizadorVelocidade <= 0.0f) {
            j->temVelocidade = false;
        }
    }
    if (j->temDefesa == true) {
        j->temporizadorDefesa -= deltaTime;
        if (j->temporizadorDefesa <= 0.0f) {
            j->temDefesa = false;
        }
    }

    float velocidadeAtual = j->velocidade * multiplicadorVelocidade; 

    if (j->recargaBomba > 0.0f) { 
        j->recargaBomba -= deltaTime; 
    }
    j->temporizadorEstadoBot -= deltaTime; 

    if (j->ehBot == true)
    {
        Vector2 posAntes = j->pos; 
        bool acabouDePlantar = false;

        switch (j->estadoBot)
        {
            //Andando
            case BOT_ESTADO_VAGANDO:
            {
                bool plantouBomba = false;

                if (j->recargaBomba <= 0.0f) 
                {
                    Vector2 minhaPosGrade = ObterPosGradeDePixels(j->pos);
                    int gradeX = (int)minhaPosGrade.x;
                    int gradeY = (int)minhaPosGrade.y;

                    if (EstaNoCanto(gradeX, gradeY) == false)
                    {
                        bool gatilhoAcionado = false;
                        //Colocar bomba em wallb.png
                        if (ObterTipoTile(gradeX, gradeY - 1) == TILE_DESTRUTIVEL || 
                            ObterTipoTile(gradeX, gradeY + 1) == TILE_DESTRUTIVEL ||
                            ObterTipoTile(gradeX - 1, gradeY) == TILE_DESTRUTIVEL || 
                            ObterTipoTile(gradeX + 1, gradeY) == TILE_DESTRUTIVEL) {
                            gatilhoAcionado = true;
                        }

                        if (gatilhoAcionado == true)
                        {
                            int direcaoFuga = -1;
                            int oposta = -1;
                            if (j->direcaoMovimentoBot == 0) {
                                oposta = 1;      
                            } else if (j->direcaoMovimentoBot == 1) {
                                oposta = 0; 
                            } else if (j->direcaoMovimentoBot == 2) {
                                oposta = 3; 
                            } else if (j->direcaoMovimentoBot == 3) {
                                oposta = 2; 
                            }

                            //DirecaoEhSegura para PLANEJAR o movimento (considera bombas)
                            if (oposta != -1 && DirecaoEhSegura(gradeX, gradeY, oposta, gBombas) == true) {
                                direcaoFuga = oposta;
                            }
                            else {
                                if (DirecaoEhSegura(gradeX, gradeY, j->direcaoMovimentoBot, gBombas) == true) {
                                    direcaoFuga = j->direcaoMovimentoBot;
                                } else if (DirecaoEhSegura(gradeX, gradeY, 0, gBombas) == true) {
                                    direcaoFuga = 0;
                                } else if (DirecaoEhSegura(gradeX, gradeY, 1, gBombas) == true) {
                                    direcaoFuga = 1;
                                } else if (DirecaoEhSegura(gradeX, gradeY, 2, gBombas) == true) {
                                    direcaoFuga = 2;
                                } else if (DirecaoEhSegura(gradeX, gradeY, 3, gBombas) == true) {
                                    direcaoFuga = 3;
                                }
                            }

                            if (direcaoFuga != -1) 
                            {
                                AlinharEPlantarBomba(j, gBombas); 
                                j->recargaBomba = TEMPO_RECARGA_BOMBA;
                                j->estadoBot = BOT_ESTADO_FUGINDO;
                                j->direcaoMovimentoBot = direcaoFuga; 
                                j->ultimaPosicaoBombaBot = (Vector2){ (float)gradeX * TAMANHO_TILE, (float)gradeY * TAMANHO_TILE };
                                
                                AlinharNaGrade(j, direcaoFuga); 
                                plantouBomba = true;
                                acabouDePlantar = true; 
                            }
                        }
                    }
                }

                //Movimento Normal
                if (plantouBomba == false && j->temporizadorEstadoBot <= 0.0f)
                {
                    int direcaoMover = -1;
                    Vector2 minhaPosGrade = ObterPosGradeDePixels(j->pos);
                    int gx = (int)minhaPosGrade.x;
                    int gy = (int)minhaPosGrade.y;

                    Vector2 posItem;
                    if (ObterExtraMaisProximo(j->pos, TAMANHO_TILE * 6, &posItem) == true) {
                        float dx = posItem.x - j->pos.x;
                        float dy = posItem.y - j->pos.y;
                        int pH;
                        if (dx > 0) {
                            pH = 3;
                        } else {
                            pH = 2;
                        }
                        int pV;
                        if (dy > 0) {
                            pV = 1;
                        } else {
                            pV = 0;
                        }
                        
                        if (fabs(dx) > fabs(dy)) {
                            if (DirecaoEhSegura(gx, gy, pH, gBombas) == true) {
                                direcaoMover = pH;
                            } else if (DirecaoEhSegura(gx, gy, pV, gBombas) == true) {
                                direcaoMover = pV;
                            }
                        } else {
                            if (DirecaoEhSegura(gx, gy, pV, gBombas) == true) {
                                direcaoMover = pV;
                            } else if (DirecaoEhSegura(gx, gy, pH, gBombas) == true) {
                                direcaoMover = pH;
                            }
                        }
                        j->temporizadorEstadoBot = 0.2f;
                    }

                    if (direcaoMover == -1) {
                        int direcoesValidas[4];
                        int contagem = 0;
                        if (DirecaoEhSegura(gx, gy, 0, gBombas) == true) {
                            direcoesValidas[contagem++] = 0;
                        }
                        if (DirecaoEhSegura(gx, gy, 1, gBombas) == true) {
                            direcoesValidas[contagem++] = 1;
                        }
                        if (DirecaoEhSegura(gx, gy, 2, gBombas) == true) {
                            direcoesValidas[contagem++] = 2;
                        }
                        if (DirecaoEhSegura(gx, gy, 3, gBombas) == true) {
                            direcoesValidas[contagem++] = 3;
                        }

                        if (contagem > 0) {
                            if (j->direcaoMovimentoBot >= 0 && j->direcaoMovimentoBot <= 3 && 
                                DirecaoEhSegura(gx, gy, j->direcaoMovimentoBot, gBombas) == true && 
                                GetRandomValue(0, 100) > 20) { 
                                direcaoMover = j->direcaoMovimentoBot;
                            } else {
                                direcaoMover = direcoesValidas[GetRandomValue(0, contagem - 1)];
                            }
                            j->temporizadorEstadoBot = (float)GetRandomValue(5, 15) / 10.0f;
                        } else {
                            direcaoMover = 4; 
                            j->temporizadorEstadoBot = 0.1f;
                        }
                    }

                    if (direcaoMover != -1) {
                        bool mudouEixo = (j->direcaoMovimentoBot >= 2 && direcaoMover <= 1) || (j->direcaoMovimentoBot <= 1 && direcaoMover >= 2);
                        if (mudouEixo == true) {
                            AlinharNaGrade(j, direcaoMover);
                        }
                        j->direcaoMovimentoBot = direcaoMover;
                    }
                }
                break; 
            }
            
            //FUGINDO (fuga com tentativa de sobrevivência a bomba)
            case BOT_ESTADO_FUGINDO:
            {
                Vector2 minhaPosGrade = ObterPosGradeDePixels(j->pos);
                Vector2 posGradeBomba = ObterPosGradeDePixels(j->ultimaPosicaoBombaBot);
                
                int distanciaAlvo = j->alcanceBomba + 3; 
                int distanciaAtual = abs((int)minhaPosGrade.x - (int)posGradeBomba.x) + abs((int)minhaPosGrade.y - (int)posGradeBomba.y);

                if (distanciaAtual >= distanciaAlvo) {
                    j->estadoBot = BOT_ESTADO_ESPERANDO;
                    j->temporizadorEstadoBot = TEMPO_RECARGA_BOMBA - 0.5f; 
                    j->direcaoMovimentoBot = 4; 
                }
                //Pânico
                else if (acabouDePlantar == false && Vector2Distance(posAntes, j->pos) < 0.05f) 
                {
                    if (distanciaAtual >= 1) {
                        j->estadoBot = BOT_ESTADO_ESPERANDO;
                        j->temporizadorEstadoBot = TEMPO_RECARGA_BOMBA - 0.5f;
                        j->direcaoMovimentoBot = 4; 
                    } else {
                        //Se travou no meio da fuga, usa DirecaoEhAndavel
                        
                        int gx = (int)minhaPosGrade.x;
                        int gy = (int)minhaPosGrade.y;
                        int novaDir = -1;
                        
                        //Tenta direção perpendicular primeiro
                        if (j->direcaoMovimentoBot <= 1) { //Vertical
                            if (DirecaoEhAndavel(gx, gy, 2) == true) {
                                novaDir = 2;
                            } else if (DirecaoEhAndavel(gx, gy, 3) == true) {
                                novaDir = 3;
                            }
                        } else { //Horizontal
                            if (DirecaoEhAndavel(gx, gy, 0) == true) {
                                novaDir = 0;
                            } else if (DirecaoEhAndavel(gx, gy, 1) == true) {
                                novaDir = 1;
                            }
                        }
                        
                        // e não achou perpendicular, tenta qualquer uma
                        if (novaDir == -1) {
                            if (DirecaoEhAndavel(gx, gy, 0) == true && j->direcaoMovimentoBot != 1) {
                                novaDir = 0;
                            } else if (DirecaoEhAndavel(gx, gy, 1) == true && j->direcaoMovimentoBot != 0) {
                                novaDir = 1;
                            } else if (DirecaoEhAndavel(gx, gy, 2) == true && j->direcaoMovimentoBot != 3) {
                                novaDir = 2;
                            } else if (DirecaoEhAndavel(gx, gy, 3) == true && j->direcaoMovimentoBot != 2) {
                                novaDir = 3;
                            }
                        }

                        if (novaDir != -1) {
                            j->direcaoMovimentoBot = novaDir;
                            AlinharNaGrade(j, novaDir);
                        }
                    }
                }
                break;
            }

            //Robô esperando o tempo da bomba
            case BOT_ESTADO_ESPERANDO:
            {
                j->direcaoMovimentoBot = 4; 
                if (j->temporizadorEstadoBot <= 0.0f)
                {
                    j->estadoBot = BOT_ESTADO_VAGANDO; 
                    j->temporizadorEstadoBot = 0.5f; 
                    j->direcaoMovimentoBot = GetRandomValue(0, 4); 
                }
                break;
            }
        }

        //Em FUGINDO, o robô ignora a colisão da bomba pra não travar e morrer
        bool ignorarBombas;
        if (j->estadoBot == BOT_ESTADO_FUGINDO) {
            ignorarBombas = true;
        } else {
            ignorarBombas = false;
        }

        j->direcaoAtual = DIR_PARADO; 
        switch (j->direcaoMovimentoBot)
        {
            case 0: MoverJogadorY(j, -velocidadeAtual, gBombas, ignorarBombas); j->direcaoAtual = DIR_CIMA; break; 
            case 1: MoverJogadorY(j, velocidadeAtual, gBombas, ignorarBombas);  j->direcaoAtual = DIR_BAIXO; break; 
            case 2: MoverJogadorX(j, -velocidadeAtual, gBombas, ignorarBombas); j->direcaoAtual = DIR_ESQUERDA; break; 
            case 3: MoverJogadorX(j, velocidadeAtual, gBombas, ignorarBombas);  j->direcaoAtual = DIR_DIREITA; break; 
            case 4: j->frameAtual = 0; break; 
        }
    }
    //Lógica pro robô
    else
    {
        float dx = 0.0f, dy = 0.0f;
        bool moveu = false;

        if (IsKeyDown(teclaCima) == true) { 
            dy -= velocidadeAtual; 
            j->direcaoAtual = DIR_CIMA; 
            moveu = true; 
        }
        if (IsKeyDown(teclaBaixo) == true) { 
            dy += velocidadeAtual; 
            j->direcaoAtual = DIR_BAIXO; 
            moveu = true; 
        }
        if (IsKeyDown(teclaEsquerda) == true) { 
            dx -= velocidadeAtual; 
            j->direcaoAtual = DIR_ESQUERDA; 
            moveu = true; 
        }
        if (IsKeyDown(teclaDireita) == true) { 
            dx += velocidadeAtual; 
            j->direcaoAtual = DIR_DIREITA; 
            moveu = true; 
        }

        if (moveu == false) {
            j->direcaoAtual = DIR_PARADO;
            j->frameAtual = 0;
        }

        MoverJogadorX(j, dx, gBombas, false);
        MoverJogadorY(j, dy, gBombas, false);
        
        if (IsKeyPressed(teclaBomba) == true && j->recargaBomba <= 0.0f)
        {
            AlinharEPlantarBomba(j, gBombas);
            j->recargaBomba = TEMPO_RECARGA_BOMBA; 
        }
    }
    
    if (j->direcaoAtual != DIR_PARADO) {
        j->temporizadorFrame += deltaTime;
        if (j->temporizadorFrame >= VELOCIDADE_FRAME_ANIMACAO) {
            j->temporizadorFrame = 0.0f;
            j->frameAtual = (j->frameAtual + 1) % NUM_FRAMES_ANDAR; 
        }
    } else {
        j->frameAtual = 0;
    }
}


//Desenhar jogador 
void DesenharJogador(const Jogador* j)
{
    if (j->vivo == false) {
        return; 
    }
    
    Rectangle recDestino = { j->pos.x, j->pos.y, TAMANHO_TILE, TAMANHO_TILE };
    Vector2 origem = { 0, 0 };
    Texture2D texDesenhar;

    switch (j->direcaoAtual) {
        case DIR_CIMA:    texDesenhar = j->texCima[j->frameAtual]; break;
        case DIR_BAIXO:   texDesenhar = j->texBaixo[j->frameAtual]; break;
        case DIR_ESQUERDA:texDesenhar = j->texEsquerda[j->frameAtual]; break;
        case DIR_DIREITA: texDesenhar = j->texDireita[j->frameAtual]; break;
        case DIR_PARADO:  texDesenhar = j->texParado; break; 
        default:          texDesenhar = j->texParado; break;
    }
    
    if (texDesenhar.id == 0) {
        texDesenhar = j->texParado;
    }
    
    Rectangle recFonte = { 0.0f, 0.0f, (float)texDesenhar.width, (float)texDesenhar.height };
    
    Color matiz = WHITE;
    if (j->temDefesa == true) {
        matiz = SKYBLUE;        
    } else if (j->temVelocidade == true) {
        matiz = LIME;  
    }

    DrawTexturePro(texDesenhar, recFonte, recDestino, origem, 0.0f, matiz);
}

//Matar jogador se ele explodir 
void DestruirJogador(Jogador* j)
{
    UnloadTexture(j->texParado);
    for (int i = 0; i < NUM_FRAMES_ANDAR; i++) {
        if (j->texCima[i].id > 0 && j->texCima[i].id != j->texParado.id) {
            UnloadTexture(j->texCima[i]);
        }
        //j->texBaixo[i] usa j->texParado, então a verificação deve ignorar se for a mesma ID
        if (j->texBaixo[i].id > 0 && j->texBaixo[i].id != j->texParado.id) {
            UnloadTexture(j->texBaixo[i]);
        }
        if (j->texEsquerda[i].id > 0 && j->texEsquerda[i].id != j->texParado.id) {
            UnloadTexture(j->texEsquerda[i]);
        }
        if (j->texDireita[i].id > 0 && j->texDireita[i].id != j->texParado.id) {
            UnloadTexture(j->texDireita[i]);
        }
    }
}