	PRESERVE8
	THUMB   

	;Import des variables
	IMPORT LongueurSon 
	IMPORT PeriodeSonMicroSec
	IMPORT Son


	;Fonctions exportées
	EXPORT GestionSon_callback
	;Variables exportées
	EXPORT GestionSon_Index


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
		
	;Fonction GestionSon_Callback utilisée dans principal.c
GestionSon_callback PROC
	PUSH {r4,r5,r6}
	
ParcoursTableau
	;--------Partie recuperation de l'index courant
	LDR r1, =GestionSon_Index ;recuperation de l'adresse memoire de la var GestionSon_Index
	LDR r0, [r1] ;recuperation de la valeur de la var dans r0 avec l'adresse de r1
	
	;--------Partie recuperation de la taille du tableau et comparaison, direction fin de la boucle 
	LDR r6, =LongueurSon
	LDR r6, [r6]
	CMP r0, r6
	BGE FinParcoursTableau ;Si index courant plus grand ou egal a taille tableau, direction fin de boucle
	
	;--------Partie lecture du tab son[index]
	LDR r2, =Son ;recup adresse du tab
    LSL r0, r0, #1 ;multiplication de l'index par 2 (taille d'un short int, soit 2 octets)
    ;ADD r2, r2, r0 ;ajout de l'offset à l'adresse du tableau pour lire l'élément voulu
    LDRH r4, [r2, r0] ;lecture du tableau avec offset indexé (Son[index]) dans r4 (preincrement, remplace le add)
	
	;--------Partie mise à l'échelle sur [0,719]: (valeur+32768)*(719/65535)
	ADD r4, #32768
	MOV r5, #719
	MOV r6, #65535
	UDIV r5, r5, r6
	MUL r4,  r5
	
	;--------Partie stockage de la valeur du tableau a l'echelle dans SortieSon
	LDR r5, =SortieSon ;Adresse
	STR r4, [r5] ;stockage de la valeur lue dans SortieSon
	
	;--------Partie incrémentation de l'index
	LDR r0, [r1] ; récupération de l'index initial pour l'incrémenter (car r0 est multiplié par 2 pour la lecture du bon index)
	ADD r0, #1
	STR r0, [r1] ;stockage de la valeur de r0 +1 a l'adresse r1, r1 etant GestionSon_Index
	
	;--------Bouclage
	B ParcoursTableau

;--------Sortie de la boucle
FinParcoursTableau	
	BX lr ;return
	POP {r4,r5,r6}
	ENDP ;indique la fin de la procedure



	END	