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

double Sauter_formula ( double E_e, double theta ) {

    double mass_e = 511e3;

    double beta = (sqrt ( E_e * ( E_e + 2 * mass_e ) ) ) / ( E_e + mass_e );
    double gamma = 1. + E_e / mass_e;

    double num = pow ( sin ( theta ), 2 );
    double den = pow ( 1. - beta * cos ( theta ) , 4 );
    double last = 1. + ( 0.5 * gamma * ( gamma - 1. ) * ( gamma - 2. ) * ( 1. -  beta * cos ( theta ) ) );

    double val = num / den * last;
    return val;
}


int main() {
    TApplication *myapp=new TApplication("myapp",0,0);

    int nBinsPhi = 100;
    int nBinsTheta = 500;

    TH1F * hPhi = new TH1F("","",nBinsPhi,0,2*M_PI);
    TH1F * hTheta = new TH1F("","",nBinsTheta,0,M_PI);

    TGraph2D * gDirectionZ = new TGraph2D();
    TGraph2D * gDirectionY = new TGraph2D();

    double E_gamma = 1e6;
    double E_bind = 88.01e3;
    double E_photo_electron = E_gamma - E_bind;

    double theta;
    double phi;

    double rgn_x;
    double rgn_y;

    double coor_x;
    double coor_y;
    double coor_z;

    double N = 1e6;

    double efficiency=0;

    for ( int i = 0; i < N; i++ ) {

        // Photon (0,0,1) //

        phi = f_rng ( 0, 2 * M_PI);
        hPhi -> Fill ( phi );

        rgn_x = f_rng ( 0 , M_PI);
        rgn_y = f_rng ( 0., 900 );

        if ( rgn_y < Sauter_formula ( E_photo_electron, rgn_x ) ){

            hTheta -> Fill ( rgn_x );
            theta = rgn_x;
            efficiency++;
        }

        if ( i % 100 == 0 ) {

            coor_x = sin ( theta ) * cos ( phi );
            coor_y = sin ( theta ) * sin ( phi );
            coor_z = cos ( theta );

            gDirectionZ -> SetPoint ( i, coor_x, coor_y, coor_z); 
        }        

        // Photon (0,1,0) //

        phi = f_rng ( 0, 2 * M_PI);
        rgn_x = f_rng ( 0 , M_PI);
        rgn_y = f_rng ( 0., 900 );

        if ( rgn_y < Sauter_formula ( E_photo_electron, rgn_x ) ) theta = rgn_x;
        
        if ( i % 100 == 0 ) {

            coor_x = sin ( theta ) * sin ( phi );
            coor_y = cos ( theta );
            coor_z = sin ( theta ) * cos ( phi );

            gDirectionY -> SetPoint ( i, coor_x, coor_y, coor_z); 
        }    

    } 
    efficiency /= N;
    cout << "The efficiency of the sampling of the theta angle was: " << efficiency*100  << endl;

    int binmax = hTheta->GetMaximumBin(); double x_max = hTheta->GetXaxis()->GetBinCenter(binmax);
    cout << "Maximum at the x value of : " << x_max << endl;
    cout << "The maximum is, then: " << (double) Sauter_formula(E_photo_electron,x_max)<< endl;

    TCanvas * c3 = new TCanvas("c3","c3",1000,700);
	c3->cd();
    hPhi->SetLineColor(kAzure-5);
    hPhi->SetTitle("Distribution of #phi");
    hPhi->SetLineWidth(2);
    hPhi->SetFillStyle(3003);
    hPhi->SetFillColor(kAzure+5);
    hPhi->GetYaxis()->SetTitle("Counts");
    hPhi->GetYaxis()->SetTitleOffset(1.0);
    hPhi->GetYaxis()->SetTitleSize(0.045);
    hPhi->GetXaxis()->SetTitleOffset(1.0);
    hPhi->GetXaxis()->SetTitleSize(0.045);
    hPhi->GetXaxis()->SetTitle("#phi [rad]");
    hPhi->GetYaxis()->SetRangeUser(0,1.2*N/nBinsPhi);
    hPhi->Draw();  
    c3->SaveAs("Distribution_of_phi.pdf");


    gStyle->SetOptStat(0);
    gStyle->SetOptFit(101);
    TCanvas * c4 = new TCanvas("c4","c4",1000,700);
	c4->cd();
    hTheta->SetLineColor(kAzure-5);
    hTheta->SetTitle("Distribution of #theta");
    hTheta->SetLineWidth(2);
    hTheta->SetFillStyle(3003);
    hTheta->SetFillColor(kAzure+5);
    hTheta->GetYaxis()->SetTitle("Counts");
    hTheta->GetYaxis()->SetTitleOffset(1.0);
    hTheta->GetYaxis()->SetTitleSize(0.045);
    hTheta->GetXaxis()->SetTitleOffset(1.0);
    hTheta->GetXaxis()->SetTitleSize(0.045);
    hTheta->GetXaxis()->SetTitle("#theta [rad]");
    hTheta->Draw();

    TF1 * fit = new TF1("fit","[0] * (  pow(sin(x),2) / pow(1.-[1]*cos(x),4) * (1.+0.5*[2]*([2]-1.)*([2]-2.)*(1.-[1]*cos(x)))   )", 0.,M_PI);
    fit->SetParName ( 0, "Normalization" );
    fit->SetParName ( 1, "#beta" );
    fit->SetParName ( 2, "#gamma" );
    hTheta -> Fit ( fit );

    fit->Draw("same");
    c4->SaveAs("Distribution_of_theta.pdf");

    TCanvas * c5 = new TCanvas("c5","c5",1000,700);
    c5->cd();
    gDirectionZ->SetTitle("Photo-electron outgoing direction - Photon (0,0,1)");
    gDirectionZ->GetXaxis()->SetTitle("x");
    gDirectionZ->GetYaxis()->SetTitle("y");
    gDirectionZ->GetZaxis()->SetTitle("z");
    gDirectionZ->SetMarkerStyle(20);
    gDirectionZ->SetMarkerSize(0.5);
    gStyle->SetPalette(1);
    gDirectionZ->Draw("pcol");
    c5->SaveAs("Distribution_directionsZ.pdf");


    TCanvas * c6 = new TCanvas("c6","c6",1000,700);
    c6->cd();
    gDirectionY->SetTitle("Photo-electron outgoing direction - Photon (0,1,0)");
    gDirectionY->GetXaxis()->SetTitle("x");
    gDirectionY->GetYaxis()->SetTitle("y");
    gDirectionY->GetZaxis()->SetTitle("z");
    gDirectionY->SetMarkerStyle(20);
    gDirectionY->SetMarkerSize(0.5);
    gStyle->SetPalette(1);
    gDirectionY->Draw("pcol");
    c6->SaveAs("Distribution_directionsY.pdf");

    myapp->Run();
    return 0;
}