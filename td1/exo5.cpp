#include <iostream>

void afficherTableau (int tableau[10], int taille) {
    std::cout << "{";
    std::cout << tableau[0];
    for (int index = 1; index < taille; index ++) { 
        std::cout << "," << tableau[index];
    }
    std::cout << "}" << std::endl;
}

int main () {
    const int TAILLETABLEAU = 10;
    int nombres[TAILLETABLEAU];
    std::cout << "Entrer les dix nombres entiers: " << std::endl;
    for (int i = 0; i < 10; i ++) {
        std::cin >> nombres[i];
    }
    //for (int nombre: nombres) { std::cout << nombre << "-";}

    int nombresOrdonnés[TAILLETABLEAU];
    int emplacementLibre = 0;
    for (int parité = 0; parité < 2; parité ++) {
        for (int nombre: nombres) { 
            if (nombre % 2 == parité) {
                nombresOrdonnés[emplacementLibre] = nombre;
                emplacementLibre += 1;
            }
        }
    }
    afficherTableau(nombresOrdonnés, TAILLETABLEAU);
    return 0;
}