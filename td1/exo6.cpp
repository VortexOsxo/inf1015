#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const int NOMBREMOTS= 4;
const std::string FICHIERDICTIONNAIRE = "dictionnaire.txt";

struct Mot {
    std::string nom, natureGenre, définition;
};

Mot créerMotDepuisString(std::string ligne) {
    char tabulation = '\t';
    int index = 0, longueur = 0;
    std::vector<std::string> composantes;
    for (char charactère: ligne) {
        if (charactère == tabulation) {
            composantes.push_back(ligne.substr(index, longueur));
            index += longueur + 1;
            longueur = 0;
        }
        else { longueur += 1; }
    }
    composantes.push_back(ligne.substr(index));
    return {composantes[0], composantes[1], composantes[2]};
}

int main() {
    std::fstream fichierSource;
    Mot dictionnaire[NOMBREMOTS];
    fichierSource.open(FICHIERDICTIONNAIRE, std::ios::in); 
    if (fichierSource.is_open()) {
        std::string ligne;
        for (int i = 0; i < NOMBREMOTS; i ++) {
            getline(fichierSource, ligne);
            dictionnaire[i] = créerMotDepuisString(ligne);
        }
    }
    fichierSource.close();

    Mot mot;
    int longueur, longueurMax = -1, indexMax = -1;
    for (int i = 0; i < NOMBREMOTS; i ++) {
        mot = dictionnaire[i];
        longueur = mot.nom.length();
        if (longueur > longueurMax) {
            longueurMax = longueur;
            indexMax = i;
        }
    }
    mot = dictionnaire[indexMax];
    std::cout << mot.nom << " (" << mot.natureGenre << ") : " << mot.définition << std::endl;
}

