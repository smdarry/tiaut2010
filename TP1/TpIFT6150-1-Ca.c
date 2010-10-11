/*------------------------------------------------------*/
/* Prog    : TpIFT6150-1-Ca.c                           */
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
#define NAME_IMG_OUT_SQUARE "image-TpIFT6150-1-Ca_square"
#define NAME_IMG_OUT_CONV "image-TpIFT6150-1-Ca_convolution"
#define NAME_IMG_OUT_SPECTRE "image-TpIFT6150-1-Ca_spectre"

/**
 * Cree un masque de convolution avec le reste de l'image remplie avec des 0.
 */
float** convMasque(int length, int width, int size)
{
    int i, j;
    int startX = 0;
    int endX = startX + size;

    int startY = 0;
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

void RecalLog(float** mat, int length, int width)
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

    /*Recalibre la matrice*/
    for(i=0;i<length;i++) for(j=0;j<width;j++)
        mat[i][j] = log10(fabs(mat[i][j]) + 1);
}

void shiftSpatial(float** MatriceR,int length,int width)
{
    int factor, i, j;
    for(i = 0; i < length; i++)
    {
        for(j = 0; j < width; j++)
        {
            factor = pow(-1, i+j);
            MatriceR[i][j] *= factor;
        }
    }
}

void shift(float** MatriceInR,float** MatriceInI,int length,int width,int shiftLength,int shiftWidth)
{
    float** MatriceShiftR = fmatrix_allocate_2d(length,width);
    float** MatriceShiftI = fmatrix_allocate_2d(length,width);
    float** MatriceTmpR = fmatrix_allocate_2d(length,width);
    float** MatriceTmpI = fmatrix_allocate_2d(length,width);

    // Calcul de la matrice de decalage
    double theta;
    int u,v;
    for(u = 0; u < length; u++)
    {
        for(v = 0; v < width; v++)
        {
            theta = 2 * PI;
            
            double u1 = (u * shiftLength) / (double)length;
            double v1 = (v * shiftWidth) / (double)width;

            theta *= (u1 + v1);

            MatriceShiftR[u][v] = cos(theta);
            MatriceShiftI[u][v] = -sin(theta); 
        }
    }

    // Application du facteur de decalage
    MultMatrix(MatriceTmpR,MatriceTmpI,MatriceInR,MatriceInI,MatriceShiftR,MatriceShiftI,length,width);

    // Copie de la matrice resultante dans l'input
    int i,j;
    for(i=0;i<length;i++)
    {
        for(j=0;j<width;j++)
        {
            MatriceInR[i][j] = MatriceTmpR[i][j];
            MatriceInI[i][j] = MatriceTmpI[i][j];
        }
    }

    free_fmatrix_2d(MatriceShiftR);
    free_fmatrix_2d(MatriceShiftI);
    free_fmatrix_2d(MatriceTmpR);
    free_fmatrix_2d(MatriceTmpI);
}

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
{
    int i,j,k;
    int length,width,tailleMasque;
    float** MatriceImgR;
    float** MatriceImgI;
    float** MatriceImgM;
    float** MatriceConvR;
    float** MatriceConvI;

    float** MatriceImgR2;
    float** MatriceImgI2;

    // Generation d'un masque de convolution
    printf("Veuillez entrer la taille du masque: ");
    scanf("%d", &tailleMasque);

    length = 128;
    width = 128;

    MatriceImgR = convMasque(length, width, tailleMasque);
    MatriceImgI = fmatrix_allocate_2d(length,width);

    MatriceImgR2 = convMasque(length, width, tailleMasque);
    MatriceImgI2 = fmatrix_allocate_2d(length,width);

    // Sauvegarde de l'image du carre
    SaveImagePgm(NAME_IMG_OUT_SQUARE,MatriceImgR2,length,width);

    // Allocation memoire pour la FFT
    MatriceImgM=fmatrix_allocate_2d(length,width);
    MatriceConvR=fmatrix_allocate_2d(length,width);
    MatriceConvI=fmatrix_allocate_2d(length,width);

    // Initialisation a zero des parties imaginaires
    for(i=0;i<length;i++) 
    {
        for(j=0;j<width;j++) 
        {
	        MatriceImgI[i][j]=0.0;
	        MatriceImgI2[i][j]=0.0;
        }
    }

    // Decalage de l'image de N/2
    //shiftSpatial(MatriceImgR,length,width);

    // FFT
    FFTDD(MatriceImgR,MatriceImgI,length,width);

    // Module
    Mod(MatriceImgM,MatriceImgR,MatriceImgI,length,width);

    RecalLog(MatriceImgM,length,width);
    Recal(MatriceImgM,length,width);

    // Sauvegarde du spectre
    SaveImagePgm(NAME_IMG_OUT_SPECTRE,MatriceImgM,length,width);

    // FFT du masque
    FFTDD(MatriceImgR2,MatriceImgI2,length,width);

    // Multiplication complexe
    MultMatrix(MatriceConvR, MatriceConvI, MatriceImgR, MatriceImgI, MatriceImgR2, MatriceImgI2, length, width);

    // Decalage du spectre de la moitiee de la taille du masque
    //shift(MatriceConvR,MatriceConvI,length,width,tailleMasque/2,tailleMasque/2);

    IFFTDD(MatriceConvR,MatriceConvI,length,width);

    // Changement de dynamique
    RecalLog(MatriceConvR,length,width);
    Recal(MatriceConvR,length,width);

    // Sauvegarde de la convolution
    SaveImagePgm(NAME_IMG_OUT_CONV,MatriceConvR,length,width);

    // Liberation memoire pour les matrices
    free_fmatrix_2d(MatriceImgR);
    free_fmatrix_2d(MatriceImgI); 
    free_fmatrix_2d(MatriceImgM);
    free_fmatrix_2d(MatriceConvR);
    free_fmatrix_2d(MatriceConvI);

    // Retour sans probleme
    printf("\n C'est fini ... \n\n\n");
    return 0;
}
