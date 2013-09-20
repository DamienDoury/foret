#ifndef DEF_BESTIOLE
#define DEF_BESTIOLE

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <SDL/SDL.h>

#include"Arbre.h"

#define PI 3.14159265358979323846

class ActionEnCours
{
public:
	ActionEnCours();
	~ActionEnCours();
	void effacer();

public:
	std::string action;
	int valeur1;
	int valeur2;
	int x;
	int y;
	int priorite; //valeur de 1 (priorite absolue) à 10 (priorite faible)
};

struct MemoireCentrale
{
	std::string source; //les 3 chaines de caractères peuvent avoir un contenu ne concordant pas avec le nom de la variable.
	std::string action;
	std::string cible;
	int valeur1;
	int valeur2;
	int x;
	int y;
	clock_t date;
	int sourceInfo; //correspond à un id d'un membre de sa race (on ne communique pas avec les autres races).
	int confianceInfo;
	int variationBienEtre;
};

struct Nourriture
{
	std::string nomAliment;
	int valeurEnergetique;
	clock_t dateDerniereConsommation;
};

class Bestiole
{
public:
	Bestiole();
	~Bestiole();
	void vivre(std::vector<Bestiole> &troupeau, std::vector<Arbre> &foret, std::vector<std::vector<short> > &map);
	void deplacer(std::vector<std::vector<short> > &map, int xDestination = -1, int yDestination = -1);
	void observer(std::vector<Arbre> &foret, std::vector<std::vector<short> > &map);
	void tourner(double angleVise);
	void suivreSouris(int x, int y);
	void consommerEnergie(int consommation);
	void decider();
	void agir(std::vector<Bestiole> &troupeau, std::vector<Arbre> &foret, std::vector<std::vector<short> > &map);
	void manger(std::vector<Arbre> &foret, int n);
	void oublier();

	bool estMort();
	int id();
	void coordonnees(Sint16 &x, Sint16 &y);
	int porteeVue();
	int angle();
	void setAngle(int x);

protected:
	int m_id;
	std::string m_espece;
	bool m_estMort;
	
	std::vector<std::vector<short> > m_memoireGeographique;
	int m_porteeVue;
	int m_taille;
	int m_depenseEnergieDeBase;
	int m_faim;
	double m_esperanceDeVie; //en secondes.
	clock_t m_dateNaissance;
	int m_x;
	int m_y;
	double m_angle;

	ActionEnCours m_objectif;
	ActionEnCours m_action;
	std::vector<MemoireCentrale> m_memoire;
	std::vector<Nourriture> m_estomac;
};

#endif