//
//  Utility.cpp
//  Mario_Kart_Rankings
//
//  Created by Mackenzie Young on 1/30/21.
//

#include <vector>
#include <cmath>

#include "Utility.h"

using namespace std;

double Utility::percentile(double rating) {
    double z_score = (rating - 600) / 875;
    return erfc(-1 * z_score /sqrt(2))/2;
}

// constant
vector<double> Utility::mk_points_to_place = {
    0,
    0.25,
    0.5,
    0.75,
    1,
    1.25,
    1.5,
    1.75,
    2,
    2.25,
    2.5,
    2.75,
    3,
    3.25,
    3.5,
    3.75,
    4,
    4.25,
    4.5,
    4.75,
    5,
    5.25,
    5.5,
    5.75,
    6,
    6.25,
    6.5,
    6.75,
    7,
    7.25,
    7.5,
    7.75,
    8.0,
    8.25,
    8.5,
    8.75,
    9,
    9.25,
    9.5,
    9.75,
    10,
    10.1583,
    10.3028,
    10.4365,
    10.5616,
    10.6794,
    10.7913,
    10.8979,
    11,
    11.0981,
    11.1926,
    11.2839,
    11.3723,
    11.4580,
    11.5414,
    11.6225,
    11.7016,
    11.7787,
    11.8541,
    11.9278,
    12
};
