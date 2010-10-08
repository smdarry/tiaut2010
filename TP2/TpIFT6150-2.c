/*------------------------------------------------------*/
/* Prog    : TpIFT6150-2.c                              */
/* Auteur  :                                            */
/* Date    :                                            */
/* version :                                            */ 
/* langage : C                                          */
/* labo    : DIRO                                       */
/*------------------------------------------------------*/

/*------------------------------------------------*/
/* FICHIERS INCLUS -------------------------------*/
/*------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "FonctionDemo2.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/                              
/*------------------------------------------------*/
#define NAME_IMG_IN  "photograph"
#define NAME_IMG_GRADIENT "TpIFT6150-2-gradient"
#define NAME_IMG_SUPPRESSION "TpIFT6150-2-suppression"
#define NAME_IMG_CANNY "TpIFT6150-2-canny"

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,int** argv)
 {
  int i,j,k,l;
  int length,width;
  float tau_L;
  float tau_H;
  float p_H;
  float sigma;
	
  /* Entrer des valeurs */
  printf("Entrez la valeur de tau_L: ");
  scanf("%f",&tau_L);
  printf("Entrez la valeur de tau_H: ");
  scanf("%f",&tau_H);
  printf("Entrez l'ecart type du filter Gaussien: ");
  scanf("%f",&sigma);
  
  /* Implementer detecteur de Canny */
  
  /* Sauvegarder les images 
     TpIFT6150-2-gradient.pgm
     TpIFT6150-2-suppression.pgm
     TpIFT6150-2-canny.pgm */

   printf("Entrez la valeur de p_H: ");
   scanf("%f",&p_H);
 
   /* Implementer detecteur de Canny semi-automatique */
  
   /* Sauvegarder l'image TpIFT6150-2-cannySemiAuto.pgm */
   
   /*retour sans probleme*/  
 
  printf("\n C'est fini ... \n\n\n");
  return 0; 	 
}
