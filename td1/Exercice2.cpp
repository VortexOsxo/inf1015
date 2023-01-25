/*
Exercice2.cpp
Philippe Martin Skander Hellal

Ce programme permet de calculer des intérêts pour un prêt. L'utilisateur
entre la valeur du montant emprunté, le taux d'intérêt annuel et le paiment 
mensuel qu'il effectueet le programme calcule la somme totale des intérêts 
qu'il va payer et le nombre de mois nécessaire pour rembourser son prêt.
*/ 

#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
using namespace std;

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
    double sommeInterets = 0.0;
    double tauxMensuel = sommePret * tauxAnnuel / 100 / 12;

    while (sommePret >= 0){ 
        sommePret = sommePret - paiementMois + tauxMensuel;
        sommeInterets += tauxMensuel;
        nombreMois +=1; 
        tauxMensuel = sommePret * tauxAnnuel / 100 / 12; 
    }
        
    cout << "Nombre de mois nécessaires pour rembourser la dette : " << nombreMois << endl;
    cout << "Somme d'interets perçus par le preteur: " << sommeInterets << endl;
}