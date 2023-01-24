#include <iostream>
#include <string>
#include <cmath>

const float G = 9.80665;

double vérifierValeur(std::string texte, double min, double max) {
    double valeur;
    std::cout << texte << std::endl;
    std::cin >> valeur;
    while (valeur < min or valeur > max) {
        std::cout << texte << std::endl;
        std::cin >> valeur;
    }
    return valeur;
}


int main() {
    int nRebond;
    float hauteurInitiale, coefficientRebond;

    hauteurInitiale = vérifierValeur("Entrer la hauteur initiale: ", 0, INFINITY);
    nRebond = vérifierValeur("Entrer le nombre de rebond après lequelle vouz-voulez la hauteur: ", 0, INFINITY);
    coefficientRebond = vérifierValeur("Entrer le coefficient de rebond",0,1);
    float vitesseInitiale, vitesseFinale, hauteurFinale;
    for (int i = 0; i < nRebond; i--) {
        vitesseInitiale = std::pow((2*G*hauteurInitiale), 0.5);
        vitesseFinale = coefficientRebond * vitesseInitiale;
        hauteurFinale = (float) (std::pow(vitesseFinale,2)) / (2 * G);

        hauteurInitiale = hauteurFinale;
    }

    std::cout << "La hauteur finale est :" << hauteurFinale << std::endl;
    return 0;
}

