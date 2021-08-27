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

int main() {
    TApplication *myapp=new TApplication("myapp",0,0);

    TGraph * gDiff = new TGraph();

    vector<double> Ns = {1e2, 1e3, 1e4, 1e5, 1e6, 1e7};
    double N;

    float X_float;
    float one_over_N_float;

    double X_double;
    double one_over_N_double;

    double difference;

    for(int k = 0; k < Ns.size(); k++){

        N = Ns[k];

        X_float = 0.;
        X_double = 0.;

        difference = 0.;

        for(int i = 0; i < N; i++){

            one_over_N_float = (float)1/N;
            one_over_N_double = (double)1/N;

            X_float += one_over_N_float;
            X_double += one_over_N_double;
        }

        difference = abs(X_double - X_float);

        gDiff->SetPoint(k,N,difference);

        cout.precision(15);
        cout << "Using floats:" << endl;
        cout << "N = " << N << ", X = " << X_float << endl;
        cout << "" << endl;
        cout << "Using doubles:" << endl;
        cout << "N = " << N << ", X = " << X_double << endl;
        cout << "" << endl;
        cout << "Difference between double and float = " << difference << endl;
        cout << "" << endl;
        cout << "" << endl;
    }

    TCanvas * c2 = new TCanvas("c2","c2",900,700);
    c2->cd();
    c2->SetLogy();
    c2->SetLogx();
    gDiff->SetTitle("");
    gDiff->GetXaxis()->SetTitle("N");
    gDiff->GetXaxis()->SetTitleSize(0.045);
    gDiff->GetXaxis()->SetTitleOffset(1.0);
    gDiff->GetXaxis()->SetLimits(1e1,1e8);
    gDiff->GetYaxis()->SetTitle("| double - float |");
    gDiff->GetYaxis()->SetTitleSize(0.045);
    gDiff->GetYaxis()->SetTitleOffset(1.0);
    gDiff->GetYaxis()->SetRangeUser(1e-7,1);
    gDiff->SetMarkerStyle(8);
    gDiff->SetMarkerSize(1.5);
    gDiff->SetMarkerColor(kAzure-5);
    gDiff->Draw("AP");
    c2->SaveAs("double_minus_float.pdf");

    myapp->Run();
    return 0;
}
