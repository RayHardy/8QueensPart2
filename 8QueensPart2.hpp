//
//  8QueensPart2.hpp
//  8QueensPart2
//
//  Created by Raymon Hardy on 4/12/18.
//  Copyright © 2018 Raymon Hardy. All rights reserved.
//

//#ifndef _QueensPart2_hpp
#define _QueensPart2_hpp

#include <stdio.h>

#pragma once

#include <vector>
#include <stack>

using namespace std;

#define BOARD_SIZE 8

typedef vector<int> queen_vec_t;

class Board;

class ChessPiece {
public:
    
    enum
    {
        PAWN = 1,
        ROOK,
        KNIGHT,
        BISHOP,
        QUEEN,
        KING
    };
    
    ChessPiece( int type, int col ) : _type(type), _column( col ) {}
    
    int returnColumn() const { return _column; }
    int returnRow()    const { return _row; }
    int returnType()   const { return _type; }
    
    virtual int mask_board( Board & b, const int i, const int j ) = 0;
    
protected:
    int         _row;
    const int   _column;
    const int   _type;
};

class Board
{
public:
    typedef vector<int>       row_vec_t;
    typedef vector<row_vec_t> col_mat_t;
    
    Board(const size_t rows = 8, const size_t cols = 8) { initizlizeBoard( rows, cols ); }
    
    bool allowedMove( const int i, const int j );
    
    bool addPiece( ChessPiece & p, int i, int j );
    
    void initizlizeBoard( const size_t rows = 8, const size_t cols = 8, const int val = 0 );
    bool verifySolution( const int count = 8 ) { return _inserted == count; }
    void print();
    
    int  get( const int i, const int j ) const;
    bool set( const int i, const int j, int v );
    int  get_inserted() const { return _inserted; }
    int  get_cols()     const { return _rows; }
    int  get_rows()     const { return _cols; }
    
protected:
    int       _inserted;
    col_mat_t _v;
    int       _rows;
    int       _cols;
};

class Player
{
public:
    
    Player() : _solutions(0) {}
    ~Player() { _deallocate(); }
    
    
    int get_solutions() const { return _solutions; }
    void add_solution()       { _solutions++; }
    
    bool place_piece_1( Board b );
    bool place_piece_2( Board b );
    
    void give_piece( ChessPiece * p );
    
    void undo();
    
    void reset();
    
protected:
    stack< ChessPiece * > _source;
    stack< ChessPiece * > _used;
    int                   _solutions;
    
    ChessPiece * _get_next_piece()
    {
        ChessPiece * p = _source.top();
        _source.pop();
        _used.push( p );
        
        return p;
    }
    
    void _deallocate();
    
};


class Queen : public ChessPiece
{
public:
    
    // constructor
    Queen( int );
    
    virtual int mask_board( Board & b, const int i, const int j );
};

class Rook : public ChessPiece
{
public:
    
    // constructor
    Rook( int );
    
    virtual int mask_board( Board & b, const int i, const int j );
};

class Knight : public ChessPiece
{
public:
    
    // constructor
    Knight( int );
    
    virtual int mask_board( Board & b, const int i, const int j );
};

class Bishop : public ChessPiece
{
public:
    
    // constructor
    Bishop( int );
    
    virtual int mask_board( Board & b, const int i, const int j );
};
