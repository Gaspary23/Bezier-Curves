//
//  InstanciaBZ.cpp
//  Metodos_Colisao
//
//  Created by Marcio Sarroglia Pinho on 22/09/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//
//  Alterado para o trabalho da disciplina por:
//  Pedro da Cunha Gaspary - 21101429 e Lucas Marchesan Cunha - 21101060
//

#include "../include/InstanciaBZ.hpp"
#include "../include/Bezier.hpp"
#include "../include/Poligono.hpp"
#include "../include/Ponto.hpp"

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
    Escala = Ponto(1, 1, 1);
    
    Rotacao = 0;
    Posicao = Ponto(0, 0, 0);

    nroDaCurva = 0;
    proxCurva = -1;
    tAtual = 0.0;
    direcao = 1;
    cor = rand() % 97;
}

InstanciaBZ::InstanciaBZ(Bezier *C, int nro, TipoFuncao *mod, Poligono *ref, float velocidade, int dir) {
    Escala = Ponto(1, 1, 1);

    Curva = C;
    tAtual = 0.5;  // Inicia no meio da curva
    Posicao = C->Calcula(tAtual);

    direcao = dir;
    nroDaCurva = nro;
    Velocidade = velocidade;
    
    modelo = *mod;
    modelRef = *ref;
    cor = rand() % 97;
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
    // calcula a nova posicao do personagem
    float distanciaPercorrida = Velocidade * tempoDecorrido;
    float deltaT = direcao * Curva->CalculaT(distanciaPercorrida);
    tAtual += deltaT;

    // atualiza a posicao do personagem
    Posicao = Curva->Calcula(tAtual);

    // calcula o proximo ponto da curva e o angulo entre o vetor atual e o proximo
    float tFuturo = tAtual + deltaT;
    Ponto v0 = Ponto(1, 0, 0);
    Ponto v1 = Curva->Calcula(tFuturo) - Curva->Calcula(tAtual);
    float angulo = acos(ProdEscalar(v0, v1) / v1.modulo()) * 180 / M_PI;

    // atualiza a rotacao do personagem
    Rotacao = v1.y >= 0 ? angulo : -angulo;
}
