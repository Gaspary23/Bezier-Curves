#include "../include/CalculosPersonagens.hpp"
// **********************************************************************
//  bool Colide(Ponto min1, Ponto max1, Ponto min2, Ponto max2)
//      Verifica se o envelope 1 esta colidindo com o envelope 2
// **********************************************************************
bool Colide(Ponto min1, Ponto max1, Ponto min2, Ponto max2) {
    if (min1.x <= max2.x && max1.x >= min2.x &&
        min1.y <= max2.y && max1.y >= min2.y) {
        return true;
    }
    return false;
}
// **********************************************************************
//  void CriaEnvelope(Poligono* envelope, int id, InstanciaBZ *Personagens)
//      Cria um envelope para o personagem indicado pelo id
//          utilizando o modelo do personagem como referencia
// **********************************************************************
void CriaEnvelope(Poligono *envelope, int id, InstanciaBZ *Personagens) {
    float esquerda, direita, cima, baixo, x, y;
    // Cria um envelope para o modelo de poligono especifico do personagem
    Poligono ref = Personagens[id].modelRef;

    for (int i = ref.getNVertices() - 1; i >= 0; i--) {
        if (i == ref.getNVertices() - 1) {
            // Para o primeiro vertice analisado, inicializa os limites
            esquerda = direita = ref.getVertice(i).x;
            cima = baixo = ref.getVertice(i).y;
        } else {
            // Para os demais vertices, atualiza os limites
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
    // Cria o envelope
    envelope->insereVertice(Ponto(esquerda, baixo, 0));
    envelope->insereVertice(Ponto(direita, baixo, 0));
    envelope->insereVertice(Ponto(direita, cima, 0));
    envelope->insereVertice(Ponto(esquerda, cima, 0));
}
// **********************************************************************
//  int EscolheProxCurva(int i, int shift, InstanciaBZ *Personagens, Poligono CurvasDeControle, map<int, vector<tuple<int, int>>> mapa)
//      Escolhe a proxima curva que o personagem de indice i deve seguir
//       shift: 0 -> curva aleatoria dentre as conectadas ao ponto de chegada
//       shift: 1 -> proxima curva no vetor de curvas conectadas ao ponto de chegada
//       shift: -1 -> curva anterior no vetor de curvas conectadas ao ponto de chegada
// **********************************************************************
int EscolheProxCurva(
    int i, int shift, InstanciaBZ *Personagens,
    Poligono CurvasDeControle, map<int, vector<tuple<int, int>>> mapa) {
    // Pega o indice do ponto de chegada da curva em que o personagem esta
    int ponto = PontoSaida(i, Personagens, CurvasDeControle);

    // Vector local com as curvas conectadas ao ponto onde o personagem vai chegar
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

        // garante que a curva escolhida nao seja a mesma que o personagem ja esta
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
//  void MovimentaPersonagens(float tempoDecorrido)
//      Esta funcao atualiza a posicao de todos os personagens do jogo
// **********************************************************************
void MovimentaPersonagens(
    float tempoDecorrido, int nInstancias, InstanciaBZ *Personagens, Poligono CurvasDeControle,
    Bezier *Curvas, map<int, vector<tuple<int, int>>> mapa, bool movimentaPrincipal) {
    for (size_t i = 0; i < nInstancias; i++) {
        if (i != 0) {
            Personagens[i].AtualizaPosicao(tempoDecorrido);
            // Verifica se o personagem i colidiu com o jogador
            VerificaColisao(i, Personagens);
        } else if (movimentaPrincipal) {
            Personagens[0].AtualizaPosicao(tempoDecorrido);
        }

        // Verifica se o personagem chegou ao fim da curva
        if (Personagens[i].tAtual >= 1.0 || Personagens[i].tAtual <= 0.0) {
            // Muda a curva em que o personagem esta para a proxima
            MudaCurva(i, Personagens, CurvasDeControle, Curvas, mapa);
        }
    }
}
// **********************************************************************
//  void MudaCurva(int i, InstanciaBZ *Personagens, Poligono CurvasDeControle, Bezier *Curvas, map<int, vector<tuple<int, int>>> mapa)
//      Muda a curva em que o personagem de indice i se encontra pela
//          proxima curva que foi escolhida
// **********************************************************************
void MudaCurva(
    int i, InstanciaBZ *Personagens, Poligono CurvasDeControle,
    Bezier *Curvas, map<int, vector<tuple<int, int>>> mapa) {
    // Pega o indice do ponto de chegada da curva em que o personagem esta
    int ponto = PontoSaida(i, Personagens, CurvasDeControle);

    // Atualiza as variaveis do personagem com a nova curva
    int prox = Personagens[i].proxCurva;
    Personagens[i].Curva = &Curvas[prox];
    Personagens[i].nroDaCurva = prox;

    // Vector local com as curvas conectadas ao ponto onde o personagem vai chegar
    vector<tuple<int, int>> curvas = mapa[ponto];
    int pos;
    // Encontra o id da nova curva no vetor de curvas conectadas ao ponto
    //  para verificar se ela comeca ou termina naquele ponto
    for (pos = 0; pos < curvas.size(); pos++)
        if (get<0>(curvas[pos]) == Personagens[i].nroDaCurva)
            break;

    // Se a nova curva comeca no ponto, o personagem deve seguir o sentido
    //  contrario ao da curva e vice-versa
    Personagens[i].direcao = get<1>(curvas[pos]) == 0 ? 1 : -1;
    Personagens[i].tAtual = get<1>(curvas[pos]);

    Personagens[i].proxCurva = EscolheProxCurva(i, 0, Personagens, CurvasDeControle, mapa);
}
// **********************************************************************
//  int PontoSaida (int i, InstanciaBZ *Personagens, Poligono CurvasDeControle)
//      Encontra o ponto de saida da curva em que o personagem de indice
//          i se encontra, usando sua direcao
// **********************************************************************
int PontoSaida(int i, InstanciaBZ *Personagens, Poligono CurvasDeControle) {
    int ponto;

    // Escolhe o ponto de saida ou entrada da curva em que o personagem esta,
    //  dependendo do sentido de movimento
    if (Personagens[i].direcao == 1)
        ponto = CurvasDeControle.getVertice(Personagens[i].nroDaCurva).z;
    else if (Personagens[i].direcao == -1)
        ponto = CurvasDeControle.getVertice(Personagens[i].nroDaCurva).x;

    return ponto;
}
// **********************************************************************
//  void VerificaColiao(int i, InstanciaBZ *Personagens)
//    Verifica se o personagem i colidiu com o jogador
// **********************************************************************
void VerificaColisao(int i, InstanciaBZ *Personagens) {
    Poligono EnvelopeMain, EnvelopeEnemy;
    Ponto pos, modelEnvMin, modelEnvMax, min1, max1, min2, max2;
    float angleRad;

    // Cria o envelope do jogador e do personagem i
    for (int idx = 0; idx < 2; idx++) {
        idx = (idx == 0) ? 0 : i;
        // Realiza transformacoes geometricas para posicionar o envelope
        glPushMatrix();
            glTranslatef(Personagens[idx].Posicao.x, Personagens[idx].Posicao.y, 0);
            glRotatef(Personagens[idx].Rotacao, 0, 0, 1);
            glScalef(Personagens[idx].Escala.x, Personagens[idx].Escala.y, Personagens[idx].Escala.z);

            if (idx == 0) {
                CriaEnvelope(&EnvelopeMain, idx, Personagens);
            } else {
                CriaEnvelope(&EnvelopeEnemy, idx, Personagens);
            }
        glPopMatrix();
    }

    // Define os pontos minimos e maximos dos envelopes
    //      no sistema de coordenadas padrao do jogo
    for (int idx = 0; idx < 2; idx++) {
        idx = (idx == 0) ? 0 : i;

        // Pos = posicao atual do personagem
        pos = Personagens[idx].Posicao;
        // angleRad = angulo de rotacao do personagem em radianos
        angleRad = Personagens[idx].Rotacao * M_PI / 180;

        // modelEnvMin = Ponto minimo do envelope do personagem
        // modelEnvMax = Ponto maximo do envelope do personagem
        modelEnvMin = (idx == 0) ? EnvelopeMain.getVertice(0) : EnvelopeEnemy.getVertice(0);
        modelEnvMax = (idx == 0) ? EnvelopeMain.getVertice(2) : EnvelopeEnemy.getVertice(2);

        if (idx == 0) {
            // Define os pontos minimos e maximos do envelope do jogador
            min1.x = pos.x + modelEnvMin.x * cos(angleRad) - modelEnvMin.y * sin(angleRad);
            min1.y = pos.y + modelEnvMin.x * sin(angleRad) + modelEnvMin.y * cos(angleRad);

            max1.x = pos.x + modelEnvMax.x * cos(angleRad) - modelEnvMax.y * sin(angleRad);
            max1.y = pos.y + modelEnvMax.x * sin(angleRad) + modelEnvMax.y * cos(angleRad);
        } else {
            // Define os pontos minimos e maximos do envelope do personagem i
            min2.x = pos.x + modelEnvMin.x * cos(angleRad) - modelEnvMin.y * sin(angleRad);
            min2.y = pos.y + modelEnvMin.x * sin(angleRad) + modelEnvMin.y * cos(angleRad);

            max2.x = pos.x + modelEnvMax.x * cos(angleRad) - modelEnvMax.y * sin(angleRad);
            max2.y = pos.y + modelEnvMax.x * sin(angleRad) + modelEnvMax.y * cos(angleRad);
        }
    }

    // Verifica se os envelopes colidem considerando que os pontos minimos 
    //  e maximos estao invertidos dependendo da direcao do personagem
    if (Colide(min1, max1, min2, max2) || Colide(max1, min1, max2, min2)) {
        cout << "Colisao!" << endl;
        // Encerra o programa
        cout << "Programa encerrado" << endl;
        exit(0);
    }
}
