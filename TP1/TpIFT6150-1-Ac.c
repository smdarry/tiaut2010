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

    /*Allocation memoire pour la matrice image*/
    MatriceImgR=LoadImagePgm(NAME_IMG_IN,&length,&width);
    MatriceImgI=fmatrix_allocate_2d(length,width);
    MatriceImgM=fmatrix_allocate_2d(length,width);

    /*Initialisation a zero de toutes les matrices */
    for(i=0;i<length;i++) 
    {
        for(j=0;j<width;j++) 
        {
	        MatriceImgI[i][j]=0.0;
	        MatriceImgM[i][j]=0.0;
        }
    }

    // Decalage de l'image pour obtenir un spectre centre
    int factor;
    for(i = 0; i < length; i++)
    {
        for(j = 0; j < width; j++)
        {
            factor = pow(-1, i+j);
            MatriceImgR[i][j] *= factor;
        }
    }

    /*FFT*/
    FFTDD(MatriceImgR,MatriceImgI,length,width);

    /*Module*/
    Mod(MatriceImgM,MatriceImgR,MatriceImgI,length,width);

    // Application de la fonction logarithmique
    float c = RecalLog(MatriceImgM, length, width);
    Mult(MatriceImgM,c,length,width);                     
  
    /*Sauvegarde de MatriceImgM sous forme d'image pgm*/
    SaveImagePgm(NAME_IMG_OUT,MatriceImgM,length,width);

    /*Liberation memoire pour les matrices*/
    free_fmatrix_2d(MatriceImgR);
    free_fmatrix_2d(MatriceImgI); 
    free_fmatrix_2d(MatriceImgM);    

    /*Commande systeme: visualisation de Ingout.pgm*/
    system("display image-TpIFT6150-1-Ac.pgm&");

    /*retour sans probleme*/ 
    printf("\n C'est fini ... \n\n\n");
    return 0; 	 
}
