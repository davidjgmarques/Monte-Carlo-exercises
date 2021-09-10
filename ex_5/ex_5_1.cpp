#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <chrono>
#include <ctime>
#include <time.h>
#include <vector>
#include <random>

#include "TCanvas.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TEllipse.h"
#include "TAxis.h"
#include "TH1F.h"
#include "TF1.h"
#include "TApplication.h"

using namespace std;

double f_rng (double min, double max) {

    double val = ( (double)rand() / RAND_MAX ) * ( max - min ) + min;
    return val;
}

double exact_solution(int order){

    double val = 1./ (  1. + order );
    return val;
}

double func(double x, int order){

    double val = pow ( x, order );
    return val;
}

int main() {

    TApplication *myapp=new TApplication("myapp",0,0);

    vector<int> exponent = {1,2,3,4,5,6};
    vector<double> N_extr_list = {1e2,1e3,1e4,1e5,1e6,1e7};
    double N_extr;
    double N_points = 1e5;
    double N_rep = 1e3;

    double limMin_X = 0;
    double limMax_X = 1;
    double limMin_Y = 0;
    double limMax_Y = 1;
    double rng_x;
    double rng_y;

    double func_val;
    double points_in;
    //double sum;     // avg int
    double obtained;    // hit-or-miss
    double inst_diff;
    double mean_diff;

    double error = 1e-3;
    double anticipated;

    TH1F ** hVar;
    hVar = new TH1F*[ exponent.size() ];
    TGraph ** gDiff;
    gDiff = new TGraph*[ exponent.size() ];

    for(int histogramIT = 0; histogramIT < exponent.size(); histogramIT++){
                
        hVar[ histogramIT ] = new TH1F("","", 100 , -0.02, 0.02);
        gDiff[ histogramIT ] = new TGraph();

        // // could also the the histogram list like this to have them with diff names.
        // ostringstream histogramNameStream;
        // histogramNameStream << "n = " << histogramIT;
        // hVar[ histogramIT ] = new TH1F( histogramNameStream.str().c_str() , "" , 100. ,lim_hist_min, lim_hist_max);
    }

    for ( int k = 0; k < exponent.size(); k++){

        for (int j = 0; j < N_extr_list.size(); j++){

            N_extr = N_extr_list[j];
            points_in = 0;      // hit-or-miss
            //sum = 0;    // avg int

            for ( int i = 0; i< N_extr; i++){
                
                rng_x = f_rng ( limMin_X, limMax_X );
                rng_y = f_rng ( limMin_Y, limMax_Y );   // hit-or-miss

                func_val = func ( rng_x, exponent[ k ]);
      
                if( rng_y <= func_val ) points_in++;    // hit-or-miss
                //sum += func_val;    // avg int
            }

            obtained = points_in/N_extr;    // hit-or-miss
            //obtained = 1./N_extr * sum;    // avg int
            inst_diff = abs (obtained - exact_solution ( exponent[ k ] ) );

            gDiff[k]->SetPoint ( j, N_extr, inst_diff); 
        }   
    }

    for ( int k = 0; k < exponent.size(); k++){

        for (int p = 0; p < N_rep; p++){

            points_in = 0;      // hit-or-miss
            //sum = 0;    // avg int

            for (int i = 0; i < N_points; i++){

                rng_x = f_rng ( limMin_X, limMax_X );
                rng_y = f_rng ( limMin_Y, limMax_Y );   // hit-or-miss

                func_val = func ( rng_x, exponent[ k ]);
      
                if( rng_y <= func_val ) points_in++;    // hit-or-miss
                //sum += func_val;    // avg int
            }

            obtained = points_in/N_points;    // hit-or-miss
            //obtained = 1./N_points * sum;    // avg int
            inst_diff = obtained - exact_solution ( exponent[ k ] );

            hVar[ k ]->Fill( inst_diff );
        }

        hVar[ k ]->Fit("gaus","Q");
        TF1 * fit = (TF1*)hVar[ k ]->GetListOfFunctions()->FindObject("gaus");
        double sigma = fit->GetParameter(2);
        double mean = fit->GetParameter(1);

        mean_diff = abs (mean - exact_solution ( exponent[ k ] ) );
    }

    gStyle->SetOptStat(0);
    gStyle->SetOptFit(101);
    TCanvas * cVarAll = new TCanvas("cVarAll","cVarAll",1000,700);
	cVarAll->Divide(2,3);

    for ( int i = 0; i < exponent.size(); i++ ){
        cVarAll->cd( i+1 );
        hVar[i]->SetTitle("");
        hVar[i]->SetLineColor(kAzure-5);
        hVar[i]->SetLineWidth(2);
        hVar[i]->SetFillStyle(3003);
        hVar[i]->SetFillColor(kAzure+5);
        hVar[i]->GetYaxis()->SetTitleOffset(0.8);
        hVar[i]->GetYaxis()->SetTitleSize(0.065);
        hVar[i]->GetYaxis()->SetLabelSize(0.05);

        hVar[i]->GetXaxis()->SetTitleOffset(1.1);
        hVar[i]->GetXaxis()->SetTitleSize(0.045);
        hVar[i]->GetXaxis()->SetLabelSize(0.05);

        hVar[i]->GetYaxis()->SetTitle("Counts");
        hVar[i]->GetXaxis()->SetTitle("Obtained");
        hVar[i]->Draw("HIST");  
    }

    cVarAll->SaveAs("Integral_histograms.pdf"); 

    TCanvas * cDiffAll = new TCanvas("cDiffAll","cDiffAll",1000,700);
	cDiffAll->Divide(2,3);
    TPad * pad = new TPad();
    for ( int i = 0; i < exponent.size(); i++ ){
        cDiffAll->cd( i + 1 );
        cDiffAll->SetLogy();
        cDiffAll->SetLogx();

        pad = (TPad*) cDiffAll->FindObject(Form("cDiffAll_%i", i+1));
        pad->SetLogy();
        pad->SetLogx();
        gDiff[i]->SetTitle(Form("f(x) = x^{%i}", i+1));
        gDiff[i]->SetMarkerStyle(8);
        gDiff[i]->SetMarkerSize(1.5);
        gDiff[i]->SetMarkerColor(kAzure+5);
        gDiff[i]->GetYaxis()->SetTitleOffset(0.8);
        gDiff[i]->GetYaxis()->SetTitleSize(0.065);
        gDiff[i]->GetYaxis()->SetLabelSize(0.05);
        gDiff[i]->GetYaxis()->SetRangeUser(1e-6,1);

        gDiff[i]->GetXaxis()->SetTitleOffset(1.1);
        gDiff[i]->GetXaxis()->SetTitleSize(0.045);
        gDiff[i]->GetXaxis()->SetLabelSize(0.05);
        gDiff[i]->GetXaxis()->SetLimits(1e1,1e8);

        gDiff[i]->GetYaxis()->SetTitle("I_{sim} - I_{real}");
        gDiff[i]->GetXaxis()->SetTitle("N");
        gDiff[i]->Draw("AP");  
    }
    cDiffAll->SaveAs("Integral_diff_real.pdf"); 

    myapp->Run();
    return 0;
}  
