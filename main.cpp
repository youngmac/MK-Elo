//
//  main.cpp
//  Mario_Kart_Rankings
//
//  Created by Mackenzie Young on 1/25/21.
//

#include <iostream>
#include <string>

#include "rankings.h"

using namespace std;

int main(int argc, const char * argv[]) {
    string infile = "/Users/youngmac/Documents/Mario_Kart_Rankings/Mario_Kart_Rankings/data.txt";
    string outfile = "/Users/youngmac/Documents/Mario_Kart_Rankings/Mario_Kart_Rankings/747 Packard Definitive Mario Kart Rankings";
    KartRankings rankings(infile);
    rankings.calculate(outfile);
    return 0;
}
