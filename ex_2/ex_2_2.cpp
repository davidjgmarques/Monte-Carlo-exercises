#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <chrono>
#include <ctime>
#include <vector>

using namespace std;

using Clock = std::chrono::steady_clock;
using std::chrono::time_point;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

double f_rng (double min, double max) {

    double val = ( (double)rand() / RAND_MAX ) * ( max - min ) + min;
    return val;
}

double coor_x ( double a, double b, double R ){
    
    double val = R * sqrt(a) * cos ( 2 * M_PI * b );
    return val;
}

double coor_y ( double a, double b, double R ){

    double val = R * sqrt(a) * sin ( 2 * M_PI * b );
    return val;
}


int main() {

    long int N = 1e8;
    double rgn_1, rgn_2, x, y, x_rej, y_rej;
    double R = 1.;

    double n_hits;
    double n_shot;
    double length;

    ///////////////// Analytical //////////////////

    time_point<Clock> start0 = Clock::now();
    n_hits = 0;
    n_shot = 0;
    for (int i = 0; i < N; i++ ){

        rgn_1 = f_rng (0,1);
        rgn_2 = f_rng (0,1);

        x = coor_x ( rgn_1, rgn_2,R );
        y = coor_y ( rgn_1, rgn_2,R );

        length = pow(x,2) + pow(y,2);

        n_shot++;

        if ( length <= pow(R,2) ) n_hits++;
    }

    time_point<Clock> end0 = Clock::now();
    milliseconds diff0 = duration_cast<milliseconds>(end0 - start0);
    cout << "* Inversion method *" << endl;
    cout << "Time to create the circle with 1e8 points: " << (double) (diff0.count())/1000 << " seconds." << endl;
    cout << "Efficiency (hits/shot): " << (double) n_hits/n_shot << endl;

    ///////////////// Rejection  //////////////////

    time_point<Clock> start2 = Clock::now();
    n_hits = 0;
    n_shot = 0;
    while(true){

        x_rej = f_rng ( -1, 1);
        y_rej = f_rng ( -1, 1);

        length = pow(x_rej,2) + pow(y_rej,2);

        n_shot++;

        if ( length <= pow (R,2) ) n_hits++;

        if ( n_hits == N ) break;
    }

    time_point<Clock> end2 = Clock::now();
    milliseconds diff2 = duration_cast<milliseconds>(end2 - start2);
    cout << "\n* Rejection method *" << endl;
    cout << "Time to create the circle with 1e8 points: " << (double) (diff2.count())/1000 << " seconds." << endl;
    cout << "Efficiency (hits/shot): " << (float) n_hits/n_shot << endl;

    double v1 = diff0.count();
    double v2 = diff2.count();

    double ratio = (v1/v2);
    cout << "\nThe rejection method is " << ratio << " times faster." << endl;

    return 0;
}
