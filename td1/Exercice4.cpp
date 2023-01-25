/*
Exercice4.cpp
Philippe Martin Skander Hellal

Ce programme permet d'estimer la valeur de pi en utilisant la méthode de Monte-Carlo. L'utilisateur 
indique le nombre d'itération souhaiter et le programme retourne ensuite la valeur de pi estimé
 et la différence avec la valeur réelle précise jusqu'à 6 chiffres significatifs 
*/ 
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <math.h>

struct Point {
    float x,y;
};

const float PI = 3.141593;

Point generateRandomPoint() {
    float x = (float) rand()/RAND_MAX * std::pow(-1, rand());
    float y = (float) rand()/RAND_MAX * std::pow(-1, rand());
    Point point = {x, y};
    return point;
}

void roundToSix(float &number) {
    number = (floor(number * pow(10,6)))/pow(10,6);
}


bool estDansCercleR1(Point point) {
    float distance = (std::pow(point.x,2) + std::pow(point.y,2));
    return (distance < 1);
}

int main() {
    std::srand(unsigned(std::time(nullptr)));
    std::cout << "Entrer le nombre d'itération désiré: " << std::endl;
    int nItération;
    std::cin >> nItération;
    if (nItération <= 0) {
        std::cout << "Nombre d'itération invalide" << std::endl;
        return 0;
    }

    int nPointsDansCercle = 0;
    for (int i = 0; i < nItération; i ++) {
        Point point = generateRandomPoint();
        if (estDansCercleR1(point)) {
            nPointsDansCercle += 1;
        }
    }
    float estimation = ((float) nPointsDansCercle / nItération) * 4;
    roundToSix(estimation);
    float écartRelatif = std::abs(estimation - PI);
    roundToSix(écartRelatif);

    std::cout << "La valeur estimée de pi est: " << estimation << std::endl;
    std::cout << "La différence avec la valeur réelle est: " << écartRelatif << std::endl;

    return 0;
}