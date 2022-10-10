// **********************************************************************
// PUCRS/Escola Polit�cnica
// COMPUTA��O GR�FICA
//
// Programa basico para criar aplicacoes 2D em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************

// Para uso no Xcode:
// Abra o menu Product -> Scheme -> Edit Scheme -> Use custom working directory
// Selecione a pasta onde voce descompactou o ZIP que continha este arquivo.
//

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

#ifdef WIN32
#include <glut.h>
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "include/InstanciaBZ.h"
#include "include/ListaDeCoresRGB.h"
#include "include/Poligono.h"
#include "include/Ponto.h"
#include "include/Temporizador.h"

Temporizador T;
double AccumDeltaT = 0;
Temporizador T2;

InstanciaBZ Personagens[10];

unsigned int nCurvas;
Bezier Curvas[20];

// Limites logicos da area de desenho
Ponto Min, Max;

bool desenha = false;

Poligono Mapa, MeiaSeta, Triangulo, PontosDeControle, auxCurvas;
int nInstancias = 0;

float angulo = 0.0;

double nFrames = 0;
double TempoTotal = 0;

// **********************************************************************
//
// **********************************************************************
void animate() {
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;

    if (AccumDeltaT > 1.0 / 30)  // fixa a atualizacao da tela em 30
    {
        AccumDeltaT = 0;
        angulo += 2;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0) {
        // cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
        // cout << "Nros de Frames sem desenho: " << nFrames << endl;
        // cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
    }
}
// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
// **********************************************************************
void reshape(int w, int h) {
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a area a ser ocupada pela area OpenGL dentro da Janela
    glViewport(0, 0, w, h);
    // Define os limites logicos da area OpenGL dentro da Janela
    glOrtho(Min.x, Max.x, Min.y, Max.y, -10, +10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
// **********************************************************************
// **********************************************************************
void DesenhaEixos() {
    Ponto Meio;
    Meio.x = (Max.x + Min.x) / 2;
    Meio.y = (Max.y + Min.y) / 2;
    Meio.z = (Max.z + Min.z) / 2;

    glBegin(GL_LINES);
    //  eixo horizontal
    glVertex2f(Min.x, Meio.y);
    glVertex2f(Max.x, Meio.y);
    //  eixo vertical
    glVertex2f(Meio.x, Min.y);
    glVertex2f(Meio.x, Max.y);
    glEnd();
}

// **********************************************************************
void DesenhaSeta() {
    glPushMatrix();
    MeiaSeta.desenhaPoligono();
    glScaled(1, -1, 1);
    MeiaSeta.desenhaPoligono();
    glPopMatrix();
}
// **********************************************************************
void DesenhaMastro() {
    Triangulo.desenhaPoligono();
}
// **********************************************************************
// Esta funcao deve instanciar todos os personagens do cenario
// **********************************************************************
void CriaInstancias() {
    Personagens[0].Posicao = Ponto(0, 0);
    Personagens[0].modelo = DesenhaMastro;
    Personagens[0].Escala = Ponto(0.5, 0.5, 0.5);

    nInstancias = 1;
}
// **********************************************************************
//
// **********************************************************************
void CarregaModelos() {
    Triangulo.LePoligono("tests/Triangulo", false);
    PontosDeControle.LePoligono("tests/PontosDeControle", false);
    auxCurvas.LePoligono("tests/Curvas", true);
}
void CriaCurvas() {
    nCurvas = auxCurvas.getNVertices();  // topo do txt
    for (size_t i; i < nCurvas; i++) {
        Ponto ponto[3];
        Ponto aux = auxCurvas.getVertice(i);

        ponto[0] = PontosDeControle.getVertice(aux.x);
        ponto[1] = PontosDeControle.getVertice(aux.y);
        ponto[2] = PontosDeControle.getVertice(aux.z);
        Curvas[i] = Bezier(ponto);
    }
}
// **********************************************************************
//
// **********************************************************************
void init() {
    // Define a cor do fundo da tela
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    CarregaModelos();
    CriaInstancias();
    CriaCurvas();

    float d = 4.5;
    Min = Ponto(-d, -(d-1));
    Max = Ponto(d, d-1);
}

// **********************************************************************
void DesenhaPersonagens(float tempoDecorrido) {
    for (int i = 0; i < nInstancias; i++) {
        Personagens[i].AtualizaPosicao(tempoDecorrido);
        //glColor3f(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
        Personagens[i].desenha();
    }
}
// **********************************************************************
//
// **********************************************************************
void DesenhaCurvas() {
    for (int i = 0; i < nCurvas; i++) {
        Curvas[i].Traca();
    }
}
// **********************************************************************
//  void display( void )
// **********************************************************************
void display(void) {
    // Limpa a tela coma cor de fundo
    glClear(GL_COLOR_BUFFER_BIT);

    // Define os limites l�gicos da �rea OpenGL dentro da Janela
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // Coloque aqui as chamadas das rotinas que desenham os objetos
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    glLineWidth(1);
    glColor3f(0, 0, 0);  // R, G, B  [0..1]

    if(desenha)
        DesenhaEixos();

    DesenhaPersonagens(T2.getDeltaT());
    DesenhaCurvas();

    glutSwapBuffers();
}
// **********************************************************************
// ContaTempo(double tempo)
//      conta um certo n�mero de segundos e informa quanto frames
// se passaram neste per�odo.
// **********************************************************************
void ContaTempo(double tempo) {
    Temporizador T;

    unsigned long cont = 0;
    // cout << "Inicio contagem de " << tempo << "segundos ..." << flush;
    while (true) {
        tempo -= T.getDeltaT();
        cont++;
        if (tempo <= 0.0) {
            // cout << "fim! - Passaram-se " << cont << " frames." << endl;
            break;
        }
    }
}
// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
// **********************************************************************
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'e':
            desenha = !desenha;
            break;
        case 't':
            ContaTempo(3);
            break;
        case 27:      // Termina o programa qdo
            exit(0);  // a tecla ESC for pressionada
            break;
        default:
            break;
    }
}
// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
// **********************************************************************
void arrow_keys(int a_keys, int x, int y) {
    switch (a_keys) {
        case GLUT_KEY_LEFT:
            Personagens[0].Posicao.x -= 0.5;
            break;
        case GLUT_KEY_RIGHT:
            Personagens[0].Rotacao++;
            break;
        case GLUT_KEY_UP:      // Se pressionar UP
            glutFullScreen();  // Vai para Full Screen
            break;
        case GLUT_KEY_DOWN:  // Se pressionar DOWN
                             // Reposiciona a janela
            glutPositionWindow(50, 50);
            glutReshapeWindow(700, 500);
            break;
        default:
            break;
    }
}
// **********************************************************************
//  void main ( int argc, char** argv )
//
// **********************************************************************
int main(int argc, char** argv) {
    cout << "Programa OpenGL" << endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowPosition(0, 0);

    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize(650, 500);

    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de titulo da janela.
    glutCreateWindow("Animacao com Bezier");

    // executa algumas inicializacoes
    init();

    // Define que o tratador de evento para
    // o redesenho da tela. A funcao "display"
    // sera chamada automaticamente quando
    // for necessario redesenhar a janela
    glutDisplayFunc(display);

    // Define que o tratador de evento para
    // o invalidacao da tela. A funcao "display"
    // sera chamada automaticamente sempre que a
    // maquina estiver ociosa (idle)
    glutIdleFunc(animate);

    // Define que o tratador de evento para
    // o redimensionamento da janela. A funcao "reshape"
    // sera chamada automaticamente quando
    // o usuario alterar o tamanho da janela
    glutReshapeFunc(reshape);

    // Define que o tratador de evento para
    // as teclas. A funcao "keyboard"
    // sera chamada automaticamente sempre
    // o usuario pressionar uma tecla comum
    glutKeyboardFunc(keyboard);

    // Define que o tratador de evento para
    // as teclas especiais(F1, F2,... ALT-A,
    // ALT-B, Teclas de Seta, ...).
    // A funcao "arrow_keys" sera chamada
    // automaticamente sempre o usuario
    // pressionar uma tecla especial
    glutSpecialFunc(arrow_keys);

    // inicia o tratamento dos eventos
    glutMainLoop();

    return 0;
}
