#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
using std::string;
using std::cout; using std::cin;
using std::endl;

//mois n-1
// moisn = moins n-1 - paiement + moisn-1 * taux/12

void emprunt(){  

    double sommePret, paiementMois, tauxAnnuel;

    cout << "La somme d'argent prétée : "<< endl;
    cin >> sommePret;

    cout << "Le montant remboursé chaque mois : "<< endl;
    cin >> paiementMois;

    cout << "Taux d'interets annuels : "<< endl;
    cin >> tauxAnnuel;
   
    int nombreMois = 0;
    double sommeInterets, sommeRestante = sommePret, interetsMois, tauxMensuel;


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