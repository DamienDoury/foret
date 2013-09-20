/*
OBJECTIFS :
Ensuite et seulement ensuite, on créera une bê-bête qui se promène joyeusement dans l'herbe :
	- Tout d'abord, on va la faire apparaitre.
	- Puis on va la faire se déplacer aléatoirement (les arbres ne se reproduisent plus pour le test).
	- Ensuite on lui créé une mémoire pour qu'elle se souvienne de la place des arbres.
	- Enfin, on va la faire vivre en lui donnant faim :)

NOTES :
Au bout d'un certain moment, les arbres arretent de se reproduire et meurent.
Cela se produit lorsque un arbre mère meure (ou devient stérile), alors tous ses enfants commencent à mourir.
Lorsqu'un seul arbre a la capacité de se reproduire, tout se passe bien : les arbres meurrent dans le bon ordre.
C'est sans doute dû à un problème de pointeur et/ou de "operateur=" (les 2 raisons peuvent être la même) ...
Perdu, c'était un problème de SDL_Flip(ecran) (inexpliqué ...) !
*/

#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>

#include "Arbre.h"
#include "Bestiole.h"

using namespace std;

void afficherPorteeVision(SDL_Surface *&ecran, Bestiole &bestiole)
{
	SDL_Rect coord;
	SDL_Surface *imageChampDeVision = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, 1, 32, 0, 0, 0, 0);
	SDL_FillRect(imageChampDeVision, NULL, SDL_MapRGB(imageChampDeVision->format, 255, 255, 255));
	SDL_SetAlpha(imageChampDeVision, SDL_SRCALPHA, 64);

	bestiole.coordonnees(coord.x, coord.y);

	for(int i=coord.x-bestiole.porteeVue(); i<coord.x+bestiole.porteeVue(); i++)
	{
		for(int j=coord.y-bestiole.porteeVue(); j<coord.y+bestiole.porteeVue(); j++)
		{
			bestiole.coordonnees(coord.x, coord.y);

			if(((coord.x - i)*(coord.x - i) + (coord.y - j)*(coord.y - j)) <= bestiole.porteeVue()*bestiole.porteeVue())
			{
				if(i>0 && i<800 && j>0 && j<600)
				{
					coord.x = i;
					coord.y = j;
					SDL_BlitSurface(imageChampDeVision, NULL, ecran, &coord);
					bestiole.coordonnees(coord.x, coord.y);
				}
			}
		}
	}
}

void rafraichirEcran(SDL_Surface *&ecran, SDL_Surface *&fond, vector<Arbre> &foret, vector<Bestiole> &troupeau)
{
	SDL_Rect coord;

	SDL_Surface *imageArbre = NULL;
	imageArbre = SDL_LoadBMP("arbre.bmp");
	SDL_SetColorKey(imageArbre, SDL_SRCCOLORKEY, SDL_MapRGB(imageArbre->format, 255, 255, 255));
	SDL_SetAlpha(imageArbre, SDL_SRCALPHA, 128); //Pour un effet plus sombre et réaliste, mettre la transparence à 230.

	SDL_Surface *imageBestiole = NULL;
	SDL_Surface *imageBestioleBis = NULL;
	imageBestiole = SDL_LoadBMP("bestiole.bmp");
	imageBestioleBis = SDL_LoadBMP("bestiole.bmp");
	SDL_SetColorKey(imageBestioleBis, SDL_SRCCOLORKEY, SDL_MapRGB(imageBestiole->format, 255, 255, 255));

	SDL_BlitSurface(fond, NULL, ecran, NULL);

	for(unsigned int x=0; x<troupeau.size(); x++)
	{
		imageBestiole = rotozoomSurface(imageBestioleBis, troupeau[x].angle(), 1.0, 1);
		troupeau[x].coordonnees(coord.x, coord.y);
		coord.x -= imageBestiole->w/2;
		coord.y -= imageBestiole->h/2;
		//afficherPorteeVision(ecran, troupeau[x]); //a virer, juste pour test (fait sans doute bugger le prog).
		if(!troupeau[x].estMort()) SDL_BlitSurface(imageBestiole, NULL, ecran, &coord);
	}

	for(unsigned int x=0; x<foret.size(); x++)
	{
		foret[x].coordonnees(coord.x, coord.y);
		coord.x -= imageArbre->w/2;
		coord.y -= imageArbre->h/2;
		SDL_BlitSurface(imageArbre, NULL, ecran, &coord);
	}

	SDL_Flip(ecran);
}

int main(int argc, char *argv[])
{
	vector<vector<short> > map(800, vector<short> (600, 1));

	srand((unsigned int)time(NULL));

	Sint16 x, y;

	vector<Bestiole> troupeau(3);

	vector<Arbre> foret(1);
	foret[0].coordonnees(x, y);

	for(int i=x-15; i<x+15; i++)
	{
		for(int j=y-15; j<y+15; j++)
		{
			if(i>=0 && i<800 && j>=0 && j<600)map[i][j] = 2;
		}
	}

	SDL_Init(SDL_INIT_VIDEO);
	//SDL_WM_SetIcon(SDL_LoadBMP("Images/icone.bmp"), NULL);
	SDL_WM_SetCaption("Foret sauvage", NULL);

	SDL_Surface *ecranDeFond = SDL_LoadBMP("terrain.bmp");
	SDL_Surface *ecran = SDL_SetVideoMode(ecranDeFond->w, ecranDeFond->h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF); // | SDL_RESIZABLE

	SDL_Flip(ecran);
	SDL_Event event;
	while(troupeau.size() > 0)
	{
		for(unsigned int i=0; i<foret.size(); i++)foret[i].vivre(foret, map);
		for(unsigned int i=0; i<troupeau.size(); i++) troupeau[i].vivre(troupeau, foret, map);

		SDL_PollEvent(&event);
		if(event.type == SDL_QUIT) break;
		if(event.type == SDL_MOUSEBUTTONDOWN) troupeau[0].suivreSouris(event.button.x, event.button.y);
		
		rafraichirEcran(ecran, ecranDeFond, foret, troupeau);
	}

	if(troupeau.size() == 0) SDL_Delay(750);

	SDL_Quit();
	return EXIT_SUCCESS;
}