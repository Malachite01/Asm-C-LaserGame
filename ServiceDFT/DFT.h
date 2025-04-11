#ifndef DFT_H__
#define DFT_H__

/**
	* @brief  Calcule la DFT et renvoie le module au carr�
	* @param  nb_echantillons nb echantillons M (ici 64)
	* @param  k nb de periode sur lesquelles les echantillons sont �tal�s
  * @retval module au carr�
  */
float calculer_DFT(float *Signal, int nb_echantillons, int k);

#endif