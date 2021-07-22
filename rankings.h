//
//  rankings.h
//  Mario_Kart_Rankings
//
//  Created by Mackenzie Young on 1/25/21.
//

#ifndef rankings_h
#define rankings_h

#include <string>
#include <vector>
#include <unordered_map>
#include <vector>
#include <utility>

#include "Utility.h"

using std::string;
using std::unordered_map;
using std::vector;
using std::ofstream;
using std::pair;

class KartRankings {
public:
    // INPUT: input filename
    KartRankings(string filename);
    
    // INPUT: output filename
    void calculate(string filename);
    
private:
    
    //--------------------------------------------------------------------------
    // CONSTANTS
    //--------------------------------------------------------------------------
    // points per place: ratio of Mario Kart points to place points
    const double ppp = 4;
    // max victory margin: victory magin considered a "slaughter"
    const double mvm = 4;
    // min victory difference: victory margin needed to guarantee no rating loss
    const double msd = .9999;
    // k-values
    const double k_2 = 32;
    const double k_3 = 24;
    const double k_4 = 12;
    // maps points to a place value
    const vector<double> points_to_place = Utility::Utility::mk_points_to_place;
    
    //--------------------------------------------------------------------------
    // HELPER STRUCTS
    //--------------------------------------------------------------------------
    // represents player data of one score
    struct PlayerScore {
        PlayerScore(string& name, double score) : name{name}, score{score} {}
        string name;
        double score;
    };
    
    // attributes of a player
    struct PlayerData {
        PlayerData() : rating{0}, cups{0}, hth{0} {}
        PlayerData(double rating, int cups, int hth) : rating{rating}, cups{cups}, hth{hth} {}
        double rating;
        int cups;
        int hth;
    };
    
    //--------------------------------------------------------------------------
    // PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------
    // reads in data from file
    void read_data(string& filename);
    // updates rankings based on one cup
    void update(vector<PlayerScore>& cup_data);
    void write_data(ofstream& fout);
    void write_preliminary_data(ofstream& fout);
    void write_cup_data(vector<PlayerScore>& cup_data, ofstream& fout);
    
    //--------------------------------------------------------------------------
    // PRIVATE MEMBER VARIABLES
    //--------------------------------------------------------------------------
    unordered_map<string, PlayerData> player_data;
    vector<vector<PlayerScore>> cup_data;
};


#endif /* rankings_h */
