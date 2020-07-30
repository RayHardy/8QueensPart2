//
//  main.cpp
//  8QueensPart2
//
//  Created by Raymon Hardy on 4/12/18.
//  Copyright © 2018 Raymon Hardy. All rights reserved.
//

#include <iostream>
#include <list>
using namespace std;
#include "8QueensPart2.hpp"

int main( int argc, const char * argv[] )
{
    Player p;
    Board b;
    
    b.print();
    
    int i;
    for( i = b.get_cols()-1; i>=0; i-- )
    {
        p.give_piece( new Queen (i) );
    }
    
    p.place_piece_1( b );
    
    cout << "Player found " << p.get_solutions() << " solutions." << endl;
    
    char c;
    cin >> c;
}
