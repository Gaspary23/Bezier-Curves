//
//  Instancia.h
//  OpenGLTest
//
//  Created by Marcio Sarroglia Pinho on 22/09/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//
#ifndef Instancia_hpp
#define Instancia_hpp
#include <iostream>
#include "Bezier.hpp"
#include "Poligono.hpp"

typedef void TipoFuncao();

class InstanciaBZ{

public:
    InstanciaBZ();
    InstanciaBZ(Bezier *Curva, int nro, TipoFuncao *mod, Poligono *ref, float velocidade, int dir);
    
    TipoFuncao *modelo; // Modelo
    Poligono modelRef; // Modelo de referencia

    Bezier *Curva; // referencia para a curva onde esta a instancia
    Ponto Posicao, Escala;
    float Rotacao;
    int cor;
    int nroDaCurva;
    int proxCurva;
    float Velocidade;
    float tAtual;
    int direcao; // andando do fim para o inicio, ou ao contrario

    void desenha();
    void AtualizaPosicao(float tempoDecorrido);
    Ponto ObtemPosicao();
};

#endif /* Instancia_hpp */
