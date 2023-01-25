/*
Exercice6.cpp
Philippe Martin Skander Hellal

Ce programme créer un dictionnaire, soit un tableay de définition, 
à partir d'un fichier texte. Puis il retourne le nom, la nature, le genre 
et la définition du mot le plus long.
*/ 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

const int NOMBREMOTS= 4;
const std::string FICHIERDICTIONNAIRE = "dictionnaire.txt";

struct Mot {
    std::string nom, natureGenre, définition;
};

int main() {
    std::fstream fichierSource;
    Mot dictionnaire[NOMBREMOTS];
    fichierSource.open(FICHIERDICTIONNAIRE, std::ios::in); 
    if (fichierSource.is_open()) {
        std::string ligne;
        for (int i = 0; i < NOMBREMOTS; i ++) {
            getline(fichierSource, ligne);
            std::stringstream stringLigne(ligne);
            getline(stringLigne, dictionnaire[i].nom, '\t');
            getline(stringLigne, dictionnaire[i].natureGenre, '\t');
            getline(stringLigne, dictionnaire[i].définition);
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

