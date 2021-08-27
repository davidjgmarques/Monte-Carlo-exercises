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

double p_x(double x){

    double val = (2 * x) / pow (( 1 + pow (x, 2)), 2);
    return val;
}

double c_x(double x){

    double val = - ( 1 / ( 1 - pow ( x, 2) ) ) + 1;
    return val;
}

double c_x_inv(double x){
    double val = sqrt ( x / ( 1 - x ) );
    return val;
}

int main() {
    TApplication *myapp=new TApplication("myapp",0,0);
    gStyle->SetOptStat(11);
    gStyle->SetOptFit(101);

    int nBin = 10e3;
	double xMin = 0;
    double xMax = 10;
    TH1F * hRuther = new TH1F("Rutherford Distribution", "Rutherford Distribution", nBin, xMin, xMax);

    double N = 1e6;
    double t_rgn; 
    double x;
    
    vector<double> sampled_p;
    vector<double> function_p;
     
    for (int i = 0; i < N; i++){

        t_rgn = f_rng (0, 1);
        x = c_x_inv( t_rgn );
        hRuther-> Fill ( x );
    }

    TCanvas * c1 = new TCanvas("c1","c1",1000,700);
	double factor = 100.;
    hRuther->Scale(factor/hRuther->Integral(), "width");
	c1->cd();
    hRuther->SetTitle("");
    hRuther->SetLineWidth(2);
    hRuther->SetFillStyle(3003);
    hRuther->SetFillColor(kAzure+5);
    hRuther->SetLineColor(kAzure-5);
    hRuther->GetYaxis()->SetTitle("Probability [%]");
    hRuther->GetYaxis()->SetTitleOffset(1);
    hRuther->GetYaxis()->SetTitleSize(0.045);
    hRuther->GetXaxis()->SetTitle("x");
    hRuther->GetXaxis()->SetTitleOffset(1);
    hRuther->GetXaxis()->SetTitleSize(0.045);
    hRuther->Draw("hist");

  	TF1 *fRuther = new TF1("fRuther","(2*x)/(TMath::Power(1+TMath::Power(x,2),2))*[0]",xMin,xMax);
    hRuther->Fit("fRuther");
    fRuther->Draw("samePL");
       
    c1->SaveAs("Rutherford.pdf");

    myapp->Run();
    return 0;
}
