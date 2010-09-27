/*------------------------------------------------------*/
/* Prog    : TpIFT6150-1-Ae.c                           */
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

#define NAME_IMG_OUT "image-TpIFT6150-1-Ae-img"
#define NAME_SPC_OUT "image-TpIFT6150-1-Ae-spc"

float** squareImage(int length, int width, int size)
{
    int i, j;
    int startX = 0.5 * (width - size);
    int endX = startX + size;

    int startY = 0.5 * (length - size);
    int endY = startY + size;

    float** square = fmatrix_allocate_2d(length, width);
    for(i = 0; i < length; i++)
    {
        for(j = 0; j < width; j++)
        {
            if(i >= startY && i < endY
            && (j >= startX && j < endX))
            {
                square[i][j] = 255.0f;
            }
            else
                square[i][j] = 0.0f;
        }
    }
    return square;
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

    // Generation d'une image carre blanc sur fond noir
    length = 128;
    width = 128;
    MatriceImgR = squareImage(length, width, 16);

    // Sauvegarde de MatriceImgR sous forme d'image pgm
    SaveImagePgm(NAME_IMG_OUT,MatriceImgR,length,width);

    // Allocation memoire pour la FFT
    MatriceImgI=fmatrix_allocate_2d(length,width);
    MatriceImgM=fmatrix_allocate_2d(length,width);

    // Initialisation a zero de toutes les matrices
    for(i=0;i<length;i++) 
    {
        for(j=0;j<width;j++) 
        {
	        MatriceImgI[i][j]=0.0;
	        MatriceImgM[i][j]=0.0;
        }
    }

    // Decalage de l'image pour obtenir un spectre au centre
    int factor;
    for(i = 0; i < length; i++)
    {
        for(j = 0; j < width; j++)
        {
            factor = pow(-1, i+j);
            MatriceImgR[i][j] *= factor;
        }
    }
  
    // FFT
    FFTDD(MatriceImgR,MatriceImgI,length,width);

    // Module
    Mod(MatriceImgM,MatriceImgR,MatriceImgI,length,width);

    // Pour visu
    Recal(MatriceImgM,length,width);
    Mult(MatriceImgM,100.0,length,width);                     
  
    // Sauvegarde de MatriceImgM sous forme d'image pgm
    SaveImagePgm(NAME_SPC_OUT,MatriceImgM,length,width);

    // Liberation memoire pour les matrices
    free_fmatrix_2d(MatriceImgR);
    free_fmatrix_2d(MatriceImgI); 
    free_fmatrix_2d(MatriceImgM);

    // Commande systeme: visualisation de Ingout.pgm
    system("display image-TpIFT6150-1-Ae-spc.pgm&");

    // Retour sans probleme
    printf("\n C'est fini ... \n\n\n");
    return 0; 	 
}
