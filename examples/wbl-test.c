/**
 * testconsole.cpp
 * Copyright 2011, Securics Inc.
   See accompanying LICENSE agrement for details on rights.

Parts of this technology are subject to SBIR data rights and as described in DFARS 252.227-7018 (June 1995) SBIR Data Rights which apply to Contract Number: N00014-11-C-0243 and STTR N00014-07-M-0421 to Securics Inc, 1870 Austin Bluffs Parkway, Colorado Springs, CO 80918

The Government's rights to use, modify, reproduce, release, perform, display, or disclose technical data or computer software marked with this legend are restricted during the period shown as provided in paragraph (b)(4) of the Rights in Noncommercial Technical Data and Computer Software-Small Business Innovative Research (SBIR) Program clause contained in the above identified contract.  Expiration of SBIR Data Rights: Expires four years after completion of the above cited project work for this or any other follow-on SBIR contract, whichever is later.

No restrictions on government use apply after the expiration date shown above.  Any reproduction of technical data, computer software, or portions thereof marked with this legend must also reproduce the markings.
 *
 */

// Example libmr usage 

/* 
takes no arguments, and if all is well it produces the following output:

Data Values values:
  0.745800 0.670480 0.650880 0.645070 0.645000 0.644020 0.642950 0.642790 0.642440 0.640790 
Weibull fit of non-match data, (i.e. ignoring top score)
parmhat: 0.652101 56.733058
parmci: 0.644125 0.660176 36.642715 87.838467
Weibull CDF values: 
  1.000000 0.992092 0.593083 0.417624 0.415688 0.389154 0.361439 0.357410 0.348705 0.309668 
Inverse Weibull values (expect inf for first one, rest should equal non-match data ):
       inf 0.670480 0.650880 0.645070 0.645000 0.644020 0.642950 0.642790 0.642440 0.640790 


*/

#include <stdio.h>
#include <stdlib.h>
#include "libMR/weibull.h"

#define ALPHA 0.05
#define FITTING_SIZE 9

double tail[] = {0.74580, 0.67048, 0.65088, 0.64507, 0.64500, 0.64402, 0.64295, 0.64279, 0.64244, 0.64079};
double tail_fit[] = {0.67048, 0.65088, 0.64507, 0.64500, 0.64402, 0.64295, 0.64279, 0.64244, 0.64079};

int main(int argc, char **argv)
{
   int i;


   printf("Data Values values:\n  ");
   for (i = 0; i < FITTING_SIZE + 1; i++)
   {
      printf("%f ", tail[i]);
   }
   printf("\n");

   // Example of Weibull_fit()
   double* parmhat = (double *)malloc(sizeof(double)* 2);
   double* parmci  = (double *)malloc(sizeof(double)* 4);
   weibull_fit(parmhat, parmci, tail_fit, ALPHA, FITTING_SIZE);

   printf("Weibull fit of non-match data, (i.e. ignoring top score)\n");
   printf("parmhat (shape/scale): %f %f\n", parmhat[0], parmhat[1]);
   printf("parmci (Shape/scale lower then upper bounds): %f %f %f %f\n", parmci[0], parmci[1], parmci[2], parmci[3]);

   // Example of Weibull_cdf()
   // Note: contents of tail are destroyed after call
   double prob_values[10];

   printf("Weibull CDF values: \n  ");
   for (i = 0; i < FITTING_SIZE + 1; i++)
   {
      double tempCdf = prob_values[i] =  weibull_cdf(tail[i], parmhat[0], parmhat[1]);
      printf("%f ", tempCdf);
   }
   printf("\n");

   // Example of Weibull_inv()
   printf("Inverse Weibull values (expect inf for first one, rest should equal non-match data ):\n       ");
   for (i = 0; i < FITTING_SIZE + 1; i++)
   {
     double tempInv = weibull_inv( prob_values[i], parmhat[0], parmhat[1]);
      printf("%f ", tempInv);
   }
   printf("\n");

   return 0;
}
