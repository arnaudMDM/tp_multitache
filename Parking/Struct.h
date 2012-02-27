#include "/public/tp/tp-multitache/Outils.h"

struct t_requete{
	time_t dateArrivee;
	TypeUsager usager;
};

struct t_voiture{
	TypeUsager usager;
	int numVoiture;
};

struct t_mem{
	int nbVoiture;
	t_requete tabRequete[3];
};
