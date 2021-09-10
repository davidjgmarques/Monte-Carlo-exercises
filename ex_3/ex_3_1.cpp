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

int main() {
    TApplication *myapp=new TApplication("myapp",0,0);
    gStyle->SetOptStat(11);
    gStyle->SetOptFit(1111);

    TH1F * hUncert_100 = new TH1F("","",38,2.3,3.9);//38
    TH1F * hUncert_1000 = new TH1F("","",100,2.8,3.6);
    TH1F * hUncert_5000 = new TH1F("","",100,3.0,3.3);

    TGraph * gSigma = new TGraph();

    vector<double> Ns = {1e2, 1e3, 5e3};
    double N_unc = 1e4;
    double N;
    
    double limMin = -1;
    double limMax = 1;

    double new_rng_x;
    double new_rng_y;

    double pi = M_PI;
    
    double norm;
    double radius = 1;

    double area;    
    double p_in_unc;


    for ( int p = 0; p < Ns.size(); p++){
        
        N = Ns[p];
        p_in_unc = 0;

        for(int k = 0; k < N_unc; k++){

            for (int i = 0; i < N; i++){

                new_rng_x = f_rng ( limMin, limMax );
                new_rng_y = f_rng ( limMin, limMax );

                norm = pow (new_rng_x, 2 ) + pow (new_rng_y, 2 );

                if (norm <= pow ( radius, 2 )) p_in_unc++ ;
            }

        area = (p_in_unc/N) * 4.;

        if ( p == 0 ) hUncert_100->Fill(area);
        if ( p == 1 ) hUncert_1000->Fill(area);
        if ( p == 2 ) hUncert_5000->Fill(area);

        p_in_unc = 0;
        }   
    }

    
    hUncert_100->Fit("gaus","Q");
    TF1 * fit_100 = (TF1*)hUncert_100->GetListOfFunctions()->FindObject("gaus");
    double sigma_100 = fit_100->GetParameter(2);
    cout << "sigma_100: " << sigma_100 << endl;
    gSigma->SetPoint(0,Ns[0],sigma_100);
    double k_100 = sigma_100*sqrt(Ns[0]);

    
    hUncert_1000->Fit("gaus","Q");
    TF1 * fit_1000 = (TF1*)hUncert_1000->GetListOfFunctions()->FindObject("gaus");
    double sigma_1000 = fit_1000->GetParameter(2);
    cout << "sigma_1000: " << sigma_1000 << endl;
    gSigma->SetPoint(1,Ns[1],sigma_1000);
    double k_1000 = sigma_1000*sqrt(Ns[1]);

    hUncert_5000->Fit("gaus","Q");
    TF1 * fit_5000 = (TF1*)hUncert_5000->GetListOfFunctions()->FindObject("gaus");
    double sigma_5000 = fit_5000->GetParameter(2);
    cout << "sigma_5000: " << sigma_5000 << endl;
    gSigma->SetPoint(2,Ns[2],sigma_5000);
    double k_5000 = sigma_5000*sqrt(Ns[2]);


    TF1 *sigma_func = new TF1("sig","[0]/sqrt(x)",1,1e5);
    double k_ave = (k_100+k_1000+k_5000)/3.;
    sigma_func->SetParameter(0,k_100);

    double N_suf = pow(k_ave/(pow(10,-4)),2);

    cout << "averaged k = " << k_ave << endl;
    cout << "Given the calculated k, to have sigma < 10e-4, one should use \n N >= (k/(10e-4))^2 (=)\nN = " << N_suf << endl;

    TCanvas * c1 = new TCanvas("c1","c1",1000,700);
	c1->cd();
    hUncert_100->SetLineColor(kAzure-5);
    hUncert_100->SetTitle("Distribution of #pi values obtained in different 100 points runs.");
    hUncert_100->SetLineWidth(2);
    hUncert_100->SetFillStyle(3003);
    hUncert_100->SetFillColor(kAzure+5);
    hUncert_100->GetYaxis()->SetTitle("Counts");
    hUncert_100->GetYaxis()->SetTitleOffset(1.0);
    hUncert_100->GetYaxis()->SetTitleSize(0.045);
    hUncert_100->GetXaxis()->SetTitleOffset(1.0);
    hUncert_100->GetXaxis()->SetTitleSize(0.045);
    hUncert_100->GetXaxis()->SetTitle("#pi obtained");
    hUncert_100->Draw();  
    c1->SaveAs("Distribution_of_the_pi_value_N_100.pdf");

    TCanvas * c2 = new TCanvas("c2","c2",1000,700);
    c2->cd();
    hUncert_1000->SetLineColor(kAzure-5);
    hUncert_1000->SetTitle("Distribution of #pi values obtained in different 1000 points runs.");
    hUncert_1000->SetLineWidth(2);
    hUncert_1000->SetFillStyle(3003);
    hUncert_1000->SetFillColor(kAzure+5);
    hUncert_1000->GetYaxis()->SetTitle("Counts");
    hUncert_1000->GetYaxis()->SetTitleOffset(1.0);
    hUncert_1000->GetYaxis()->SetTitleSize(0.045);
    hUncert_1000->GetXaxis()->SetTitleOffset(1.0);
    hUncert_1000->GetXaxis()->SetTitleSize(0.045);
    hUncert_1000->GetXaxis()->SetTitle("#pi obtained");
    hUncert_1000->Draw();  
    c2->SaveAs("Distribution_of_the_pi_value_N_1000.pdf");

    TCanvas * c3 = new TCanvas("c3","c3",1000,700);
    c3->cd();
    hUncert_5000->SetLineColor(kAzure-5);
    hUncert_5000->SetTitle("Distribution of #pi values obtained in different 5000 points runs.");
    hUncert_5000->SetLineWidth(2);
    hUncert_5000->SetFillStyle(3003);
    hUncert_5000->SetFillColor(kAzure+5);
    hUncert_5000->GetYaxis()->SetTitle("Counts");
    hUncert_5000->GetYaxis()->SetTitleOffset(1.0);
    hUncert_5000->GetYaxis()->SetTitleSize(0.045);
    hUncert_5000->GetXaxis()->SetTitleOffset(1.0);
    hUncert_5000->GetXaxis()->SetTitleSize(0.045);
    hUncert_5000->GetXaxis()->SetTitle("#pi obtained");
    hUncert_5000->Draw();  
    c3->SaveAs("Distribution_of_the_pi_value_N_5000.pdf");

    TCanvas * c4 = new TCanvas("c4","c4",1000,700);
    c4->cd();
    c4->SetLogx();
    gSigma->SetMarkerStyle(8);
    gSigma->SetMarkerSize(1.5);
    gSigma->SetMarkerColor(kAzure+5);
    gSigma->GetYaxis()->SetTitle("Standard deviation");
    gSigma->GetYaxis()->SetTitleOffset(1.0);
    gSigma->GetYaxis()->SetTitleSize(0.045);
    gSigma->GetXaxis()->SetTitleOffset(1.0);
    gSigma->GetXaxis()->SetTitleSize(0.045);
    gSigma->GetXaxis()->SetTitle("N");
    gSigma->GetXaxis()->SetLimits(80,1e4);
    gSigma->Draw("AP");  
    sigma_func->SetLineWidth(2);
    sigma_func->SetLineStyle(2);
    sigma_func->SetLineColor(kGray+2);
    sigma_func->Draw("same");

    c4->SaveAs("k_val_distribution.pdf");
    
    myapp->Run();
    return 0;
}