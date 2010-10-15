/*------------------------------------------------------*/
/* Prog    : TpIFT6150-1-D.c                            */
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

#define NAME_IMG_OUT_A "image-TpIFT6150-1-Da"
#define NAME_IMG_OUT_B "image-TpIFT6150-1-Db"

void genererImage(float** imR, int length, int width)
{
    int x, y;
    for(x = 0; x < length; x++)
    {
        for(y = 0; y < width; y++)
        {
            imR[x][y] = 128 + 127*cos((2*PI*(25*x + 31*y))/128);
        }
    }
}

void genererImage2(float** imR, int length, int width)
{
    int x, y;
    for(x = 0; x < length; x++)
    {
        for(y = 0; y < width; y++)
        {
            imR[x][y] = 128 + 127*cos((2*PI*(25*4*x + 31*4*y))/128);
        }
    }
}

void genererSinus(float** imR, int length, int width)
{
    int x, y;
    for(x = 0; x < length; x++)
    {
        for(y = 0; y < width; y++)
        {
            imR[x][y] = 128 + 127*cos((2*PI*2*x)/128);
        }
    }
}

void echantillonne(float** imR,int length,int width,int interval,float** outImR)
{
    int x, y;
    for(x = 0; x < length; x += interval)
    {
        for(y = 0; y < width; y += interval)
        {
            outImR[x/interval][y/interval] = imR[x][y];
        }
    }
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

    // Taille d'image predeterminee
    length = 128, width = 128;

    // Allocation memoire pour la matrice image
    MatriceImgR=fmatrix_allocate_2d(length,width);
    MatriceImgI=fmatrix_allocate_2d(length,width);
    MatriceImgM=fmatrix_allocate_2d(length,width);

    /* Generation d'une image */
    //genererImage(MatriceImgR, length, width);
    genererSinus(MatriceImgR, length, width);

    // Initialisation a zero de toutes les matrices
    for(i=0;i<length;i++) 
    {
        for(j=0;j<width;j++) 
        {
	        MatriceImgI[i][j]=0.0;
	        MatriceImgM[i][j]=0.0;
        }
    }

    SaveImagePgm(NAME_IMG_OUT_A,MatriceImgR,length,width);

    // Sous-echantillonnage de l'image
    int interval = 4;
    int sLength = length / interval, sWidth = width / interval;
    float** SousMatriceImgR = fmatrix_allocate_2d(sLength,sWidth);
    echantillonne(MatriceImgR,length,width,interval,SousMatriceImgR);

    // Sauve l'image echantillonnee
    SaveImagePgm(NAME_IMG_OUT_B,SousMatriceImgR,sLength,sWidth);

/*
    // FFT
    FFTDD(MatriceImgR,MatriceImgI,length,width);

    // Recalage de l'image
    RecalLineaire(MatriceImgR,length,width);
    //Mult(MatriceImgR,1.0,length,width);

    // Sauvegarde de MatriceImgR sous forme d'image pgm
    SaveImagePgm(NAME_IMG_OUT_Z0,MatriceImgR,length,width);

    // Liberation memoire pour les matrices
    free_fmatrix_2d(MatriceImgR);
    free_fmatrix_2d(MatriceImgI);
    free_fmatrix_2d(MatriceImgM);

*/
    // Retour sans probleme
    printf("\n C'est fini ... \n\n\n");
    return 0;
}
