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
#include "libMR/MetaRecognition.h"


/* This is a basic test function for the user accessable function and to check precision is reasonable when run with no argument it produces 1 line of output
--> mr-test  
A clean test (without verbose) should print out only this message

if run with an argument it produece scores you an see.
--> mr-test verb
C-test of weibull cdf values:
 1.0000000000 0.9921211956 0.5899513281 0.4171057966 0.4152063721 0.3891817521 0.3620155874 0.3580682635 0.3495390670 0.3113010327 
Test class-based model EVTHight weibull cdf values:
 1.0000000000 0.9921211956 0.5899513281 0.4171057966 0.4152063721 0.3891817521 0.3620155874 0.3580682635 0.3495390670 0.3113010327 
EVTlow weibull cdf values:
 1.0000000000 0.9921211956 0.5899513281 0.4171057966 0.4152063721 0.3891817521 0.3620155874 0.3580682635 0.3495390670 0.3113010327 
EVTHigh of translated values:
 1.0000000000 0.9921211956 0.5899513281 0.4171057966 0.4152063721 0.3891817521 0.3620155874 0.3580682635 0.3495390670 0.3113010327 
ed weibull cdf values:
 1.0000000000 0.9921211956 0.5899513281 0.4171057966 0.4152063721 0.3891817521 0.3620155874 0.3580682635 0.3495390670 0.3113010327 
Fit SVM postive:
 1.0000000000 0.9921211956 0.5899513281 0.4171057966 0.4152063721 0.3891817521 0.3620155874 0.3580682635 0.3495390670 0.3113010327 
Fit SVM postive without rejection:
 1.0000000000 0.9921211956 0.5899513281 0.4171057966 0.4152063721 0.3891817521 0.3620155874 0.3580682635 0.3495390670 0.3113010327 
Fit SVM negative:
 1.0000000000 0.9921211956 0.5899513281 0.4171057966 0.4152063721 0.3891817521 0.3620155874 0.3580682635 0.3495390670 0.3113010327 
A clean test (without verbose) should print out only this message


In all cases it should also produce files (ignoring format)
mr1.txt:100.011420143645765   8637.05870009515638   100.057959588252601   99.9649023456916552   106793.395633022272   698.533673797915299  1 10.000000 9 100  0.640789999999999971 false
mr2.txt:100.011420143645765 8637.05870009515638 100.057959588252601 99.9649023456916552 106793.395633022272  698.533673797915299 -1 10.000000 9 100 -0.359209999999999974 true
mr1-svm.txt:100.011420143645765 8637.05870009515638 100.057959588252601 99.9649023456916552 106793.395633022272 698.533673797915299 1 10.000000 9 100 -0.359209999999999974 false
mr2-svm.txt:100.011420143645765 8637.05870009515638 100.057959588252601 99.9649023456916552 106793.395633022272 698.533673797915299 -1 10.000000 9 100 -0.359209999999999974 false
mr3-svm.txt:100.026471571177552 23948.2512984431596 100.042567031384564 100.010378700506649 737812.186093952972 777.323485655131776 1 10.000000 9 100 0.32951999999999998 false
*/



int verbose=0;

/* defien some test data including expected answers */ 
double tail[] = {0.74580, 0.67048, 0.65088, 0.64507, 0.64500, 0.64402, 0.64295, 0.64279, 0.64244, 0.64079};
double tail_fit[] = {0.67048, 0.65088, 0.64507, 0.64500, 0.64402, 0.64295, 0.64279, 0.64244, 0.64079};

// data for testing with C-code and testing shifting invariance of the enhacned C++ version.
double tail_shifted[] = {100.10501, 100.02969, 100.01009,100.00428,100.00421,100.00323,100.00216,100.00200,100.00165,100}; 
double tail_fit_shifted[] = {100.02969, 100.01009,100.00428,100.00421,100.00323,100.00216,100.00200,100.00165,100}; 

// example where data "smaller" is better 
double tail_low[]  = {0.25419999999999998, .32952, .34912, .35493, .35500, .35598, .35705, .35721, .35756,  .35921 };




// example SVM with both postive and negative classes that work for svm renormalization  fitting
svm_node_libsvm svm_test[] =  {                              
  {1,.32952}, {1,.34912}, {1,.35493}, {1,.35500}, {1,.35598}, {1,.35705}, {1,.35721}, {1,.35756},  {1, .35921},
  {-1,-.32952}, {-1,-.34912}, {-1,-.35493}, {-1,-.35500}, {-1,-.35598}, {-1,-.35705}, {-1,-.35721}, {-1,-.35756}, {-1, -.35921},
  {1,.52952}, {1,.54912}, {1,.55493}, {1,.55500}, {1,.55598}, {1,.55705}, {1,.55721}, {1,.55756},  {1, .55921},
  {-1,-.52952}, {-1,-.54912}, {-1,-.55493}, {-1,-.55500}, {-1,-.55598}, {-1,-.55705}, {-1,-.55721}, {-1,-.55756}, {-1, -.55921}
};

double renorm_test[] = {.32952,.34912,.35493,.35500,.35598,.35705,.35721,.35756, .35921,-.32952,-.34912,-.35493,-.35500,-.35598,-.35705,-.35721,-.35756, -.35921,.52952,.54912,.55493,.55500,.55598,.55705,.55721,.55756, .55921,-.52952,-.54912,-.55493,-.55500,-.55598,-.55705,-.55721,-.55756, -.55921};

double renorm_out[] = {0.99212139,0.58993097,0.41710247,0.41520328,0.38918195,0.36201934,0.35807254,0.34954448,0.31131160, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, .000000151, .000000028, .000000017, .000000017, .000000015, .000000014,.000000014,.000000013,.000000012, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000};


/* expected answers */ 
double W_answers[] = { 1.0000000000, 0.99212139,0.58993097,0.41710247,0.41520328,0.38918195,0.36201934,0.35807254,0.34954448,0.31131160 };
/* expected answers */ 
double C_answers[] = { 1.0000000000, 0.9921211956, 0.5899513281, 0.4171057966, 0.4152063721, 0.3891817521, 0.3620155874, 0.3580682635, 0.3495390670, 0.3113010327 };
double Wsmall_ans[] = { 1.000000000, 0.94423579,  0.39834863,  0.26183566,  0.26044828,  0.24164993,  0.22243081,  0.21967131,  0.21373671,  0.18758790};




void Test_C_Code()
{
	int i;
	// example of weibull_fit()
	double* weibull_parms = (double *)malloc(sizeof(double)* 2);
	double* parms_confidence_internal  = (double *)malloc(sizeof(double)* 4);
#define ALPHA 0.05
#define FITTING_SIZE 9
#define TAIL_SIZE 10

	weibull_fit(weibull_parms, parms_confidence_internal, tail_fit_shifted, ALPHA, FITTING_SIZE);

      if(verbose)	printf("weibull_parms: %f %f\n", weibull_parms[0], weibull_parms[1]);
      if(verbose)	printf("parms_confidence_internal: %f %f %f %f\n", parms_confidence_internal[0], parms_confidence_internal[1], parms_confidence_internal[2], parms_confidence_internal[3]);

      if(verbose)	printf("C-test of weibull cdf values:\n ");
	for (i = 0; i < TAIL_SIZE ; i++)
	{
		double tempcdf = weibull_cdf(tail_shifted[i], weibull_parms[0], weibull_parms[1]);
                if(fabs(tempcdf - C_answers[i]) > 1e-8) printf("WARNING C_CODE TEST fails %12.8f should be %12.8f\n", tempcdf, C_answers[i]);
                if(verbose)		printf("%12.10f ", tempcdf);
	}
      if(verbose)	printf("\n");

        free(weibull_parms);
        free(parms_confidence_internal);

}

int main(int argc, char **argv)
{

  if(argc > 1) verbose=1;
  
  if(verbose) printf("Test C-code models\n");
	Test_C_Code();

        int length=sizeof(tail)/sizeof(double);
 
      if(verbose)	printf("Test class-based model ");
        MetaRecognition mr1;  /* use all defaults, i.e. we don't have it drop the top item (i.e. no prediction test here)  */
	mr1.FitHigh(tail_fit, length-1);  /* but we ignore the top score (using data tail_fit not tail) so we can still predict */ 
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

	MetaRecognition mr2(true); //  choose to ignore best score..this time we test prediction  (default is ignore) */ 
	mr2.FitLow(tail_low, length,9);
        mr2.Save((char*)"mr2.txt");
	if(verbose) printf("EVTlow weibull cdf values:\n ");
	for (int i = 0; i < length; i++)
	{
                bool predict = mr2.Predict_Match(tail_low[i]);  
                if(i==0 && !predict )  printf("WARNING FitLow Predict TEST fails, should be positive for  %12.8f\n", tail[i] );                
                else   if(i> 0 && predict )  printf("WARNING Fitlowh Predict TEST fails, should be negative for  %12.8f\n", tail[i] );                

		double tempCdf = mr2.CDF(tail_low[i]);
                if(fabs(tempCdf - W_answers[i]) > 1e-8) printf("WARNING FitLow TEST fails %12.8f should be %12.8f\n", tempCdf, W_answers[i]);
		if(verbose) printf("%12.10f ", tempCdf);
	} 
	if(verbose) printf("\n");

	mr2.FitLow(tail_low, length,4);
        mr2.Save((char*)"mr2-tail4.txt");
	if(verbose) printf("EVTlow weibull cdf values using only 4 tail items:\n ");
	for (int i = 0; i < length; i++)
	{
                bool predict = mr2.Predict_Match(tail_low[i]);  
                if(i==0 && !predict )  printf("WARNING FitLow Predict TEST fails, should be positive for  %12.8f\n", tail[i] );                
                else   if(i> 0 && predict )  printf("WARNING Fitlowh Predict TEST fails, should be negative for  %12.8f\n", tail[i] );                

		double tempCdf = mr2.CDF(tail_low[i]);
                if(fabs(tempCdf - Wsmall_ans[i]) > 1e-8) printf("WARNING FitLow TEST fails %12.8f should be %12.8f\n", tempCdf, W_answers[i]);
		if(verbose) printf("%12.10f ", tempCdf);
	} 
	if(verbose) printf("\n");



	mr2.FitHigh(tail_shifted, length,9);
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
        mr1.FitSVM(svm_test,4*(length-1),1); // fit on positive class (label=1) doing rejection, so it estimates probabilty of being in negative class
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


	if(verbose) printf("Fit SVM postive tail of 4 and reject:\n ");
        mr1.FitSVM(svm_test,4*(length-1),1,true,MetaRecognition::positive_reject,4); // fit on positive class (label=1) doing rejection, so it estimates probabilty of being in negative class
        mr1.Save((char*)"mr1-svm-4.txt");
	for (int i = 0; i < length; i++)
	{
		double tempCdf = mr1.W_score(-tail_low[i]);
                if(fabs(tempCdf - Wsmall_ans[i]) > 1e-7) printf("WARNING SVM TEST 1 fails %12.8f should be %12.8f\n", tempCdf, W_answers[i]);
		if(verbose) printf("%12.10f ", tempCdf);
		tempCdf = mr1.CDF(tail[i]);
                if(tempCdf != 1.00 ) printf("WARNING SVM TEST 2 fails %12.8f should be %12.8f\n", tempCdf, 1.000);
	}
	if(verbose)  printf("\n");





	if(verbose) printf("Fit SVM postive model tail 9 (not rejection (all should =1)):\n ");
        // fit on positive class (label=1) without rejection, so it directly estimates probabilty of being in positive class
        mr3.FitSVM(svm_test,4*(length-1),1, false, MetaRecognition::positive_model,9); 
        mr3.Save((char*)"mr3-svm.txt");
	for (int i = 0; i < length; i++)
	{
		double tempCdf = mr3.W_score(tail_low[i]);
		if(verbose) printf("%12.10f ", tempCdf);
                if(tempCdf != 1.00 ) printf("WARNING SVM TEST 1pp fails %12.8f should be %12.8f\n", tempCdf, 1.000);
	}
	if(verbose)  printf("\n");


	if(verbose) printf("Fit SVM negative:\n ");

        // fit on negative class of SVM, (doing rejection), so it estimates probabilty of being in negative class. NOTE the -1 and added "false"
        mr2.FitSVM(svm_test,4*(length-1),-1,false);  
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

        // example of and testing of the ReNormalize function.. renormalizing svm decision score into w-scores
        double *renormout = (double*) malloc(sizeof(renorm_test));
        if(renormout){
          int rlen=sizeof(renorm_test)/sizeof(double);
          mr2.ReNormalize(renorm_test,renormout, rlen);
          for(int i=0; i< rlen; i++){
            if(fabs(renormout[i] - renorm_out[i]) >  .00000001) printf("WARNING Renorm TEST fails at %d where %12.9f should be %12.9f\n", i, renormout[i], renorm_out[i]);
          }
          free(renormout);
        }

	printf("A clean test (without verbose) should print out only this message.  run with -v to show data\n");
#ifdef _WIN32
	printf("Pause for windows users double clicking. Press Enter to Continue.");
    getchar();
#endif
	return 0;
}
