#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream> //added this for the ofstream
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
#include "TMultiGraph.h" 
#include "TLegend.h" 
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

double exact_solution (int dim ){

    double val = pow (1. - 1./exp(1), dim);
    return val;

}

double func ( double x ){

    double val = exp ( -x );
    return val;
}

int main() {

    TApplication *myapp=new TApplication("myapp",0,0);

    TMultiGraph *gDiff = new TMultiGraph();
    TGraph * gDiff_MC = new TGraph();
    TGraph * gDiff_MPS = new TGraph();

    vector<double> dimension = {1,2,3,4,5,6,7,8};
    vector<double> Nhistories = {65536,65536,64000,65536,59049,46656,78125,65536};
    vector<double> Ncell = {65536, 256, 40, 16, 9, 6, 5, 4};

    double N_hits;

    vector<double> difference;  

    double limMin_X = 0;
    double limMax_X = 1;
    double limMin_Y = 0.;
    double limMax_Y = 1;
    double rng_x;
    double rng_y;

    double func_val;
    double points_in;   // hit-or-miss
    //double sum;     // avg int
    double integral;
    double diff;
    double max_D;

    ////////// MC - integration for different Dimensionality
    cout << "\n*Monte Carlo hit-or-miss*" << endl;
    for (int k = 0; k < dimension.size(); k++){
    
        max_D = k + 1;
        integral = 1.;

        N_hits = Nhistories[k];

        for (int d = 1; d <= max_D ; d++){

            points_in = 0;      // hit-or-miss
            //sum = 0;    // avg int

            for (int i = 0; i < N_hits; i++){

                rng_x = f_rng ( limMin_X, limMax_X );
                rng_y = f_rng ( limMin_Y, limMax_Y );   // hit-or-miss

                func_val = func ( rng_x );
      
                if( rng_y <= func_val ) points_in++;    // hit-or-miss
                //sum += func_val;    // avg int
            }

            integral *= ( points_in/N_hits * (limMax_X - limMin_X) * (limMax_Y - limMin_Y) );   // hit-or-miss
            //integral *= 1./N_hits * sum;    // avg int  
        }

        diff = abs(integral - exact_solution(max_D));

        cout << "Dimensions: " <<  max_D << endl;
        cout << "integral: " <<  integral << endl;
        cout << "solution: " <<  exact_solution(max_D) << endl;
        cout << "difference: " << diff << endl;
        cout << "" << endl;

        gDiff_MC->SetPoint(k,max_D,diff);
    }

    ////////////////   Mid-point summation (MPS)  ///////////

    double middle_point;
    cout << "\n*Mid-point summation*" << endl;
    for ( int dim = 0; dim < dimension.size(); dim ++){
        
        double MPS_integral = 1.;
        double MPS_N = Ncell[dim] + 1;
        double delta_x = (limMax_X - limMin_X)/MPS_N;

        cout << "dx: " <<  delta_x << endl;

        max_D = dim + 1;

        for (int d = 1; d <= max_D ; d++){

            double MPS_sum = 0.;

            for(int step = 0; step < MPS_N ; step++){

                middle_point = delta_x*step + delta_x/2;

                func_val = func ( middle_point ) * delta_x;

                MPS_sum += func_val;
            }

            MPS_integral *= MPS_sum;
        }

        diff = abs(MPS_integral - exact_solution(max_D));
        
        cout << "Dimensions: " <<  max_D << endl;
        cout << "integral: " <<  MPS_integral << endl;
        cout << "solution: " <<  exact_solution(max_D) << endl;
        cout << "difference: " << diff << endl;
        cout << "" << endl;

        gDiff_MPS->SetPoint( dim + 8 , max_D, diff);
    }

    TCanvas * c2 = new TCanvas("c2","c2",1000,700);
    c2->cd();
    c2->SetLogy();
    gDiff->Add(gDiff_MC);
    gDiff->Add(gDiff_MPS);
    gDiff->SetTitle("");
    gDiff->GetXaxis()->SetTitle("Dimension");
    gDiff->GetXaxis()->SetTitleSize(0.045);
    gDiff->GetXaxis()->SetTitleOffset(1);
    gDiff->GetYaxis()->SetTitle("Difference");
    gDiff->GetYaxis()->SetTitleSize(0.045);
    gDiff->GetYaxis()->SetTitleOffset(1);
    gDiff->GetYaxis()->SetRangeUser(1e-13,1);

    gDiff_MC->SetMarkerStyle(21);
    gDiff_MC->SetMarkerSize(1.5);
    gDiff_MC->SetMarkerColor(kGray+2);
    gDiff_MPS->SetMarkerStyle(8);
    gDiff_MPS->SetMarkerSize(1.5);
    gDiff_MPS->SetMarkerColor(kAzure-5);

    gDiff->Draw("AP");

    auto legend = new TLegend(0.5,0.18,0.88,0.4);
    legend->AddEntry(gDiff_MC,"Monte Carlo","p");
    legend->AddEntry(gDiff_MPS,"Middle point summation","p");
    legend->Draw();

    c2->SaveAs("Difference_true_sim_value.pdf");  

    myapp->Run();
    return 0;
}

