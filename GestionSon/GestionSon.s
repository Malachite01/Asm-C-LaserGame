	PRESERVE8
	THUMB   

	;Import des variables
	IMPORT LongueurSon 
	IMPORT PeriodeSonMicroSec
	IMPORT Son

	;Variables exportées
	EXPORT GestionSon_Index
	EXPORT SortieSon ;pour le logic visu

	;Fonctions exportées
	EXPORT GestionSon_Start	
	EXPORT GestionSon_Stop	
	EXPORT GestionSon_callback
	
	;Fonctions importées
	IMPORT PWM_Set_Value_TIM3_Ch3
		
	; ====================== zone de réservation de données,  ======================================
	;Section RAM (read only) :
	area    mesdata,data,readonly

	;Section RAM (read write):
	area    maram,data,readwrite
GestionSon_Index DCD 0 ;Creation de la variable initialise la variable à 0
;SortieSon DCW 0 ;Variable de sortie de valeur du tableau Son[index] à l'échelle [0,719], var sur 16 bits (DCW)
SortieSon DCD 0 ;var sur 32 bits (DCD)
		
	; ===============================================================================================

	
	;Section ROM code (read only) :		
	area    moncode,code,readonly


	; Fonction qui met l'index courant a 0 pour jouer le son
GestionSon_Start PROC
	LDR r1, =GestionSon_Index 
	MOV r0, #0
	STR r0, [r1]
	BX lr
	ENDP
	
	; Fonction qui met l'index courant a LongueurSon pour stopper le son (arriver a la fin du bruit)
GestionSon_Stop PROC
	LDR r1, =GestionSon_Index 
	LDR r2, =LongueurSon
	LDR r0, [r2]
	STR r0, [r1]
	BX lr
	ENDP
	

	;Fonction GestionSon_Callback utilisée dans principal.c
GestionSon_callback PROC
	PUSH {r4,r5}
	
	;--------Partie recuperation de l'index courant
	LDR r1, =GestionSon_Index ;recuperation de l'adresse memoire de la var GestionSon_Index
	LDR r0, [r1] ;recuperation de la valeur de la var dans r0 avec l'adresse de r1
	
	;--------Partie recuperation de la taille du tableau et comparaison, direction fin de la boucle 
	LDR r5, =LongueurSon
	LDR r5, [r5]
	CMP r0, r5
	BGE FinParcoursTableau ;Si index courant plus grand ou egal a taille tableau, direction fin de fonction
	
	;--------Partie lecture du tab son[index]
	LDR r2, =Son ;recup adresse du tab
    LSL r0, r0, #1 ;multiplication de l'index par 2 (taille d'un short int, soit 2 octets)
    ADD r2, r2, r0 ;ajout de l'offset à l'adresse du tableau pour lire l'élément voulu
    LDRSH r3, [r2] ;lecture du tableau avec offset indexé (Son[index]) dans r3 (preincrement, remplace le add) (ATTENTION LDRSH car elements negatifs)
	
	;--------Partie mise à l'échelle sur [0,719]: ((valeur+32768)*719)/65535)  
	ADD r3, #32768 ;décalage de la valeur pour être positive
	MOV r4, #719
	MUL r3, r4 ;fait la multiplication d'abord
	MOV r5, #65535
	UDIV r3, r3, r5 ;division après la multiplication
	
	;--------Partie stockage de la valeur du tableau a l'echelle dans SortieSon
	LDR r4, =SortieSon ;Adresse
	MOV r0, r3
	STR r0, [r4] ;stockage de la valeur lue dans SortieSon (on a mis r0 au lieu de r3)
	PUSH {lr, r3, r2, r1}
	BL PWM_Set_Value_TIM3_Ch3
	POP{lr, r3, r2, r1}
	
	;--------Partie incrémentation de l'index
	LDR r0, [r1] ; récupération de l'index initial pour l'incrémenter (car r0 est multiplié par 2 pour la lecture du bon index)
	ADD r0, #1
	STR r0, [r1] ;stockage de la valeur de r0 +1 a l'adresse r1, r1 etant GestionSon_Index

;--------Forcer la sortie de la fonction quand tableau parcouru (vu qu'appelé toutes les 91µs dans main)
FinParcoursTableau	
	POP {r4,r5}
	BX lr ;return
	ENDP ;indique la fin de la procedure

	END