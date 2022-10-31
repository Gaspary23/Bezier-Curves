#ifndef AnimacaoComBezier_Hpp
#define AnimacaoComBezier_Hpp

#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <tuple>
#include <vector>

using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "CalculosPersonagens.hpp"
#include "InstanciaBZ.hpp"
#include "ListaDeCoresRGB.hpp"
#include "Poligono.hpp"
#include "Ponto.hpp"
#include "Temporizador.hpp"

void CarregaModelos();
void CriaPersonagens();
void CriaCurvas();
void CriaMapaCurvas();

void DesenhaTriangulo();
void DesenhaSeta();
void DesenhaPersonagens(bool atualizaMain);
void DesenhaCurvas();

void contaTempo(double tempo);
void init();
void display(void);
void animate();
void keyboard(unsigned char key, int x, int y);
void arrow_keys(int a_keys, int x, int y);
void reshape(int w, int h);
int main(int argc, char** argv);

#endif