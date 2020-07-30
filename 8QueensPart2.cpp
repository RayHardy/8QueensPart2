//
//  8QueensPart2.cpp
//  8QueensPart2
//
//  Created by Raymon Hardy on 4/12/18.
//  Copyright © 2018 Raymon Hardy. All rights reserved.
//

#include "8QueensPart2.hpp"
#include <iostream>
#include <algorithm>
#include <iomanip>
using namespace std;

//Queen * Queen::lastQueen = nullptr;
static int qc = 0;

//--------------------------------------------------------------------------------------
// class Board
//--------------------------------------------------------------------------------------
void Board::initizlizeBoard( const size_t rows, const size_t cols, const int val )
{
    _v.resize( rows );
    
    for( int i = 0; i < _v.size(); i++ )
    {
        _v[i].resize( cols );
        for( int j = 0; j<_v[i].size(); j++ )
        {
            _v[i][j] = 0;
        }
    }
    _inserted = 0;
    
    _cols = cols;
    _rows = rows;
}

//--------------------------------------------------------------------------------------
bool Board::addPiece( ChessPiece & p, int i, int j )
{
    if( i >= _v.size() ) return false;
    if( j >= _v[i].size() ) return false;
    
    if( 0 != _v[i][j] )  return false;
    
    _v[i][j] = p.returnType();
    _inserted++;
    
    p.mask_board( *this, i, j );
    
    return true;
}

//--------------------------------------------------------------------------------------
int Board::get( const int i, const int j ) const
{
    if( i >= _v.size() ) return -1;
    if( j >= _v[i].size() ) return -1;
    
    return _v[i][j];
}

//--------------------------------------------------------------------------------------
bool Board::set( const int i, const int j, int v )
{
    if( i >= _v.size() ) return false;
    if( j >= _v[i].size() ) return false;
    
    bool rv = (_v[i][j] != v);
    _v[i][j] = v;
    
    return rv;
}

//--------------------------------------------------------------------------------------
void Board::print()
{
    for( int i = 0; i < _v.size(); i++ )
    {
        for( int j = 0; j<_v[i].size(); j++ )
        {
            int itmp = max( _v[i][j], -1 );
            
            char c = '?';
            switch( itmp )
            {
                case -1:  c = '.'; break;
                case  0:  c = ' '; break;
                case ChessPiece::PAWN: c='p'; break;
                case ChessPiece::ROOK: c = 'r'; break;
                case ChessPiece::KNIGHT: c = 'k'; break;
                case ChessPiece::QUEEN: c = 'Q'; break;
                case ChessPiece::KING: c = 'K'; break;
            }
            cout << c;
        }
        cout << endl;
    }
    cout << endl << endl;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
bool Board::allowedMove( int i, int j )
{
    if( i >= _v.size() ) return false;
    if( j >= _v[i].size() ) return false;
    
    return !_v[i][j];
}

//--------------------------------------------------------------------------------------
// class Player
//--------------------------------------------------------------------------------------

void Player::give_piece( ChessPiece * p )
{
    if( nullptr != p ) _source.push( p );
}

//--------------------------------------------------------------------------------------
void Player::undo()
{
    if( !_used.empty() )
    {
        ChessPiece * p = _used.top();
        _used.pop();
        
        if( nullptr != p ) _source.push( p );
    }
}

//--------------------------------------------------------------------------------------
void Player::reset()
{
    while( !_used.empty() )
    {
        ChessPiece * p = _used.top();
        _used.pop();
        
        if( nullptr != p ) _source.push( p );
    }
}

//--------------------------------------------------------------------------------------
void Player::_deallocate()
{
    while( !_source.empty() )
    {
        ChessPiece * p = _source.top();
        _source.pop();
        
        if( nullptr != p ) delete p;
    }
    
    while( !_used.empty() )
    {
        ChessPiece * p = _used.top();
        _used.pop();
        
        if( nullptr != p ) delete p;
    }
}

//--------------------------------------------------------------------------------------
bool Player::place_piece_1( Board b )
{
    if( _source.empty() || b.verifySolution() )
    {
        cout << "SUCCESS" << endl;
        b.print();
        add_solution();
        return true;
    }
    
    ChessPiece * p = _get_next_piece();
    
    int placed = 0;
    const int col = p->returnColumn();
    for( int i = 0; i<b.get_rows(); i++ )
    {
        if( b.allowedMove(i,col) )
        {
            Board c = b;
            c.addPiece( *p, i, col );
            
            place_piece_1( c );
            placed++;
        }
    }
    
    //if( 0 == placed )
    //{
    //    cout << "FAILED @ " << b.get_inserted() << endl;
    //    b.print();
    //}
    
    undo();
    
    return 0 != placed;
}

//--------------------------------------------------------------------------------------
bool Player::place_piece_2( Board b )
{
    if( _source.empty() || b.verifySolution() )
    {
        cout << "SUCCESS" << endl;
        b.print();
        add_solution();
        return true;
    }
    
    ChessPiece * p1 = _get_next_piece();
    ChessPiece * p2 = _get_next_piece();
    
    const int col1 = p1->returnColumn();
    const int col2 = p2->returnColumn();
    
    int placed = 0;
    for( int i = 0; i< b.get_rows(); i++)
    {
        for( int j = 0; j< b.get_cols(); j++ )
        {
            if( abs(i - j) < 1 ) continue;
            
            Board c = b;
            
            if( c.addPiece( *p1,  i, col1 ) && c.addPiece( *p2, j, col2 ) )
            {
                placed += 2;
                place_piece_2( c );
            }
        }
    }
    
    undo();
    undo();
    
    return true;
}

//--------------------------------------------------------------------------------------
// class Queen
//--------------------------------------------------------------------------------------
Queen::Queen( int col ) : ChessPiece( QUEEN, col )
{
    _row = 1;
}

//--------------------------------------------------------------------------------------
int Queen::mask_board( Board & b, const int i, const int j )
{
    int mask_cnt= 0;
    
    for( int r = 0; r < b.get_rows(); r++ )
    {
        for( int c = 0; c < b.get_cols(); c++ )
        {
            if ( ( r == i ) || (j == c) || (abs( r - i ) == abs( c - j )) )
            {
                if( 0 == b.get( r, c ) )
                {
                    b.set( r, c, -_type );
                    mask_cnt++;
                }
            }
        }
    }
    
    return mask_cnt;
}

//--------------------------------------------------------------------------------------
// class Rook
//--------------------------------------------------------------------------------------
Rook::Rook( int col ) : ChessPiece( ROOK, col )
{
    _row = 1;
}

//--------------------------------------------------------------------------------------
int Rook::mask_board( Board & b, const int i, const int j )
{
    int mask_cnt = 0;
    
    for( int r = 0; r < b.get_rows(); r++ )
    {
        for( int c = 0; c < b.get_cols(); c++ )
        {
            if( (r == i) || (j == c) )
            {
                if( 0 == b.get( r, c ) )
                {
                    b.set( r, c, -_type );
                    mask_cnt++;
                }
            }
        }
    }
    
    return mask_cnt;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// class Knight
//--------------------------------------------------------------------------------------
Knight::Knight( int col ) : ChessPiece( KNIGHT, col )
{
    _row = 1;
}

//--------------------------------------------------------------------------------------
int Knight::mask_board( Board & b, const int i, const int j )
{
    int mask_cnt = 0;
    
    for( int r = 0; r < b.get_rows(); r++ )
    {
        for( int c = 0; c < b.get_cols(); c++ )
        {
            if( (r == i) || (j == c) )
            {
                if( 0 == b.get( r, c ) )
                {
                    b.set( r, c, -_type );
                    mask_cnt++;
                }
            }
        }
    }
    
    return mask_cnt;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

// class Bishop
//--------------------------------------------------------------------------------------
Bishop::Bishop( int col ) : ChessPiece( BISHOP, col )
{
    _row = 1;
}

//--------------------------------------------------------------------------------------
int Bishop::mask_board( Board & b, const int i, const int j )
{
    int mask_cnt = 0;
    
    for( int r = 0; r < b.get_rows(); r++ )
    {
        for( int c = 0; c < b.get_cols(); c++ )
        {
            if( (r == i) || (j == c) )
            {
                if( 0 == b.get( r, c ) )
                {
                    b.set( r, c, -_type );
                    mask_cnt++;
                }
            }
        }
    }
    
    return mask_cnt;
}

