// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file basic.hpp
    \brief board state, global board variable, note that type definitions at
     the very bottom.
	\author Yueh-Ting Chen (eopXD)
	\course Theory of Computer Game (TCG)
*/
#ifndef BASIC_HPP
#define BASIC_HPP
// STL
#include <vector>
// eopXD
#include "board_state.hpp"

// structure containing all basic informations per test case
struct basic {
	// input
	int case_num;
	int n, m;
	int board_idx;
	// info
	int player_sx, player_sy;
	int box_num;
	int box_sx[105], box_sy[105];
	int box_ex[105], box_ey[105];
	// search candidates/terminal position
	STATE starting;
	VECTOR ending; // vector<STATE>
	HASHMAP goal_state;
	// for searching
	QUEUE qFront, qRev; // queue<STATE>
	HASHMAP walkedFront, walkedRev;
};
typedef basic BASIC;
// solution information
struct solution {
	int case_num;
	int total_step;
	char step[500];
	solution () {}
	solution ( int _case_num, int step_cnt, char *_step ) {
		case_num = _case_num;
		total_step = step_cnt;
		for ( int i=0; i<step_cnt; ++i ) {
			step[i] = _step[i];
		}
	}
	bool operator< ( const solution &rhs ) const {
		return (case_num < rhs.case_num);
	}
};
typedef solution SOLUTION;
typedef std::vector<SOLUTION> SOLUTION_VEC;
#endif