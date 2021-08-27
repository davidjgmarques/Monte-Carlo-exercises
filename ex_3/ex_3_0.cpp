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

    TGraph * gTarget = new TGraph();
    TEllipse *circle = new TEllipse(0,0,1);
    TGraph * gDiff = new TGraph();
    TGraph * gPiObtained = new TGraph();

    vector<long double> Ns = {1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9};
    double N;

    double limMin = -1;
    double limMax = 1;

    double new_rng_x;
    double new_rng_y;

    double pi = M_PI;

    double norm;
    double radius = 1;

    double points_in;
    double area;
    double diff;


    for(int k = 0; k < Ns.size(); k++){

        N = Ns[k];
        points_in = 0;

        for (int i = 0; i < N; i++){

            new_rng_x = f_rng ( limMin, limMax );
            new_rng_y = f_rng ( limMin, limMax );

            norm = pow (new_rng_x, 2 ) + pow (new_rng_y, 2 );

            if (norm <= pow ( radius, 2 )) points_in++ ;

            if ( k == 2 ) gTarget -> SetPoint ( i, new_rng_x, new_rng_y );
        }

        area = 4 * points_in/N;
        gPiObtained -> SetPoint ( k, N, area );

        diff = abs ( pi - area );
        gDiff -> SetPoint ( k, N, diff );
        
        cout << "N: " << N << "\nand diff: " << diff << endl;
    }
    
    //////////////////////////////////////////////  
    
    TCanvas * c1 = new TCanvas("c1","c1",700,700);
    c1->cd();
    gTarget->SetTitle("1000 events shot");
    gTarget->SetMarkerStyle(8);
    gTarget->SetMarkerSize(1);
    gTarget->SetMarkerColor(kRed-4);
    gTarget->GetXaxis()->SetTitle("x coordinate");
    gTarget->GetXaxis()->SetTitleSize(0.045);
    gTarget->GetXaxis()->SetTitleOffset(0.9);
    gTarget->GetYaxis()->SetTitle("y coordinate");
    gTarget->GetYaxis()->SetTitleSize(0.045);
    gTarget->GetYaxis()->SetTitleOffset(1.1);
    gTarget->Draw("AP");
    circle->SetLineWidth(3);
    circle->SetLineColor(kAzure-5);
    circle->SetFillStyle(0);
    circle->Draw("same");
    c1->SaveAs("CircleToss_1000_evt.pdf");

    TCanvas * c2 = new TCanvas("c2","c2",1000,700);
    c2->cd();
    gDiff->SetTitle("Difference between #pi_MC and #pi ");
    c2->SetLogx();
    c2->SetLogy();
    gDiff->SetMarkerStyle(8);
    gDiff->SetMarkerSize(2);
    gDiff->SetMarkerColor(kAzure-5);
    gDiff->GetXaxis()->SetTitle("N");
    gDiff->GetXaxis()->SetTitleSize(0.045);
    gDiff->GetXaxis()->SetTitleOffset(1);
    gDiff->GetXaxis()->SetLimits(1,1e10);
    gDiff->GetYaxis()->SetTitle("Difference");
    gDiff->GetYaxis()->SetTitleSize(0.045);
    gDiff->GetYaxis()->SetTitleOffset(1);
    gDiff->GetYaxis()->SetRangeUser(1e-7,10);
    gDiff->Draw("AP");
    c2->SaveAs("Difference_in_pi.pdf");


    TGraph * gPi = new TGraph();
    gPi -> SetPoint ( 0, 1 , pi);
    gPi -> SetPoint ( 1, 1e9 , pi);

    TCanvas * c3 = new TCanvas("c3","c3",1000,700);
    c3->cd();
    gPiObtained->SetTitle("");
    c3->SetLogx();
    gPiObtained->SetLineColor(kAzure-5);
    gPiObtained->SetLineWidth(3);
    gPiObtained->GetXaxis()->SetTitle("N");
    gPiObtained->GetYaxis()->SetTitle("#pi obtained ");
    gPiObtained->GetXaxis()->SetTitleSize(0.045);
    gPiObtained->GetXaxis()->SetTitleOffset(1);
    gPiObtained->GetXaxis()->SetLimits(1,1e10);
    gPiObtained->GetYaxis()->SetTitleSize(0.045);
    gPiObtained->GetYaxis()->SetTitleOffset(1);
    gPiObtained->GetYaxis()->SetRangeUser(2.6,3.6);
    gPiObtained->Draw("AL");
    gPi->SetLineWidth(3);
    gPi->SetLineStyle(7);
    gPi->SetLineColor(kGray+2);
    gPi->Draw("same");
    c3->SaveAs("Pi_obtained.pdf");  

    myapp->Run();
    return 0;
}