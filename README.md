# Árvores Balanceadas: AVL e Rubro-Negra

Trabalho da Unidade II de IMD0039 - Estruturas de Dados Básicas II (UFRN / Instituto Metrópole Digital)

Autor: Brunno César de Almeida Penha - matrícula 20240070366

## O que foi implementado

- Árvore AVL e árvore Rubro-Negra, feitas do zero em C++17 (sem bibliotecas de balanceamento)
- Operações: inserir, remover, buscar, percurso em ordem, imprimir a árvore, altura/estatísticas e validação das propriedades
- Na AVL a impressão mostra o fator de balanceamento (fb). Na Rubro-Negra mostra a cor: (V) vermelho e (P) preto
- Valor repetido: a inserção é recusada e a árvore não muda
- Busca ou remoção de valor inexistente: retorna falso e nada é alterado
- Rotação dupla é contada como 2 rotações

## Arquivos

- `src/arvore.hpp`: interface comum
- `src/avl.hpp`: árvore AVL
- `src/rb.hpp`: árvore Rubro-Negra
- `src/main.cpp`: programa com menu (o usuário escolhe a árvore ao iniciar)
- `src/testes.cpp`: testes e comparação das duas árvores
- `bin/`: versão compilada (`arvores.exe` e `testes.exe` para Windows; `arvores` e `testes` para Linux)

## Como compilar e executar

No Windows, basta executar `bin\arvores.exe` e `bin\testes.exe` (já compilados, sem dependências).

Para compilar, precisa de g++ com suporte a C++17.

```
make
./arvores
./testes
```

Sem make (no Windows, com MinGW/MSYS2, adicione `-static` e `.exe` ao nome da saída):

```
g++ -O2 -std=c++17 -o arvores src/main.cpp
g++ -O2 -std=c++17 -o testes src/testes.cpp
```

## Exemplo de uso

```
Escolha a arvore:
1 - AVL
2 - Rubro-Negra
> 2
```

Depois, o menu permite inserir (1), remover (2), buscar (3), percurso em ordem (4), imprimir (5), ver altura e contadores (6) e validar propriedades (7).

## Testes

`./testes` roda 50 verificações (árvore vazia, um elemento, repetidos, inexistentes, remoção de folha/um filho/dois filhos/raiz, inserção crescente/decrescente/aleatória, 20000 operações aleatórias validando a cada uma) e depois imprime a comparação de altura, rotações e recolorações entre as duas árvores, usando as mesmas sequências.
