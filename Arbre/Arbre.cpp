#include "Arbre.h"

using namespace std;

int idArbre = 200;

string choixVariete()
{
	int choix = rand()%4;

	if(choix == 0) return "Woumpanier";
	else if(choix == 1) return "Poirier";
	else if(choix == 2) return "Cefibus";
	else if(choix == 3) return "Flan";
	else return "Inconnu(e)";
}

int affectationNombreFruits(string variete)
{
	if(variete == "Woumpanier") return 10;
	else if(variete == "Poirier") return 20;
	else if(variete == "Cefibus") return 30;
	else if(variete == "Flan") return 40;
	else return 0;
}

Arbre::Arbre() : m_id(++idArbre), m_variete(choixVariete()), m_etat(1), m_estMort(false), m_valeurEnergetique(15), m_nombreFruits(0), m_nombreFruitsMax(affectationNombreFruits(m_variete)), m_dureeVie(20), m_dureeCycle(10), m_dejaFleuri(false), m_datePlantation(clock()), m_dateDerniereFloraison(m_datePlantation), m_rayonTronc(15), m_x(rand()%799), m_y(rand()%599)
{
}

Arbre::Arbre(string variete, int x, int y) : m_id(++idArbre), m_variete(variete), m_etat(1), m_estMort(false), m_valeurEnergetique(15), m_nombreFruits(0), m_nombreFruitsMax(affectationNombreFruits(m_variete)), m_dureeVie(20), m_dureeCycle(10), m_dejaFleuri(false), m_datePlantation(clock()), m_dateDerniereFloraison(m_datePlantation), m_rayonTronc(15), m_x(x), m_y(y)
{
}

Arbre::~Arbre()
{
}

void Arbre::vivre(vector<Arbre> &foret, vector<vector<short> > &map)
{
	if(!m_estMort)
	{
		clock_t dateActuelle = clock();

		if((double)((dateActuelle - m_datePlantation) / CLOCKS_PER_SEC) > 22*m_dureeCycle)
		{
			m_etat = 3;
		}
		else if((double)((dateActuelle - m_datePlantation) / CLOCKS_PER_SEC) > 2*m_dureeCycle)
		{
			m_etat = 2;
		}
	
		if((double)((dateActuelle - m_datePlantation) / CLOCKS_PER_SEC) >= 24*m_dureeCycle)
		{
			mourir(foret, map);
			return;
		}

		if(m_etat == 2)
		{
			if((double)((dateActuelle - m_dateDerniereFloraison) / CLOCKS_PER_SEC) < 2*m_dureeCycle && (double)((dateActuelle - m_dateDerniereFloraison) / CLOCKS_PER_SEC) >= m_dureeCycle && m_dejaFleuri)
			{
				pourrir(foret, map);
			}
			else if((double)((dateActuelle - m_dateDerniereFloraison) / CLOCKS_PER_SEC) >= 2*m_dureeCycle && !m_dejaFleuri) // && m_id == 201 pour ne permettre qu'à l'arbre racine de se reproduire.
			{
				fleurir();
			}
		}
	}
}

void Arbre::mourir(vector<Arbre> &foret, std::vector<std::vector<short> > &map)
{
	m_estMort = true; //inutile, permet juste de vérifier les bugs.

	for(int i=m_x-m_rayonTronc; i<m_x+m_rayonTronc; i++)
	{
		for(int j=m_y-m_rayonTronc; j<m_y+m_rayonTronc; j++)
		{
			if(i>=0 && i<800 && j>=0 && j<600)map[i][j] = 1;
		}
	}

	int x = 0;
	while(1)
	{
		if(foret[x].id() == m_id) break;
		x++;
	}

	for(x; x<foret.size()-1; x++)
	{
		foret[x] = foret[x+1];
	}

	foret.pop_back();
}

void Arbre::fleurir()
{
	m_dejaFleuri = true;
	m_dateDerniereFloraison = clock();
	m_nombreFruits = m_nombreFruitsMax;
}

void Arbre::pourrir(vector<Arbre> &foret, vector<vector<short> > &map)
{
	int nb = m_nombreFruits;
	m_dejaFleuri = false;
	m_nombreFruits = 0;
	if(nb > 0)planter(foret, map);
}

void Arbre::planter(vector<Arbre> &foret, vector<vector<short> > &map)
{
	short continuer;
	int x = m_x;
	int y = m_y;
	int m_rayonFeuillage = 2*m_rayonTronc + 25;

	for(int chances=100; chances>0; chances--)
	{
		continuer = 1;

		do
		{
			x = m_x-m_rayonFeuillage+(rand()%(2*m_rayonFeuillage+1));
		}
		while(x > 799 || x < 1);

		do
		{
			y = m_y-m_rayonFeuillage+(rand()%(2*m_rayonFeuillage+1));
		}
		while(y > 599 || y < 1);

		for(int i=x-m_rayonTronc; i<x+m_rayonTronc; i++)
		{
			for(int j=y-m_rayonTronc; j<y+m_rayonTronc; j++)
			{
				if(i<1 || i>799 || j<1 || j>599 || map[i][j] != 1)
				{
					continuer = 2;
					break;
				}
			}
			if(continuer == 2) break;
		}

		if((((m_x - x)*(m_x - x) + (m_y - y)*(m_y - y)) > m_rayonTronc*m_rayonTronc) && continuer == 1)
		{
			continuer = 3;
			break;
		}
	}

	if(continuer == 3)
	{
		if(map[x][y] == 1)
		{
			for(int i=x-m_rayonTronc; i<x+m_rayonTronc; i++)
			{
				for(int j=y-m_rayonTronc; j<y+m_rayonTronc; j++)
				{
					if(i>=0 && i<800 && j>=0 && j<600)map[i][j] = 2;
				}
			}

			foret.push_back(Arbre(m_variete, x, y));
		}
	}
}

bool Arbre::donnerFruit(string &nomFruit, int &nutrition, clock_t &date)
{
	if(m_nombreFruits > 0)
	{
		nomFruit = m_variete;
		nutrition = m_valeurEnergetique;
		date = clock();
		m_nombreFruits--;
		return true;
	}

	return false;
}

string Arbre::variete()
{
	return m_variete;
}

bool Arbre::estMort()
{
	return m_estMort;
}

int Arbre::nombreFruits()
{
	return m_nombreFruits;
}

int Arbre::id()
{
	return m_id;
}

int Arbre::rayonTronc()
{
	return m_rayonTronc;
}

void Arbre::coordonnees(Sint16 &x, Sint16 &y)
{
	x = m_x;
	y = m_y;
}