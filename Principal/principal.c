#include "DriverJeuLaser.h"
#include "ServiceJeuLaser.h"
#include "../GestionSon/GestionSon.h"
#include "../ServiceDFT/DFT.h"
#include "../ServiceDFT_Fract/DFT_Fract.h"
#include "../ServiceDFT/Signal_float.h" // Import du tableau (x(n)) (le signal) pour DFT float
#include "../ServiceDFT_Fract/Signal_4_12.h" // Import du tableau (x(n)) (le signal) pour DFT fract
#include <stdint.h>

// Prototype de la fonction, d�finie en ASM dans le fichier GestionSon.s
extern void GestionSon_callback(void);

extern int SortieSon;

extern int PeriodeSonMicroSec;

extern int GestionSon_Index;

extern int LongueurSon;

// Nombre d'�chantillons
int M = 64;

float ModuleCarre;

extern int16_t LeSignalFract[];


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
	GestionSon_Stop(); //Arr�ter le son au d�marrage
	ServJeuLASER_Son_Init(PeriodeSonMicroSec, 0, GestionSon_callback);
	
	
	for (int k = 0; k<64; k++){
		ModuleCarre = calculer_DFT_Fract(LeSignalFract, M, k);
	}
	
	while	(1) {
		
	}
}
