/**
* Programme qui permet d'organiser plusieurs informations sur une série de films populaires.
* Contient aussi des informations sur certains acteurs de ces films.
* \fichier   td2.cpp
* \auteurs Philippe Martin et Skander Hella
* \date   8 février 2022
* Créé le 1 février 2022
*/

#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "structures.hpp"      // Structures de données pour la collection de films en mémoire.

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>

#include "cppitertools/range.hpp"
#include "gsl/span"

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" // Nos fonctions pour le rapport de fuites de mémoire.
#include "debogage_memoire.hpp"        // Ajout des numéros de ligne des "new" dans le rapport de fuites.  Doit être après les include du système, qui peuvent utiliser des "placement new" (non supporté par notre ajout de numéros de lignes).

using namespace std;
using namespace iter;
using namespace gsl;

#pragma endregion//}

typedef uint8_t UInt8;
typedef uint16_t UInt16;

#pragma region "Fonctions de base pour lire le fichier binaire"//{

UInt8 lireUint8(istream& fichier)
{
	UInt8 valeur = 0;
	fichier.read((char*)&valeur, sizeof(valeur));
	return valeur;
}
UInt16 lireUint16(istream& fichier)
{
	UInt16 valeur = 0;
	fichier.read((char*)&valeur, sizeof(valeur));
	return valeur;
}
string lireString(istream& fichier)
{
	string texte;
	texte.resize(lireUint16(fichier));
	fichier.read((char*)&texte[0], streamsize(sizeof(texte[0])) * texte.length());
	return texte;
}

#pragma endregion//}

void ajouterFilm(ListeFilms& listeFilms, Film* ptrFilm) {
	if (listeFilms.capacite == 0) {
		listeFilms.capacite = 1; listeFilms.nElements = 1;
		listeFilms.elements = new Film * [1];
		listeFilms.elements[0] = ptrFilm;
	}

	else if (listeFilms.capacite == listeFilms.nElements) {
		Film** nouvelleListe = new Film * [listeFilms.capacite * 2];
		for (int i = 0; i <= listeFilms.nElements; i++) {
			nouvelleListe[i] = listeFilms.elements[i];
		}
		delete[] listeFilms.elements;
		listeFilms.elements = nouvelleListe;
		listeFilms.capacite *= 2;
		listeFilms.elements[listeFilms.nElements++] = ptrFilm;
	}
	else {
		listeFilms.elements[listeFilms.nElements++] = ptrFilm;
	}
}

void enleverFilm(ListeFilms& listeFilms, Film* ptrFilm) {
	span<Film*> spanListeFilm(listeFilms.elements, listeFilms.nElements);
	for (Film*& film: spanListeFilm) { 
		if (film == ptrFilm) {
			if (listeFilms.nElements > 1)
				film = listeFilms.elements[listeFilms.nElements - 1];
			listeFilms.nElements--;
			return;
		}
	}
}

Acteur* trouverActeur(const ListeFilms& listeFilms, const string acteurCible) {
	span<Film*> spanListeFilms(listeFilms.elements, listeFilms.nElements);
	for (Film* film : spanListeFilms) {
		span<Acteur*> spanListeActeurs(film->acteurs.elements, film->acteurs.nElements);
		for (Acteur* acteurVue : spanListeActeurs) {
			if (acteurVue->nom == acteurCible) {
				return acteurVue;
			}
		}
	}
	return nullptr;
}

Acteur* lireActeur(istream& fichier, const ListeFilms listeFilms) {
	Acteur acteur = {};
	acteur.nom = lireString(fichier);
	acteur.anneeNaissance = lireUint16(fichier);
	acteur.sexe = lireUint8(fichier);

	Acteur* acteurExistant = trouverActeur(listeFilms, acteur.nom);

	if (acteurExistant == nullptr) {
		//cout << acteur.nom << endl;
		acteur.joueDans.capacite = 0;
		Acteur *ptrActeur = new Acteur;
		*ptrActeur = acteur;
		return ptrActeur;
	}
	else {
		return acteurExistant;
	}
}

Film* lireFilm(istream& fichier, ListeFilms& listeFilms) {
	Film film = {};
	film.titre = lireString(fichier);
	film.realisateur = lireString(fichier);
	film.anneeSortie = lireUint16(fichier);
	film.recette = lireUint16(fichier);
	film.acteurs.nElements = lireUint8(fichier);	
	Film* ptrFilm = new Film(film);
	ptrFilm->acteurs.elements = new Acteur*[ptrFilm->acteurs.nElements];

	span<Acteur*> spanListeActeur(ptrFilm->acteurs.elements, ptrFilm->acteurs.nElements);
	for (Acteur*& acteur: spanListeActeur) {
		acteur = lireActeur(fichier, listeFilms);
		ajouterFilm(acteur->joueDans, ptrFilm);
	}
	return ptrFilm; 
}

ListeFilms creerListe(string nomFichier) {
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	int nElements = lireUint16(fichier);

	ListeFilms listeFilms;
	listeFilms.capacite = nElements; listeFilms.nElements = 0;
	listeFilms.elements = new Film * [listeFilms.capacite];
	for (int i = 0; i < nElements; i++) {
		ajouterFilm(listeFilms, lireFilm(fichier, listeFilms));
	}
	return listeFilms;
}

void détruireFilm(Film* film) {
	span<Acteur*> spanListeActeur(film->acteurs.elements, film->acteurs.nElements);
	for (Acteur* acteur : spanListeActeur) {
		enleverFilm(acteur->joueDans, film);

		if (acteur->joueDans.nElements == 0) {
			delete[] acteur->joueDans.elements;
			delete acteur;
		}
	}
	delete[] film->acteurs.elements;
	delete film;
}

void détruireListeFilms(ListeFilms& listeFilms) {
	for (int i = 0; i < listeFilms.nElements; i++) {
		détruireFilm(listeFilms.elements[i]);
	}
	delete[] listeFilms.elements;
}

void afficherActeur(const Acteur& acteur) {
	std::cout << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}

void afficherFilm(const Film* film) {
	std::cout << "Le film " << film->titre << " mets en scène les acteurs:" << std::endl;
	span<Acteur*> spanListeActeurs(film->acteurs.elements, film->acteurs.nElements);
	for (Acteur* acteur: spanListeActeurs) {
		afficherActeur(*acteur);
	}
}

void afficherListeFilms(const ListeFilms& listeFilms) {
	static const string ligneDeSeparation = "\u2970\u2970\u2970\u2970\u2970\u2970\u2970\u2970\u2970\n";
	std::cout << ligneDeSeparation;
	span<Film*> spanListeFilms(listeFilms.elements, listeFilms.nElements);
	for (Film* film : spanListeFilms) {
		afficherFilm(film);
		std::cout << ligneDeSeparation;
	}
}

void afficherFilmographieActeur(const ListeFilms& listeFilms, const string& nomActeur) {
	const Acteur* acteur = trouverActeur(listeFilms, nomActeur);
	if (acteur == nullptr)
		std::cout << "Aucun acteur de ce nom" << endl;
	else
		afficherListeFilms(acteur->joueDans);
}

int main()
{
	bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";


	ListeFilms listeFilms = creerListe("films.bin");

	cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;

	afficherFilm(listeFilms.elements[0]);

	cout << ligneDeSeparation << "Les films sont:" << endl;
	afficherListeFilms(listeFilms);

	Acteur* ptrBenedict = trouverActeur(listeFilms, "Benedict Cumberbatch");
	ptrBenedict->anneeNaissance = 1976;

	cout << ligneDeSeparation << "Liste des films où Benedict Cumberbatch joue sont:" << endl;
	afficherFilmographieActeur(listeFilms, "Benedict Cumberbatch");

	détruireFilm(listeFilms.elements[0]);
	enleverFilm(listeFilms, listeFilms.elements[0]);

	cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
	afficherListeFilms(listeFilms);

	trouverActeur(listeFilms, "jenesuispasunacteur");
	afficherFilmographieActeur(listeFilms, "jenesuispasunacteur");


	détruireListeFilms(listeFilms);
}
