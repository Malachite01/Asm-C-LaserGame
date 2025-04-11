#include <stdint.h>
#include "../ServiceDFT_Fract/DFT_Fract.h"
#include "CosSin_Fract_1_15.h" // TabCosFract[64], TabSinFract[64] en 1.15

// Signal est un tableau de 64 valeurs en format 4.12 (int16_t)
// TabCos et TabSin sont en format 1.15 (int16_t)
int32_t calculer_DFT_Fract(int16_t *Signal, int nb_echantillons, int k) {
    int64_t partie_reelle = 0, partie_imaginaire = 0;

    for (int n = 0; n < nb_echantillons; n++) {
        int indice = (k * n) % nb_echantillons; //Calcul initial de l'indice

        partie_reelle += (int32_t)Signal[n] * (int32_t)TabCosFract[indice]; //TabCos[indice] (1.15) * Signal[n] (4.12) = 5.27
        partie_imaginaire += (int32_t)Signal[n] * (int32_t)TabSinFract[indice]; //TabSin[indice] (1.15) * Signal[n] (4.12) = 5.27
    }
		
		int32_t result = (partie_reelle * partie_reelle+partie_imaginaire * partie_imaginaire) >> 32; //Format final : 10.54
    return result;
}