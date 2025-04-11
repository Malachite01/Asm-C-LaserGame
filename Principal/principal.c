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

extern int16_t LeSignalFract[]; //Signal fractionnaire pour tester

short int DMA_Tab[64]; //Tableau du relevé périodique des 64 echantillons

unsigned int DFT_Tab[64]; //Tableau de résultat de DFT

//Relevé périodique d'un bloc de 64 échantillons, en faire une DFT
void DMA_callback(void) {
	ServJeuLASER_StartDMA(); //Acquisition d'un nouveau bloc de 64 echantillons pour la DFT
	for (int k = 0; k<64; k++){
		DFT_Tab[k] = calculer_DFT_Fract(DMA_Tab, M, k); //Remplissage du tableau avec les nouveaux résultats de la DFT
	}
}

int main(void) {
	// ===========================================================================
	// ============= INIT PERIPH (faites qu'une seule fois)  =====================
	// ===========================================================================

	/* Après exécution : le coeur CPU est clocké à 72MHz ainsi que tous les timers */
	CLOCK_Configure();

	/* Configuration du son (voir ServiceJeuLaser.h) 
	 Insérez votre code d'initialisation des parties matérielles gérant le son ....*/	

	/* Initialisation de l'adresse du tableau DMA, pour le relevé périodique d'un bloc de 64 échantillons */
	ServJeuLASER_ADC_DMA(DMA_Tab);
	//============================================================================	
	
	GestionSon_Stop(); //Ne pas jouer le son au démarrage
	ServJeuLASER_Son_Init(PeriodeSonMicroSec, 0, GestionSon_callback); //Appel de la fonction toutes les PeriodeSonMicroSec
	
	
	ServJeuLASER_Systick_IT_Init(5000, 1, DMA_callback); //Configure le timer Systick pour qu'il génère une interruption périodique
	
	while	(1) {
		
	}
}
