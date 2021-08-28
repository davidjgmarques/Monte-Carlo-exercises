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
#include "TGraph2D.h"

using namespace std;

double f_rng ( double min, double max ) {

    double val = ( (double)rand() / RAND_MAX ) * ( max - min ) + min;
    return val;
}



int main() {
    TApplication *myapp=new TApplication("myapp",0,0);

    TH1F * hPhotonEnergyK = new TH1F("","",10000,0,100);
    TH1F * hPhotonEnergyRem = new TH1F("","",10000,0,100);

    TGraph2D * gPhotonK = new TGraph2D();
    TGraph2D * gPhotonRem = new TGraph2D();

    vector<double> L_energies = {15.86, 15.20, 13.04};
    vector<double> L_population = {2, 2, 4};
    
    double sum_pop = 8;
    vector<double> cumulative_prob;

    for ( int t = 0; t < L_population.size(); t++ ){

        if ( t == 0) cumulative_prob.push_back(L_population[t]/sum_pop);
        else cumulative_prob.push_back(L_population[t]/sum_pop + cumulative_prob[t-1]);
    } 

    double E_bind = 88.01;

    double energy_k;
    double energy_rem;

    double rgn;
    double rgn_theta;
    double rgn_phi;

    double N = 1e6;

    int it0 = 0;
    int it1 = 0;
    int it2 = 0;

    for ( int i = 0; i < N; i++ ) {

        if ( i % 100 == 0 ) {

            rgn_theta = f_rng (0., M_PI);
            rgn_phi = f_rng (0., 2*M_PI);

            gPhotonK -> SetPoint ( i, sin(rgn_theta)*cos(rgn_phi),
                sin(rgn_theta)*sin(rgn_phi),
                cos(rgn_theta));

            rgn_theta = f_rng (0., M_PI);
            rgn_phi = f_rng (0., 2*M_PI);

            gPhotonRem -> SetPoint ( i, sin(rgn_theta)*cos(rgn_phi),
                sin(rgn_theta)*sin(rgn_phi),
                cos(rgn_theta));
        }
            
        rgn = f_rng (0., 1.);

        for ( int k = 0; k < L_population.size() ; k++ ) {

            if ( rgn < cumulative_prob[k] ) {

                energy_k = E_bind - L_energies[k];
                energy_rem = L_energies[k];

                hPhotonEnergyK->Fill(energy_k);
                hPhotonEnergyRem->Fill(energy_rem);
                break;
            }
        }
    }

    gStyle->SetOptStat(0);
    TCanvas * c1 = new TCanvas("c1","c1",1000,700);
	c1->cd();
    hPhotonEnergyK->SetLineColor(kAzure-5);
    hPhotonEnergyK->SetTitle("");
    hPhotonEnergyK->SetLineWidth(2);
    hPhotonEnergyK->SetFillStyle(3003);
    hPhotonEnergyK->SetFillColor(kAzure-5);
    hPhotonEnergyK->GetYaxis()->SetTitle("Counts");
    hPhotonEnergyK->GetYaxis()->SetTitleOffset(1.0);
    hPhotonEnergyK->GetYaxis()->SetTitleSize(0.045);
    hPhotonEnergyK->GetXaxis()->SetTitleOffset(1.0);
    hPhotonEnergyK->GetXaxis()->SetTitleSize(0.045);
    hPhotonEnergyK->GetXaxis()->SetTitle("Energy [keV]");
    hPhotonEnergyK->Draw();  

    hPhotonEnergyRem->SetLineColor(kGray+2);
    hPhotonEnergyRem->SetLineWidth(2);
    hPhotonEnergyRem->SetFillStyle(3003);
    hPhotonEnergyRem->SetFillColor(kGray+2);
    hPhotonEnergyRem->Draw("same");  

    c1->SaveAs("Distribution_of_photon_energies.pdf");

    TCanvas * c2 = new TCanvas("c2","c2",1000,700);
    c2->cd();
    gPhotonK->SetTitle("Photons direction");
    gPhotonK->GetXaxis()->SetTitle("x");
    gPhotonK->GetYaxis()->SetTitle("y");
    gPhotonK->GetZaxis()->SetTitle("z");
    gPhotonK->SetMarkerStyle(20);
    gPhotonK->SetMarkerSize(0.5);
    gPhotonK->SetMarkerColor(kRed);
    gPhotonK->Draw("P");

    gPhotonRem->SetMarkerStyle(20);
    gPhotonRem->SetMarkerSize(0.5);
    gPhotonRem->SetMarkerColor(kBlue);
    gPhotonRem->Draw("P same");

    c2->SaveAs("Distribution_photons.pdf");

    myapp->Run();
    return 0;
}