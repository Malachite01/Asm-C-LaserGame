#include "../ServiceDFT/DFT.h"
#include "CosSin_float.h" // Import des tableaux sin cos pour e^jx = cosx + jsinx dans notre cas e^-j2 pi kn/M devient cos(2 pikn/M)-jsin(2 pi kn/M)

// Fonction pour calculer la dft
float calculer_DFT(float *Signal, int nb_echantillons, int k) {
    float partie_reelle = 0.0;
    float partie_imaginaire = 0.0;
    
    for (int n = 0; n < nb_echantillons; n++) {
        int indice = (k * n)%nb_echantillons;  // Calcul initial de l'indice

        partie_reelle += Signal[n] * TabCos[indice];
        partie_imaginaire += Signal[n] * TabSin[indice];
    }
    return (partie_reelle * partie_reelle + partie_imaginaire * partie_imaginaire);  // Module au carré 
}
