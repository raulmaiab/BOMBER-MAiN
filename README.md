## 💣 BOMBERMAIN

Um jogo inspirado em Bomberman, desenvolvido totalmente em C + Raylib para o projeto da cadeira de PIF da CESAR School.

---
## 🎮 Sobre o Jogo

O **_BomberMain_** é um jogo estilo grid-based onde você controla um personagem capaz de posicionar bombas, destruir blocos, avançar em fases e navegar por diferentes menus e modos de jogo.

Tudo foi desenvolvido utilizando apenas:

- Lógica em C

- Renderização com raylib

- Sprites 2D, realizados com Inteligência Artificial

- Menus criados pela equipe

- Telas de vitória/derrota

- Sistema de história e de batalha

---
## 🧩 Funcionalidades Principais

👤 Controle de movimentação do personagem

💣 Colocação e gerenciamento de bombas

💥 Explosão com física e colisão

🧱 Mapa destrutível

🧭 Múltiplos menus (início, história, batalha, vitória, derrota, opções)

🎨 Tela inicial programática (neon)

☑️ Estrutura modular em C

---
## ⚙️ Como Instalar e Jogar

Abaixo está o passo a passo completo para baixar, compilar e rodar o jogo em sua máquina.



📥 **1. Baixar o Repositório**


✔️ Opção 1 — Clonar via Git
git clone https://github.com/raulmaiab/BOMBER-MAiN
cd BOMBER-MAIN

✔️ Opção 2 — Baixar ZIP

- Abra o repositório no GitHub

- Clique no botão verde Code

- Clique em Download ZIP

- Extraia numa pasta

- Abra a pasta no VSCode


📚 **2. Instalar a Raylib**


🔵 Linux (Debian/Ubuntu):
sudo apt install libraylib-dev

🟣 Windows (MSYS2 + MinGW):
pacman -S mingw-w64-ucrt-x86_64-raylib


Abra o terminal MSYS2 UCRT64 para compilar corretamente no Windows.


🧱 **3. Compilar o Jogo**


No terminal, dentro da pasta do projeto, execute:

gcc bombermain.c inicio.c menu.c menu_battle.c mapa.c jogador.c bomba.c explosao.c extras.c derrota.c vitoria.c menu_story.c options.c menu_prox.c vitoria_battle.c historico.c -o BomberMain $(pkg-config --cflags --libs raylib)


Se tudo correr bem, isso irá gerar o executável:

./BomberMain

▶️ **4. Rodar o Jogo**

Linux:
./BomberMain

Windows:
./BomberMain.exe

---
## 🖥️ Requisitos Mínimos

 - CPU dual-core

- 2GB RAM

- Placa com OpenGL 3.3+

- Raylib instalada corretamente

- VSCode recomendado

---
## 🤝 Como Contribuir

_Contribuições são bem-vindas!_
Siga o fluxo:

1. Faça um fork do repositório

2. Crie sua branch

3. Commit suas mudanças

4. Abra um Pull Request

---
## 🕹️ Controles do Jogo


Mover:	WASD ou Setas

Colocar bomba: Espaço ou J

Confirmar:	Enter

Sair/Tela cheia:	ESC / F11

---
## 👨‍💻 Autores do Projeto

**Arthur Moury**

**L. Guilherme Silvestre**

**Raul Maia**

---
## 🎥 Vídeo de Demonstração (Screencast)

Assista ao vídeo de demonstração completa do jogo no YouTube:

[Screencast - BOMBERMAIN](https://youtu.be/J-59WFnmf6s)

---
### ⭐ Deixe uma Estrela!

Se curtiu o projeto, deixe uma ⭐ no GitHub!

Isso ajuda muito e fortalece o trabalho do grupo.
