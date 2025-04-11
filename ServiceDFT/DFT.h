#ifndef DFT_H__
#define DFT_H__

/**
	* @brief  Calcule la DFT et renvoie le module au carré
	* @param  nb_echantillons nb echantillons M (ici 64)
	* @param  k nb de periode sur lesquelles les echantillons sont étalés
  * @retval module au carré
  */
float calculer_DFT(float *Signal, int nb_echantillons, int k);

#endif