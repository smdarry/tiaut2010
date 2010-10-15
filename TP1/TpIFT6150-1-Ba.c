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
#define NAME_IMG_OUT_BASSES "image-TpIFT6150-1-Ba_0_1"
#define NAME_IMG_OUT_MOYS "image-TpIFT6150-1-Ba_2_5"
#define NAME_IMG_OUT_HAUTES "image-TpIFT6150-1-Ba_6_15"
#define NAME_IMG_OUT_THAUTES "image-TpIFT6150-1-Ba_16_63"

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

void imageTronquee(float** imgR, float** imgI, int length, int width, int zoneStart, int zoneEnd, char* imgName)
{
    // FFT
    FFTDD(imgR,imgI,length,width);

    // Troncature des basses frequences
    int i;
    for(i = zoneStart; i <= zoneEnd; i++)
        clearCorners(imgR,imgI,length,width,i);

    // Transformee inverse du spectre tronque
    IFFTDD(imgR,imgI,length,width);

    // Recalage et sauvegarde de l'image
    Recal(imgR,length,width);
    SaveImagePgm(imgName,imgR,length,width);
}

/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
{
    int i,j,k;
    int length,width;

    // Image des basses frequences
    float** imgBassesR = LoadImagePgm(NAME_IMG_IN,&length,&width);
    float** imgBassesI = fmatrix_allocate_2d(length,width);

    // Image des frequences moyennes
    float** imgMoysR = LoadImagePgm(NAME_IMG_IN,&length,&width);
    float** imgMoysI = fmatrix_allocate_2d(length,width);

    // Image des frequences hautes
    float** imgHautesR = LoadImagePgm(NAME_IMG_IN,&length,&width);
    float** imgHautesI = fmatrix_allocate_2d(length,width);

    // Image des frequences tres hautes
    float** imgTresHautesR = LoadImagePgm(NAME_IMG_IN,&length,&width);
    float** imgTresHautesI = fmatrix_allocate_2d(length,width);

    /*Initialisation des parties imaginaires */
    for(i=0;i<length;i++) 
    {
        for(j=0;j<width;j++) 
        {
            imgBassesI[i][j] = 0.0;
            imgMoysI[i][j] = 0.0;
            imgHautesI[i][j] = 0.0;
            imgTresHautesI[i][j] = 0.0;
        }
    }

    // Troncature des basses frequences
    imageTronquee(imgBassesR,imgBassesI,length,width,0,1,NAME_IMG_OUT_BASSES);

    // Troncature des frequences moyennes
    imageTronquee(imgMoysR,imgMoysI,length,width,2,5,NAME_IMG_OUT_MOYS);

    // Troncature des frequences hautes 
    imageTronquee(imgMoysR,imgMoysI,length,width,6,15,NAME_IMG_OUT_HAUTES);

    // Troncature des frequences tres hautes 
    imageTronquee(imgMoysR,imgMoysI,length,width,16,63,NAME_IMG_OUT_THAUTES);

    /*Liberation memoire pour les matrices*/
    free_fmatrix_2d(imgBassesR);
    free_fmatrix_2d(imgBassesI);
    free_fmatrix_2d(imgMoysR);
    free_fmatrix_2d(imgMoysI);
    free_fmatrix_2d(imgHautesR);
    free_fmatrix_2d(imgHautesI);
    free_fmatrix_2d(imgTresHautesR);
    free_fmatrix_2d(imgTresHautesI);

    /*retour sans probleme*/ 
    printf("\n C'est fini ... \n\n\n");
    return 0; 	 
}
