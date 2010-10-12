/*------------------------------------------------------*/
/* Prog    : TpIFT6150-2.c                              */
/* Auteur  : Sylvain Bouchard                           */
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

float** gaussianMask(int k, float sigma)
{
    int length = 2 * k + 1;
    int width = length;

    float** gauss = fmatrix_allocate_2d(length,width);
    int i,j;
    for(i = -k; i <= k; i++)
    {
        for(j = -k; j <= k; j++)
        {
            gauss[i+k][j+k] = funcgauss2D(i, j, sigma);
        }
    }
    return gauss;
}

float** padWithZeros(float** mask, int mLength, int mWidth, int length, int width)
{
    float** padded = fmatrix_allocate_2d(length,width);
    int i,j;
    for(i = 0; i < length; i++)
    {
        for(j = 0; j < width; j++)
        {
            if(i < mLength && j < mWidth)
                padded[i][j] = mask[i][j];
            else
                padded[i][j] = 0.0;
        }
    }
    return padded;
}

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
  
    // Chargement de l'image
    float** MatriceImgR=LoadImagePgm(NAME_IMG_IN,&length,&width);
    float** MatriceImgI=fmatrix_allocate_2d(length,width);
    float** gaussMaskI=fmatrix_allocate_2d(length,width);

    // Initialisation a zero de la partie imaginaire
    for(i=0;i<length;i++) 
    {
        for(j=0;j<width;j++) 
        {
	        MatriceImgI[i][j]=0.0;
	        gaussMaskI[i][j]=0.0;
        }
    }

    /* Implementer detecteur de Canny */
    int halfMaskWidth = 2;
    int maskSize = halfMaskWidth*2 + 1;

    // Etape 1: application d'un filtre Gaussien
    float** mask = gaussianMask(halfMaskWidth, sigma);
    float** gaussMaskR = padWithZeros(mask, maskSize, maskSize, length, width);

    // Convolution
    float** convR = fmatrix_allocate_2d(length,width);
    float** convI = fmatrix_allocate_2d(length,width);
    FFTDD(MatriceImgR,MatriceImgI,length,width);
    FFTDD(gaussMaskR,gaussMaskI,length,width);
    MultMatrix(convR,convI,MatriceImgR,MatriceImgI,gaussMaskR,gaussMaskI,length,width);
    IFFTDD(convR,convI,length,width);

    Recal(convR, length, width);

    // Sauvegarde de l'image filtree
    SaveImagePgm(NAME_IMG_CANNY,convR,length,width);

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
