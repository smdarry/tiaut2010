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
#define NAME_IMG_IN  "images/D1r"

#define NAME_IMG_OUT_SQUARE "image-TpIFT6150-1-Ca_square"
#define NAME_IMG_OUT_CONV "image-TpIFT6150-1-Ca_convolution"

typedef struct _ImageComplexe
{
    float** reel;
    float** imag;

    int length;
    int width;
} ImageComplexe;

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

float** squareCornerImage(int length, int width, int size)
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

void convolution(ImageComplexe* img, ImageComplexe* masque, ImageComplexe* conv)
{
    int u,v,i_r,i_i,m_r,m_i;
    for(u = 0; u < img->length; u++)
    {
        for(v= 0; v < img->width; v++)
        {
            i_r = img->reel[u][v];
            i_i = img->imag[u][v];

            m_r = masque->reel[u][v];
            m_i = masque->imag[u][v];

            conv->reel[u][v] = (i_r * m_r) - (i_i * m_i);
            conv->imag[u][v] = (i_i * m_r) + (i_r * m_i);
        }
    }

    // FFT inverse
    IFFTDD(conv->reel, conv->imag, conv->length, conv->width);
}

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
{
    int i,j,k;
    int length,width,tailleCarre;
    float** MatriceImgR;
    float** MatriceImgI;
    float** MatriceImgM;
    float** MatriceConvR;
    float** MatriceConvI;

    float** masqueR;
    float** masqueI;

    // Generation d'une image carre blanc sur fond noir
    length = 128;
    width = 128;

    printf("Veuillez entrer la taille du carre: ");
    scanf("%d", &tailleCarre);

    MatriceImgR = squareImage(length, width, tailleCarre);

    // Sauvegarde de MatriceImgR sous forme d'image pgm
    SaveImagePgm(NAME_IMG_OUT_SQUARE,MatriceImgR,length,width);

    // Allocation memoire pour la FFT
    MatriceImgI=fmatrix_allocate_2d(length,width);
    MatriceImgM=fmatrix_allocate_2d(length,width);
    MatriceConvR=fmatrix_allocate_2d(length,width);
    MatriceConvI=fmatrix_allocate_2d(length,width);

    masqueR = squareImage(length, width, tailleCarre);
    masqueI = fmatrix_allocate_2d(length,width);

    // Initialisation a zero de toutes les matrices
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

    // FFT
    FFTDD(MatriceImgR,MatriceImgI,length,width);

    // Multiplication complexe de la transformee avec elle-meme
    ImageComplexe img, conv;
    img.reel = MatriceImgR;
    img.imag = MatriceImgI;
    img.length = length;
    img.width = width;

    conv.reel = MatriceConvR;
    conv.imag = MatriceConvI;
    conv.length = length;
    conv.width = width;

    convolution(&img, &img, &conv);

    // Changement de dynamique
    Recal(MatriceConvR,length,width);
    //Mult(MatriceConvR,100.0,length,width);                     

    // Sauvegarde de MatriceImgM sous forme d'image pgm
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
