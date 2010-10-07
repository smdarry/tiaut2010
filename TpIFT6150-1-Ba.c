/*------------------------------------------------------*/
/* Prog    : TpIFT6150-1-Ba.c                           */
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

#define NAME_IMG_OUT "image-TpIFT6150-1-Ba"
#define NAME_IMG_OUT_Z0 "image-TpIFT6150-1-Ba-z0"
#define NAME_IMG_OUT_Z1 "image-TpIFT6150-1-Ba-z1"
#define NAME_IMG_OUT_Z6 "image-TpIFT6150-1-Ba-z6"
#define NAME_IMG_OUT_Z16 "image-TpIFT6150-1-Ba-z16"

void RecalLineaire(float** mat,int lgth,int wdth)
{
 int i,j;
 float max,min;

 /*Initialisation*/
 min=mat[0][0];

 /*Recherche du min*/
  for(i=0;i<lgth;i++) for(j=0;j<wdth;j++)
    if (mat[i][j]<min) min=mat[i][j];

 /*plus min*/
   for(i=0;i<lgth;i++) for(j=0;j<wdth;j++)
    mat[i][j]-=min;

   max=mat[0][0];
 /*Recherche du max*/
  for(i=0;i<lgth;i++) for(j=0;j<wdth;j++) 
    if (mat[i][j]>max) max=mat[i][j];

  printf("Min = %.2f\n", min);
  printf("Max = %.2f\n", max);

 /*Recalibre la matrice*/
 for(i=0;i<lgth;i++) for(j=0;j<wdth;j++)
   mat[i][j]*=(GREY_LEVEL/(max-min));      
}

void clearCorners(float** imR, float** imI, int length, int width, int dist)
{
    int i,j;
    int N_2 = length / 2;
    int M_2 = width / 2;
    int k = dist + 1;

    if(dist == 0)
    {
        imR[0][0] = 0.0f;
        imI[0][0] = 0.0f;

        return;
    }

    // Coin superieur gauche
    for(j = 0; j <= dist; j++)
    {
        imR[dist][j] = 0.0f;
        imI[dist][j] = 0.0f;
    }
    for(i = 0; i <= dist; i++)
    {
        imR[i][dist] = 0.0f;
        imI[i][dist] = 0.0f;
    }

    // Coin superieur droit
    for(j = (width-dist); j < width; j++)
    {
        imR[dist][j] = 0.0f;
        imI[dist][j] = 0.0f;
    }
    for(i = 0; i <= dist; i++)
    {
        imR[i][width-dist] = 0.0f;
        imI[i][width-dist] = 0.0f;
    }

    // Coin inferieur droit
    for(j = (width-dist); j < width; j++)
    {
        imR[length-dist][j] = 0.0f;
        imI[length-dist][j] = 0.0f;
    }
    for(i = (length-dist); i < length; i++)
    {
        imR[i][width-dist] = 0.0f;
        imI[i][width-dist] = 0.0f;
    }

    // Coin inferieur gauche 
    for(j = 0; j <= dist; j++)
    {
        imR[length-dist][j] = 0.0f;
        imI[length-dist][j] = 0.0f;
    }
    for(i = (length-dist); i < length; i++)
    {
        imR[i][dist] = 0.0f;
        imI[i][dist] = 0.0f;
    }
}

void clearZones(float** imR, float** imI, int length, int width, int dist)
{
    int i,j;
    int N_2 = length / 2;
    int M_2 = width / 2;
    int k = dist + 1;

    // Clear la partie gauche des images
    for(i = 0; i < length; i++)
    {
        for(j = 0; j <= (M_2 - k); j++)
        {
            imR[i][j] = 0.0f;
            imI[i][j] = 0.0f;
        }
    }

    // Clear la partie droite des images
    for(i = 0; i < length; i++)
    {
        for(j = (M_2 + k); j < width; j++)
        {
            imR[i][j] = 0.0f;
            imI[i][j] = 0.0f;
        }
    }

    // Clear le haut
    for(i = 0; i <= (N_2 - k); i++)
    {
        for(j = (M_2 - k); j < (M_2 + k); j++)
        {
            imR[i][j] = 0.0f;
            imI[i][j] = 0.0f;
        }
    }

    // Clear le bas
    for(i = (N_2 + k); i < length; i++)
    {
        for(j = (M_2 - k); j < (M_2 + k); j++)
        {
            imR[i][j] = 0.0f;
            imI[i][j] = 0.0f;
        }
    }
}

void clearZone(float** imR, float** imI, int length, int width, int zone)
{
    // La frequence fondamentale (centre) est a [N/2][N/2]
    // TODO: les coins sont faits deux fois

    int N_2 = length / 2;
    int M_2 = width / 2;

    int i,j;

    // Reset des lignes
    for(j = -zone; j <= zone; j++)
    {
        imR[N_2-zone][M_2+j] = 0.0f;
        imI[N_2-zone][M_2+j] = 0.0f;

        imR[N_2+zone][M_2+j] = 0.0f;
        imI[N_2+zone][M_2+j] = 0.0f;
    }

    // Reset des colonnes
    for(i = -zone; i <= zone; i++)
    {
        imR[N_2+i][M_2-zone] = 0.0f;
        imI[N_2+i][M_2-zone] = 0.0f;

        imR[N_2+i][M_2+zone] = 0.0f;
        imI[N_2+i][M_2+zone] = 0.0f;
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

    /*
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
    */
  
    /*FFT*/
    FFTDD(MatriceImgR,MatriceImgI,length,width);

    // Troncature des basses frequences
/*
    for(i = 0; i <= 1; i++)
        clearCorners(MatriceImgR,MatriceImgI,length,width,i);
*/
/*
    // Troncature des moyennes frequences
    for(i = 2; i <= 5; i++)
        clearCorners(MatriceImgR,MatriceImgI,length,width,i);
*/
/*
    // Troncature des hautes frequences
    for(i = 6; i <= 15; i++)
        clearCorners(MatriceImgR,MatriceImgI,length,width,i);
*/
/*
    // Troncature des tres hautes frequences
    for(i = 16; i <= 63; i++)
        clearCorners(MatriceImgR,MatriceImgI,length,width,i);
*/

    /*Module*/
    //Mod(MatriceImgM,MatriceImgR,MatriceImgI,length,width);

    /* Transformee inverse du spectre tronque */
    IFFTDD(MatriceImgR,MatriceImgI,length,width);

    /* Recalage de l'image */
    Recal(MatriceImgR,length,width);
    //Mult(MatriceImgR,1.0,length,width); 

    /*Sauvegarde de MatriceImgM sous forme d'image pgm*/
    SaveImagePgm(NAME_IMG_OUT,MatriceImgR,length,width);

    /*Liberation memoire pour les matrices*/
    free_fmatrix_2d(MatriceImgR);
    free_fmatrix_2d(MatriceImgI); 
    free_fmatrix_2d(MatriceImgM);

    /*retour sans probleme*/ 
    printf("\n C'est fini ... \n\n\n");
    return 0; 	 
}
