#include "DriverJeuLaser.h"
#include "ServiceJeuLaser.h"
#include "../GestionSon/GestionSon.h"
#include "../ServiceDFT/DFT.h"
#include "../ServiceDFT_Fract/DFT_Fract.h"
#include "../ServiceDFT/Signal_float.h" // Import du tableau (x(n)) (le signal) pour DFT float
#include "../ServiceDFT_Fract/Signal_4_12.h" // Import du tableau (x(n)) (le signal) pour DFT fract
#include <stdint.h>

// Prototype de la fonction, définie en ASM dans le fichier GestionSon.s
extern void GestionSon_callback(void);

extern int SortieSon;

extern int PeriodeSonMicroSec;

extern int GestionSon_Index;

extern int LongueurSon;

// Nombre d'échantillons
int M = 64;

float ModuleCarre;

extern int16_t LeSignalFract[];


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
	GestionSon_Stop(); //Arréter le son au démarrage
	ServJeuLASER_Son_Init(PeriodeSonMicroSec, 0, GestionSon_callback);
	
	
	for (int k = 0; k<64; k++){
		ModuleCarre = calculer_DFT_Fract(LeSignalFract, M, k);
	}
	
	while	(1) {
		
	}
}
