/**
 * testconsole.cpp
 * Copyright 2011, Securics Inc.
   See accompanying LICENSE agrement for details on rights.

Parts of this technology are subject to SBIR data rights and as described in DFARS 252.227-7018 (June 1995) SBIR Data Rights which apply to Contract Number: N00014-11-C-0243 and STTR N00014-07-M-0421 to Securics Inc, 1870 Austin Bluffs Parkway, Colorado Springs, CO 80918

The Government's rights to use, modify, reproduce, release, perform, display, or disclose technical data or computer software marked with this legend are restricted during the period shown as provided in paragraph (b)(4) of the Rights in Noncommercial Technical Data and Computer Software-Small Business Innovative Research (SBIR) Program clause contained in the above identified contract.  Expiration of SBIR Data Rights: Expires four years after completion of the above cited project work for this or any other follow-on SBIR contract, whichever is later.

No restrictions on government use apply after the expiration date shown above.  Any reproduction of technical data, computer software, or portions thereof marked with this legend must also reproduce the markings.
 *
 */

//Weibull/MetaRecogntion test console
//Authors Vijay Iyer, Tboult.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "MetaRecognition.h"


int verbose=0;

/* defien some test data including expected answers */ 
double tail[] = {0.74580, 0.67048, 0.65088, 0.64507, 0.64500, 0.64402, 0.64295, 0.64279, 0.64244, 0.64079};
double tail_fit[] = {0.67048, 0.65088, 0.64507, 0.64500, 0.64402, 0.64295, 0.64279, 0.64244, 0.64079};

// data for testing with C-code and testing shifting invariance of the enhacned C++ version.
double tail_shifted[] = {100.10501, 100.02969, 100.01009,100.00428,100.00421,100.00323,100.00216,100.00200,100.00165,100}; 
double tail_fit_shifted[] = {100.02969, 100.01009,100.00428,100.00421,100.00323,100.00216,100.00200,100.00165,100}; 

// example where data "smaller" is better 
double tail_low[]  = {0.25419999999999998, .32952, .34912, .35493, .35500, .35598, .35705, .35721, .35756,  .35921 };

// example SVM with both postive and negative classes that work for "near zero" fitting
svm_node_libsvm svm_test[] = {{1,.32952}, {1,.34912}, {1,.35493}, {1,.35500}, {1,.35598}, {1,.35705}, {1,.35721}, {1,.35756},  {1, .35921},
                       {-1,-.32952}, {-1,-.34912}, {-1,-.35493}, {-1,-.35500}, {-1,-.35598}, {-1,-.35705}, {-1,-.35721}, {-1,-.35756}, {-1, -.35921}};

/* expected answers */ 
double W_answers[] = { 1.0000000000, 0.9921211956, 0.5899513281, 0.4171057966, 0.4152063721, 0.3891817521, 0.3620155874, 0.3580682635, 0.3495390670, 0.3113010327 };




void Test_C_Code()
{
	int i;
	// example of wblfit()
	double* parmhat = (double *)malloc(sizeof(double)* 2);
	double* parmci  = (double *)malloc(sizeof(double)* 4);
#define ALPHA 0.05
#define FITTING_SIZE 9
#define TAIL_SIZE 10

	wblfit(parmhat, parmci, tail_fit_shifted, ALPHA, FITTING_SIZE);

      if(verbose)	printf("parmhat: %f %f\n", parmhat[0], parmhat[1]);
      if(verbose)	printf("parmci: %f %f %f %f\n", parmci[0], parmci[1], parmci[2], parmci[3]);

      if(verbose)	printf("C-test of weibull cdf values:\n ");
	for (i = 0; i < TAIL_SIZE ; i++)
	{
		double tempcdf = wblcdf(tail_shifted[i], parmhat[0], parmhat[1]);
                if(fabs(tempcdf - W_answers[i]) > 1e-8) printf("WARNING C_CODE TEST fails %12.8f should be %12.8f\n", tempcdf, W_answers[i]);
                if(verbose)		printf("%12.10f ", tempcdf);
	}
      if(verbose)	printf("\n");

	// example of wblinv()
        //	printf("inverse weibull values: ");
        //	for (i = 0; i < FITTING_SIZE + 1; i++)
        //	{
        //		double tempinv = wblinv(tail_fit_shifted[i], parmhat[0], parmhat[1]);
        //		printf("%f ", tempinv);
        //	}
	//printf("\n");
        free(parmhat);
        free(parmci);

}

int main(int argc, char **argv)
{

  if(argc > 1) verbose=1;
  
  if(verbose) printf("Test C-code models\n");
	Test_C_Code();

        int length=sizeof(tail)/sizeof(double);
 
      if(verbose)	printf("Test class-based model ");
	MetaRecognition mr1;  /*  use all defaults, i.e. we drop the top item (to see if can predict recognition)  */
	mr1.FitHigh(tail_fit, length-1);
        mr1.Save((char*)"mr1.txt");
        double tempCdf;
        if(verbose)	printf("EVTHight weibull cdf values:\n ");
	for (int i = 0; i < length; i++)
	{
                bool predict = mr1.Predict_Match(tail[i]);  
                if(i==0 && !predict )  printf("WARNING FitHigh Predict TEST fails, should be positive for  %12.8f\n", tail[i] );                
                else   if(i> 0 && predict )  printf("WARNING FitHigh Predict TEST fails, should be positive for  %12.8f\n", tail[i] );                
		tempCdf = mr1.CDF(tail[i]);
                if(fabs(tempCdf - W_answers[i]) > 1e-8) printf("WARNING FitHigh TEST fails CDF %12.8f should be %12.8f\n", tempCdf, W_answers[i]);

                if(verbose)		printf("%12.10f ", tempCdf);
	}
        if(verbose) printf("\n");        

	MetaRecognition mr2(true); //  choose to ignore best score..we want to fit all data (default is ignore) */ 
	mr2.FitLow(tail_low, length);
        mr2.Save((char*)"mr2.txt");
	if(verbose) printf("EVTlow weibull cdf values:\n ");
	for (int i = 0; i < length; i++)
	{
		double tempCdf = mr2.CDF(tail_low[i]);
                if(fabs(tempCdf - W_answers[i]) > 1e-8) printf("WARNING FitLow TEST fails %12.8f should be %12.8f\n", tempCdf, W_answers[i]);
		if(verbose) printf("%12.10f ", tempCdf);
	} 
	if(verbose) printf("\n");

	mr2.FitHigh(tail_shifted, length);
	if(verbose) printf("EVTHigh of translated values:\n ");
	for (int i = 0; i < length; i++)
	{
		double tempCdf = mr2.CDF(tail_shifted[i]); 
                if(fabs(tempCdf - W_answers[i]) > 1e-8) printf("WARNING FitHigh Translated TEST fails %12.8f should be %12.8f\n", tempCdf, W_answers[i]);
		if(verbose) printf("%12.10f ", tempCdf);
	} 
	if(verbose) printf("\n");


        MetaRecognition mr3;
        mr3.Load((char*)"mr1.txt");
	if(verbose) printf("Loaded weibull cdf values:\n ");
	for (int i = 0; i < length; i++)
	{
		double tempCdf = mr3.W_score(tail[i]);
                if(fabs(tempCdf - W_answers[i]) > 1e-8) printf("WARNING Reloasd TEST fails %12.8f should be %12.8f\n", tempCdf, W_answers[i]);
		if(verbose) printf("%12.10f ", tempCdf);
	}
	if(verbose) printf("\n");

	if(verbose) printf("Fit SVM postive:\n ");
        mr1.FitSVM(svm_test,2*(length-1),1); // fit on positive class (label=1) doing rejection, so it estimates probabilty of being in negative class
        mr1.Save((char*)"mr1-svm.txt");
	for (int i = 0; i < length; i++)
	{
		double tempCdf = mr1.W_score(-tail_low[i]);
                if(fabs(tempCdf - W_answers[i]) > 1e-8) printf("WARNING SVM TEST 1 fails %12.8f should be %12.8f\n", tempCdf, W_answers[i]);
		if(verbose) printf("%12.10f ", tempCdf);
		tempCdf = mr1.CDF(tail[i]);
                if(tempCdf != 1.00 ) printf("WARNING SVM TEST 2 fails %12.8f should be %12.8f\n", tempCdf, 1.000);
	}
	if(verbose)  printf("\n");


	if(verbose) printf("Fit SVM postive without rejection:\n ");
        // fit on positive class (label=1) without rejection, so it directly estimates probabilty of being in positive class (in general a bad idea but with this data is okaY0. 
        mr3.FitSVM(svm_test,2*(length-1),1, false, false); 
        mr3.Save((char*)"mr3-svm.txt");
	for (int i = 0; i < length; i++)
	{
		double tempCdf = mr1.W_score(-tail_low[i]);
                if(fabs(tempCdf - W_answers[i]) > 1e-8) printf("WARNING SVM TEST 1p fails %12.8f should be %12.8f\n", tempCdf, W_answers[i]);
		if(verbose) printf("%12.10f ", tempCdf);
		tempCdf = mr1.CDF(tail[i]);
                if(tempCdf != 1.00 ) printf("WARNING SVM TEST 1pp fails %12.8f should be %12.8f\n", tempCdf, 1.000);

	}
	if(verbose)  printf("\n");


	if(verbose) printf("Fit SVM negative:\n ");

        // fit on negative class of SVM, (doing rejection), so it estimates probabilty of being in negative class. NOTE the -1 and added "false"
        mr2.FitSVM(svm_test,2*(length-1),-1,false);  
        mr2.Save((char*)"mr2-svm.txt");
	for (int i = 0; i < length; i++)
	{
		double tempCdf = mr2.W_score(tail_low[i]);
                if(fabs(tempCdf - W_answers[i]) > 1e-8) printf("WARNING SVM TEST 3 fails %12.8f should be %12.8f\n", tempCdf, W_answers[i]);
		if(verbose) printf("%12.10f ", tempCdf);
		tempCdf = mr2.CDF(-tail_low[i]);
                if(tempCdf != 1.00 ) printf("WARNING SVM TEST 4 fails %12.8f should be %12.8f\n", tempCdf, 1.000);
	}
	if(verbose)  printf("\n");

	printf("A clean test (without verbose) should print out only this message\n");
	return 0;
}
