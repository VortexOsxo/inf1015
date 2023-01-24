#include <iostream>
#include <string>
#include <cmath>

double vérifierValeur(std::string texte, double min, double max) {
    double valeur = max + 1;
    while ((valeur < min) || (valeur > max)) {
        std::cout << texte << std::endl;
        std::cin >> valeur;
    }
    return valeur;
}


int main() {
    int hauteurInitiale, nRebond;
    float coefficientRebond;

    hauteurInitiale = vérifierValeur("Entrer la hauteur initiale: ", 0, INFINITY);
    return 0;

}

