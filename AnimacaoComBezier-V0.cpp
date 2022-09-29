// **********************************************************************
// PUCRS/Escola PolitŽcnica
// COMPUTA‚ÌO GRçFICA
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

#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>


using namespace std;

#ifdef WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <glut.h>
#endif

#include "Ponto.h"
#include "Instancia.h"

#include "Temporizador.h"
#include "ListaDeCoresRGB.h"

Temporizador T;
double AccumDeltaT=0;

Instancia Universo[10];

// Limites l—gicos da ‡rea de desenho
Ponto Min, Max;

bool desenha = false;

Poligono Mapa, MeiaSeta, Mastro;
int nInstancias=0;

float angulo=0.0;

void CriaInstancias();

Ponto Curva1[3];

bool animando = false;

// **************************************************************
void TracaPontosDeControle(Ponto PC[])
{
    glPointSize(10);
    glBegin(GL_POINTS);
        glVertex3f(PC[0].x, PC[0].y, PC[0].z);
        glVertex3f(PC[1].x, PC[1].y, PC[1].z);
        glVertex3f(PC[2].x, PC[2].y, PC[2].z);
    glEnd();
}
// **************************************************************
Ponto CalculaBezier3(Ponto PC[], double t)
{
    Ponto P;
    double UmMenosT = 1-t;
    
    P =  PC[0] * UmMenosT * UmMenosT + PC[1] * 2 * UmMenosT * t + PC[2] * t*t;
    return P;
}

// *****************************************************************
void TracaBezier3Pontos()
{
    double t=0.0;
    double DeltaT = 1.0/50;
    Ponto P;
    //cout << "DeltaT: " << DeltaT << endl;
    glBegin(GL_LINE_STRIP);
    
    while(t<1.0)
    {
        P = CalculaBezier3(Curva1, t);
        glVertex2f(P.x, P.y);
        t += DeltaT;
    }
    P = CalculaBezier3(Curva1, 1.0); // faz o acabamento da curva
    glVertex2f(P.x, P.y);
    
    glEnd();
}
// **************************************************************
//
// **************************************************************
void CarregaModelos()
{
    Mapa.LePoligono("EstadoRS.txt");
    MeiaSeta.LePoligono("MeiaSeta.txt");
    Mastro.LePoligono("Mastro.txt");
}
// **************************************************************
//
// **************************************************************
void CriaCurvas()
{
    Curva1[0] = Ponto (-5,-5);
    Curva1[1] = Ponto (0,6);
    Curva1[2] = Ponto (5,-5);
}
// **************************************************************
//
// **************************************************************
void init()
{
    // Define a cor do fundo da tela (AZUL)
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    CarregaModelos();
    CriaInstancias();
    CriaCurvas();
    float d = 7;
    Min = Ponto(-d,-d);
    Max = Ponto(d,d);
    
}

double nFrames=0;
double TempoTotal=0;

// **************************************************************
//
// **************************************************************
void animate()
{
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;

    if (AccumDeltaT > 1.0/30) // fixa a atualiza‹o da tela em 30
    {
        AccumDeltaT = 0;
        angulo+=2;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
        cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
        cout << "Nros de Frames sem desenho: " << nFrames << endl;
        cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
    }
}
// **************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
// **************************************************************
void reshape( int w, int h )
{
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a area a ser ocupada pela area OpenGL dentro da Janela
    glViewport(0, 0, w, h);
    // Define os limites logicos da area OpenGL dentro da Janela
    glOrtho(Min.x,Max.x, Min.y,Max.y, -10,+10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
// ****************************************************************
//
// ****************************************************************
void RotacionaAoRedorDeUmPonto(float alfa, Ponto P)
{
    glTranslatef(P.x, P.y, P.z);
    glRotatef(alfa, 0,0,1);
    glTranslatef(-P.x, -P.y, -P.z);
}
// **************************************************************
//
// **************************************************************
void DesenhaEixos()
{
    Ponto Meio;
    Meio.x = (Max.x+Min.x)/2;
    Meio.y = (Max.y+Min.y)/2;
    Meio.z = (Max.z+Min.z)/2;

    glBegin(GL_LINES);
    //  eixo horizontal
        glVertex2f(Min.x,Meio.y);
        glVertex2f(Max.x,Meio.y);
    //  eixo vertical
        glVertex2f(Meio.x,Min.y);
        glVertex2f(Meio.x,Max.y);
    glEnd();
}

// **********************************************************************
void DesenhaSeta()
{
    glPushMatrix();
        MeiaSeta.desenhaPoligono();
        glScaled(1,-1, 1);
        MeiaSeta.desenhaPoligono();
    glPopMatrix();
}
// **********************************************************************
void DesenhaApontador()
{
    glPushMatrix();
        glTranslated(-4, 0, 0);
        DesenhaSeta();
    glPopMatrix();
}
// ****************************************************************
void DesenhaHelice()
{
    glPushMatrix();
    for(int i=0;i < 4; i++)
    {
        glRotatef(90, 0, 0, 1);
        DesenhaApontador();
    }
    glPopMatrix();
}
// ****************************************************************

void DesenhaHelicesGirando()
{
    glPushMatrix();
        glRotatef(angulo, 0, 0, 1);
        DesenhaHelice();
   glPopMatrix();
}
// ****************************************************************

void DesenhaMastro()
{
    Mastro.desenhaPoligono();
}
// ****************************************************************
void DesenhaCatavento()
{
    glLineWidth(3);
    glPushMatrix();
        defineCor(BrightGold);
        DesenhaMastro();
        glPushMatrix();
            glColor3f(1,0,0); // R, G, B  [0..1]
            glTranslated(0,3,0);
            glScaled(0.2, 0.2, 1);
            defineCor(YellowGreen);
            DesenhaHelicesGirando();
        glPopMatrix();
    glPopMatrix();
}
// ****************************************************************
// Esta fun‹o deve instanciar todos os personagens do cen‡rio
// ****************************************************************

void CriaInstancias()
{
    Universo[0].posicao = Ponto (0,0);
    Universo[0].rotacao = 0;
    Universo[0].modelo = DesenhaCatavento;
    Universo[0].escala = Ponto (1,1,1);

    Universo[1].posicao = Ponto (3,0);
    Universo[1].rotacao = -90;
    Universo[1].modelo = DesenhaCatavento;
    
    Universo[2].posicao = Ponto (0,-5);
    Universo[2].rotacao = 0;
    Universo[2].modelo = DesenhaCatavento;
    
    nInstancias = 3;

}


// ****************************************************************
void DesenhaUniverso()
{
    for(int i=0; i<nInstancias;i++)
    {
        Universo[i].desenha();
    }
}
// ****************************************************************
//  void display( void )
// ****************************************************************
void display( void )
{

	// Limpa a tela coma cor de fundo
	glClear(GL_COLOR_BUFFER_BIT);

    // Define os limites lógicos da área OpenGL dentro da Janela
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// Coloque aqui as chamadas das rotinas que desenham os objetos
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	glLineWidth(1);
	glColor3f(1,1,1); // R, G, B  [0..1]
    DesenhaEixos();
    
    glLineWidth(1);
   
    DesenhaUniverso();
    defineCor(VioletRed);
    TracaBezier3Pontos();
    defineCor(MandarinOrange);
    TracaPontosDeControle(Curva1);
    
    double dt;
    dt = T.getDeltaT();
    
    
    // Como instanciar o mapa de forma que ocupe a tela toda?
    //Mapa.desenhaPoligono();
    
    if (animando)
    {
        //AvancaPersonagens(dt);
    }
	glutSwapBuffers();
}
// ****************************************************************
// ContaTempo(double tempo)
//      conta um certo nœmero de segundos e informa quanto frames
// se passaram neste per’odo.
// ****************************************************************
void ContaTempo(double tempo)
{
    Temporizador T;

    unsigned long cont = 0;
    cout << "Inicio contagem de " << tempo << "segundos ..." << flush;
    while(true)
    {
        tempo -= T.getDeltaT();
        cont++;
        if (tempo <= 0.0)
        {
            cout << "fim! - Passaram-se " << cont << " frames." << endl;
            break;
        }
    }
}
// ****************************************************************
//  void keyboard ( unsigned char key, int x, int y )
// ****************************************************************
void keyboard ( unsigned char key, int x, int y )
{

	switch ( key )
	{
		case 27:        // Termina o programa qdo
			exit ( 0 );   // a tecla ESC for pressionada
			break;
        case 't':
            ContaTempo(3);
            break;
        case ' ':
            desenha = !desenha;
        break;
		default:
			break;
	}
}
// ****************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
// ****************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
	switch ( a_keys )
	{
        case GLUT_KEY_LEFT:
            Universo[1].posicao.x -= 0.5;
            break;
        case GLUT_KEY_RIGHT:
            Universo[1].posicao.x += 0.5;
            break;
		case GLUT_KEY_UP:       // Se pressionar UP
			glutFullScreen ( ); // Vai para Full Screen
			break;
	    case GLUT_KEY_DOWN:     // Se pressionar UP
								// Reposiciona a janela
            glutPositionWindow (50,50);
			glutReshapeWindow ( 700, 500 );
			break;
		default:
			break;
	}
}
// ****************************************************************
//  void main ( int argc, char** argv )
// ****************************************************************
int  main ( int argc, char** argv )
{
    cout << "Programa OpenGL" << endl;

    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);

    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize  ( 650, 500);

    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de título da janela.
    glutCreateWindow    ( "Transformacoes Geometricas em OpenGL" );

    // executa algumas inicializações
    init ();

    // Define que o tratador de evento para
    // o redesenho da tela. A funcao "display"
    // será chamada automaticamente quando
    // for necessário redesenhar a janela
    glutDisplayFunc ( display );

    // Define que o tratador de evento para
    // o invalida‹o da tela. A funcao "display"
    // será chamada automaticamente sempre que a
    // m‡quina estiver ociosa (idle)
    glutIdleFunc(animate);

    // Define que o tratador de evento para
    // o redimensionamento da janela. A funcao "reshape"
    // será chamada automaticamente quando
    // o usuário alterar o tamanho da janela
    glutReshapeFunc ( reshape );

    // Define que o tratador de evento para
    // as teclas. A funcao "keyboard"
    // será chamada automaticamente sempre
    // o usuário pressionar uma tecla comum
    glutKeyboardFunc ( keyboard );

    // Define que o tratador de evento para
    // as teclas especiais(F1, F2,... ALT-A,
    // ALT-B, Teclas de Seta, ...).
    // A funcao "arrow_keys" será chamada
    // automaticamente sempre o usuário
    // pressionar uma tecla especial
    glutSpecialFunc ( arrow_keys );

    // inicia o tratamento dos eventos
    glutMainLoop ( );

    return 0;
}
