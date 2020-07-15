// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file build.hpp
    \brief call build() to construct candidates for searching. Both candidates
     from front search and reverse search are built.
	\author Yueh-Ting Chen (eopXD)
	\course Theory of Computer Game (TCG)
*/
#ifndef BUILD_HPP
#define BUILD_HPP
// eopXD
#include "global.hpp"
#include "util.hpp"

// clear assets
void initialize ( BASIC &t ) {
	clear(t.ending);
	clear(t.goal_state);
	clear(t.qFront);
	clear(t.qRev);
	clear(t.walkedFront);
	clear(t.walkedRev);
}
// acquire informations from board
void gain_asset ( BASIC &t ) {
// get starting position
	for ( int i=0; i<t.n; ++i ) {
		for ( int j=0; j<t.m; ++j ) {
			if ( global_board[t.board_idx][i][j]=='+' 
				or global_board[t.board_idx][i][j]=='@' ) {
				t.player_sx = i;
				t.player_sy = j;
			}
		}
	}	
// get starting boxes
	t.box_num = 0;
	for ( int i=0; i<t.n; ++i ) {
		for ( int j=0; j<t.m; ++j ) {
			if ( global_board[t.board_idx][i][j]=='$' 
				or global_board[t.board_idx][i][j]=='*' ) {
				t.box_sx[t.box_num] = i;
				t.box_sy[t.box_num] = j;
				++t.box_num;
			} 
		}
	}
// get ending box positions
	t.box_num = 0;
	for ( int i=0; i<t.n; ++i ) {
		for ( int j=0; j<t.m; ++j ) {
			if ( global_board[t.board_idx][i][j]=='+' 
				or global_board[t.board_idx][i][j]=='*' 
				or global_board[t.board_idx][i][j]=='.' ) {
				t.box_ex[t.box_num] = i;
				t.box_ey[t.box_num] = j;
				++t.box_num;
			}
		}
	}
}
// construct candidate for 2-way search
void build ( BASIC &t ) {
	initialize(t);
	gain_asset(t);

// initial position will be candidate for qFront
	t.starting = STATE(t.n, t.m, t.board_idx);
	t.qFront.push(t.starting);
	t.walkedFront[t.starting.state] = 1;
	/* debug
	fprintf(stderr, "***** starting position *****\n");
	t.starting.display(t.n, t.m, t.board, 0);*/

// generate all  possible terminal positions as candidate of qRev
	for ( int i=0; i<t.box_num; ++i ) {
		int x = t.box_ex[i];
		int y = t.box_ey[i];
		for ( int j=0; j<4; ++j ) {
			int xx = x+dx[j];
			int yy = y+dy[j];
			if ( out(t.n, t.m, xx, yy) 
				or global_board[t.board_idx][xx][yy]=='#' 
				or global_board[t.board_idx][xx][yy]=='+' 
				or global_board[t.board_idx][xx][yy]=='.' 
				or global_board[t.board_idx][xx][yy]=='*' ) {
				continue;
			}
			STATE goal=STATE(t.m, xx, yy, t.box_ex, t.box_ey, t.box_num);
			//puts("goal");
			if ( t.goal_state.find(goal.state) == t.goal_state.end() ) {
				//goal.display(t.n, t.m, t.board_idx, 0);
				t.ending.push_back(goal);
				t.goal_state[goal.state] = 1;
				t.qRev.push(goal);
				t.walkedRev[goal.state] = 1;
			}
		}
	}
	/* debug
	fprintf(stderr, "****** ending position ******\n");
	for ( int i=0; i<SZ(t.ending); ++i ) {
		t.ending[i].display(t.n, t.m, global_board[t.board_idx], 1);
	}*/
}
#endif
