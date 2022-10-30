// **********************************************************************
// PUCRS/Escola Politecnica
// COMPUTACAO GRAFICA
//
// Programa basico para criar aplicacoes 2D em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************
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

#include "include/InstanciaBZ.h"
#include "include/ListaDeCoresRGB.h"
#include "include/Poligono.h"
#include "include/Ponto.h"
#include "include/Temporizador.h"

Temporizador T;
double AccumDeltaT = 0;
Temporizador T2;

const unsigned int nCurvas = 28;
Bezier Curvas[nCurvas];
// chave: indice do ponto de controle
// Valor: indice da curva e direcao
map<int, vector<tuple<int, int>>> mapa;

const unsigned int nInstancias = 11;
InstanciaBZ Personagens[nInstancias];
float velocidade = 3;

// Limites logicos da area de desenho
Ponto Min, Max;
bool desenha = false, movimentaPrincipal = true;

Poligono Triangulo, MeiaSeta, PontosDeControle, CurvasDeControle;
float angulo = 0.0;
double nFrames = 0;
double TempoTotal = 0;

int col;

// **********************************************************************
//
// **********************************************************************
void criaEnvelope(Poligono* envelope, int id) {
    float esquerda, direita, cima, baixo, x, y;
    Poligono ref = Personagens[id].modelRef;

    for (int i = ref.getNVertices() - 1; i >= 0; i--) {
        if (i == ref.getNVertices() - 1) {
            esquerda = direita = ref.getVertice(i).x;
            cima = baixo = ref.getVertice(i).y;
        } else {
            x = ref.getVertice(i).x;
            y = ref.getVertice(i).y;

            if (x < esquerda)
                esquerda = x;
            else if (x > direita)
                direita = x;

            if (y < baixo)
                baixo = y;
            else if (y > cima)
                cima = y;
        }
    }
    envelope->insereVertice(Ponto(esquerda, baixo, 0));
    envelope->insereVertice(Ponto(direita, baixo, 0));
    envelope->insereVertice(Ponto(direita, cima, 0));
    envelope->insereVertice(Ponto(esquerda, cima, 0));
}
// **********************************************************************
//  bool colide(Ponto min1, Ponto max1, Ponto min2, Ponto max2)
//   verifica se o envelope 1 esta colidindo com o envelope 2
// **********************************************************************
bool colide(Ponto min1, Ponto max1, Ponto min2, Ponto max2) {
    if (min1.x <= max2.x && max1.x >= min2.x &&
        min1.y <= max2.y && max1.y >= min2.y) {
        return true;
    }
    return false;
}
void checaColisao(int i) {
    Poligono EnvelopeMain, EnvelopeEnemy;
    Ponto pos, modelPosI, modelPosF, min1, max1, min2, max2;
    float angleRad;

    for (int idx = 0; idx < 2; idx++) {
        idx = (idx == 0) ? 0 : i;
        glPushMatrix();
        glTranslatef(Personagens[idx].Posicao.x, Personagens[idx].Posicao.y, 0);
        glRotatef(Personagens[idx].Rotacao, 0, 0, 1);
        glScalef(Personagens[idx].Escala.x, Personagens[idx].Escala.y, Personagens[idx].Escala.z);

        if (idx == 0) {
            criaEnvelope(&EnvelopeMain, idx);
            defineCor(Green);
            glLineWidth(2);
            glPushMatrix();
                EnvelopeMain.desenhaPoligono();
                glScaled(1, -1, 1);
                EnvelopeMain.desenhaPoligono();
            glPopMatrix();
        } else {
            criaEnvelope(&EnvelopeEnemy, idx);
        }
        glPopMatrix();
    }

    for (int idx = 0; idx < 2; idx++) {
        idx = (idx == 0) ? 0 : i;

        pos = Personagens[idx].Posicao;
        angleRad = Personagens[idx].Rotacao * M_PI / 180;

        modelPosI = (idx == 0) ? EnvelopeMain.getVertice(0) : EnvelopeEnemy.getVertice(0);
        modelPosF = (idx == 0) ? EnvelopeMain.getVertice(2) : EnvelopeEnemy.getVertice(2);

        if (idx == 0) {
            min1.x = pos.x + modelPosI.x * cos(angleRad) - modelPosI.y * sin(angleRad);
            min1.y = pos.y + modelPosI.x * sin(angleRad) + modelPosI.y * cos(angleRad);

            max1.x = pos.x + modelPosF.x * cos(angleRad) - modelPosF.y * sin(angleRad);
            max1.y = pos.y + modelPosF.x * sin(angleRad) + modelPosF.y * cos(angleRad);
        } else {
            min2.x = pos.x + modelPosI.x * cos(angleRad) - modelPosI.y * sin(angleRad);
            min2.y = pos.y + modelPosI.x * sin(angleRad) + modelPosI.y * cos(angleRad);

            max2.x = pos.x + modelPosF.x * cos(angleRad) - modelPosF.y * sin(angleRad);
            max2.y = pos.y + modelPosF.x * sin(angleRad) + modelPosF.y * cos(angleRad);
        }
    }

    glBegin(GL_LINES);
    defineCor(Black);
    glLineWidth(5);
    glVertex2f(min1.x, min1.y);
    glVertex2f(max1.x, max1.y);
    glEnd();

    if (colide(min1, max1, min2, max2)) {
        col++;
        cout << "Colisao: " << col << endl;
        // cout << "Programa encerrado" << endl;
        // exit(0);
    }
}
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
//      Trata o redimensionamento da janela OpenGL
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
//
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
//
// **********************************************************************
void DesenhaTriangulo() {
    Triangulo.desenhaPoligono();
}
// **********************************************************************
//
// **********************************************************************
void DesenhaSeta() {
    glPushMatrix();
    MeiaSeta.desenhaPoligono();
    glScaled(1, -1, 1);
    MeiaSeta.desenhaPoligono();
    glPopMatrix();
}
// **********************************************************************
//
// **********************************************************************
void CarregaModelos() {
    Triangulo.LePoligono("tests/Triangulo", false);
    MeiaSeta.LePoligono("tests/MeiaSeta", false);
    PontosDeControle.LePoligono("tests/Pontos", false);
    CurvasDeControle.LePoligono("tests/Curvas", true);
}
// **********************************************************************
//
// **********************************************************************
void CriaCurvas() {
    for (size_t i; i < nCurvas; i++) {
        Ponto ponto[3];
        Ponto aux = CurvasDeControle.getVertice(i);

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
    for (size_t i = 0; i < nCurvas; i++) {
        Ponto aux = CurvasDeControle.getVertice(i);
        int inicial = aux.x, final = aux.z;

        for (int j = 0; j < 2; j++) {
            int ponto = j == 0 ? inicial : final;
            if (mapa.find(ponto) != mapa.end()) {
                mapa[ponto].push_back(make_tuple(i, j));
            } else {
                vector<tuple<int, int>>* vec = &mapa[ponto];
                vec->push_back(make_tuple(i, j));
            }
        }
    }

    /*map<int, vector<tuple<int,int>>>::iterator it;
    for(it = mapa.begin(); it != mapa.end(); it++) {
        cout << "ponto: " << it->first << endl;
        for (size_t i = 0; i < it->second.size(); ++i) {
            cout << "  curva: " << get<0>(it->second[i]) << " ";
            cout << "  t: " << get<1>(it->second[i]) << endl;
        }
    }*/
}
// **********************************************************************
//  int pontoSaida (int i, int direcao)
//      Encontra o ponto de saida da curva em que o personagem de indice
//          i se encontra, usando sua direcao
// **********************************************************************
int pontoSaida(int i, int direcao) {
    int ponto;
    // Escolhe o ponto de saida ou entrada da curva em que o jogador esta,
    //  dependendo do sentido de movimento
    if (Personagens[i].direcao == 1)
        ponto = CurvasDeControle.getVertice(Personagens[i].nroDaCurva).z;
    else if (Personagens[i].direcao == -1)
        ponto = CurvasDeControle.getVertice(Personagens[i].nroDaCurva).x;

    return ponto;
}
// **********************************************************************
//  int escolheProxCurva(int i, int shift = 0)
//      Escolhe a proxima curva que o jogador de indice i deve seguir
//       shift: 0 -> curva aleatoria dentre as conectadas ao ponto de chegada
//       shift: 1 -> proxima curva no vetor de curvas conectadas ao ponto de chegada
//       shift: -1 -> curva anterior no vetor de curvas conectadas ao ponto de chegada
// **********************************************************************
int escolheProxCurva(int i, int shift = 0) {
    // Pega o indice do ponto de chegada da curva em que o jogador esta
    int ponto = pontoSaida(i, Personagens[i].direcao);

    // Vector local com as curvas conectadas ao ponto onde o jogador vai chegar
    vector<tuple<int, int>> curvas = mapa[ponto];
    int id, new_id;
    int size = curvas.size() - 1;

    // Se nao houver shift, escolhe uma curva aleatoria
    if (shift == 0) {
        int id1, id2;

        // Encontra o id da curva atual no vetor de curvas conectadas ao ponto
        //  para garantir que a proxima curva nao seja a mesma
        for (id = 0; id < size + 1; id++)
            if (get<0>(curvas[id]) == Personagens[i].nroDaCurva)
                break;

        /*
         * Gera dois ids aleatorios dentro de intervalos definidos como:
         *   id1 -> [0, id-1]
         *   id2 -> [id+1, size]
         */
        // Se o nro da curva atual for 0, id1 = 1 por convencao
        id1 = (id != 0) ? (rand() % ((id - 1) + 1)) : 1;
        // Se o nro da curva atual for size, id2 = size - 1 por convencao
        id2 = (id != size) ? ((id + 1) + (rand() % (size - (id + 1) + 1))) : size - 1;

        // escolhe um dos ids aleatorios
        int x = rand() % 2;
        new_id = x == 0 ? id1 : id2;
    }  // Se houver shift, escolhe uma curva "adjacente"
    else {
        // Encontra o id da proxima curva no vetor de curvas conectadas ao ponto
        //  para garantir que a nova proxima curva nao seja a mesma
        for (id = 0; id < size + 1; id++)
            if (get<0>(curvas[id]) == Personagens[i].proxCurva)
                break;

        // garante que a curva escolhida nao seja a mesma que o jogador ja esta
        //  e que a curva esteja nos limites logicos do vetor
        new_id = id + shift;
        if (new_id < 0)
            new_id = size;
        else if (new_id > size)
            new_id = 0;

        if (get<0>(curvas[new_id]) == Personagens[i].nroDaCurva) {
            new_id += shift;
            if (new_id < 0)
                new_id = size;
            else if (new_id > size)
                new_id = 0;
        }
    }

    return get<0>(curvas[new_id]);
}
// **********************************************************************
//  void MudaCurva(int i)
//      Muda a curva em que o jogador de indice i se encontra pela
//          proxima curva que foi escolhida
// **********************************************************************
void MudaCurva(int i) {
    // Pega o indice do ponto de chegada da curva em que o jogador esta
    int ponto = pontoSaida(i, Personagens[i].direcao);

    // Atualiza as variaveis do personagem com a nova curva
    int prox = Personagens[i].proxCurva;
    Personagens[i].Curva = &Curvas[prox];
    Personagens[i].nroDaCurva = prox;

    // Vector local com as curvas conectadas ao ponto onde o jogador vai chegar
    vector<tuple<int, int>> curvas = mapa[ponto];
    int pos;
    // Encontra o id da nova curva no vetor de curvas conectadas ao ponto
    //  para verificar se ela comeca ou termina naquele ponto
    for (pos = 0; pos < curvas.size(); pos++)
        if (get<0>(curvas[pos]) == Personagens[i].nroDaCurva)
            break;

    // Se a nova curva comeca no ponto, o jogador deve seguir o sentido
    //  contrario ao da curva e vice-versa
    Personagens[i].direcao = get<1>(curvas[pos]) == 0 ? 1 : -1;
    Personagens[i].tAtual = get<1>(curvas[pos]);

    Personagens[i].proxCurva = escolheProxCurva(i);
}
// **********************************************************************
//  void CriaInstancias()
//      Esta funcao instancia todos os personagens do jogo
// **********************************************************************
void CriaInstancias() {
    // Cria e popula um vetor com todos os indices das curvas
    vector<int> indiceCurvas;
    for (size_t i = 0; i < nCurvas; i++) {
        indiceCurvas.push_back(i);
    }
    // Cria um geador unifrome de bits para embaralhar o vetor
    random_device rd;
    mt19937 g(rd());
    shuffle(indiceCurvas.begin(), indiceCurvas.end(), g);

    // Cada jogador comeca em uma curva aleatoria, escolhida do vetor embaralhado
    //  na posicao equivalente a sua, para que nao haja dois jogadores na mesma curva
    for (size_t i = 0; i < nInstancias; i++) {
        int id = indiceCurvas[i];
        // Metade dos jogadores comeca com a direcao 1 e outros com -1
        int direcao = i < nInstancias / 2 ? 1 : -1;
        // Diferencia o modelo do jogador principal e dos outros
        TipoFuncao* modelo = i == 0 ? DesenhaSeta : DesenhaTriangulo;
        Poligono* modelRef = i == 0 ? &MeiaSeta : &Triangulo;
        Personagens[i] = InstanciaBZ(&Curvas[id], id, modelo, modelRef, velocidade, direcao);
        // Escolhe a proxima curva para o jogador
        Personagens[i].proxCurva = escolheProxCurva(i);
    }
}
// **********************************************************************
//  void init()
//     Esta funcao inicializa os objetos globais do jogo
// **********************************************************************
void init() {
    // Define a cor do fundo da tela
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    CarregaModelos();
    CriaCurvas();
    CriaMapaCurvas();
    CriaInstancias();

    float d = 10;
    Min = Ponto(-d, -d / 3);
    Max = Ponto(d, d / 3);
}
// **********************************************************************
//
// **********************************************************************
void MovimentaPersonagens(float tempoDecorrido) {
    for (size_t i = 0; i < nInstancias; i++) {
        if (i != 0) {
            Personagens[i].AtualizaPosicao(tempoDecorrido);
            // if (Personagens[i].nroDaCurva == Personagens[0].nroDaCurva) {
            checaColisao(i);
            // }
        } else if (movimentaPrincipal) {
            Personagens[0].AtualizaPosicao(tempoDecorrido);
        }

        if (Personagens[i].tAtual >= 1.0 || Personagens[i].tAtual <= 0.0) {
            MudaCurva(i);
        }
    }
}
// **********************************************************************
//  void DesenhaPersonagens(bool atualizaMain)
//      Desenha os personagens na tela. O parametro so nao eh falso
//          quando o personagem principal muda de direcao
// **********************************************************************
void DesenhaPersonagens(bool atualizaMain = false) {
    for (size_t i = 0; i < nInstancias; i++) {
        if (i == 0) {
            glLineWidth(3);
            defineCor(Green);
        } else {
            defineCor(Personagens[i].cor);
        }
        Personagens[i].desenha();
        if (atualizaMain) break;
    }
}
// **********************************************************************
//  void DesenhaCurvas()
//      Desenhas as curvas na tela, deixando a curva atual e a
//          proxima do personagem principal com espessura diferente
// **********************************************************************
void DesenhaCurvas() {
    for (size_t i = 0; i < nCurvas; i++) {
        if (Personagens[0].nroDaCurva == i || Personagens[0].proxCurva == i) {
            glLineWidth(6);
        } else
            glLineWidth(2);
        Curvas[i].Traca();
    }
}
// **********************************************************************
//  void display( void )
//      Funcao que desenha os objetos na tela
// **********************************************************************
void display(void) {
    // Limpa a tela coma cor de fundo
    glClear(GL_COLOR_BUFFER_BIT);

    // Define os limites logicos da area OpenGL dentro da Janela
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // Coloque aqui as chamadas das rotinas que desenham os objetos
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<s
    if (desenha) {
        glLineWidth(1);
        glColor3f(0, 0, 0);  // R, G, B  [0..1]
        DesenhaEixos();
    }

    // Trata do movimento e desenho dos personagens
    MovimentaPersonagens(T2.getDeltaT());
    glLineWidth(2);
    DesenhaPersonagens();

    // Trata do desenho das curvas
    DesenhaCurvas();

    // Redesenha a tela
    glutSwapBuffers();
}
// **********************************************************************
//  void ContaTempo(double tempo)
//      Conta um certo numero de segundos e informa quanto frames
//          se passaram neste periodo.
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
//      Funcao que trata das teclas pressionadas no teclado
// **********************************************************************
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'd':
            // Inverte a direcao do jogador principal
            Personagens[0].direcao = -Personagens[0].direcao;
            Personagens[0].proxCurva = escolheProxCurva(0);
            Personagens[0].AtualizaPosicao(T2.getDeltaT());
            DesenhaPersonagens(true);
            break;
        case 'e':
            // Alterna entre desenhar e nao desenhar os eixos
            desenha = !desenha;
            break;
        case 'f':
            // Vai para Full Screen
            glutFullScreen();
            break;
        case 'r':
            // Reposiciona a janela
            glutPositionWindow(50, 50);
            glutReshapeWindow(700, 500);
            break;
        case 't':
            ContaTempo(3);
            break;
        case ' ':
            // Controla se o jogador principal se move ou nao
            movimentaPrincipal = !movimentaPrincipal;
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
//      Funcao que trata das setas pressionadas no teclado
// **********************************************************************
void arrow_keys(int a_keys, int x, int y) {
    float vel;
    switch (a_keys) {
        case GLUT_KEY_LEFT:
            // Muda a proxima curva selecionada pelo personagem principal
            Personagens[0].proxCurva = escolheProxCurva(0, -1);
            break;
        case GLUT_KEY_RIGHT:
            // Muda a proxima curva selecionada pelo personagem principal
            Personagens[0].proxCurva = escolheProxCurva(0, 1);
            break;
        case GLUT_KEY_UP:
            // Aumenta a velocidade do personagem principal
            vel = Personagens[0].Velocidade + 0.5;
            vel = vel > 8 ? 8 : vel;
            Personagens[0].Velocidade = vel;
            break;
        case GLUT_KEY_DOWN:
            // Diminui a velocidade do personagem principal
            vel = Personagens[0].Velocidade - 0.5;
            vel = vel <= 0.5 ? 0.5 : vel;
            Personagens[0].Velocidade = vel;
            break;
        default:
            break;
    }
}
// **********************************************************************
//  void main ( int argc, char** argv )
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
