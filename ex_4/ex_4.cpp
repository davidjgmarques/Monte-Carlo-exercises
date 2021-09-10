#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <random>

#include "TCanvas.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TEllipse.h"
#include "TAxis.h"
#include "TLine.h"
#include "TH1F.h"
#include "TF1.h"
#include "TApplication.h"

using namespace std;

double f_rng (double min, double max) {

    double val = ( (double)rand() / RAND_MAX ) * ( max - min ) + min;
    return val;
}

int f_rng_int ( int min, int max ) {
    
    int val = rand() % max + min;
    return val;
}

vector <int> throw_n_dice ( int n ) {

    vector<int> val;
    int rgn;

    for(int i = 0; i < n; i++){
        rgn = f_rng_int(1,6);
        val.push_back(rgn);
    }

    sort ( val.begin(), val.end(), greater<int>());
    return val;
}

tuple < int , int > Single_round ( vector<int> attack, vector<int> def ) {   

    int won = 0.;
    int lost = 0.;

    int diff = min ( attack.size(), def.size() );

    for ( size_t i = 0; i < diff; ++i ) {

        if ( attack[i] > def[i] ) won++;
        else lost++;
    }

    return make_tuple ( won, lost );
}

tuple < int , int > Complete_war ( int initial_attack, int initial_defence ) {

    int attack = initial_attack;
    int defence = initial_defence;

    int win_attack;
    int lose_attack;

    vector <int> dice_attack;
    vector <int> dice_defence;

    while ( ( attack > 1 ) && ( defence > 0 ) ) {

        dice_attack = throw_n_dice ( min ( 3, attack - 1 ) );
        dice_defence = throw_n_dice ( min ( 3, defence ) );

        tie ( win_attack, lose_attack ) = Single_round ( dice_attack, dice_defence );        
        
        defence -= win_attack;
        attack -= lose_attack;
    }

    return make_tuple(attack,defence);
}




int main() {
    TApplication *myapp=new TApplication("myapp",0,0);

    int nBins1 = 26; 
    int nBins2 = 10;

    TH1F * hDistrNumb = new TH1F("","",6,0.5,6.5);
    TH1F * hQuickFight = new TH1F("","",6,-1.5,4.5);
    TH1F * hArmiesWon80 = new TH1F("","",nBins1,-0.5,25.5);
    TH1F * hLeftArmiesWon80 = new TH1F("","",nBins2,-0.5,9.5);
    TH1F * h6LeftArmiesWon80 = new TH1F("","",nBins1,-0.5,25.5);

    vector<int> quick_attack;
    vector<int> quick_defence;

    ////////////////// 4.0 //////////////////
    
    double rolls = 1e5;
    for ( int k = 0; k < rolls; k++ ){

        quick_attack = throw_n_dice(3);
        quick_defence = throw_n_dice(3);

        int quick_survivors;
        int quick_deseased;

        for ( size_t i = 0; i < quick_attack.size(); ++i )  hDistrNumb->Fill( quick_attack [i] );

        tie ( quick_survivors, quick_deseased ) = Single_round ( quick_attack, quick_defence );
            
        hQuickFight -> Fill( quick_survivors );  
    }

    double average_survivors = hQuickFight->GetMean()/3.;
    cout << "Average number of armies won by dice roll: " << average_survivors << endl;

    ////////////////// 4.1 //////////////////

    int initial_def = 3;

    int attackers_left;
    int defencers_left;

    for ( int init_at = 0; init_at < nBins1; init_at++){

        for ( int k = 0; k < rolls; k++ ){
            
            tie ( attackers_left, defencers_left ) = Complete_war ( init_at, initial_def );

            if ( attackers_left > defencers_left )  {
                
                hArmiesWon80 -> Fill ( init_at );

                if ( attackers_left > 5 ) {

                    h6LeftArmiesWon80 -> Fill ( init_at );
                }
            }

            if ( init_at == 9){

                hLeftArmiesWon80 -> Fill ( attackers_left );
            }
        }
    }

    double ThreshProb = 80;
    TLine * L_ThreshProb = new TLine(-0.5,ThreshProb,25.5,ThreshProb);
    L_ThreshProb->SetLineColor(kRed);
    L_ThreshProb->SetLineWidth(2);
    L_ThreshProb->SetLineStyle(2);

    gStyle->SetOptStat(110);
    TCanvas * c2 = new TCanvas("c2","c2",1000,700);
    c2->cd();
    double factor = (double) 100. / rolls;
    hQuickFight->Scale((double) factor);
    hQuickFight->SetLineColor(kAzure-5);
    hQuickFight->SetTitle("");
    hQuickFight->SetLineWidth(2);
    hQuickFight->SetFillStyle(3003);
    hQuickFight->SetFillColor(kAzure+5);
    hQuickFight->GetYaxis()->SetTitle("Probability [%]");
    hQuickFight->GetYaxis()->SetTitleOffset(1.0);
    hQuickFight->GetYaxis()->SetTitleSize(0.045);
    hQuickFight->GetXaxis()->SetTitleSize(0.045);
    hQuickFight->GetXaxis()->SetTitle("Number of surviving armies");
    hQuickFight->GetYaxis()->SetRangeUser(0,50);
    hQuickFight->Draw("hist");  
    c2->SaveAs("Distr_won_armies_single_round.pdf");   

    gStyle->SetOptStat(0);
    TCanvas * c3 = new TCanvas("c3","c3",1000,700);
    c3->cd();
    double factor1 = (double) 100. / rolls;
    hArmiesWon80->Scale((double) factor1);
    hArmiesWon80->SetLineColor(kAzure-5);
    hArmiesWon80->SetTitle("");
    hArmiesWon80->SetLineWidth(2);
    hArmiesWon80->SetFillStyle(3003);
    hArmiesWon80->SetFillColor(kAzure+5);
    hArmiesWon80->GetYaxis()->SetTitle("Probability [%]");
    hArmiesWon80->GetYaxis()->SetTitleOffset(1.0);
    hArmiesWon80->GetYaxis()->SetTitleSize(0.045);
    hArmiesWon80->GetXaxis()->SetTitleOffset(1.0);
    hArmiesWon80->GetXaxis()->SetTitleSize(0.045);
    hArmiesWon80->GetXaxis()->SetTitle("Number of initial armies");
    hArmiesWon80->Draw("HIST");  
    L_ThreshProb->Draw("same");
    c3->SaveAs("Distribution_80_percent.pdf");   


    // TCanvas * c4 = new TCanvas("c4","c4",1000,700);
    // c4->cd();
    // hDistrNumb->SetLineColor(kAzure-5);
    // hDistrNumb->SetTitle("");
    // hDistrNumb->SetLineWidth(2);
    // hDistrNumb->SetFillStyle(3003);
    // hDistrNumb->SetFillColor(kAzure+5);
    // hDistrNumb->GetYaxis()->SetTitle("Counts");
    // hDistrNumb->GetYaxis()->SetTitleOffset(1.0);
    // hDistrNumb->GetYaxis()->SetTitleSize(0.045);
    // hDistrNumb->GetXaxis()->SetTitleOffset(1.0);
    // hDistrNumb->GetXaxis()->SetTitleSize(0.045);
    // hDistrNumb->GetXaxis()->SetTitle("Dice outcome");
    // hDistrNumb->GetYaxis()->SetRangeUser(0,int((rolls*3/6)+rolls*0.2));
    // hDistrNumb->Draw();  
    // c4->SaveAs("Distribution_numbers_per_die.pdf"); 

    
    TCanvas * c5 = new TCanvas("c5","c5",1000,700);
    c5->cd();
    double factor2 = (double) 100. / rolls;
    hLeftArmiesWon80->Scale((double) factor2);
    hLeftArmiesWon80->SetLineColor(kAzure-5);
    hLeftArmiesWon80->SetTitle("");
    hLeftArmiesWon80->SetLineWidth(2);
    hLeftArmiesWon80->SetFillStyle(3003);
    hLeftArmiesWon80->SetFillColor(kAzure+5);
    hLeftArmiesWon80->GetYaxis()->SetTitle("Probability [%]");
    hLeftArmiesWon80->GetYaxis()->SetTitleOffset(1.0);
    hLeftArmiesWon80->GetYaxis()->SetTitleSize(0.045);
    hLeftArmiesWon80->GetXaxis()->SetTitleOffset(1.0);
    hLeftArmiesWon80->GetXaxis()->SetTitleSize(0.045);
    hLeftArmiesWon80->GetXaxis()->SetTitle("Number of attacker surviving armies");
    hLeftArmiesWon80->Draw("HIST");  
    c5->SaveAs("Distribution_of_armies_won2.pdf");  

    TCanvas * c6 = new TCanvas("c6","c6",1000,700);
    c6->cd();
    double factor3 = (double) 100. / rolls;
    h6LeftArmiesWon80->Scale((double) factor3);
    h6LeftArmiesWon80->SetLineColor(kAzure-5);
    h6LeftArmiesWon80->SetTitle("");
    h6LeftArmiesWon80->SetLineWidth(2);
    h6LeftArmiesWon80->SetFillStyle(3003);
    h6LeftArmiesWon80->SetFillColor(kAzure+5);
    h6LeftArmiesWon80->GetYaxis()->SetTitle("Probability [%]");
    h6LeftArmiesWon80->GetYaxis()->SetTitleOffset(1.0);
    h6LeftArmiesWon80->GetYaxis()->SetTitleOffset(1.0);
    h6LeftArmiesWon80->GetYaxis()->SetTitleSize(0.045);
    h6LeftArmiesWon80->GetXaxis()->SetTitleOffset(1.0);
    h6LeftArmiesWon80->GetXaxis()->SetTitleSize(0.045);
    h6LeftArmiesWon80->GetXaxis()->SetTitle("Number of initial armies");
    h6LeftArmiesWon80->Draw("HIST");
    L_ThreshProb->Draw("same");  
    c6->SaveAs("Distribution_of_armies_won_6_80.pdf");     



    int minArmies;
    for ( int i = 0; i < nBins1 + 1; i++ ) {

        cout << "i: " << i << ", with prob: " << hArmiesWon80 -> GetBinContent( i + 1 ) << endl;

        if ( hArmiesWon80 -> GetBinContent( i + 1 ) >= 80){
            
            minArmies = i;
            break;
        }
    }
    cout << "Minimum armies to win the war with probability superior to 80%: " << minArmies << "\n" << endl;


    int n_rem_min = 6;
    double prob_n_rem_min = 0.;
    for ( int i = 0; i < nBins2 + 1; i++ ) {

        cout << "i: " << i << ", with prob: " << hLeftArmiesWon80 -> GetBinContent ( i + 1 ) << endl;

        if ( i >= n_rem_min ) prob_n_rem_min += hLeftArmiesWon80 -> GetBinContent ( i + 1 ); 
    }
    cout << "The prob. of finishing the attack with 6 or more armies is: " << prob_n_rem_min << "\n" << endl;
    
    
    int minArmiesAt80;
    for ( int i = 0; i < nBins1 + 1; i++ ) {

        cout << "i: " << i << ", with prob: " << h6LeftArmiesWon80 -> GetBinContent( i + 1 ) << endl;

        if ( h6LeftArmiesWon80 -> GetBinContent( i + 1 ) >= 80){
            
            minArmiesAt80 = i;
            break;
        }
    }
    cout << "Minimum armies to finish the war with more than 6 armies in 80% of the cases: " << minArmiesAt80 << "\n" << endl;

    myapp->Run();
    return 0;
}