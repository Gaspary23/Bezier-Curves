//
//  Temporizador.hpp
//  Imagens
//
//  Created by Márcio Sarroglia Pinho on 23/03/20.
//  Copyright © 2020 rmd. All rights reserved.
//
#ifndef Temporizador_hpp
#define Temporizador_hpp

#include <sys/time.h>
#include <iostream>
using namespace std;

class Temporizador {
    struct timeval start_time;

   public:
    Temporizador();      // Inicializa o temporizador
    double getDeltaT();  // Retorna o tempo decorrido desde a última chamada desta mesma função
};

#endif /* ControlaTempo_hpp */
