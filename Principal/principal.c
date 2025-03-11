

#include "DriverJeuLaser.h"
#include "ServiceJeuLaser.h"


// Prototype de la fonction, d�finie en ASM dans le fichier GestionSon.s
extern void GestionSon_callback(void);

int main(void) {
	// ===========================================================================
	// ============= INIT PERIPH (faites qu'une seule fois)  =====================
	// ===========================================================================

	/* Apr�s ex�cution : le coeur CPU est clock� � 72MHz ainsi que tous les timers */
	CLOCK_Configure();

	/* Configuration du son (voir ServiceJeuLaser.h) 
	 Ins�rez votre code d'initialisation des parties mat�rielles g�rant le son ....*/	


	//============================================================================	

	// Appel de la fonction toutes les 1�s
	ServJeuLASER_Son_Init(1000, 0, GestionSon_callback);

	while	(1) {
		
	}
}
