# BATALHA NAVAL – Projeto PIF 2025.2  
Implementação do clássico jogo **Batalha Naval** utilizando a linguagem **C**, da disciplina **Programação Imperativa Fundamental (PIF)**.

---

### Nosso Propósito
Desenvolver uma implementação sólida, modular e didática do jogo Batalha Naval, servindo como:

- Base de estudo para ponteiros, matrizes, structs e enums.  
- Exercício prático de organização de código em múltiplos módulos.  
- Projeto incremental para consolidar fundamentos da programação imperativa em C.  


### Objetivos do Projeto

- Criar um tabuleiro dinâmico com validação de limites.  
- Implementar uma frota completa de navios com controle de acertos e afundamentos.  
- Desenvolver o posicionamento manual e automático dos navios.  
- Controlar o loop principal do jogo, alternando turnos entre jogadores.  
- Registrar e exibir estatísticas: tiros, acertos e precisão.  
- Garantir alocação e liberação seguras de memória.  

---
## Como Rodar o Projeto

### **1. Clonar o repositório**

```bash
git clone (https://github.com/malupmelo/Jogo-Batalha-Naval.git)
cd Jogo-Batalha-Naval
```

### **2. Compilar o projeto**

```bash
make
```

### **3. Executar o projeto**

```bash
./battleship
```

### **4. Limpar arquivos compilados**

```bash
make clean
```

---

### Funcionalidades Implementadas
1. Menus completos
- Menu principal
- Menu de configurações
- Validação de todas as entradas

2. Configurações do jogo
- Alteração de apelidos
- Tamanho do tabuleiro ajustável (6 a 26)
- Modos de posicionamento:
- J1 manual / J2 automático
- Ambos automáticos
- Ambos manuais

3. Posicionamento da frota
- Automático (aleatório e seguro)
- Manual (com validação de posição e orientação H/V)

4. Mecânica do jogo
- Turnos alternados
- Leitura de coordenadas estilo A5, C10…
- Tratamento de:
  Tiro repetido
  Acerto
  Água
  Afundamento com nome do navio
  Contagem de navios restantes
  Detecção automática de vitória

5. Estatísticas por jogador
- Tiros disparados
- Tiros acertados
- Precisão %
- Impressão ao final da partida

6. Interface visual aprimorada
- Dois tabuleiros lado a lado (tiros + navios)
- Cores ANSI:
  Azul → água
  Amarelo → erro
  Verde → acerto
  Vermelho → afundado
  Feedback claro a cada jogada

## Nossa Equipe

- Denise Hollanda  
- João Victor Astle  
- Maria Luiza Melo  








