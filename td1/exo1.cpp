#include <iostream>
#include <math.h>

int premier () {
    std::cout << "Entrer un nombre entier : " << std::endl;
    int nombre;
    std::cin >> nombre;
    int racineArrondie = std::pow(nombre, 0.5);
    if (nombre % 2 == 0) {
        std::cout << "Ce nombre n'est pas premier car il est divisible par 2." << std::endl;
        return 0;
    }
    for (int i = 3; i <= racineArrondie; i += 2) {
        if (nombre % i == 0) {
            std::cout << "Ce nombre n'est pas premier car il est divisible par " << i << "." << std::endl;
            return 0;
        }
    }
    std::cout << "Ce nombre est premier." << std::endl;
    return 0;
} 