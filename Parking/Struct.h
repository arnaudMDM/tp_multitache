/*************************************************************************
                           Struct  -  description
                             -------------------
    début                : Fevrier
    copyright            : (C) XXX par XXX
    e-mail               : XXX
*************************************************************************/

//---------- Interface du module <Struct> (fichier Struct.h) ---------
#if ! defined ( STRUCT_H )
#define STRUCT_H
//------------------------------------------------------------------------
// Rôle du module <Struct>
// Ce module permet de d�finir les diff�rents types utilis� par les taches
//
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées
#include "/public/tp/tp-multitache/Outils.h"

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types
struct t_requete{
	time_t dateArrivee;
	TypeUsager usager;
};

struct t_voiture{
	TypeUsager usager;
	int numVoiture;
};

struct t_place{
	t_voiture voiture;
	time_t dateArrivee;
};

struct t_mem{
	int nbVoiture;
	t_requete tabRequete[3];
	t_place placesParking[8];
};

#endif
