/*------------------------------------------------------*/
/* Prog    : TpIFT6150-1-Ba.c                           */
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
#define NAME_IMG_IN  "images/D16r"

#define NAME_IMG_OUT "image-TpIFT6150-1-Ba"

void clearZones(float** imR, float** imI, int length, int width, int dist)
{
    int i,j;
    int N_2 = length / 2;
    int M_2 = width / 2;

    // Clear la partie gauche des images
    for(j = 0; j < (M_2 - dist); j++)
    {
        
    }

    // Clear la partie droite des images
    for(j = (M_2 + dist); j < width; j++)
    {
    }

    // Clear le haut
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

    clearZone(MatriceImgR, MatriceImgI, length, width, 3);

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
    system("display image-TpIFT6150-1-Ba.pgm&");

    /*retour sans probleme*/ 
    printf("\n C'est fini ... \n\n\n");
    return 0; 	 
}
