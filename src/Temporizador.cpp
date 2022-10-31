//
//  Temporizador.cpp
//  Metodos_Colisao
//
//  Created by Marcio Sarroglia Pinho on 23/03/20.
//  Copyright © 2020 rmd. All rights reserved.
//
//  Alterado para o trabalho da disciplina por:
//  Pedro da Cunha Gaspary - 21101429 e Lucas Marchesan Cunha - 21101060
//
#include "../include/Temporizador.hpp"

// Inicializa o temporizador
Temporizador::Temporizador() {
    // Figure out time elapsed since last call to idle function
    gettimeofday(&start_time, NULL);
}
// Retorna o tempo decorrido desde a última chamada desta mesma função
double Temporizador::getDeltaT() {
    double dt;

    // Figure out time elapsed since last call to idle function
    struct timeval end_time;
    gettimeofday(&end_time, NULL);
    dt = (float)(end_time.tv_sec - start_time.tv_sec) + 1.0e-6 * (end_time.tv_usec - start_time.tv_usec);
    start_time = end_time;

    return dt;
}
