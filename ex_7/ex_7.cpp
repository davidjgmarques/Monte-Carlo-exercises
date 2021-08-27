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

double inverseCDF ( double t, double a) {

    double val = - ( 1. / a ) * log ( 1 - t) ;
    return val;
}

int main() {
    TApplication *myapp=new TApplication("myapp",0,0);

    double xMin = 0.;
    double xMax = 4.5;

    TH1F * h_s_distr = new TH1F("","",100,xMin,xMax);

    double N = 1e5;

    double rgn1;
    double rgn2;

    double s1;
    double s2;

    double mu_1 = 1.;
    double mu_2 = 2.;

    double minimum;

    double counter1 = 0.;
    double counter2 = 0.;

    for ( int i = 0; i < N; i++) {

        rgn1 = f_rng ( 0. , 1. );
        rgn2 = f_rng ( 0. , 1. );

        s1 = inverseCDF ( rgn1, mu_1);
        s2 = inverseCDF ( rgn2, mu_2);

        minimum = min ( s1, s2 );

        h_s_distr -> Fill ( minimum );

        if ( s1 < s2 ) counter1++;
        else counter2++;
    }

    cout << "\nRatio mu_1: " << (double) counter1/N << 
    "\nRatio mu_2: " << (double) counter2/N << "\n\n" << endl; 

    gStyle->SetOptStat(0);
    gStyle->SetOptFit(101);
    TCanvas * c1 = new TCanvas("c1","c1",1000,700);
	c1->cd();
    c1->SetLogy();
    h_s_distr->SetLineColor(kAzure-5);
    h_s_distr->SetTitle("");
    h_s_distr->SetLineWidth(2);
    h_s_distr->SetFillStyle(3003);
    h_s_distr->SetFillColor(kAzure+5);
    h_s_distr->GetYaxis()->SetTitle("Counts");
    h_s_distr->GetYaxis()->SetTitleOffset(1.0);
    h_s_distr->GetYaxis()->SetTitleSize(0.045);
    h_s_distr->GetXaxis()->SetTitleOffset(1.0);
    h_s_distr->GetXaxis()->SetTitleSize(0.045);
    h_s_distr->GetXaxis()->SetTitle("x");
    h_s_distr->Draw();  

    TF1 * fit = new TF1("fit", "[0]*exp(-[1]*x)", xMin,xMax);
    fit->SetParName ( 0, "Normalization" );
    fit->SetParName ( 1, "#mu" );
    h_s_distr -> Fit ( fit );

    fit->Draw("same");
    c1 -> SaveAs("Distribution_of_s.pdf");

    myapp->Run();
    return 0;
}

