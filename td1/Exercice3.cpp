/*
Exercice3.cpp
Philippe Martin Skander Hellal

Ce programme permet de calculer la hauteur d'une balle après un certai nombre de rebond.
L'utilisateur doit entrer la hauteur initiale, le coefficient de rebond de la balle
et le nombre de rebond après lequelle il veut connaitre la hauteur de la balle.
Le programme retourne alors la hauteur de la balle après ce nombre de rebond.
*/ 

#include <iostream>
#include <string>
#include <cmath>

const float G = 9.80665;

double vérifierValeur(std::string texte, double min, double max) {
    double valeur;
    std::cout << texte << std::endl;
    std::cin >> valeur;
    while (valeur < min || valeur > max) {
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
    for (int i = 0; i < nRebond; i++) {
        vitesseInitiale = std::pow((2*G*hauteurInitiale), 0.5);
        vitesseFinale = coefficientRebond * vitesseInitiale;
        hauteurFinale = (float) (std::pow(vitesseFinale,2)) / (2 * G);

        hauteurInitiale = hauteurFinale;
    }

    std::cout << "La hauteur finale est :" << hauteurFinale << std::endl;
    return 0;
}

