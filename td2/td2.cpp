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

void ajouterFilm(ListeFilms& listeFilms, Film* const ptrFilm) {
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

	else
	{
		listeFilms.elements[listeFilms.nElements++] = ptrFilm;
	}

}

void enleverFilm(ListeFilms& listeFilms, Film* const ptrFilm) {
	for (int i = 0; i <= listeFilms.nElements; i++) { //span?
		if (listeFilms.elements[i] = ptrFilm) {
			listeFilms.nElements -= 1;
			listeFilms.elements[i] = listeFilms.elements[listeFilms.nElements];
			//listeFilms.elements[listeFilms.nElements] = nullptr;
			return;
		}
	}
}

Acteur* trouverActeur(const ListeFilms& listeFilms, string acteurCible) {
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

Acteur* lireActeur(istream& fichier, const ListeFilms listeFilms)
{
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

Film* lireFilm(istream& fichier, ListeFilms& listeFilms)
{
	Film film = {};
	film.titre = lireString(fichier);
	//cout << "\t-" << film.titre << endl;
	film.realisateur = lireString(fichier);
	film.anneeSortie = lireUint16(fichier);
	film.recette = lireUint16(fichier);
	film.acteurs.nElements = lireUint8(fichier);	
	film.acteurs.elements = new Acteur*[film.acteurs.nElements];


	for (int i = 0; i < film.acteurs.nElements; i++) {
		film.acteurs.elements[i] = lireActeur(fichier, listeFilms); //TODO: Placer l'acteur au bon endroit dans les acteurs du film.
		//TODO: Ajouter le film à la liste des films dans lesquels l'acteur joue.
		ajouterFilm(film.acteurs.elements[i]->joueDans, &film);

	}
	Film *ptrFilm = new Film;
	*ptrFilm = film;
	cout << "avant " << ptrFilm->titre << endl;
	cout << ptrFilm->acteurs.elements[0]->joueDans.elements[0]->titre << endl;
	return ptrFilm; //TODO: Retourner le pointeur vers le nouveau film
}

ListeFilms creerListe(string nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	int nElements = lireUint16(fichier);

	//TODO: Créer une liste de films vide.
	ListeFilms listeFilms;
	listeFilms.capacite = nElements; listeFilms.nElements = 0;
	listeFilms.elements = new Film * [listeFilms.capacite];
	for (int i = 0; i < nElements; i++) {
		Film* filmLu = lireFilm(fichier, listeFilms); //TODO: Ajouter le film à la liste.
		cout << "apres " << filmLu->titre << endl;
		//cout << filmLu->acteurs.elements[0]->joueDans.elements[0]->titre << endl;
		ajouterFilm(listeFilms, filmLu);
	}
	return listeFilms; //TODO: Retourner la liste de films.
}

void détruireFilm(ListeFilms& listeFilms, Film* film) {
	for (int i = 0; i < film->acteurs.nElements; i++) {
		Acteur* acteur = film->acteurs.elements[i];
		if (acteur->joueDans.nElements == 1) {
			delete[] acteur->joueDans.elements;
			delete acteur;
		}
		else {
			enleverFilm(acteur->joueDans, film);
		}
		delete[] film->acteurs.elements;
		delete film;
	}
}

void détruireListeFilms(ListeFilms& listeFilms) {
	for (int i = 0; i < listeFilms.nElements; i++) {
		détruireFilm(listeFilms, listeFilms.elements[i]);
	}
}

void afficherActeur(const Acteur& acteur)
{
	std::cout << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}

void afficherFilm(const Film* film) {
	std::cout << "Le film " << film->titre << " mets en scène les acteurs:" << std::endl;
	span<Acteur*> spanListeActeurs(film->acteurs.elements, film->acteurs.nElements);
	for (Acteur* acteur: spanListeActeurs) {
		afficherActeur(*acteur);
	}
}

void afficherListeFilms(const ListeFilms& listeFilms)
{
	//TODO: Utiliser des caractères Unicode pour définir la ligne de séparation 
	//(différente des autres lignes de séparations dans ce progamme).
	static const string ligneDeSeparation = string(10, '\u2970') + "\n";
	std::cout << ligneDeSeparation;
	//TODO: Changer le for pour utiliser un span.
	span<Film*> spanListeFilms(listeFilms.elements, listeFilms.nElements);
	for (Film* film : spanListeFilms) {
		//TODO: Afficher le film.
		afficherFilm(film);
		std::cout << ligneDeSeparation;
	}
}

void afficherFilmographieActeur(const ListeFilms& listeFilms, const string& nomActeur)
{
	//TODO: Utiliser votre fonction pour trouver l'acteur (au lieu de le mettre à nullptr).
	const Acteur* acteur = trouverActeur(listeFilms, nomActeur);
	if (acteur == nullptr)
		std::cout << "Aucun acteur de ce nom" << endl;
	else
		afficherListeFilms(acteur->joueDans);
}

int main()
{
	bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

	//int* fuite = new int; //TODO: Enlever cette ligne après avoir vérifié qu'il y a bien un "Fuite detectee" de "4 octets" affiché à la fin de l'exécution, qui réfère à cette ligne du programme.

	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

	//TODO: Chaque TODO dans cette fonction devrait se faire en 1 ou 2 lignes, en appelant les fonctions écrites.

	ListeFilms listeFilms = creerListe("films.bin");

	cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;

	afficherFilm(listeFilms.elements[0]);

	cout << ligneDeSeparation << "Les films sont:" << endl;
	afficherListeFilms(listeFilms);

	//TODO: Modifier l'année de naissance de Benedict Cumberbatch pour être 1976 (elle était 0 dans les données lues du fichier).  Vous ne pouvez pas supposer l'ordre des films et des acteurs dans les listes, il faut y aller par son nom.
	Acteur* ptrBenedict = trouverActeur(listeFilms, "Benedict Cumberbatch");
	ptrBenedict->anneeNaissance = 1976;

	cout << "wtf" << endl;
	//cout << ptrBenedict->joueDans.elements[0]->titre;

	cout << ligneDeSeparation << "Liste des films où Benedict Cumberbatch joue sont:" << endl;
	//TODO: Afficher la liste des films où Benedict Cumberbatch joue.  Il devrait y avoir Le Hobbit et Le jeu de l'imitation.
	//afficherFilmographieActeur(listeFilms, "Benedict Cumberbatch");

	//TODO: Détruire et enlever le premier film de la liste (Alien).  Ceci devrait "automatiquement" (par ce que font vos fonctions) détruire les acteurs Tom Skerritt et John Hurt, mais pas Sigourney Weaver puisqu'elle joue aussi dans Avatar.

	cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
	//TODO: Afficher la liste des films.
	afficherListeFilms(listeFilms);

	//TODO: Faire les appels qui manquent pour avoir 0% de lignes non exécutées dans le programme (aucune ligne rouge dans la couverture de code; c'est normal que les lignes de "new" et "delete" soient jaunes).  Vous avez aussi le droit d'effacer les lignes du programmes qui ne sont pas exécutée, si finalement vous pensez qu'elle ne sont pas utiles.

	//TODO: Détruire tout avant de terminer le programme.  La bibliothèque de verification_allocation devrait afficher "Aucune fuite detectee." a la sortie du programme; il affichera "Fuite detectee:" avec la liste des blocs, s'il manque des delete.
	détruireListeFilms(listeFilms);
}
