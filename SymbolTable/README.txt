Compilação:
g++ -std=c++17 -Wall -Wextra -o EP1 EP1.cpp

Uso:
Devem ser fornecidos 2 parâmetros obrigatoriamente: O nome do texto a ser inserido na tabela de símbolos e o código
para o tipo de tabela. A saber:

VD: Vetor Desordenado
VO: Vetor Ordenado
LD: Lista Desordenada
LO: Lista Ordenada
AB: Árvore Binária
TR: Treap
RN: Árvore Rubro Negra
A23: Árvore 2-3
HA: Tabela de Hashing

Além disso podem ser fornecidos um terceiro e um quarto parâmetros:

Terceiro parâmetro:

Se o terceiro parâmetro for o caractere D, o programa entra em modo interativo puro,
que não lê o texto: apenas processa input do usuário. Esse é o "modo de depuração".

Se o terceiro parâmetro for o caractere E, o programa executa o módulo de experimentos,
e não faz a parte interativa. Esse é o "modo experimental".

Se o terceiro parâmetro for a letra S, ou não for fornecido, o programa executa o "modo padrão":
Lê o texto, insere as palavras na tabela, e permite ao usuario chamar as funções (modo interativo).

Quarto parâmetro:

Se, e apenas se, o segundo parâmetro for TR ou HA, o quarto parâmetro será um parâmetro opcional,
que tem de ser um número inteiro positivo, que servirá para semente do Treap ou para tamanho da 
tabela de hashing. Caso não seja fornecido, serão usados valores padrão. Para fornecer o quarto
parâmetro, é necessário fornecer o terceiro. Embora possa ser interessante para depuração, é recomendado
não fornecer o quarto parâmetro, e usar os defaults que são corretamente gerados (aleatório para o TR e
o primo mais próximo a um quinto do tamanho do texto, para o HA).