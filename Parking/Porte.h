/*************************************************************************
                           XXX  -  description
                             -------------------
    début                : XXX
    copyright            : (C) XXX par XXX
    e-mail               : XXX
*************************************************************************/

//---------- Interface de la tâche <Porte> (porte XXX.h) -------
#if ! defined ( PORTE_H )
#define PORTE_H

//------------------------------------------------------------------------
// Rôle de la tâche <Porte>
//
//
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées
#include "/public/tp/tp-multitache/Outils.h"
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Porte(int uneDescR, TypeBarriere uneBarriere, key_t keySM, key_t keySem);
// Mode d'emploi :
//
// Contrat :
//

#endif // PORTE_H
