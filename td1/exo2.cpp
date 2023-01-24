#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
using std::string;
using std::cout; using std::cin;
using std::endl;


double verifieValeur(string texte, double min, double max) {
    double valeur = max + 1;
    while ((valeur < min) || (valeur > max)) {
        cout << texte << endl;
        cin >> valeur;
    }
    return valeur;
}

void emprunt(){  

    double sommePret = verifieValeur("La somme d'argent prétée : ", 0, INFINITY);
    double paiementMois = verifieValeur("Le montant remboursé chaque mois : ", 0, INFINITY);
    double tauxAnnuel = verifieValeur("Taux d'interets annuels = ", 0, INFINITY);

    int nombreMois; double sommeInterets, sommeRestante = sommePret, interetsMois, tauxMensuel;

    while (sommePret >= 0) { 

        tauxMensuel = sommePret * tauxAnnuel / 100 / 12;
        sommePret = sommePret - paiementMois + tauxMensuel;
        sommeInterets += tauxMensuel;
        nombreMois +=1;  

    }
        
    cout << "Nombre de mois nécessaires pour rembourser la dette : " << nombreMois << endl;
    cout << "Somme d'interets perçus par le preteur: " << sommeInterets << endl;
}

int main(){
    emprunt();
}