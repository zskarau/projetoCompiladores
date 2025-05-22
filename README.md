# Projeto Compiladores
Trabalho 1 feito por Thales Janisch Santos e Lucas Stadler Karau para a matéria de Compiladores

## Etapas

### 1ª Etapa: Entendimento e Representação da Linguagem
Nessa etapa, cada equipe deverá entender por completo a linguagem vista em aula e identificar os elementos que precisam ser adicionados, tanto em termos das regras léxicas quanto sintáticas.

Apresentação e Representação da Gramática
Para representar a gramática da linguagem, devem ser utilizadas duas notações:

- [X] (a) A notação BNF (Backus-Naur Form), ou alguma extensão como a EBNF.

- [X] (b) Diagramas de sintaxe, que constituem uma representação gráfica para a BNF.

Observações:

Existem ferramentas que podem facilitar a criação desses diagramas, por exemplo:

- [X] Railroad Diagram

O entendimento dessas notações deve ser pesquisado pela própria equipe, pois faz parte do trabalho.

Apresentação e Representação das Regras de Análise Léxica
Para representar as regras léxicas, devem ser usados diagramas de transição.

- [X] Debuggex

### 2ª Etapa: Implementação do Analisador Léxico
- [X] Nesta etapa, será implementado o analisador léxico conforme as definições de tokens feitas na 1ª etapa.

É parte do trabalho compreender claramente a separação de responsabilidades entre:

- Analisador Léxico: responsável por reconhecer os tokens da linguagem.

- Analisador Sintático: responsável por interpretar a estrutura gramatical.

### 3ª Etapa: Implementação do Analisador Sintático
- [X] As regras sintáticas representadas na 1ª etapa devem ser implementadas por meio da ferramenta Bison.

### 4ª Etapa: Criação da Tabela de Símbolos (TS)
- [X] É obrigatória a utilização de uma tabela de símbolos para gerenciar identificadores e demais elementos da linguagem.

Gerenciamento de Entrada e Saída
A entrada e saída das informações devem ser gerenciadas e manipuladas através de arquivos.

### 5ª Etapa: Conjunto de Testes
- [X] Elaborar um conjunto de testes significativo, ou seja, que represente todos os elementos da linguagem, como comandos, funções, variáveis, etc.

Incluir testes que contenham erros de análise sintática, para garantir que o parser seja capaz de identificá-los corretamente.

### 6ª Etapa: Escrita do Relatório
- [X] Elaborar o relatório do trabalho, conforme especificação fornecida posteriormente.

### 7ª Etapa: Preparação da Defesa
- [X] Preparar a defesa do trabalho, com todos os integrantes da equipe cientes e aptos a apresentar qualquer parte do projeto.

## O que deve ser adicionado?

Tendo a linguagem de Programação Básica (baseada na calculadora vista em aula),
faz-se necessário adicionar os seguintes elementos:

- [X] Comando FOR, conforme a sintaxe:
      FOR(init; cond; inc) lista-comandos

      onde:
        init representa a inicialização da variável de controle;
        cond é a condição do laço;
        inc é o incremento do laço.
    
- [X] Operadores lógicos AND e OR, considerando que:
ambos são associativos à esquerda;
possuem prioridade menor que os operadores aritméticos.
