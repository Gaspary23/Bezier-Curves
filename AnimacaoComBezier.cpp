// **********************************************************************
// PUCRS/Escola Politecnica
// COMPUTACAO GRAFICA
//
// Programa basico para criar aplicacoes 2D em OpenGL
//
// Marcio Sarroglia Pinho - pinho@pucrs.br
//
// Alterado para o trabalho da disciplina por:
// Pedro da Cunha Gaspary - 21101429 e Lucas Marchesan Cunha - 21101060
// **********************************************************************
#include "include/AnimacaoComBezier.hpp"

// Variaveis que tratam do desenho e animacao
Temporizador T, T2;
double AccumDeltaT = 0, nFrames = 0, TempoTotal = 0;
float angulo = 0.0;

// Cruvas do cenario
const unsigned int nCurvas = 28;
Bezier Curvas[nCurvas];
// chave: indice do ponto de controle
// Valor: vector com indices da curva e direcao
map<int, vector<tuple<int, int>>> mapa;

// Personagens do jogo
const unsigned int nInstancias = 11;
InstanciaBZ Personagens[nInstancias];
// Velocidade dos personagens
float velocidade = 2;
// Controle do movimento do jogador
bool movimentaPrincipal = true;

// Limites logicos da area de desenho
Ponto Min, Max;
// Modelos de objetos e cenario
Poligono Triangulo, Seta, PontosDeControle, CurvasDeControle;

// **********************************************************************
//  void main ( int argc, char** argv )
// **********************************************************************
int main(int argc, char** argv) {
    cout << "Programa OpenGL" << endl;
    // Inicia uma semente para gerar numeros aleatorios
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
    // a invalidacao da tela. A funcao "display"
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
    CriaPersonagens();

    float d = 10;
    // Define os limites da janela
    Min = Ponto(-d, -d / 3);
    Max = Ponto(d, d / 3);
}
// **********************************************************************
//  void CarregaModelos()
//      Carrega os modelos dos personagens e do cenario
// **********************************************************************
void CarregaModelos() {
    // Carrega os modelos de personagens
    Triangulo.LePoligono("tests/Triangulo", false);
    Seta.LePoligono("tests/Seta", false);
    // Carrega os modelos do cenario
    PontosDeControle.LePoligono("tests/Pontos", false);
    CurvasDeControle.LePoligono("tests/Curvas", true);
}
// **********************************************************************
//  void CriaCurvas()
//    Cria as curvas Bezier do cenario
// **********************************************************************
void CriaCurvas() {
    for (size_t i; i < nCurvas; i++) {
        Ponto ponto[3];
        // Pega a definicao da curva no arquivo definido
        Ponto aux = CurvasDeControle.getVertice(i);

        // Ponto incial da curva
        ponto[0] = PontosDeControle.getVertice(aux.x);
        // Ponto de controle da curva
        ponto[1] = PontosDeControle.getVertice(aux.y);
        // Ponto final da curva
        ponto[2] = PontosDeControle.getVertice(aux.z);
        Curvas[i] = Bezier(ponto);
    }
}
// **********************************************************************
//  void CriaMapaCurvas()
//    Cria o mapa de curvas do cenario
//    O mapa de curvas eh um mapa, em que cada chave representa
//      o indice de um ponto no cenario e o valor representa
//      um vetor de tuplas em que cada tupla contem
//      o indice de uma curva que passa por esse ponto
//      e a direcao com que essa curva passa no ponto (0 = inicio, 1 = fim)
// **********************************************************************
void CriaMapaCurvas() {
    for (size_t i = 0; i < nCurvas; i++) {
        // Pega a definicao da curva no arquivo definido
        Ponto aux = CurvasDeControle.getVertice(i);
        // Define o indice do ponto inicial e final da curva
        int inicial = aux.x, final = aux.z;

        for (int j = 0; j < 2; j++) {
            int ponto = j == 0 ? inicial : final;
            // Verifica se o ponto ja esta no mapa
            if (mapa.find(ponto) != mapa.end()) {
                // Se o ponto ja esta no mapa, adiciona a curva ao vetor de curvas
                mapa[ponto].push_back(tuple(i, j));
            } else {
                // Se o ponto nao esta no mapa, cria um novo vetor de curvas
                vector<tuple<int, int>>* vec = &mapa[ponto];
                vec->push_back(tuple(i, j));
            }
        }
    }
}
// **********************************************************************
//  void CriaPersonagens()
//      Esta funcao instancia todos os personagens do jogo
// **********************************************************************
void CriaPersonagens() {
    // Cria e popula um vetor com todos os indices das curvas
    vector<int> indiceCurvas;
    for (size_t i = 0; i < nCurvas; i++) {
        indiceCurvas.push_back(i);
    }
    // Cria um geador uniforme de bits para embaralhar o vetor
    random_device rd;
    mt19937 g(rd());
    shuffle(indiceCurvas.begin(), indiceCurvas.end(), g);

    // Cada personagem comeca em uma curva aleatoria, escolhida do vetor embaralhado
    //  no indice equivalente ao seu, para que nao haja dois personagens na mesma curva
    for (size_t i = 0; i < nInstancias; i++) {
        int id = indiceCurvas[i];
        // Metade dos personagens comeca com a direcao 1 e outros com -1
        int direcao = i < nInstancias / 2 ? 1 : -1;
        // Diferencia o modelo do jogador e dos outros
        TipoFuncao* modelo = i == 0 ? DesenhaSeta : DesenhaTriangulo;
        Poligono* modelRef = i == 0 ? &Seta : &Triangulo;
        Personagens[i] = InstanciaBZ(&Curvas[id], id, modelo, modelRef, velocidade, direcao);
        // Escolhe a proxima curva para o personagem
        Personagens[i].proxCurva = EscolheProxCurva(i, 0, Personagens, CurvasDeControle, mapa);
    }
}
// **********************************************************************
//  void DesenhaCurvas()
//      Desenhas as curvas na tela, deixando a curva atual e a
//          proxima do jogador com espessura diferente
// **********************************************************************
void DesenhaCurvas() {
    for (size_t i = 0; i < nCurvas; i++) {
        if (Personagens[0].nroDaCurva == i || Personagens[0].proxCurva == i) {
            // Aumenta a espessura da curva atual e da proxima do jogador
            glLineWidth(6);
        } else
            glLineWidth(2);
        Curvas[i].Traca();
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
            if (atualizaMain) break;
        } else {
            defineCor(Personagens[i].cor);
        }
        Personagens[i].desenha();
    }
}
// **********************************************************************
//  void DesenhaTriangulo()
//    Desenha um triangulo na tela
// **********************************************************************
void DesenhaTriangulo() {
    Triangulo.desenhaPoligono();
}
// **********************************************************************
//  void DesenhaSeta()
//      Desenha uma seta na tela
// **********************************************************************
void DesenhaSeta() {
    Seta.desenhaPoligono();
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

    // Trata do movimento e desenho dos personagens
    MovimentaPersonagens(T2.getDeltaT(), nInstancias, Personagens,
                         CurvasDeControle, Curvas, mapa, movimentaPrincipal);
    glLineWidth(2);
    DesenhaPersonagens();

    // Trata do desenho das curvas
    DesenhaCurvas();

    // Redesenha a tela
    glutSwapBuffers();
}
// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//      Funcao que trata das teclas pressionadas no teclado
// **********************************************************************
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'd':
            // Inverte a direcao do jogador
            Personagens[0].direcao = -Personagens[0].direcao;
            Personagens[0].proxCurva = EscolheProxCurva(0, 0, Personagens, CurvasDeControle, mapa);
            Personagens[0].AtualizaPosicao(T2.getDeltaT());
            // Atualiza o desenho do jogador depois de inverter a direcao
            DesenhaPersonagens(true);
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
            contaTempo(3);
            break;
        case ' ':
            // Controla se o jogador se move ou nao
            movimentaPrincipal = !movimentaPrincipal;
            break;
        case 27:      // ESC
            // Finaliza do programa
            exit(0);
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
            Personagens[0].proxCurva = EscolheProxCurva(0, -1, Personagens, CurvasDeControle, mapa);
            break;
        case GLUT_KEY_RIGHT:
            // Muda a proxima curva selecionada pelo personagem principal
            Personagens[0].proxCurva = EscolheProxCurva(0, 1, Personagens, CurvasDeControle, mapa);
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
//  void contaTempo(double tempo)
//      Conta um certo numero de segundos e informa quanto frames
//          se passaram neste periodo.
// **********************************************************************
void contaTempo(double tempo) {
    Temporizador T;

    unsigned long cont = 0;
    while (true) {
        tempo -= T.getDeltaT();
        cont++;

        if (tempo <= 0.0) {
            break;
        }
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
//  void animate()
//      Funcao que chama o redesenho da tela depois de certo tempo
// **********************************************************************
void animate() {
    double dt;
    dt = T.getDeltaT();

    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;

    if (AccumDeltaT > 1.0 / 30) {  // fixa a atualizacao da tela em 30
        AccumDeltaT = 0;
        angulo += 2;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0) {
        TempoTotal = 0;
        nFrames = 0;
    }
}
