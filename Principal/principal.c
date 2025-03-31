#include "DriverJeuLaser.h"
#include "ServiceJeuLaser.h"
#include "../GestionSon/GestionSon.h"


// Prototype de la fonction, définie en ASM dans le fichier GestionSon.s
extern void GestionSon_callback(void);

extern int SortieSon;

extern int PeriodeSonMicroSec;

extern int GestionSon_Index;

extern int LongueurSon;


int main(void) {
	// ===========================================================================
	// ============= INIT PERIPH (faites qu'une seule fois)  =====================
	// ===========================================================================

	/* Après exécution : le coeur CPU est clocké à 72MHz ainsi que tous les timers */
	CLOCK_Configure();

	/* Configuration du son (voir ServiceJeuLaser.h) 
	 Insérez votre code d'initialisation des parties matérielles gérant le son ....*/	


	//============================================================================	
	
	
	// Appel de la fonction toutes les PeriodeSonMicroSec
	ServJeuLASER_Son_Init(PeriodeSonMicroSec, 0, GestionSon_callback);
	GestionSon_Stop(); //Arréter le son au démarrage
	
	while	(1) {
		
	}
}
