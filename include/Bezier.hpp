//
//  CurvaBezier.hpp
//  Metodos_Colisao
//
//  Created by Marcio Sarroglia Pinho on 10/10/21.
//  Copyright © 2021 Marcio Sarroglia Pinho. All rights reserved.
//
#ifndef Bezier_hpp
#define Bezier_hpp

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include <iostream>
#include "Ponto.hpp"
using namespace std;

class Bezier
{
    Ponto Coords[3];
public:
    float ComprimentoTotalDaCurva;
    int cor;
    
    Bezier();
    Bezier(Ponto P0, Ponto P1, Ponto P2);
    Bezier(Ponto V[]);
    Ponto Calcula(double t);
    Ponto getPC(int i);
    void Traca();
    double CalculaT(double distanciaPercorrida);
    void calculaComprimentoDaCurva();
};
#endif 
