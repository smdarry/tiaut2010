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

#define NAME_IMG_OUT_MASK "image-TpIFT6150-1-Cb_mask"
#define NAME_IMG_OUT_FILTER "image-TpIFT6150-1-Cb_filtered"

typedef struct _ImageComplexe
{
    float** reel;
    float** imag;

    int length;
    int width;
} ImageComplexe;

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

float RecalLog(float** mat, int length, int width)
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

    /* Calcule le facteur de calibation */
    float c = GREY_LEVEL / log(max - min + 1);

    /*Recalibre la matrice*/
    for(i=0;i<length;i++) for(j=0;j<width;j++)
        mat[i][j] = log(mat[i][j] + 1);

    return c;
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

    float** masqueR;
    float** masqueI;
    int masqueLength, masqueWidth;

    // Generation d'un masque de convolution
    printf("Veuillez entrer la taille du masque: ");
    scanf("%d", &tailleMasque);

    MatriceImgR=LoadImagePgm(NAME_IMG_IN,&length,&width);
    masqueR = convMasque(length, width, tailleMasque);
    masqueI = fmatrix_allocate_2d(length,width);

    masqueLength = length;
    masqueWidth = width;

    // Sauvegarde du masque
    SaveImagePgm(NAME_IMG_OUT_MASK,masqueR,masqueLength,masqueWidth);

    // Allocation memoire pour la FFT
    MatriceImgI=fmatrix_allocate_2d(length,width);
    MatriceImgM=fmatrix_allocate_2d(length,width);
    MatriceConvR=fmatrix_allocate_2d(length,width);
    MatriceConvI=fmatrix_allocate_2d(length,width);

    // Initialisation a zero de toutes les matrices
    for(i=0;i<length;i++) 
    {
        for(j=0;j<width;j++) 
        {
	        MatriceImgI[i][j]=0.0;
	        MatriceImgM[i][j]=0.0;
        }
    }

    // Initialisation de la partie imaginaire du masque
    for(i=0;i<masqueLength;i++) 
    {
        for(j=0;j<masqueWidth;j++) 
        {
	        masqueI[i][j]=0.0;
        }
    }

    // FFT
    FFTDD(MatriceImgR,MatriceImgI,length,width);

    // FFT du masque
    FFTDD(masqueR,masqueI,masqueLength,masqueWidth);

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

    ImageComplexe masque;
    masque.reel = masqueR;
    masque.imag = masqueI;
    masque.length = length;
    masque.width = width;

    convolution(&img, &masque, &conv);

    // Changement de dynamique
    float c = RecalLog(MatriceConvR,length,width);
    Mult(MatriceConvR,c,length,width);                     

    // Sauvegarde de la convolution
    SaveImagePgm(NAME_IMG_OUT_FILTER,MatriceConvR,length,width);

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
