/*------------------------------------------------------*/
/* Prog    : TpIFT6150-1-Ae.c                           */
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
#define NAME_IMG_IN  "image-TpIFT6150-1-Ae-img"

#define NAME_IMG_OUT "image-TpIFT6150-1-Af-img"

#define NAME_SPC_OUT_1 "image-TpIFT6150-1-Af-spc-1"
#define NAME_SPC_OUT_2 "image-TpIFT6150-1-Af-spc-2"

void phase(float** matP,float** matR,float** matI,int length,int width)
{
    int i,j;

    /* Calcul de la phase */
    for(i=0;i<length;i++)
    {
        for(j=0;j<width;j++)
        {
            matP[i][j] = atan(matI[i][j]/matR[i][j]);
        }
    }
}

float** translate(float** image, int length, int width, int delta, int dir)
{
    int i, j;

    float** translated = fmatrix_allocate_2d(length,width);
    for(i = 0; i < length; i++)
    {
        for(j = 0; j < width; j++)
        {
            if(dir == 0)
            {
                // Translation en X
                if(j < delta)
                    translated[i][j] = 0.0f;
                else
                    translated[i][j] = image[i][j-delta];
            }
            else
            {
                // Translation en Y
                if(i < delta)
                    translated[i][j] = 0.0f;
                else
                    translated[i][j] = image[i-delta][j];
            }
        }
    }
    return translated;
}

void translateSpectrum(float** mR, int length, int width)
{
    int i,j,factor;
    for(i = 0; i < length; i++)
    {
        for(j = 0; j < width; j++)
        {
            factor = pow(-1, i+j);
            mR[i][j] *= factor;
        }
    }
}

void computeModuleAndPhase(float** mR, float** mI, float** mM, float** mP, int length, int width)
{
    // FFT
    FFTDD(mR,mI,length,width);

    // Module
    Mod(mM,mR,mI,length,width);

    // Phase
    phase(mP,mR,mI,length,width);

    // Pour visu
    Recal(mM,length,width);
    Mult(mM,100.0,length,width);
}

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
{
    int i,j,k;
    int length,width;
    float** MatriceImgR1;
    float** MatriceImgR2;

    float** MatriceImgI1;
    float** MatriceImgI2;

    float** MatriceImgM1;
    float** MatriceImgP1;
    float** MatriceImgM2;
    float** MatriceImgP2;

    // Chargement de l'image carree
    MatriceImgR1 = LoadImagePgm(NAME_IMG_IN,&length,&width);

    // Translation de l'image en X
    MatriceImgR2 = translate(MatriceImgR1, length, width, 4, 0);

    // Sauvegarde de MatriceImgR2 sous forme d'image pgm
    SaveImagePgm(NAME_IMG_OUT,MatriceImgR2,length,width);

    // Allocation memoire pour la FFT
    MatriceImgI1=fmatrix_allocate_2d(length,width);
    MatriceImgI2=fmatrix_allocate_2d(length,width);
    MatriceImgM1=fmatrix_allocate_2d(length,width);
    MatriceImgP1=fmatrix_allocate_2d(length,width);
    MatriceImgM2=fmatrix_allocate_2d(length,width);
    MatriceImgP2=fmatrix_allocate_2d(length,width);

    // Initialisation a zero de toutes les matrices
    for(i=0;i<length;i++) 
    {
        for(j=0;j<width;j++) 
        {
	        MatriceImgI1[i][j]=0.0;
	        MatriceImgI2[i][j]=0.0;
	        MatriceImgM1[i][j]=0.0;
	        MatriceImgP1[i][j]=0.0;
	        MatriceImgM2[i][j]=0.0;
	        MatriceImgP2[i][j]=0.0;
        }
    }

    // Decalage des images pour obtenir un spectre au centre
    translateSpectrum(MatriceImgR1, length, width);
    translateSpectrum(MatriceImgR2, length, width);
  
    // FFT de l'image non decalee
    computeModuleAndPhase(MatriceImgR1, MatriceImgI1, MatriceImgM1, MatriceImgP1, length, width);

    // FFT de l'image decalee
    computeModuleAndPhase(MatriceImgR2, MatriceImgI2, MatriceImgM2, MatriceImgP2, length, width);

    // Sauvegarde des spectres
    SaveImagePgm(NAME_SPC_OUT_1,MatriceImgM1,length,width);
    SaveImagePgm(NAME_SPC_OUT_2,MatriceImgM2,length,width);
  
    // Impression du changement de phase pour une harmonique
    for(i = 1; i <= 2; i++)
    {
        printf("Harmonique %d,%d:\n", i, i);
        printf("1) Module: %.2f <--> %.2f\n", MatriceImgM1[i][i], MatriceImgM2[i][i]);
        printf("2) Phase: %.2f <--> %.2f\n", MatriceImgP1[i][i], MatriceImgP2[i][i]);
    }

    // Liberation memoire pour les matrices
    free_fmatrix_2d(MatriceImgR1);
    free_fmatrix_2d(MatriceImgR2);
    free_fmatrix_2d(MatriceImgI1); 
    free_fmatrix_2d(MatriceImgI2); 
    free_fmatrix_2d(MatriceImgM1);
    free_fmatrix_2d(MatriceImgP1);
    free_fmatrix_2d(MatriceImgM2);
    free_fmatrix_2d(MatriceImgP2);

    // Retour sans probleme
    printf("\n C'est fini ... \n\n\n");
    return 0; 	 
}
