//
//  Bezier.cpp
//  OpenGL
//
#include "../include/Bezier.hpp"
#include "../include/ListaDeCoresRGB.hpp"

// **********************************************************************
//
// **********************************************************************
void Bezier::calculaComprimentoDaCurva() {
    double DeltaT = 1.0 / 50;
    double t = DeltaT;
    Ponto P1, P2;
    ComprimentoTotalDaCurva = 0;

    P1 = Calcula(0.0);
    while (t < 1.0) {
        P2 = Calcula(t);
        ComprimentoTotalDaCurva += calculaDistancia(P1, P2);
        P1 = P2;
        t += DeltaT;
    }
    P2 = Calcula(1.0);  // faz o fechamento da curva
    ComprimentoTotalDaCurva += calculaDistancia(P1, P2);
}
// **********************************************************************
//
// **********************************************************************
Bezier::Bezier() {
    for (int i = 0; i < 3; i++)
        Coords[i] = Ponto(0, 0, 0);
    ComprimentoTotalDaCurva = 0;

    cor = rand() % 100;
}
// **********************************************************************
//
// **********************************************************************
Bezier::Bezier(Ponto V[]) {
    for (int i = 0; i < 3; i++)
        Coords[i] = V[i];
    calculaComprimentoDaCurva();

    cor = rand() % 100;
}
// **********************************************************************
//
// **********************************************************************
Ponto Bezier::Calcula(double t) {
    Ponto P;
    double UmMenosT = 1 - t;

    P = Coords[0] * UmMenosT * UmMenosT + Coords[1] * 2 * UmMenosT * t + Coords[2] * t * t;
    return P;
}
// **********************************************************************
//
// **********************************************************************
double Bezier::CalculaT(double distanciaPercorrida) {
    return (distanciaPercorrida / ComprimentoTotalDaCurva);
}
// **********************************************************************
//
// **********************************************************************
Ponto Bezier::getPC(int i) {
    return Coords[i];
}
// **********************************************************************
//
// **********************************************************************
void Bezier::Traca() {
    double t = 0.0;
    double DeltaT = 1.0 / 50;
    Ponto P;

    defineCor(cor);
    glBegin(GL_LINE_STRIP);

    while (t < 1.0) {
        P = Calcula(t);
        glVertex2f(P.x, P.y);
        t += DeltaT;
    }

    P = Calcula(1.0);  // faz o fechamento da curva
    glVertex2f(P.x, P.y);
    glEnd();
}
