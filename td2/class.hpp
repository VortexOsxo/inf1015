/**
* Classes et Structures mémoires pour une collection de films.
* \fichier   class.hpp
* \auteurs Philippe Martin et Skander Hella
* \date   8 février 2022
* Créé le 8 février 2022
*/

#include <string>
#include <fstream>
#include "gsl/span"
#include <iostream>

using namespace gsl;
using namespace std;

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
struct Film; struct Acteur; 


void afficherActeur(const Acteur& acteur)
{
	std::cout << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}

void afficherFilm(const Film* film) {
	std::cout << "Le film " << film->titre << " mets en scène les acteurs:" << std::endl;
	span<Acteur*> spanListeActeurs(film->acteurs.elements, film->acteurs.nElements);
	for (Acteur* acteur : spanListeActeurs) {
		afficherActeur(*acteur);
	}
}

class ListeFilms {
public:
	ListeFilms(const std::string& nomFichier) {
		ifstream fichier(nomFichier, ios::binary);
		fichier.exceptions(ios::failbit);
		capacite_ = lireUint16(fichier);
		nElements_ = 0;
		elements_ = new Film * [capacite_];
		for (int i = 0; i < nElements_; i++) {
			ajouterFilm(lireFilm(fichier));
		}
	}

	~ListeFilms() {
		détruireListeFilms();
	}

	void ajouterFilm(Film* ptrFilm) {
		if (capacite_ == 0) {
			capacite_ = 1; nElements_ = 1;
			elements_ = new Film * [1];
			elements_[0] = ptrFilm;
		}

		else if (capacite_ == nElements_) {
			Film** nouvelleListe = new Film * [capacite_ * 2];
			for (int i = 0; i <= nElements_; i++) {
				nouvelleListe[i] = elements_[i];
			}
			delete[] elements_;
			elements_ = nouvelleListe;
			capacite_ *= 2;
			elements_[nElements_++] = ptrFilm;
		}

		else
		{
			elements_[nElements_++] = ptrFilm;
		}

	}

	void enleverFilm(Film* ptrFilm) {
		span<Film*> spanListeFilm(elements_, nElements_);
		for (Film*& film : spanListeFilm) {
			if (film == ptrFilm) {
				if (nElements_ > 1)
					film = elements_[nElements_ - 1];
				nElements_--;
				return;
			}
		}
	}
 
	Acteur* trouverActeur(const string acteurCible) const {
		span<Film*> spanListeFilms(elements_, nElements_);
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

	Acteur* lireActeur(istream& fichier) const {
		Acteur acteur = {};
		acteur.nom = lireString(fichier);
		acteur.anneeNaissance = lireUint16(fichier);
		acteur.sexe = lireUint8(fichier);

		Acteur* acteurExistant = trouverActeur(acteur.nom);

		if (acteurExistant == nullptr) {
			//cout << acteur.nom << endl;
			acteur.joueDans.capacite_ = 0;
			Acteur* ptrActeur = new Acteur;
			*ptrActeur = acteur;
			return ptrActeur;
		}
		else {
			return acteurExistant;
		}
	}

	Film* lireFilm(istream& fichier) {
		Film film = {};
		film.titre = lireString(fichier);
		film.realisateur = lireString(fichier);
		film.anneeSortie = lireUint16(fichier);
		film.recette = lireUint16(fichier);
		film.acteurs.nElements = lireUint8(fichier);
		Film* ptrFilm = new Film(film);
		ptrFilm->acteurs.elements = new Acteur * [ptrFilm->acteurs.nElements];

		span<Acteur*> spanListeActeur(ptrFilm->acteurs.elements, ptrFilm->acteurs.nElements);
		for (Acteur*& acteur : spanListeActeur) {
			acteur = lireActeur(fichier);
			ajouterFilm(ptrFilm);
		}
		return ptrFilm;
	}

	void détruireFilm(Film* film) {
		span<Acteur*> spanListeActeur(film->acteurs.elements, film->acteurs.nElements);
		for (Acteur* acteur : spanListeActeur) {
			enleverFilm(film);

			if (acteur->joueDans.nElements_ == 0) {
				delete[] acteur->joueDans.elements_;
				delete acteur;
			}
		}
		delete[] film->acteurs.elements;
		delete film;
	}

	void détruireListeFilms() {
		for (int i = 0; i < nElements_; i++) {
			détruireFilm(elements_[i]);
		}
		delete[] elements_;
	}

	void afficherListeFilms() const {
		static const string ligneDeSeparation = "\u2970\u2970\u2970\u2970\u2970\u2970\u2970\u2970\u2970\n";
		std::cout << ligneDeSeparation;
		span<Film*> spanListeFilms(elements_, nElements_);
		for (Film* film : spanListeFilms) {
			afficherFilm(film);
			std::cout << ligneDeSeparation;
		}
	}


private:
	int capacite_ = 0, nElements_ = 0;
	Film** elements_ = nullptr;
};

struct ListeActeurs {
	int capacite, nElements;
	Acteur** elements; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
};

struct Film
{
	std::string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie, recette; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;
};

struct Acteur
{
	std::string nom; int anneeNaissance; char sexe;
	ListeFilms joueDans;
};
