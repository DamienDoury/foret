/*
Ici ça a été la galère car les problèmes conceptuels, mathématiques et de programmation se sont accumulés.
Conception : de quels éléments doit-etre constituée la mémoire ? Comment la gérer ? Que doit-elle enregistrer ? Ajuster les valeurs ...
Mathématiques : formules de trigo (cos, sin) pour le déplacement, leur ensemble de définition, théorème de Pythagore, ...
Programmation : nombreux transtypage (cast), passages de pointeurs, encapsulation des données, gestion des classes, choisir la boucle la mieux appropriée ...
*/

/*
OBJECTIFSSSSS :
Ajuster les valeurs ...
Faire esquiver les obstacles à la bestiole :S

La memoireGeographique est-elle vouée à disparaitre ?
*/

#include "Bestiole.h"

using namespace std;

ActionEnCours::ActionEnCours() : action("Ne fait rien"), valeur1(0), valeur2(0), x(0), y(0), priorite(10)
{
}

ActionEnCours::~ActionEnCours()
{
}

int idBestiole = 200;

Bestiole::Bestiole() : m_id(++idBestiole), m_espece("Moufi"), m_estMort(false), m_porteeVue(175), m_taille(25), m_depenseEnergieDeBase(1), m_esperanceDeVie(20), m_dateNaissance(clock()), m_x(rand()%799), m_y(rand()%599), m_angle(0)
{
	m_estomac.push_back(Nourriture());
	m_estomac[0].nomAliment = "Lait Maternel";
	m_estomac[0].valeurEnergetique = 100;
	m_estomac[0].dateDerniereConsommation = m_dateNaissance;

	m_faim = m_estomac[0].valeurEnergetique;

	m_memoireGeographique.assign(800, vector<short> (600, 0));
}

Bestiole::~Bestiole()
{
}

void ActionEnCours::effacer()
{
	action = "Ne fait rien";
	valeur1 = 0;
	valeur2 = 0;
	x = 0;
	y = 0;
	priorite = 10;
}

void Bestiole::vivre(vector<Bestiole> &troupeau, vector<Arbre> &foret, vector<vector<short> > &map)
{
	consommerEnergie(m_depenseEnergieDeBase); //prochainnement fonctionnementInterne();
	oublier();
	observer(foret, map);
	decider();
	agir(troupeau, foret, map);
}

void Bestiole::decider()
{
	if(m_faim < 50 && m_objectif.priorite >= 2)
	{
		m_objectif.action = "Se nourrir";
		if(m_faim < 20) m_objectif.priorite = 1;
		else m_objectif.priorite = 2;
	}
	else if(m_faim >= 50) // && m_objectif.priorite >= 5
	{
		m_objectif.action = "Explorer";
		m_objectif.priorite = 9;
	}

}

void Bestiole::agir(vector<Bestiole> &troupeau, vector<Arbre> &foret, vector<vector<short> > &map) //cette fonction doit être celle qui changera le + au cours du programme, elle représente "l'intelligence" de la bestiole.
{
	if(m_objectif.action == "Se nourrir")
	{
		int xDestination;
		int yDestination;
		int distance;
		int plusCourteDistance = INT_MAX;

		for(int i=0; i<m_memoire.size(); i++)
		{
			if(m_memoire[i].action == "Trouve") // && m_memoire[i].variationBienEtre >= 0 && ...
			{
				bool sauterArbre = false;
				for(int j=0; j<m_memoire.size(); j++)
				{
					if(m_memoire[j].action == "Frustre" 
						&& m_memoire[j].x == m_memoire[i].x 
						&& m_memoire[j].y == m_memoire[i].y
						&& m_memoire[j].valeur1 == m_memoire[i].valeur1)
					{
						sauterArbre = true;
						break;
					}
				}

				if(sauterArbre) continue;

				distance = (int)sqrt((double)((m_memoire[i].x - m_x) * (m_memoire[i].x - m_x)) + ((m_memoire[i].y - m_y) * (m_memoire[i].y - m_y)));
				if(distance < plusCourteDistance)
				{
					plusCourteDistance = distance;
					m_action.x = m_memoire[i].x;
					m_action.y = m_memoire[i].y;
					m_action.valeur1 = m_memoire[i].valeur1;
				}
			}
		}

		if(plusCourteDistance != INT_MAX)
		{
			if(plusCourteDistance < 1 + m_action.valeur1*(float)sqrt(2.0) + m_taille*(float)sqrt(2.0)) //Le *(float)sqrt(2.0) est à rajouter car la bestiole et l'arbre ont une forme carrée sur la carte.
			{
				int numeroArbre;
				SDL_Rect coord;
				for(numeroArbre=0; numeroArbre<=foret.size(); numeroArbre++)
				{
					if(numeroArbre == foret.size()) break;
					foret[numeroArbre].coordonnees(coord.x, coord.y);
					if(coord.x == m_action.x && coord.y == m_action.y && foret[numeroArbre].rayonTronc() == m_action.valeur1) break;
				}

				if(numeroArbre < foret.size())
				{
					m_action.action = "Manger";
					manger(foret, numeroArbre);
				}
			}
			else if(m_objectif.priorite < 2)
			{
				m_action.action = "Déplacement";
				deplacer(map);
			}
			else 
			{
				m_action.action = "Déplacement";
				deplacer(map);
			}
		}

		if(plusCourteDistance == INT_MAX)
		{
			m_objectif.action = "Explorer";
			m_objectif.priorite = 2;
		}
	}

	if(m_objectif.action == "Explorer")
	{
		if(m_action.action == "Ne fait rien")
		{
			int xDestination;
			int yDestination;

			do xDestination = m_x - m_porteeVue + rand()%(2*m_porteeVue+1);
			while(xDestination < 1 || xDestination > 799);

			do yDestination = m_y - m_porteeVue + rand()%(2*m_porteeVue+1);
			while(yDestination < 1 || yDestination > 599);

			m_action.action = "Déplacement";
			m_action.x = xDestination;
			m_action.y = yDestination;
		}

		deplacer(map);
	}
}

void Bestiole::consommerEnergie(int consommation)
{
	m_faim = 0;
	clock_t dateActuelle = clock();

	for(int i=m_estomac.size()-1; i>=0; i--)
	{
		if((dateActuelle - m_estomac[i].dateDerniereConsommation) / CLOCKS_PER_SEC >= 1)
		{
			m_estomac[i].valeurEnergetique -= 1+consommation/m_estomac.size();
			m_estomac[i].dateDerniereConsommation = dateActuelle;
		}

		if(m_estomac[i].valeurEnergetique <= 0)
		{
			for(int x=i; x<m_estomac.size()-1; x++) //destruction de l'aliment entièrement consommé.
			{
				m_estomac[x] = m_estomac[x+1];
			}
			m_estomac.pop_back();
		}
		else
		{
			m_faim += m_estomac[i].valeurEnergetique;
		}
	}

	if(m_faim <= 0) m_estMort = true;
}

void Bestiole::manger(vector<Arbre> &foret, int n)
{
	if(foret[n].nombreFruits() <= 0)
	{
		m_memoire.push_back(MemoireCentrale());
		m_memoire.back().action = "Frustre"; //il ne faut pas effacer l'arbre de la mémoire ("Trouve")
		SDL_Rect coord;
		foret[n].coordonnees(coord.x, coord.y);
		m_memoire.back().x = coord.x;
		m_memoire.back().y = coord.y;
		m_memoire.back().valeur1 = foret[n].rayonTronc();
	}
	else
	{
		m_estomac.push_back(Nourriture());
		foret[n].donnerFruit(m_estomac.back().nomAliment, m_estomac.back().valeurEnergetique, m_estomac.back().dateDerniereConsommation);
		if(m_faim + m_estomac.back().valeurEnergetique > 50*2)
		{
			m_action.effacer();
		}
	}
}

void Bestiole::deplacer(vector<vector<short> > &map, int xDestination, int yDestination)
{
	int m_vitesseDeplacement = 6;
	double angleVise;

	if(xDestination == -1) xDestination = m_action.x;
	if(yDestination == -1) yDestination = m_action.y;

	if(m_x == xDestination && m_y == yDestination || m_action.action == "Ne fait rien") //détermination d'une nouvelle destination aléatoire.
	{
		do xDestination = m_x - m_porteeVue + rand()%(2*m_porteeVue+1);
		while(xDestination < 1 || xDestination > 799);

		do yDestination = m_y - m_porteeVue + rand()%(2*m_porteeVue+1);
		while(yDestination < 1 || yDestination > 599);

		m_action.action = "Déplacement";
		m_action.x = xDestination;
		m_action.y = yDestination;
	}
	else
	{
		int hyp = (int)sqrt((double)(xDestination - m_x)*(xDestination - m_x) + (yDestination - m_y)*(yDestination - m_y)); //détermination de l'angle à adopter pour se rendre à un point donné. Sert juste pour l'effet graphique depuis la nouvelle version.
		angleVise = acos((double)(yDestination - m_y) / hyp); //acos compris entre 0 et 180, ce qui pose problème ...
		angleVise = angleVise*180.0/PI;
		if(m_x < xDestination)angleVise = 180 + angleVise;
		if(m_x >= xDestination)angleVise = 180 - angleVise;
		if(angleVise > 359) angleVise -= 360;

		//la bestiole ne doit pas tourner et avancer en même temps, cela ne fait pas réaliste.
		//il faut juste atténuer cet effet (pas le supprimer car cela ne serait plus réaliste non plus).
		//la bestiole peut tourner et avancer en même temps seulement en cas de fuite face à une agression par exemple (à implémenter).
		//pour + de réalisme, il faudrait créer un effet de "freinage" quand la bestiole est proche de sa cible.

		if(m_angle != angleVise) //soit la bestiole tourne, soit elle marche. Elle ne fait pas les 2 en même temps.
		{
			tourner(angleVise);
		}

		if(abs(m_angle - angleVise) < 72)
		{
			//m_x -= (int)m_vitesseDeplacement * sin(m_angle); //ancien système : foireux (la bestiole boite) mais plus réaliste
			//m_y -= (int)m_vitesseDeplacement * cos(m_angle); //ancien système : foireux (la bestiole boite) mais plus réaliste
		
			int var = 1;
			int deplacementX;
			int deplacementY;
			do
			{
				deplacementX = (int)(xDestination - m_x)/var;
				deplacementY = (int)(yDestination - m_y)/var;
				var++;
			}
			while(deplacementX*deplacementX + deplacementY*deplacementY > m_vitesseDeplacement*m_vitesseDeplacement);

			int coeffEcartement = abs(m_angle - angleVise)/12; //sert à rendre réaliste la variation de vitesse lors d'un changement de trajectoire.
			if(coeffEcartement < 1) coeffEcartement = 1;

			deplacementX /= coeffEcartement;
			deplacementY /= coeffEcartement;

			bool continuer = true; 
			for(int i=m_x+deplacementX-m_taille; i<m_x+deplacementX+m_taille; i++) //On vérifie qu'on peut avancer sans se heurter à un obstacle.
			{
				for(int j=m_y+deplacementY-m_taille; j<m_y+deplacementY+m_taille; j++)
				{
					if(i>=0 && i<800 && j>=0 && j<600)
					{
						if(map[i][j] != 1 && map[i][j] != 3) //la deuxième partie de la condition est à revoir, elle autorise la bestiole à piétiner ses congénères, ce qui n'est pas très cool.
						{
							continuer = false;
							//ici il faut dire à la bestiole d'aller autre part, sinon elle reste bloquée sur l'obstacle.
							//il faut lui dire d'esquiver l'obstacle (aoutch, comment programmer ça ? ><' )
							m_action.effacer();
							//m_estMort = true; //pour le fun ^^ (à virer)
							break;
						}
					}
				}
				if(!continuer) break;
			}

			if(continuer)
			{
				for(int i=m_x-m_taille; i<m_x+m_taille; i++) //On libère notre emplacement précédent ...
				{
					for(int j=m_y-m_taille; j<m_y+m_taille; j++)
					{
						if(i>=0 && i<800 && j>=0 && j<600) map[i][j] = 1;
					}
				}

				m_x += deplacementX;
				m_y += deplacementY;

				if(m_x == m_action.x && m_y == m_action.y) m_action.effacer();

				for(int i=m_x-m_taille; i<m_x+m_taille; i++) //... et on réserve notre nouvelle place.
				{
					for(int j=m_y-m_taille; j<m_y+m_taille; j++)
					{
						if(i>=0 && i<800 && j>=0 && j<600) map[i][j] = 3;
					}
				}
			}
		}
	}
}

void Bestiole::tourner(double angleVise)
{
	int m_vitesseRotation = 6;
	double anglePrecedent = m_angle;

	double difference = m_angle - angleVise;
	if(difference < 0) difference += 360; //ce système permet de contourner le modulo qui veut des int et pas des doubles. Il est utilisé de nombreuses fois.
	if(difference > 180)
	{
		//sens trigo;
		m_angle += m_vitesseRotation;
		if(m_angle > 360) m_angle -= 360;
		if(m_angle - angleVise < m_vitesseRotation && m_angle - angleVise > 0) m_angle = angleVise;
	}
	else
	{
		//sens anti trigo;
		m_angle -= m_vitesseRotation;
		if(m_angle < 0) m_angle += 360;
		if(angleVise - m_angle < m_vitesseRotation && angleVise - m_angle > 0) m_angle = angleVise;
	}
}

void Bestiole::observer(vector<Arbre> &foret, vector<vector<short> > &map) //fonction d'observation à revoir car elle marche très mal.
{
	//BOUCLE GERANT LA DISPARITION DES ARBRES DANS LA MEMOIRE DE LA BESTIOLE.
	for(int placeMemoire=0; placeMemoire<m_memoire.size(); placeMemoire++) //on parcourt toute la mémoire.
	{
		if(m_memoire[placeMemoire].action == "Trouve"
			&& ((m_x - m_memoire[placeMemoire].x)*(m_x - m_memoire[placeMemoire].x) + (m_y - m_memoire[placeMemoire].y)*(m_y - m_memoire[placeMemoire].y)) <= m_porteeVue*m_porteeVue) //on ne prend en compte que les arbres trouvés qui sont dans notre champ de vision.
		{
			bool disparu = true;
			for(int placeForet=0; placeForet<foret.size(); placeForet++) //on parcourt toute la foret.
			{
				if(foret[placeForet].variete() == m_memoire[placeMemoire].cible) //on ne prend en compte les arbres de la foret qui sont de la même race que celui sélectionné dans la mémoire de la bestiole.
				{
					SDL_Rect coord;
					foret[placeForet].coordonnees(coord.x, coord.y);
					if(coord.x == m_memoire[placeMemoire].x && coord.y == m_memoire[placeMemoire].y) //on compare les coordonnées des 2 arbres sélectionnés.
					{
						disparu = false; //ici, l'arbre dans notre mémoire correspond bien à un arbre de la foret, il n'est donc pas disparu et on passe à la vérification de l'arbre suivant de notre mémoire.
						break;
					}
				}
			}

			if(disparu) //si l'arbre qui était dans notre mémoire n'existe plus, on le supprime de notre memoireGeographique PUIS de notre memoire centrale.
			{
				for(int a=m_memoire[placeMemoire].x-m_memoire[placeMemoire].valeur1; a<m_memoire[placeMemoire].x+m_memoire[placeMemoire].valeur1; a++) //ici, on supprime l'arbre de la memoireGeographique.
				{
					for(int b=m_memoire[placeMemoire].y-m_memoire[placeMemoire].valeur1; b<m_memoire[placeMemoire].y+m_memoire[placeMemoire].valeur1; b++)
					{
						if(a > 0 && a < 800 && b > 0 && b < 599)m_memoireGeographique[a][b] = 1; //on remplace l'arbre par du sol. Si un arbre arbre a poussé par dessus celui qui vient de disparaitre, il sera trouvé dans la boucle suivante.
					}
				}
				
				for(int a=placeMemoire; a<m_memoire.size()-1; a++) //là, on le supprime de la mémoire Centrale.
				{
					m_memoire[a] = m_memoire[a+1];
				}
				m_memoire.pop_back();
			}
		}
	}
	//FIN BOUCLE GERANT LA DISPARITION DES ARBRES DANS LA MEMOIRE DE LA BESTIOLE.

	//BOUCLE GERANT L'APPARITION DES ARBRES DANS LA MEMOIRE DE LA BESTIOLE.
	for(int i=m_x-m_porteeVue; i<m_x+m_porteeVue; i++)
	{
		for(int j=m_y-m_porteeVue; j<m_y+m_porteeVue; j++)
		{
			if(((m_x - i)*(m_x - i) + (m_y - j)*(m_y - j)) <= m_porteeVue*m_porteeVue)
			{
				if(i > 0 && i < 800 && j > 0 && j < 600)
				{
					//ici on observe s'il y a des changements sur la carte par rapport à nos souvenirs.
					if(m_memoireGeographique[i][j] != map[i][j])
					{
						//là on test les différents types de changements pour ajouter une information à la mémoire centrale.
						if(map[i][j] == 2)
						{
							SDL_Rect coord;

							for(unsigned int x=0; x<foret.size(); x++)
							{
								foret[x].coordonnees(coord.x, coord.y);
								if(i >= coord.x - foret[x].rayonTronc()
									&& i <= coord.x + foret[x].rayonTronc()
									&& j >= coord.y - foret[x].rayonTronc()
									&& j <= coord.y + foret[x].rayonTronc())
								{
									for(int a=coord.x-foret[x].rayonTronc(); a<coord.x+foret[x].rayonTronc(); a++)
									{
										for(int b=coord.y-foret[x].rayonTronc(); b<coord.y+foret[x].rayonTronc(); b++)
										{
											if(a > 0 && a < 800 && b > 0 && b < 599)m_memoireGeographique[a][b] = 2; //permet de ne pas trouver 2 fois le même arbre.
										}
									}
									//ajout d'une info dans la mémoire centrale.
									//(dans certains cas, comme la disparation d'un arbre, il faut ajouter l'info "arbre mort" et supprimer l'info "arbre présent").
									m_memoire.push_back(MemoireCentrale());
									m_memoire.back().action = "Trouve";
									m_memoire.back().cible = foret[x].variete();
									m_memoire.back().valeur1 = foret[x].rayonTronc();
									SDL_Rect temp;
									foret[x].coordonnees(temp.x, temp.y);
									m_memoire.back().x = temp.x;
									m_memoire.back().y = temp.y;
									m_memoire.back().date = clock();
									m_memoire.back().sourceInfo = m_id;
									m_memoire.back().confianceInfo = 1000;
									m_memoire.back().variationBienEtre = 0;
								}
							}
						}

						//au final, on modifie la mémoire géographique.
						m_memoireGeographique[i][j] = map[i][j];
					}
				}
			}
		}
	}
	//FIN BOUCLE GERANT L'APPARITION DES ARBRES DANS LA MEMOIRE DE LA BESTIOLE.
}

void Bestiole::oublier()
{
	clock_t dateActuelle = clock();
	for(int i=0; i<m_memoire.size(); i++)
	{
		if(m_memoire[i].action == "Frustre")
		{
			if((dateActuelle - m_memoire[i].date) / CLOCKS_PER_SEC > 3) //la bestiole oublie les informations vieilles de + de 3 secondes.
			{
				for(int x=i; x<m_memoire.size()-1; x++)
				{
					m_memoire[x] = m_memoire[x+1];
				}

				m_memoire.pop_back();
			}
		}
	}
}

void Bestiole::suivreSouris(int x, int y)
{
	m_action.x = x;
	m_action.y = y;
}

bool Bestiole::estMort()
{
	return m_estMort;
}

int Bestiole::id()
{
	return m_id;
}

void Bestiole::coordonnees(Sint16 &x, Sint16 &y)
{
	x = m_x;
	y = m_y;
}

int Bestiole::porteeVue()
{
	return m_porteeVue;
}

int Bestiole::angle()
{
	return m_angle;
}

void Bestiole::setAngle(int x)
{
	m_angle = x%360;
	while(m_angle < 0) m_angle += 360;
}
/*
Alors,
voici des précisions sur les étapes à réaliser :
tout d'abord, il faut créer la mémoire géographique de la bestiole.
Une fois créée, il faudra qu'elle s'actualise à chaque boucle pour rester à jour (disparition d'un arbre, etc.)
Et là arrive le plus important : il faut créer une seconde mémoire (VECTEUR DE CLASSE), la vraie, qui analysera (entre autres) l'évolution de la mémoire géographique (qui correspond + à de l'observation qu'à une mémoire ...)
Du genre : "tiens, avant ici il y avait un 2 et maintenant, il y a un 1". <= N'EST PAS JUSTE car si un arbre pousse en 200:100, puis meurre, puis un autre pousse par dessus, la bestiole considèrera que c'est le même arbre, alors que ce pourrait etre une variété différente.
A partir de ça, la vraie mémoire reçoit une nouvelle information : "Woupanier en 307:144 le 23 secondes".
En parallèle, il faut aussi créer une "action en cours" ou "décision" de la bestiole.
Cette VARIABLE contiendra l'action à suivre pour la bestiole.
Si elle a faim, elle devra se tourner vers l'arbre le plus proche et s'y rendre pour manger.
*/