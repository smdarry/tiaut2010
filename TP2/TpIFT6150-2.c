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

void gradient(float** image, float** gradX, float** gradY, int length, int width)
{
    int i,j;
    for(i = 0; i < length; i++)
    {
        for(j = 0; j < width; j++)
        {
            if(j == width-1)
                gradX[i][j] = 0;
            else
                gradX[i][j] = image[i][j+1] - image[i][j-1];

            if(i == length-1)
                gradY[i][j] = 0;
            else
                gradY[i][j] = image[i+1][j] - image[i-1][j];
        }
    }
}

void gradientMagnitude(float** gradX, float** gradY, float** magn, int length, int width)
{
    int i,j;
    for(i = 0; i < length; i++)
    {
        for(j = 0; j < width; j++)
        {
            magn[i][j] = sqrt(SQUARE(gradX[i][j]) + SQUARE(gradY[i][j]));
        }
    }
}

float quantize(float a)
{
    float qa;

    if(a > 180.0)
        a = a - 180.0;

    if((a >= 0 && a <= 22.5))
        qa = 0.0;
    else if(a > 22.5 && a <= 67.5)
        qa = 45.0;
    else if(a > 67.5 && a <= 112.5)
        qa = 90.0;
    else if(a > 112.5 && a <= 157.5)
        qa = 135.0;
    else if(a > 157.5 && a <= 180.0)
        qa = 0.0;

    return qa;
}

void gradientAngle(float** gradX, float** gradY, float** angle, float** angleApprox, int length, int width)
{
    float a, b;
    int i,j;
    for(i = 0; i < length; i++)
    {
        for(j = 0; j < width; j++)
        {
            b = angle[i][j] = atan2(-gradY[i][j], gradX[i][j]) * 180 / PI;
            if(angle[i][j] < 0)
                angle[i][j] = 180.0 + angle[i][j];

            // Quantisation de l'angle selon 4 directions [0,45,90,135]
            angleApprox[i][j] = quantize(angle[i][j]);
        }
    }
}

inline int isGreaterThan(float** imgIn, int i, int j, float angle)
{
    float courant = imgIn[i][j];
    float voisin1, voisin2;

    if(angle == 0.0)
    {
        voisin1 = imgIn[i][j+1];
        voisin2 = imgIn[i][j-1];
    }
    else if(angle == 45.0)
    {
        voisin1 = imgIn[i+1][j+1];
        voisin2 = imgIn[i-1][j-1];
    }
    else if(angle == 90.0)
    {
        voisin1 = imgIn[i+1][j];
        voisin2 = imgIn[i-1][j];
    }
    else if(angle == 135.0)
    {
        voisin1 = imgIn[i+1][j-1];
        voisin2 = imgIn[i-1][j+1];
    }

    if(courant > voisin1 && courant > voisin2)
        return 1;

    return 0;
}

void deleteNonMaximums(float** imgIn, float** contourAngles, int length, int width)
{
    float gradAngle;

    int i,j;
    for(i = 1; i < length-1; i++)
    {
        for(j = 1; j < width-1; j++)
        {
            gradAngle = quantize(contourAngles[i][j] + 90);
            if(!isGreaterThan(imgIn, i, j, gradAngle))
            {
                // Suppression du contour
                imgIn[i][j] = 0.0;
            }
        }
    }
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
    int halfMaskWidth = 16;
    int maskSize = halfMaskWidth*2 + 1;

    ////////////////////////////////////////////
    // Etape 1: application d'un filtre Gaussien
    //
    float** mask = gaussianMask(halfMaskWidth, sigma);
    float** gaussMaskR = padWithZeros(mask, maskSize, maskSize, length, width);

    // Convolution
    float** convR = fmatrix_allocate_2d(length,width);
    float** convI = fmatrix_allocate_2d(length,width);
    FFTDD(MatriceImgR,MatriceImgI,length,width);
    FFTDD(gaussMaskR,gaussMaskI,length,width);
    MultMatrix(convR,convI,MatriceImgR,MatriceImgI,gaussMaskR,gaussMaskI,length,width);
    IFFTDD(convR,convI,length,width);

    // Sauvegarde de l'image filtree
    SaveImagePgm(NAME_IMG_CANNY,convR,length,width);

    /////////////////////////////////////////////
    // Etape 2: calcul du gradient a chaque pixel
    //
    float** gradientX = fmatrix_allocate_2d(length,width);
    float** gradientY = fmatrix_allocate_2d(length,width);
    float** gradientMagn = fmatrix_allocate_2d(length,width);
    float** gradientAngles = fmatrix_allocate_2d(length,width);
    float** contourAngles = fmatrix_allocate_2d(length,width);

    gradient(convR, gradientX, gradientY, length, width);
    gradientMagnitude(gradientX, gradientY, gradientMagn, length, width);
    gradientAngle(gradientX, gradientY, gradientAngles, contourAngles, length, width);

    // Sauvegarde de l'image du gradient
    SaveImagePgm(NAME_IMG_GRADIENT,gradientMagn,length,width);

    // Suppression des non-maximums
    deleteNonMaximums(gradientMagn,contourAngles,length,width);

    // Sauvegarde de l'image des non-maximum supprimes
    SaveImagePgm(NAME_IMG_SUPPRESSION,gradientMagn,length,width);

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
