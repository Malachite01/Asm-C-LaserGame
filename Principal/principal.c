#include "DriverJeuLaser.h"
#include "ServiceJeuLaser.h"
#include "../GestionSon/GestionSon.h"


// Prototype de la fonction, d�finie en ASM dans le fichier GestionSon.s
extern void GestionSon_callback(void);

extern int SortieSon;

extern int PeriodeSonMicroSec;

extern int GestionSon_Index;

extern int LongueurSon;


int main(void) {
	// ===========================================================================
	// ============= INIT PERIPH (faites qu'une seule fois)  =====================
	// ===========================================================================

	/* Apr�s ex�cution : le coeur CPU est clock� � 72MHz ainsi que tous les timers */
	CLOCK_Configure();

	/* Configuration du son (voir ServiceJeuLaser.h) 
	 Ins�rez votre code d'initialisation des parties mat�rielles g�rant le son ....*/	


	//============================================================================	
	
	
	// Appel de la fonction toutes les PeriodeSonMicroSec
	ServJeuLASER_Son_Init(PeriodeSonMicroSec, 0, GestionSon_callback);
	GestionSon_Stop(); //Arr�ter le son au d�marrage
	
	while	(1) {
		
	}
}
