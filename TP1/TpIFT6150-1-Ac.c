/*------------------------------------------------------*/
/* Prog    : TpIFT6150-1-Ac.c                           */
/* Auteur  : Sylvain Bouchard                           */
/* Courriel: bouchsyl@iro.umontreal.ca                  */
/* Date    : 27/09/2010                                 */
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

#include "FonctionDemo1.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_IMG_IN  "images/D1r"

#define NAME_IMG_OUT "image-TpIFT6150-1-Ac"

float RecalLog(float** mat, int length, int width)
{
    int i,j;
    float max,min;

    /*Recherche du min*/
    min=mat[0][0];
    for(i=0;i<length;i++) for(j=0;j<width;j++)
        if (mat[i][j]<min) min=mat[i][j];

    /*Recherche du max*/
    max=mat[0][0];
    for(i=0;i<length;i++) for(j=0;j<width;j++) 
        if (mat[i][j]>max) max=mat[i][j];

    /* Calcule le facteur de calibation */
    float c = GREY_LEVEL / log10(max - min + 1);

    /*Recalibre la matrice*/
    for(i=0;i<length;i++) for(j=0;j<width;j++)
        mat[i][j] = log10(mat[i][j] + 1);

    return c;
}

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
{
    int i,j,k;
    int length,width;
    float** MatriceImgR;
    float** MatriceImgI;
    float** MatriceImgM;
    char filename[255];

    printf("Veuillez entrer le nom de l'image a charger: ");
    scanf("%s", filename);
    
    /*Allocation memoire pour la matrice image*/
    MatriceImgR=LoadImagePgm(filename,&length,&width);
    MatriceImgI=fmatrix_allocate_2d(length,width);

    // Application de la fonction logarithmique
    RecalLog(MatriceImgR, length, width);
    Recal(MatriceImgR, length, width);
  
    /*Sauvegarde de MatriceImgR sous forme d'image pgm*/
    SaveImagePgm(NAME_IMG_OUT,MatriceImgR,length,width);

    /*Liberation memoire pour les matrices*/
    free_fmatrix_2d(MatriceImgR);
    free_fmatrix_2d(MatriceImgI);

    /*retour sans probleme*/ 
    printf("\n C'est fini ... \n\n\n");
    return 0;
}
