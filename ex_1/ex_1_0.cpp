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

int Linear_increase (unsigned int initial ){

    return initial*663608941;
}

double chiSquareCalc ( TH1F * graph , int bins){

    double fChiSquare = 0;
    double instChiSquare;

    double entries = graph -> GetEntries();
    double expected = (double) entries / bins;

    for ( int i = 0; i < bins; i++){

        instChiSquare = (double) pow ( graph->GetBinContent ( i + 1) - expected , 2 ) / expected;
        fChiSquare += instChiSquare;
    }

    return fChiSquare;
}

int main() {
    TApplication *myapp=new TApplication("myapp",0,0);

    unsigned int seed = 987654321;
    unsigned int rgn = seed;

    unsigned int max_rng = pow(2,32) - 1;
    
    int iteration = 0;

    clock_t c_start0 = clock();

    while(true){

        rgn = Linear_increase ( rgn );

        iteration += 1;

        if(rgn == seed) break;
    }

    clock_t c_end0 = clock();

    cout << "Period obtained: "<< iteration << "\n";
    cout << "Period expected: 2^30 = "<< (int) pow(2,30);

    cout << "\nTime needed return to the seed: " << (double) (c_end0-c_start0) / CLOCKS_PER_SEC << " seconds." << endl;

    /////////////////////////////////////

    int nBins = 100;
    TH1F * hDistr1 = new TH1F("","",nBins,0,1);
    TH1F * hDistr2 = new TH1F("","",nBins,0,1);
    TH1F * hDistr3 = new TH1F("","",nBins,0,1);

    iteration = 0;
    rgn = seed;
    while(true){

        rgn = Linear_increase ( rgn );

        hDistr1->Fill ( (double) rgn / max_rng );
        if ( iteration < 1e3 ) hDistr2 -> Fill ( (double) rgn / max_rng );
        if ( iteration < 1e6 ) hDistr3 -> Fill ( (double) rgn / max_rng );

        iteration++;
        if(rgn == seed) break;
    }

    double chiSquare1, chiSquare2, chiSquare3;
    double p_value1, p_value2, p_value3;

    chiSquare1 = chiSquareCalc ( hDistr1 , nBins);
    p_value1 = 1. - ROOT::Math::chisquared_cdf ( chiSquare1 , nBins - 1 );

    chiSquare2 = chiSquareCalc ( hDistr2 , nBins);
    p_value2 = 1. - ROOT::Math::chisquared_cdf ( chiSquare2 , nBins - 1 );

    chiSquare3 = chiSquareCalc ( hDistr3 , nBins);
    p_value3 = 1. - ROOT::Math::chisquared_cdf ( chiSquare3 , nBins - 1 );

    cout << "\n* All events *\n"
    << "Chi Square: " << chiSquare1 << endl
    << "p-value: " << p_value1 << "\n" << endl;

    cout << "* First 1e3 events *\n"
    << "Chi Square: " << chiSquare2 << endl
    << "p-value: " << p_value2 << "\n" << endl;

    cout << "* First 1e6 events *\n"
    << "Chi Square: " << chiSquare3 << endl
    << "p-value: " << p_value3 << "\n" << endl;

    gStyle->SetOptStat(10);
    TCanvas * c1 = new TCanvas("c1","c1",1000,700);
	c1->cd();
    hDistr1->SetLineColor(kAzure-5);
    hDistr1->SetTitle("Distribution of random generated numbers");
    hDistr1->SetLineWidth(2);
    hDistr1->SetFillStyle(3003);
    hDistr1->SetFillColor(kAzure+5);
    hDistr1->GetYaxis()->SetTitle("Counts");
    hDistr1->GetYaxis()->SetTitleOffset(1.0);
    hDistr1->GetYaxis()->SetTitleSize(0.045);
    hDistr1->GetXaxis()->SetTitleOffset(1.0);
    hDistr1->GetXaxis()->SetTitleSize(0.045);
    hDistr1->GetYaxis()->SetRangeUser(1,15e6);
    hDistr1->GetXaxis()->SetTitle("Obtained number (normalized)");
    hDistr1->Draw();  
    c1->SaveAs("Distribution_of_rgn_all.pdf");

    TCanvas * c2 = new TCanvas("c2","c2",1000,700);
	c2->cd();
    hDistr2->SetLineColor(kAzure-5);
    hDistr2->SetTitle("Distribution of random generated numbers");
    hDistr2->SetLineWidth(2);
    hDistr2->SetFillStyle(3003);
    hDistr2->SetFillColor(kAzure+5);
    hDistr2->GetYaxis()->SetTitle("Counts");
    hDistr2->GetYaxis()->SetTitleOffset(1.0);
    hDistr2->GetYaxis()->SetTitleSize(0.045);
    hDistr2->GetXaxis()->SetTitleOffset(1.0);
    hDistr2->GetXaxis()->SetTitleSize(0.045);
    hDistr2->GetXaxis()->SetTitle("Obtained number (normalized)");
    hDistr2->Draw();  
    c2->SaveAs("Distribution_of_rgn_1e3.pdf");

    TCanvas * c3 = new TCanvas("c3","c3",1000,700);
	c3->cd();
    hDistr3->SetLineColor(kAzure-5);
    hDistr3->SetTitle("Distribution of random generated numbers");
    hDistr3->SetLineWidth(2);
    hDistr3->SetFillStyle(3003);
    hDistr3->SetFillColor(kAzure+5);
    hDistr3->GetYaxis()->SetTitle("Counts");
    hDistr3->GetYaxis()->SetTitleOffset(1.0);
    hDistr3->GetYaxis()->SetTitleSize(0.045);
    hDistr3->GetXaxis()->SetTitleOffset(1.0);
    hDistr3->GetXaxis()->SetTitleSize(0.045);
    hDistr3->GetXaxis()->SetTitle("Obtained number (normalized)");
    hDistr3->Draw();  
    c3->SaveAs("Distribution_of_rgn_1e6.pdf");

    myapp->Run();
    return 0;
}

