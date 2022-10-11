//
//  InstanciaBZ.cpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 22/09/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//

#include "../include/InstanciaBZ.h"

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
    Escala = Ponto(1, 1, 1);

    nroDaCurva = 0;
    proxCurva = -1;
    tAtual = 0.0;
    direcao = 1;
}

InstanciaBZ::InstanciaBZ(Bezier *C) {
    Rotacao = 0;
    Posicao = Ponto(0, 0, 0);
    Escala = Ponto(1, 1, 1);

    Curva = C;
    tAtual = 0;
    direcao = 1;
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
    //  Atualiza a posicao do personagem
    //  de acordo com a curva e o tempo decorrido
    //  desde a ultima atualizacao

    //  Calcula a posicao do personagem
    //  de acordo com a curva e o tempo decorrido
    //  desde a ultima atualizacao
    tAtual += tempoDecorrido * direcao * Velocidade;
    if (tAtual > 1.0) {
        tAtual = 1.0;
        direcao = -1;
    }
    if (tAtual < 0.0) {
        tAtual = 0.0;
        direcao = 1;
    }
    Posicao = ObtemPosicao();

    //  Calcula a rotacao do personagem
    //  de acordo com a curva e o tempo decorrido
    //  desde a ultima atualizacao
    Ponto Tangente = Curva->Calcula(tAtual);
    Rotacao = atan2(Tangente.y, Tangente.x) * 180 / M_PI;

    Posicao.x += Tangente.x * 0.1;
    Posicao.y += Tangente.y * 0.1;

    //  Verifica se o personagem chegou ao final da curva
    if (tAtual == 1.0) {
        //  Se chegou ao final da curva, verifica se
        //  existe uma proxima curva
        if (proxCurva != -1) {
            //  Se existe uma proxima curva, atualiza
            //  o ponteiro para a curva atual
            nroDaCurva = proxCurva;
            proxCurva = -1;
            tAtual = 0.0;
            direcao = 1;
        }
    }
}
