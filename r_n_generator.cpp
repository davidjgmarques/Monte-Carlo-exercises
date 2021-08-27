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

    vector<double> list1;
    vector<double> list2;
    vector<double> list3;
    vector<double> list4;

    double max = 2.;
    double min = 0.;

    for (int i = 0; i < 3; i++){

        list1.push_back ( f_rng ( min, max ) );
        list2.push_back ( f_rng ( min, max ) );
    }    

    for (int k = 0; k < 3; k++){

        list3.push_back ( f_rng ( min, max ) );
        list4.push_back ( f_rng ( min, max ) );
    }

    cout << "\nList 1: " << endl;
    for (int i = 0; i < list1.size(); i++) {
        cout << list1.at(i) << "  ";
    }
    cout << endl;

    cout << "\nList 2: " << endl;
    for (int i = 0; i < list2.size(); i++) {
        cout << list2.at(i) << "  ";
    }
    cout << endl;

    cout << "\nList 3: " << endl;
    for (int i = 0; i < list3.size(); i++) {
        cout << list3.at(i) << "  ";
    }
    cout << endl;

    cout << "\nList 4: " << endl;
    for (int i = 0; i < list4.size(); i++) {
        cout << list4.at(i) << "  ";
    }
    cout << endl;

    return 0;
}