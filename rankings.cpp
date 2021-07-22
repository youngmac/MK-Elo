//
//  rankings.cpp
//  Mario_Kart_Rankings
//
//  Created by Mackenzie Young on 1/25/21.
//

bool



#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>
#include <map>

#include "rankings.h"
#include "Utility.h"

using namespace std;

KartRankings::KartRankings(string filename) {
    KartRankings::read_data(filename);
}

void KartRankings::read_data(string &filename) {
    int num_players;
    ifstream fin;
    fin.open(filename);
    if(!fin.is_open()) {
        cout << "Error: " << filename << "  is an invalid filename" << endl;
        exit(1);
    }
    fin >> num_players;
    string name;
    double pre_rating;
    for(int i = 0; i < num_players; i++) {
        fin >> name >> pre_rating;
        player_data[name] = PlayerData(pre_rating, int(0), int(0));
    }
    
    string cup;
    string date;
    double score;
    while(getline(fin, cup)) {
        if(cup == "")
            continue;
        istringstream sin(cup);
        sin >> date;
        vector<PlayerScore> one_cup_data;
        while(sin >> name >> score) {
            one_cup_data.push_back(PlayerScore(name, score));
            player_data[name].cups++;
        }
        for(auto player_score: one_cup_data) {
            player_data[player_score.name].hth += one_cup_data.size() - 1;
        }
        cup_data.push_back(one_cup_data);
    }
    fin.close();
}

void KartRankings::calculate(string filename) {
    ofstream fout;
    fout.open(filename);
    if(!fout.is_open()) {
        cout << "Error: " << filename << "  is an invalid filename" << endl;
        exit(1);
    }
    fout << "\t<-------->" << endl;
    fout << "\tSTART DATA" << endl;
    fout << "\t<-------->" << endl;
    KartRankings::write_preliminary_data(fout);
    
    fout << "\t<------>" << endl;
    fout << "\tCUP DATA" << endl;
    fout << "\t<------>" << endl;
    for(auto one_cup_data: cup_data) {
        KartRankings::update(one_cup_data);
        KartRankings::write_cup_data(one_cup_data, fout);
    }
    
    fout << "\t<---------->" << endl;
    fout << "\tRANKING DATA" << endl;
    fout << "\t<---------->" << endl;
    KartRankings::write_data(fout);
    fout.close();
}

void KartRankings::update(vector<PlayerScore>& cup_data) {
    // initializes change for each player as 0 for this cup
    unordered_map<string, double> player_rating_changes;
    for(auto pair: player_data)
        player_rating_changes[pair.first] = 0;
    // calculates change for each pair of players in this cup and adds to change
    for(int i = 0; i < cup_data.size(); i++) {
        for(int j = 1; j < cup_data.size(); j++) {
            PlayerScore& p1 = cup_data[i];
            PlayerScore& p2 = cup_data[j];
            double p1_score = points_to_place[p1.score];
            double p2_score = points_to_place[p2.score];
            double r1 = player_data[p1.name].rating;
            double r2 = player_data[p2.name].rating;
            
            // finds expected value between 0->1
            double expected = 1 / (1 + pow(10,(r2 - r1) / 400));
            
            // actual difference (max/min of -4.5->4.5)
            double actual = max(min(p1_score - p2_score, mvm), -1 * mvm);
            // bulging actual away from the middle using sqrt
            // (offsets elo calculations that are zero sum games)
            double actual_0_1 = actual > 0 ?
                sqrt(actual / mvm) : -1 * sqrt(abs(actual / mvm));
            // finds actual value between 0->1
            actual_0_1 = (actual_0_1 + 1) / 2;
            double k_val = 0;
            if(max(i,j) == 1)
                k_val = k_2;
            else if(max(i,j) == 2)
                k_val = k_3;
            else
                k_val = k_4;
            double update_val = k_val * (actual_0_1 - expected);
            if(actual > 0) { // p1 won
                // we cannot decrease winner's score because they won by more
                // than one average place
                if(update_val < 0 and actual >= msd)
                    player_rating_changes[p2.name] -= update_val;
                // otherwise, update both
                else {
                    player_rating_changes[p1.name] += update_val;
                    player_rating_changes[p2.name] -= update_val;
                }
            } else { // p2 won
                // we cannot decrease winner's score because they won by more
                // than one average place
                if(update_val > 0 and actual <= msd)
                    player_rating_changes[p1.name] += update_val;
                // otherwise, update both
                else {
                    player_rating_changes[p1.name] += update_val;
                    player_rating_changes[p2.name] -= update_val;
                }
            }
        }
    }
    for(auto pair: player_rating_changes)
        player_data[pair.first].rating += pair.second;
    
}

void KartRankings::write_data(ofstream& fout) {
    fout << left << "---------------------------------------------------------------------------" << endl;
    fout << left << setw(15) << "Name" << setw(15) << right << "Rating" <<
        setw(15) << "Cups Played" << setw(15) << "Head-to-heads" << setw(15) <<
        "Overall" << endl;
    fout << left << "---------------------------------------------------------------------------" << endl;
    map<double, string, greater<double>> ratings_temp;
    for(auto pair: player_data) {
        ratings_temp[pair.second.rating] = pair.first;
    }
    for(auto pair: ratings_temp) {
        fout << left << setw(15) << pair.second << setw(15) << right <<
            pair.first << setw(15) << player_data[pair.second].cups <<
            setw(15) << player_data[pair.second].hth << setw(15) <<
            int(Utility::Utility::percentile(player_data[pair.second].rating)
            * 100) << endl;
    }
    fout << endl;
}

void KartRankings::write_cup_data(vector<PlayerScore>& cup_data,
ofstream& fout) {
    fout << left << "-----------------------------------" << endl;
    fout << left << setw(10) << "Name" << setw(10) << right << "Score" <<
        setw(15) << "New Rating" << endl;
    fout << left << "-----------------------------------" << endl;
    map<double, string, greater<double>> ratings_temp;
    for(auto pair: player_data) {
        ratings_temp[pair.second.rating] = pair.first;
    }
    for(auto pair: ratings_temp) {
        fout << left << setw(10) << pair.second;
        for(int i = 0; i < cup_data.size(); i++) {
            if(cup_data[i].name == pair.second) {
                fout << right << setw(10) << cup_data[i].score;
                break;
            }
            if(i == cup_data.size() - 1) {
                fout << right << setw(10) << "";
            }
        }
        fout << right << setw(15) << pair.first << endl;
    }
    fout << endl;
}

void KartRankings::write_preliminary_data(ofstream& fout) {
    fout << left << "---------------------------------------------" << endl;
    fout << left << setw(15) << "Name" << setw(15) << right << "Rating" <<
        setw(15) << "Cups Played" << endl;
    fout << left << "---------------------------------------------" << endl;
    map<double, string, greater<double>> ratings_temp;
    for(auto pair: player_data) {
        ratings_temp[pair.second.rating] = pair.first;
    }
    for(auto pair: ratings_temp) {
        fout << left << setw(15) << pair.second << setw(15) << right <<
            pair.first << setw(15) << "0" << endl;
    }
    fout << endl;
}

