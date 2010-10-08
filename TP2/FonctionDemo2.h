/*---------------------------------------------------*/
/* module  : FonctionDemo.h                          */
/* auteur  : Max Mignotte                            */
/* revision: Francois Destrempes                     */
/* date    : 21/09/99--01/09/04                      */              
/* langage : C                                       */
/* labo    : DIRO                                    */
/*---------------------------------------------------*/

#ifndef FONCTIONDEMO_H
#define FONCTIONDEMO_H

/*------------------------------------------------*/
/* FICHIERS INCLUS -------------------------------*/
/*------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/
/*------------------------------------------------*/
#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
#define SQUARE(X) ((X)*(X))
#define MAX(i,j)  ((i)>(j)?(i):(j))
#define MIN(i,j)  ((i)>(j)?(i):(j))

#define NBCHAR 200

#define FFT   1
#define IFFT -1
#define FFT2D 2

#define GREY_LEVEL 255
#define PI 3.141592654

#define WHITE 255
#define BLACK 0

/*------------------------------------------------*/
/* PROTOTYPES DES FONCTIONS  ---------------------*/
/*------------------------------------------------*/
float*  fmatrix_allocate_1d(int);
float** fmatrix_allocate_2d(int,int);
void    free_fmatrix_1d(float*);
void    free_fmatrix_2d(float**);
float** LoadImagePgm(char*,int*,int*);
void    SaveImagePgm(char*,float**,int,int);

void    fourn(float*,unsigned long*,int,int);
void    FFTDD(float**,float**,int,int);
void    IFFTDD(float**,float**,int,int);
void    Mod(float**,float**,float**,int,int);
void    Mult(float**,float,int,int);
void    Recal(float**,int,int);
void    MultMatrix(float**,float**,float**,float**,float**,float**,int,int);
void    SquareMatrix(float**,float**,float**,float**,int,int);
float   funcgauss2D(float x,float y,float var);
void    compute_histo(float** mat,int lgth,int wdth,float* hist);

#endif
