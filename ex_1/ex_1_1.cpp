#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <chrono>
#include <ctime>
#include <time.h>
#include <vector>
#include <random>
#include <Math/ProbFuncMathCore.h>

#include "TCanvas.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TEllipse.h"
#include "TAxis.h"
#include "TH1F.h"
#include "TF1.h"
#include "TApplication.h"

using namespace std;

long int MINSTD_rng ( long int initial ){

    int a = pow ( 7, 5 );
    int m = pow ( 2, 31 ) - 1;
    
    long int new_n = ( a * initial) % m;
    
    return new_n;
}

double chiSquareCalc ( TH1F * graph , int bins){

    double fChiSquare = 0;
    double instChiSquare;

    double entries = graph -> GetEntries();
    double expected = (double) entries / bins;

    for ( int i = 0; i < bins; i++){

        instChiSquare = (double) pow ( graph->GetBinContent ( i + 1 ) - expected , 2 ) / expected;
        fChiSquare += instChiSquare;
    }

    return fChiSquare;
}

int main() {

    TApplication *myapp=new TApplication("myapp",0,0);
    cout.precision(15);

    int nBins = 1000;
    TH1F * hDistr = new TH1F("","",nBins,0,1);

    vector <double> seeds_minstd = {332, 42090, 2458378, 649271822};
    double max_rng_minstd = pow(2,31) - 1;
    vector<double> iterations_minstd;

    double rgn_minstd;
    double it = 0;

    cout << "\n**MINSTD  algorithm**\nm = 2^31 -1\na = 7^5\n" << endl;

    clock_t c_start1 = clock();

    for(int k = 0; k < seeds_minstd.size(); k++){

        cout << "Seed = " << seeds_minstd[k] << endl;
        it = 0;
        rgn_minstd = seeds_minstd[k];
        
        while(true){
            
            rgn_minstd = MINSTD_rng ( rgn_minstd );

            if ( k == 2) hDistr-> Fill ( (double) rgn_minstd / max_rng_minstd );

            it++;

            if(rgn_minstd == seeds_minstd[k]){

                iterations_minstd.push_back(it);
                break;
            }      
        }
    }

    clock_t c_end1 = clock();
    cout << "\nAverage time needed return to the seed: " << (double) ((c_end1-c_start1) / CLOCKS_PER_SEC)/seeds_minstd.size() << " seconds." << endl;


    for(int k = 0; k < seeds_minstd.size(); k++){
        cout << (int) k + 1 << ". For the seed = " << seeds_minstd[k] << 
        ", the length is " << iterations_minstd[k] << " and length/m = " << 
        (double) iterations_minstd[k]/(max_rng_minstd - 1) << "." << endl;
    }

    double chiSquare1, p_value1;

    chiSquare1 = chiSquareCalc ( hDistr , nBins);
    p_value1 = 1. - ROOT::Math::chisquared_cdf ( chiSquare1 , nBins - 1 );

    cout << "\n* MINSTD generator *\n"
    << "Chi Square: " << chiSquare1 << endl
    << "p-value: " << p_value1 << "\n" << endl;
  
    gStyle->SetOptStat(10);
    TCanvas * c1 = new TCanvas("c1","c1",1000,700);
	c1->cd();
    hDistr->SetLineColor(kAzure-5);
    hDistr->SetTitle("Distribution of random generated numbers");
    hDistr->SetLineWidth(2);
    hDistr->SetFillStyle(3003);
    hDistr->SetFillColor(kAzure+5);
    hDistr->GetYaxis()->SetTitle("Counts");
    hDistr->GetYaxis()->SetTitleOffset(1.0);
    hDistr->GetYaxis()->SetTitleSize(0.045);
    hDistr->GetXaxis()->SetTitleOffset(1.0);
    hDistr->GetXaxis()->SetTitleSize(0.045);
    hDistr->GetYaxis()->SetRangeUser(0,6e6);
    hDistr->GetXaxis()->SetTitle("Obtained number (normalized)");
    hDistr->Draw();  
    c1->SaveAs("Minstd_Distribution_of_rgn.pdf");

    myapp->Run();
    return 0;
}   
