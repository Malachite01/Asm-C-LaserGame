#ifndef DFT_FRACT_H__
#define DFT_FRACT_H__
#include <stdint.h>
/**
	* @brief  Calcule la DFT et renvoie le module au carr�
	* @param  nb_echantillons nb echantillons M (ici 64)
	* @param  k nb de periode sur lesquelles les echantillons sont �tal�s
  * @retval module au carr�
  */
uint32_t calculer_DFT_Fract(int16_t *Signal, int nb_echantillons, int k);

#endif