#include "DriverJeuLaser.h"
#include "ServiceJeuLaser.h"
#include "../GestionSon/GestionSon.h"
#include "../ServiceDFT/DFT.h"
#include "../ServiceDFT_Fract/DFT_Fract.h"
#include "../ServiceDFT/Signal_float.h" // Import du tableau (x(n)) (le signal) pour DFT float
#include "../ServiceDFT_Fract/Signal_4_12.h" // Import du tableau (x(n)) (le signal) pour DFT fract
#include <stdint.h>
#include "../ModuleAffichage/Affichage_Valise.h" // Import des fonctions pour l'affihchage

// Parametres du jeu
#define NB_JOUEURS 4
#define PREMIERE_FREQ_JOUEUR 17 // Raie minimum a 85Hz donc correspond a la case 17
#define SEUIL_DFT 10000000
#define MAX_SCORE 99

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

int scores[NB_JOUEURS];
int compteur_scores[NB_JOUEURS]; // Compteur temporaire utilisé pour ajouter un point au score 

// Fonction pour initialiser les afficheurs pour les joueurs
void InitialisationScores(void) {
	Init_Affichage();
	for (int k = 1; k <= NB_JOUEURS; k++) {
		Prepare_Afficheur(k, 0);
	}
	Mise_A_Jour_Afficheurs_LED();
}

void Scores(void) {
	// On parcours le Nombre de joueurs
	for(int i = 0; i < NB_JOUEURS; i++) {
		// Detecter un tir dans DFT_Tab[]
		if(DFT_Tab[PREMIERE_FREQ_JOUEUR + i] >= SEUIL_DFT && scores[i] < MAX_SCORE) { // Si superieur ou egal au seuil alors il y'a un tir
			GestionSon_Start(); // On joue le son
			compteur_scores[i]++; // increment du compteur temporaire
			if (compteur_scores[i] >= 12){ // Si supérieur a 12 (on va capter 20 fois le signal) on incremente le score
				scores[i]++;
				compteur_scores[i] = -7; // En moyenne comme ca ca annulle les points attribués en trop apres un appel (vu qu'au max on a 20 appels)
			}
			
			// Leds
			Prepare_Afficheur(i + 1, ++scores[i]);
			Mise_A_Jour_Afficheurs_LED();
		}
	}
}


// Relevé périodique d'un bloc de 64 échantillons, en faire une DFT
void DMA_callback(void) {
	ServJeuLASER_StartDMA(); //Acquisition d'un nouveau bloc de 64 echantillons pour la DFT
	for (int k = 0; k<64; k++){
		DFT_Tab[k] = calculer_DFT_Fract(DMA_Tab, M, k); //Remplissage du tableau avec les nouveaux résultats de la DFT
	}
	Scores(); // Verifier si quelqu'un a tiré sur la cible
}

int main(void) {
	// ===========================================================================
	// ============= INIT PERIPH (faites qu'une seule fois)  =====================
	// ===========================================================================

	/* Après exécution : le coeur CPU est clocké à 72MHz ainsi que tous les timers */
	CLOCK_Configure();

	/* Configuration du son (voir ServiceJeuLaser.h) 
	 Insérez votre code d'initialisation des parties matérielles gérant le son ....*/	

	InitialisationScores(); // Initialisation de l'afficheur de joueurs
	
	/* Initialisation de l'adresse du tableau DMA, pour le relevé périodique d'un bloc de 64 échantillons */
	ServJeuLASER_ADC_DMA(DMA_Tab);
	//============================================================================	
	
	GestionSon_Stop(); //Ne pas jouer le son au démarrage
	ServJeuLASER_Son_Init(PeriodeSonMicroSec, 0, GestionSon_callback); //Appel de la fonction toutes les PeriodeSonMicroSec
	
	
	ServJeuLASER_Systick_IT_Init(5000, 1, DMA_callback); //Configure le timer Systick pour qu'il génère une interruption périodique (et appelle la fonction DMA_callback pour generer des nouvelles données)
	
	while	(1) {
		
	}
}
