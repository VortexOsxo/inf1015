/*
Exercice1.cpp
Philippe Martin Skander Hellal

Ce programme consiste à définir si un nombre est premier ou non. 
L'utilisateur entre un nombre entier positif et le programme 
lui indique si le nombre est premier, sinon il indique le premier 
diviseur qu'il trouve.
*/
#include <iostream>
#include <math.h>

int main () {
    std::cout << "Entrer un nombre entier : " << std::endl;
    int nombre;
    std::cin >> nombre;
    int racineArrondie = std::pow(nombre, 0.5);
    if (nombre % 2 == 0) {
        std::cout << "Ce nombre n'est pas premier, car il est divisible par 2." << std::endl;
        return 0;
    }
    for (int i = 3; i <= racineArrondie; i += 2) {
        if (nombre % i == 0) {
            std::cout << "Ce nombre n'est pas premier, car il est divisible par " << i << "." << std::endl;
            return 0;
        }
    }
    std::cout << "Ce nombre est premier." << std::endl;
    return 0;
} 