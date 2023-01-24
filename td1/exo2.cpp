#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
using std::string;
using std::cout; using std::cin;
using std::endl;

double verifierValeur(std::string texte, double min, double max) {
    double valeur;
    std::cout << texte << std::endl;
    std::cin >> valeur;
    while (valeur < min || valeur > max) {
        std::cout << texte << std::endl;
        std::cin >> valeur;
    }
    return valeur;
}

int main(){
    double sommePret = verifierValeur("La somme d'argent prétée : ", 0, INFINITY);
    double paiementMois = verifierValeur("Valeur du paiement par mois : ", 0, INFINITY);
    double tauxAnnuel = verifierValeur("Taux interets annuels : ", 0, INFINITY);
   
    int nombreMois = 0;
    double sommeInterets, interetsMois, tauxMensuel;

    while (sommePret >= 0){ 
        tauxMensuel = sommePret * tauxAnnuel / 100 / 12;
        sommePret = sommePret - paiementMois + tauxMensuel;
        sommeInterets += tauxMensuel;
        nombreMois +=1;  
    }
        
    cout << "Nombre de mois nécessaires pour rembourser la dette : " << nombreMois << endl;
    cout << "Somme d'interets perçus par le preteur: " << sommeInterets << endl;
}