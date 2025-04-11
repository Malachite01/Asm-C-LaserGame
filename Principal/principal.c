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

extern int16_t LeSignalFract[]; //Signal fractionnaire pour tester

short int DMA_Tab[64]; //Tableau du relev� p�riodique des 64 echantillons

unsigned int DFT_Tab[64]; //Tableau de r�sultat de DFT

//Relev� p�riodique d'un bloc de 64 �chantillons, en faire une DFT
void DMA_callback(void) {
	ServJeuLASER_StartDMA(); //Acquisition d'un nouveau bloc de 64 echantillons pour la DFT
	for (int k = 0; k<64; k++){
		DFT_Tab[k] = calculer_DFT_Fract(DMA_Tab, M, k); //Remplissage du tableau avec les nouveaux r�sultats de la DFT
	}
}

int main(void) {
	// ===========================================================================
	// ============= INIT PERIPH (faites qu'une seule fois)  =====================
	// ===========================================================================

	/* Apr�s ex�cution : le coeur CPU est clock� � 72MHz ainsi que tous les timers */
	CLOCK_Configure();

	/* Configuration du son (voir ServiceJeuLaser.h) 
	 Ins�rez votre code d'initialisation des parties mat�rielles g�rant le son ....*/	

	/* Initialisation de l'adresse du tableau DMA, pour le relev� p�riodique d'un bloc de 64 �chantillons */
	ServJeuLASER_ADC_DMA(DMA_Tab);
	//============================================================================	
	
	GestionSon_Stop(); //Ne pas jouer le son au d�marrage
	ServJeuLASER_Son_Init(PeriodeSonMicroSec, 0, GestionSon_callback); //Appel de la fonction toutes les PeriodeSonMicroSec
	
	
	ServJeuLASER_Systick_IT_Init(5000, 1, DMA_callback); //Configure le timer Systick pour qu'il g�n�re une interruption p�riodique
	
	while	(1) {
		
	}
}
