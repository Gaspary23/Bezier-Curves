// **********************************************************************
// PUCRS/Escola Politecnica
// COMPUTACAO GRAFICA
//
// Programa basico para criar aplicacoes 2D em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

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

struct PontoCompare {
    bool operator()(const Ponto& lhs, const Ponto& rhs) const {
        return lhs.x < rhs.x;
    }
}; // Nao funciona

Temporizador T;
double AccumDeltaT = 0;
Temporizador T2;

InstanciaBZ Personagens[10];
float velocidade = 1;

unsigned int nCurvas;
Bezier Curvas[20];
map<Ponto, vector<int>, PontoCompare> mapa; // Mudar chave para int

// Limites logicos da area de desenho
Ponto Min, Max;
bool desenha = false;

Poligono Triangulo, PontosDeControle, auxCurvas;
int nInstancias = 2;

float angulo = 0.0;

double nFrames = 0;
double TempoTotal = 0;

int escolheProxCurva(int i);

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
void DesenhaMastro() {
    Triangulo.desenhaPoligono();
}
// **********************************************************************
//
// **********************************************************************
void CarregaModelos() {
    Triangulo.LePoligono("tests/Triangulo", false);
    PontosDeControle.LePoligono("tests/PontosDeControle", false);
    auxCurvas.LePoligono("tests/Curvas", true);
}
// **********************************************************************
//
// **********************************************************************
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
void CriaMapaCurvas() {
    for (size_t i; i < nCurvas; i++) {
        Ponto aux = auxCurvas.getVertice(i);
        int inicial = aux.x, final = aux.z;

        Ponto ponto1 = PontosDeControle.getVertice(aux.x);
        Ponto ponto2 = PontosDeControle.getVertice(aux.z);

        for (int j = 0; j < 2; j++) {
            Ponto ponto = j == 0 ? ponto1 : ponto2;
            if (mapa.find(ponto1) != mapa.end()) {
                mapa[ponto1].push_back(i);
            } else {
                vector<int> vec;
                vec.push_back(i);
                mapa[ponto1] = vec;
            }
        }
    }
}
// **********************************************************************
// Esta funcao deve instanciar todos os personagens do cenario
// **********************************************************************
void CriaInstancias() {
    for (int i = 0; i < nInstancias; i++) {
        Personagens[i] = InstanciaBZ(&Curvas[i], i, DesenhaMastro, velocidade);
        Personagens[i].proxCurva = escolheProxCurva(i);
    }
}
// **********************************************************************
//
// **********************************************************************
void init() {
    // Define a cor do fundo da tela
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    CarregaModelos();
    CriaCurvas();
    CriaMapaCurvas();
    CriaInstancias();

    float d = 4.5;
    Min = Ponto(-d, -(d - 1));
    Max = Ponto(d, d - 1);
}
// **********************************************************************
//
// **********************************************************************
int escolheProxCurva(int i) {
    Ponto ponto;
    if(Personagens[i].direcao == 1)
        ponto = Personagens[i].Curva->getPC(2);
    else if (Personagens[i].direcao == -1)
        ponto = Personagens[i].Curva->getPC(0);

    vector<int> curvas = mapa[ponto];
    int n = curvas.size();
    int r = rand() % n;

    return curvas[r];
}
// **********************************************************************
//
// **********************************************************************
void DesenhaPersonagens(float tempoDecorrido) {
    for (int i = 0; i < nInstancias; i++) {
        if (i != 0)
            Personagens[i].AtualizaPosicao(tempoDecorrido);
        if (Personagens[i].tAtual == 1) {
            Personagens[i].tAtual = 0;
            Personagens[i].Curva = &Curvas[Personagens[i].proxCurva];
            Personagens[i].proxCurva = escolheProxCurva(i);
        }
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

    if (desenha) {
        glLineWidth(1);
        glColor3f(0, 0, 0);  // R, G, B  [0..1]
        DesenhaEixos();
    }

    glLineWidth(2);
    DesenhaPersonagens(T2.getDeltaT());

    glLineWidth(2);
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
        case ' ':
            Personagens[0].AtualizaPosicao(T2.getDeltaT());
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
    srand(time(0));

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
