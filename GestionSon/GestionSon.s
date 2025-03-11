	PRESERVE8
	THUMB   

	;Import des variables
	IMPORT LongueurSon 
	IMPORT PeriodeSonMicroSec
	IMPORT Son

	;Variables export�es
	EXPORT GestionSon_Index
	EXPORT SortieSon ;pour le logic visu

	;Fonctions export�es
	EXPORT GestionSon_callback	

	; ====================== zone de r�servation de donn�es,  ======================================
	;Section RAM (read only) :
	area    mesdata,data,readonly

	;Section RAM (read write):
	area    maram,data,readwrite
GestionSon_Index DCD 0 ;Creation de la variable initialise la variable � 0
;SortieSon DCW 0 ;Variable de sortie de valeur du tableau Son[index] � l'�chelle [0,719], var sur 16 bits (DCW)
SortieSon DCD 0 ;var sur 32 bits (DCD)
		
	; ===============================================================================================

	
	;Section ROM code (read only) :		
	area    moncode,code,readonly


	;Fonction GestionSon_Callback utilis�e dans principal.c
GestionSon_callback PROC
	PUSH {r4,r5}
	
	;--------Partie recuperation de l'index courant
	LDR r1, =GestionSon_Index ;recuperation de l'adresse memoire de la var GestionSon_Index
	LDR r0, [r1] ;recuperation de la valeur de la var dans r0 avec l'adresse de r1
	
	;--------Partie recuperation de la taille du tableau et comparaison, direction fin de la boucle 
	LDR r5, =LongueurSon
	LDR r5, [r5]
	CMP r0, r5
	BGE FinParcoursTableau ;Si index courant plus grand ou egal a taille tableau, direction fin de boucle
	
	;--------Partie lecture du tab son[index]
	LDR r2, =Son ;recup adresse du tab
    LSL r0, r0, #1 ;multiplication de l'index par 2 (taille d'un short int, soit 2 octets)
    ADD r2, r2, r0 ;ajout de l'offset � l'adresse du tableau pour lire l'�l�ment voulu
    LDRSH r3, [r2] ;lecture du tableau avec offset index� (Son[index]) dans r3 (preincrement, remplace le add) (ATTENTION LDRSH car elements negatifs)
	
	;--------Partie mise � l'�chelle sur [0,719]: ((valeur+32768)*719)/65535)  
	ADD r3, #32768 ;d�calage de la valeur pour �tre positive
	MOV r4, #719
	MUL r3, r4 ;fait la multiplication d'abord
	MOV r5, #65535
	UDIV r3, r3, r5 ;division apr�s la multiplication
	
	;--------Partie stockage de la valeur du tableau a l'echelle dans SortieSon
	LDR r4, =SortieSon ;Adresse
	STR r3, [r4] ;stockage de la valeur lue dans SortieSon
	
	;--------Partie incr�mentation de l'index
	LDR r0, [r1] ; r�cup�ration de l'index initial pour l'incr�menter (car r0 est multipli� par 2 pour la lecture du bon index)
	ADD r0, #1
	STR r0, [r1] ;stockage de la valeur de r0 +1 a l'adresse r1, r1 etant GestionSon_Index

;--------Forcer la sortie de la fonction quand tableau parcouru (vu qu'appel� toutes les 91�s dans main)
FinParcoursTableau	
	POP {r4,r5}
	BX lr ;return
	ENDP ;indique la fin de la procedure

	END