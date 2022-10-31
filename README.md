
# Trabalho sobre Métodos de Colisão

Trabalho 2 da disciplina de Computação Gráfica desenvolvido por Marcio Pinho e alterado por Pedro Gaspary e Lucas Cunha. O código foi desenvolvido em C++ e utiliza a biblioteca OpenGL.

O programa é um jogo estilo pacman, em que o jogador deve desviar dos inimigos em um cenário de curvas bezier. O modelo do jogador é uma seta verde, enquanto os inimigos são triângulos de diversas cores.

A curva em que o personagem do jogador está e a próxima em que entrará estarão destacadas, tendo sua espessura aumentada.

Quando o jogador colidir com um inimigo que está na mesma curva o jogo é encerrado.

## Como compilar

Para compilar o programa, basta executar, na pasta raíz do projeto, o comando:

    $ make

O programa será compilado automaticamente.

## Como executar

Para executar o programa, basta executar, na pasta raíz do projeto, o comando:

    $ ./Bezier

O programa será executado automaticamente. 

## Como limpar

Para limpar o projeto, basta executar, na pasta raíz do projeto, o comando:

    $ make clean

O programa será limpo automaticamente.

## Comandos do Programa
### Movimento do Personagem do Jogador

O personagem do jogador se movimenta sozinho ao longo das curvas, porém o jogador pode alterar sua direção, fazê-lo parar ou reiniciar o movimento, além de aumentar ou diminuir sua velocidade. Ademais, o jogador pode selecionar a próxima curva em que o jogador irá entrar.

`up` - Aumenta a velocidade do personagem

`down` - Diminui a velocidade do personagem

> A velocidade padrão é uma constante 3. Tanto o aumento quanto o decremento são em valores de 0.5, até um máximo de 8 e um mínimo de 0.5

`left` - Muda a próxima curva em que o personagem entrará

`right` - Muda a próxima curva em que o personagem entrará

>Os comandos 'right' e 'left' fazem a mesma coisa, mas escolhem curvas rotando em sentidos diferentes a partir do ponto de chegada

`space` - Para o personagem se estiver se movendo e reinicia o movimento se estiver parado

`d`- Muda a direção em que o personagem está se movendo na curva

### Janela

`f` - Deixa a janela do jogo em Full Screen

`r`- Retorna o tamanho da janela para o padrão 

`t` - Conta o número de frames nos próximos 3 segundos

`esc` - Termina o programa
