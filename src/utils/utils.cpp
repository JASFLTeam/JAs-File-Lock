#include "utils.h"

/*
inicio() empieza un punto nuevo
*/
void ut::tiempo::inicio()
{
    _inicio = std::chrono::high_resolution_clock::now();
}

/*
fin() asigna otro punto
para el final
*/
void ut::tiempo::fin()
{
    _fin = std::chrono::high_resolution_clock::now();
}

/*
Devuelve un double de cuantos milisegundos
*/
double ut::tiempo::cuanto()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(_fin - _inicio).count();
}