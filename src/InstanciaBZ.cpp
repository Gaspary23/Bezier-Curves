//
//  InstanciaBZ.cpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 22/09/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//

#include "../include/InstanciaBZ.h"
#include "../include/ListaDeCoresRGB.h"

// ***********************************************************
//  void InstanciaPonto(Ponto3D *p, Ponto3D *out)
//  Esta funcao calcula as coordenadas de um ponto no
//  sistema de referencia do universo (SRU), ou seja,
//  aplica as rotacoes, escalas e translacoes a um
//  ponto no sistema de referencia do objeto (SRO).
// ***********************************************************
void InstanciaPonto(Ponto &p, Ponto &out) {
    GLfloat ponto_novo[4];
    GLfloat matriz_gl[4][4];
    int i;

    glGetFloatv(GL_MODELVIEW_MATRIX, &matriz_gl[0][0]);

    for (i = 0; i < 4; i++) {
        ponto_novo[i] = matriz_gl[0][i] * p.x +
                        matriz_gl[1][i] * p.y +
                        matriz_gl[2][i] * p.z +
                        matriz_gl[3][i];
    }
    out.x = ponto_novo[0];
    out.y = ponto_novo[1];
    out.z = ponto_novo[2];
}

Ponto InstanciaPonto(Ponto P) {
    Ponto temp;
    InstanciaPonto(P, temp);
    return temp;
}

InstanciaBZ::InstanciaBZ() {
    Rotacao = 0;
    Posicao = Ponto(0, 0, 0);
    Escala = Ponto(0.5, 0.5, 0.5);

    nroDaCurva = 0;
    proxCurva = -1;
    tAtual = 0.0;
    direcao = 1;

    cor = rand() % 100;
}

InstanciaBZ::InstanciaBZ(Bezier *C, int nro, TipoFuncao *mod, float velocidade) {
    Rotacao = 0;
    Posicao = Ponto(0, 0, 0);
    Escala = Ponto(0.5, 0.5, 0.5);

    Curva = C;
    tAtual = 0;
    direcao = 1;

    nroDaCurva = nro;
    modelo = *mod;
    Velocidade = velocidade;
    
    cor = rand() % 100;
}

void InstanciaBZ::desenha() {
    //  Aplica as transformacoes geometricas no modelo
    glPushMatrix();
    glTranslatef(Posicao.x, Posicao.y, 0);
    glRotatef(Rotacao, 0, 0, 1);
    glScalef(Escala.x, Escala.y, Escala.z);

    (*modelo)();  // desenha a instancia
    glPopMatrix();
}
Ponto InstanciaBZ::ObtemPosicao() {
    // aplica as transformacoes geometricas no modelo
    // desenha a geometria do objeto
    glPushMatrix();
    glTranslatef(Posicao.x, Posicao.y, 0);
    glRotatef(Rotacao, 0, 0, 1);
    Ponto PosicaoDoPersonagem;
    Ponto Origem(0, 0, 0);
    InstanciaPonto(Origem, PosicaoDoPersonagem);
    glPopMatrix();
    return PosicaoDoPersonagem;
}

void InstanciaBZ::AtualizaPosicao(float tempoDecorrido) {
    float distanciaPercorrida = Velocidade * tempoDecorrido;
    tAtual +=  direcao * Curva->CalculaT(distanciaPercorrida);

    // Vai ate o fim e volta
    if (tAtual > 1.0) {
        tAtual = 1.0;
        direcao = -1;
    }
    if (tAtual < 0.0) {
        tAtual = 0.0;
        direcao = 1;
    }

    // atualiza a posicao do personagem
    Posicao = Curva->Calcula(tAtual);
    
    // atualiza a rotacao do personagem
    Rotacao = atan2(Posicao.y, Posicao.x) * 180 / (M_PI);
    //cout << Rotacao << endl;
}
