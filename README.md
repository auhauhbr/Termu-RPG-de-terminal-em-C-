# Termu

## Sobre o projeto

O Termu é um projeto simples de estudo desenvolvido para treinar meus conhecimentos em C++, lógica de programação, estruturas de dados, algoritmos, orientação a objetos, organização de código e solução de problemas.

A ideia não foi criar um jogo comercial ou um sistema muito grande. O objetivo foi pegar os assuntos estudados e transformar tudo em um programa completo, com começo, meio e fim, executado diretamente no terminal.

Durante o desenvolvimento, procurei praticar código limpo, separação de responsabilidades, validação de entrada, uso da biblioteca padrão do C++, organização em arquivos de cabeçalho e implementação, testes e compilação de um programa formado por vários módulos.

O projeto foi desenvolvido com base nos meus estudos do livro [Estrutura de dados e algoritmos em C++](https://www.amazon.com.br/Estrutura-dados-algoritmos-em-c/dp/8522125732/?&_encoding=UTF8&camp=1789&creative=9325), edição em português, escrito por [Adam Drozdek](https://www.amazon.com.br/Drozdek/e/B001ITRR1M/ref=dp_byline_cont_book_1) e publicado pela editora Cengage Learning.

O livro serviu como referência de estudo para pensar melhor sobre estruturas, organização dos dados, responsabilidades das classes, fluxo dos algoritmos e formas de resolver os problemas encontrados durante a construção do jogo.

## O que é o Termu

Termu é um RPG de combate por turnos que funciona inteiramente no terminal. O jogador cria um personagem, escolhe uma classe, explora áreas, enfrenta monstros, recebe experiência e ouro, compra itens, equipa armas e armaduras e evolui suas habilidades.

Todo o estado do jogo permanece na memória enquanto o programa está aberto. Esta versão não possui sistema de salvamento, interface gráfica ou modo multiplayer.

O jogo foi escrito em C++17 e utiliza somente a biblioteca padrão da linguagem e recursos do sistema operacional necessários para configurar o terminal em UTF-8.

A apresentação segue um estilo de RPG clássico de terminal, com cores, barras de atributos, molduras e artes em texto.

## Objetivos de estudo

Este projeto foi usado para praticar os seguintes assuntos:

- lógica de programação;
- estruturas condicionais e estruturas de repetição;
- funções e passagem de parâmetros;
- classes, objetos e encapsulamento;
- composição entre objetos;
- herança e polimorfismo;
- classes abstratas e métodos virtuais;
- enumerações fortemente tipadas;
- vetores, mapas, strings e outros recursos da STL;
- ponteiros inteligentes com `std::unique_ptr`;
- números aleatórios com `std::mt19937`;
- tratamento e validação de entradas;
- separação entre regras de negócio e entrada ou saída;
- organização de projetos C++ com arquivos `.hpp` e `.cpp`;
- compilação, ligação dos módulos e geração do executável;
- criação de testes automatizados;
- leitura e implementação de um documento de requisitos.

## Números do projeto

Na versão atual, o projeto possui aproximadamente:

- 2.451 linhas físicas em arquivos C++;
- 2.089 linhas de código, sem contar linhas vazias e comentários isolados;
- 34 arquivos de código entre cabeçalhos, implementações, testes e exemplos;
- 10 classes;
- 7 estruturas;
- 8 enumerações fortemente tipadas;
- 8 declarações de métodos virtuais na interface de combate;
- 4 classes jogáveis;
- 5 modelos de monstros;
- 8 habilidades;
- 6 itens disponíveis no catálogo;
- 1 conjunto de testes automatizados para regras e integração de combate.

Esses números podem mudar conforme o projeto continuar recebendo alterações.

## Organização do código

O código não foi colocado inteiro dentro do arquivo `main.cpp`. Cada parte principal possui seu próprio módulo.

O arquivo `main.cpp` é apenas o ponto de entrada. Ele configura o terminal, cria um objeto da classe `Jogo` e inicia o laço principal.

A pasta `include/termu` contém os arquivos de cabeçalho. Neles ficam as declarações das classes, estruturas, enumerações e funções.

A pasta `src` contém as implementações. Nela ficam as regras do jogo, os menus, o combate, os catálogos e o ponto de entrada.

A pasta `tests` contém os testes das fórmulas e de um combate com entradas simuladas.

Os principais módulos são:

### Jogo

A classe `Jogo` controla a tela inicial, a criação do personagem, o Hub, a exploração, a loja, o inventário, o status e o fim da sessão.
<img width="1490" height="846" alt="image" src="https://github.com/user-attachments/assets/55e92298-9b0c-4b37-bcf2-e14d8558fc14" />

### Combatente

`Combatente` é uma classe abstrata que representa o comportamento comum de qualquer participante de uma batalha.

`Personagem` e `Monstro` herdam de `Combatente` e implementam os métodos virtuais de acordo com suas próprias regras. O motor de combate consegue receber uma referência para `Combatente` e chamar a implementação correta sem precisar descobrir manualmente se o objeto é um personagem ou um monstro.

Essa parte foi usada para praticar herança, abstração e polimorfismo dinâmico.

### Personagem

A classe `Personagem` armazena nome, classe, nível, experiência, ouro, atributos, recurso, habilidades, inventário, equipamentos e efeitos de status.

Ela também é responsável por subir de nível, gastar recursos, receber dano, recuperar vida, desbloquear habilidades e recalcular seus atributos.
<img width="1403" height="837" alt="image" src="https://github.com/user-attachments/assets/9b968784-9d98-4dce-a9fa-c0862c4ef755" />

### Monstro

A classe `Monstro` representa os inimigos encontrados durante a exploração.

Cada monstro possui nível, atributos, tipo, afinidades elementais, padrão de inteligência artificial, recompensa de experiência, recompensa de ouro e indicação de chefe.

### Motor de combate

O `MotorCombate` controla as rodadas, a iniciativa, as ações do jogador, os ataques dos monstros, a defesa, a fuga, o uso de itens, as habilidades e os efeitos de status.

As fórmulas matemáticas ficam separadas no módulo `Regras`, para que possam ser testadas sem depender do terminal.

### Inventário e equipamento

O inventário utiliza um vetor de entradas contendo o identificador do item e sua quantidade.
<img width="1416" height="841" alt="image" src="https://github.com/user-attachments/assets/d9b9f52d-fa60-4308-b693-6c6c184f8c79" />

O equipamento utiliza um mapa que relaciona cada espaço de equipamento ao item equipado. Os espaços disponíveis são arma, armadura e acessório.
<img width="1415" height="822" alt="image" src="https://github.com/user-attachments/assets/e8e8e357-395e-4bba-86f9-364799a8b975" />

### Loja

A loja controla estoque, compra e venda. Antes de alterar ouro ou inventário, todas as condições são verificadas para evitar que uma operação incompleta deixe o estado do jogo incorreto.
<img width="1402" height="826" alt="image" src="https://github.com/user-attachments/assets/91ffdabf-2c51-4522-bd15-edacc4328726" />

### Catálogo

O catálogo concentra os dados de classes, habilidades, itens e monstros. Isso evita espalhar números de balanceamento por vários arquivos do projeto.

### Entrada

A classe `Entrada` centraliza a leitura do teclado. Ela rejeita opções fora do intervalo, textos vazios e valores numéricos inválidos.

O uso de `std::from_chars` evita depender de conversões que lançam exceções para entradas numéricas incorretas.

### Aleatório

A classe `Aleatorio` mantém um único gerador `std::mt19937`. Dessa forma, os sorteios não ficam espalhados pelo código e os testes podem utilizar uma semente fixa.

## Classes disponíveis
<img width="1443" height="846" alt="image" src="https://github.com/user-attachments/assets/a4fb9f48-b0ca-4780-942b-f29222a8dc0d" />


### Guerreiro

O Guerreiro possui bastante vida, ataque físico e defesa. Seu recurso é a Fúria, obtida durante o combate.

Habilidades disponíveis:

- Golpe Pesado;
- Lâmina Giratória.

### Mago

O Mago possui menos vida e defesa física, mas causa bastante dano mágico. Seu recurso é a Mana, recuperada durante as rodadas.

Habilidades disponíveis:

- Bola de Fogo;
- Lança de Gelo.

### Ladino

O Ladino possui velocidade, esquiva e chance de crítico elevadas. Seu recurso é a Energia.

Habilidades disponíveis:

- Ataque Furtivo;
- Lâmina Envenenada.

### Clérigo

O Clérigo utiliza Fé e possui uma habilidade de cura, permitindo maior sobrevivência em uma partida solo.

Habilidades disponíveis:

- Luz Curativa;
- Castigo Sagrado.

## Monstros
<img width="1391" height="845" alt="image" src="https://github.com/user-attachments/assets/bc06d075-94af-469b-a295-8f6ef029fec4" />

### Lobo Selvagem

Inimigo inicial do tipo Bestial. Possui comportamento agressivo e fraqueza contra Fogo.

### Slime de Gelo

Inimigo do tipo Elemental. Possui fraqueza contra Fogo e imunidade contra Gelo.

### Bandido

Inimigo Humanoide com comportamento misto. Pode variar entre ataques normais e ataques mais fortes.

### Golem de Pedra

Inimigo Elemental com bastante vida e defesa. Possui resistência contra dano Físico e fraqueza contra Raio.

### Guardião Sombrio

Chefe do tipo Sombrio. Não permite fuga, é imune ao elemento Sombrio e possui fraqueza contra Sagrado.

Quando sua vida fica abaixo de 50%, entra em uma fase mais agressiva e passa a causar mais dano.

## Sistema de combate

A ordem das ações é definida pela Velocidade. Se jogador e monstro tiverem a mesma velocidade, o jogador age primeiro.

Durante o turno, o jogador pode:

1. atacar;
2. usar uma habilidade;
3. usar um item;
4. defender;
5. tentar fugir.

A fuga não fica disponível durante batalhas contra chefes.

O combate possui dano físico, dano mágico, crítico, esquiva, afinidade elemental, defesa e efeitos de status.

Os efeitos implementados são Queimadura, Veneno, Lentidão e Atordoamento.

## Progressão

O jogador recebe experiência e ouro ao vencer um combate.

A experiência necessária para o próximo nível utiliza uma curva baseada na fórmula:

```text
50 * nivel^1.5
```

Ao subir de nível, o personagem recebe um Ponto de Habilidade. Esses pontos podem ser usados para aumentar uma habilidade até o nível 5.

Cada evolução aumenta a força da habilidade e reduz seu custo de recurso.

## Loja e itens
<img width="1378" height="842" alt="image" src="https://github.com/user-attachments/assets/e2fdc150-f45a-486c-8e5b-dbea71060751" />

O catálogo atual possui:

- Poção de Vida;
- Tônico de Recurso;
- Espada de Ferro;
- Cajado de Carvalho;
- Armadura de Couro;
- Anel de Prata.

Os consumíveis possuem estoque ilimitado. Alguns equipamentos possuem estoque limitado.

As armas também podem possuir restrições de classe. Por exemplo, a Espada de Ferro pode ser usada por Guerreiro e Ladino, enquanto o Cajado de Carvalho pode ser usado por Mago e Clérigo.

## Fases do desenvolvimento

O desenvolvimento foi dividido nas seguintes etapas:

1. leitura e interpretação dos requisitos;
2. definição das estruturas e enumerações;
3. criação dos catálogos de dados;
4. implementação de inventário e equipamento;
5. implementação de personagem e monstro;
6. criação das fórmulas de dano, fuga e experiência;
7. implementação da loja;
8. implementação do combate;
9. criação dos menus e do fluxo principal;
10. validação das entradas;
11. criação dos testes;
12. correção da codificação UTF-8 no terminal do Windows;
13. refatoração dos nomes e comentários para português.

## Compilação

Para compilar no Windows com MinGW:

```powershell
powershell -ExecutionPolicy Bypass -File .\build.ps1
```

Para compilar e executar:

```powershell
powershell -ExecutionPolicy Bypass -File .\build.ps1 -Executar
```

Também é possível utilizar CMake:

```powershell
cmake -S . -B build
cmake --build build
```

## Testes

Para compilar e executar os testes:

```powershell
powershell -ExecutionPolicy Bypass -File .\build.ps1 -Testar
```

Os testes verificam fórmulas de dano, modificadores elementais, chance de fuga, experiência, evolução de habilidades, compra, equipamento e um fluxo de combate com entradas simuladas.

## Como o executável é criado

Os arquivos `.hpp` e `.cpp` são escritos manualmente. O arquivo `.exe` não é escrito manualmente.

Durante a compilação, o compilador traduz os arquivos C++ para código de máquina. Depois, o ligador junta os módulos e as partes necessárias da biblioteca padrão, gerando o arquivo `termu.exe`.

Sempre que algum arquivo de código é alterado, o programa precisa ser compilado novamente para que a alteração apareça no executável.

## Possíveis melhorias
...
## Considerações finais

O Termu foi criado principalmente para estudar. Mesmo sendo um jogo simples de terminal, ele ajudou a trabalhar vários assuntos em conjunto e mostrou como um programa maior pode ser dividido em partes menores.

Mais importante que a quantidade de funcionalidades foi o processo de pensar nas regras, organizar os dados, encontrar problemas, testar soluções e refatorar o código.
