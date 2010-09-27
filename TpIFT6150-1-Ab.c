/*------------------------------------------------------*/
/* Prog    : TpIFT6150-1-Ab.c                           */
/* Auteur  : Sylvain Bouchard                           */
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

#define NAME_IMG_OUT "image-TpIFT6150-1-Ab"

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

    /*Pour visu*/
    Recal(MatriceImgM,length,width);
    Mult(MatriceImgM,100.0,length,width);                     
  
    /*Sauvegarde de MatriceImgM sous forme d'image pgm*/
    SaveImagePgm(NAME_IMG_OUT,MatriceImgM,length,width);

    /*Liberation memoire pour les matrices*/
    free_fmatrix_2d(MatriceImgR);
    free_fmatrix_2d(MatriceImgI); 
    free_fmatrix_2d(MatriceImgM);    

    /*Commande systeme: visualisation de Ingout.pgm*/
    system("display image-TpIFT6150-1-Ab.pgm&");

    /*retour sans probleme*/ 
    printf("\n C'est fini ... \n\n\n");
    return 0; 	 
}
