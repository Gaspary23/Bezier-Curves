//
//  Poligono.hpp
//  Metodos_Colisao
//
//  Created by Marcio Sarroglia Pinho on 18/08/20.
//  Copyright © 2020 Marcio Sarroglia Pinho. All rights reserved.
//

#ifndef Poligono_hpp
#define Poligono_hpp

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "Ponto.hpp"
#include <iostream>
#include <vector>
using namespace std;

class Poligono
{
    vector <Ponto> Vertices;
    Ponto Min, Max;
    
public:

    Poligono();
    Ponto getVertice(int);
    unsigned long getNVertices();
    void insereVertice(Ponto);
    void insereVertice(Ponto p, int pos);
    void desenhaPoligono();
    void desenhaVertices();
    void pintaPoligono();
    void imprime();
    void atualizaLimites();
    void obtemLimites(Ponto &Min, Ponto &Max);
    void LePoligono(const char *nome, bool is3D);
    void desenhaAresta(int n);
    void getAresta(int i, Ponto &P1, Ponto &P2);
    void alteraVertice(int i, Ponto P);
};

#endif 
