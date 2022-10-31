#ifndef CalculosPersonagens_Hpp
#define CalculosPersonagens_Hpp

#include <unistd.h>
#include <map>
#include <tuple>

#include "InstanciaBZ.hpp"
#include "ListaDeCoresRGB.hpp"
#include "Poligono.hpp"
#include "Ponto.hpp"

bool Colide(Ponto min1, Ponto max1, Ponto min2, Ponto max2);
void CriaEnvelope(Poligono *envelope, int id, InstanciaBZ *Personagens);
int EscolheProxCurva(
    int i, int shift, InstanciaBZ *Personagens, Poligono CurvasDeControle,
    map<int, vector<tuple<int, int>>> mapa);
void MovimentaPersonagens(
    float tempoDecorrido, int nInstancias, InstanciaBZ *Personagens, Poligono CurvasDeControle,
    Bezier *Curvas, map<int, vector<tuple<int, int>>> mapa, bool movimentaPrincipal);
void MudaCurva(
    int i, InstanciaBZ *Personagens, Poligono CurvasDeControle,
    Bezier *Curvas, map<int, vector<tuple<int, int>>> mapa);
int PontoSaida(int i, InstanciaBZ *Personagens, Poligono CurvasDeControle);
void VerificaColisao(int i, InstanciaBZ *Personagens);

#endif