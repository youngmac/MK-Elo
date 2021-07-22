//
//  Utility.h
//  Mario_Kart_Rankings
//
//  Created by Mackenzie Young on 1/30/21.
//

#ifndef Utility_h
#define Utility_h

#include <vector>

using std::vector;

class Utility {
public:
    // returns percentile depending on a rating
    static double percentile(double rating);
    // maps points of mario kart matches to linearly standardized place
    static vector<double> mk_points_to_place;

};

#endif /* Utility_h */
