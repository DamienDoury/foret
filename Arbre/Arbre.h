#ifndef DEF_ARBRE
#define DEF_ARBRE

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <SDL/SDL.h>

class Arbre
{
public:
	Arbre();
	Arbre(std::string variete, int x, int y);
	~Arbre();
	void vivre(std::vector<Arbre> &foret, std::vector<std::vector<short> > &map);
	void mourir(std::vector<Arbre> &foret, std::vector<std::vector<short> > &map);
	void fleurir();
	void pourrir(std::vector<Arbre> &foret, std::vector<std::vector<short> > &map);
	void planter(std::vector<Arbre> &foret, std::vector<std::vector<short> > &map);
	bool donnerFruit(std::string &nomFruit, int &nutrition, clock_t &date);

	std::string variete();
	bool estMort();
	int nombreFruits();
	int id();
	int rayonTronc();
	void coordonnees(Sint16 &x, Sint16 &y);

protected:
	int m_id;
	std::string m_variete;
	int m_etat;
	bool m_estMort;

	int m_valeurEnergetique;
	int m_nombreFruits;
	int m_nombreFruitsMax;
	int m_dureeVie; //en nombre de cycles de vie.
	double m_dureeCycle; //en secondes.
	bool m_dejaFleuri;
	clock_t m_datePlantation;
	clock_t m_dateDerniereFloraison;
	int m_rayonTronc;
	int m_x;
	int m_y;
};

#endif