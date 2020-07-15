// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file board_state.hpp
    \brief board state, global board variable, note that type definitions at
     the very bottom.
	\author Yueh-Ting Chen (eopXD)
	\course Theory of Computer Game (TCG)
*/
#ifndef BOARD_STATE_HPP
#define BOARD_STATE_HPP
// STL
#include <vector>
#include <queue>
#include <unordered_map>

typedef unsigned long long ULL;

int board_cnt;
char global_board[15][105][105];

/* Board State */
// 63~14: box status on the board
// 7~4: player x coordinate
// 3~0: player y coordinate
struct board_state {
	ULL state;
	int step_cnt;
	char step[400];
	// NULL constructor
	board_state () {
		state = 0, step_cnt = 0;
	}
	// inherit board state
	board_state ( ULL _state ) {
		state = _state, step_cnt = 0;
	}
	// initialize board state with player position(x,y) and box positions
	board_state ( int m, int x, int y, int *box_x, int *box_y, int box_num ) {
		state = 0, step_cnt = 0;
		for ( int i=0; i<box_num; ++i ) {
			ULL where = 63-(box_x[i]*m+box_y[i]);
			state |= 1ULL<<(where);
		}	
		//assert(x<=15 and y<=15);
		state |= x<<4;
		state |= y;
	}
	// initialize with _board (2D)
	board_state ( ULL n, ULL m, int board_idx ) {
		//assert(n<=15 and m<=15 and n*m<=50);
		state = 0, step_cnt = 0;
		int now = 63;
		for ( ULL i=0; i<n; ++i ) {
			for ( ULL j=0; j<m; ++j ) {
				if ( global_board[board_idx][i][j]=='$' 
					or global_board[board_idx][i][j]=='*' ) { // box
					state |= 1ULL<<now;
				}
				if ( global_board[board_idx][i][j]=='@' 
					or global_board[board_idx][i][j]=='+' ) { // player
					state |= i<<4;
					state |= j;
				}
				now--;
			}
		}
	}
	void move_player ( int x, int y ) {
		//assert(x<=15 and y<=15);
		state >>= 8;
		state <<= 8;
		state |= x<<4;
		state |= y;
	}
	void move_box ( int m, int sx, int sy, int ex, int ey ) {
		//assert(sx<=15 and sy<=15 and ex<=15 and ey<=15);
		int from = 63-(sx*m+sy);
		int to = 63-(ex*m+ey);
		state ^= 1ULL<<from;
		state |= 1ULL<<to;
	}
	bool occupied ( int m, int x, int y ) { // if (x,y) is occupied by box
		return ((state&(1ULL<<(63-(x*m+y)))) > 0);
	}
	void player ( int *player_x, int *player_y ) {
		*(player_x) = (state&240ULL)>>4;
		*(player_y) = (state&15ULL);
	}
	void box ( int n, int m, int &box_num, int *box_x, int *box_y ) { 
	// stores into parameters passed in (_box_num, _box_x, _box_y)
		box_num = 0;
		int now = 63;
		for ( int i=0; i<n; ++i ) {
			for ( int j=0; j<m; ++j ) {
				if ( state&(1ULL<<now) ) {
					box_x[box_num] = i;
					box_y[box_num] = j;
					++box_num;
				}
				now--;
			}
		}
	}
	void display_bitstring ( int n, int m ) { // display bitstring of state
		int now = 63;
		for ( int i=0; i<n; ++i ) {
			for ( int j=0; j<m; ++j ) {
				fprintf(stderr, "%d", bool(state&(1ULL<<now)));
				now--;
			}
			fprintf(stderr, "\n");
		}
		fprintf(stderr, "player: ");
		for ( int i=7; i>=0; i-- ) {
			fprintf(stderr, "%d", bool(state&(1ULL<<i)));
			if ( i==4 ) {
				fprintf(stderr, " ");
			}
		}
		fprintf(stderr, "\n");
	}
	void display ( int n, int m, int board_idx, bool rev ) { 
		int x, y;
		player(&x, &y);
		fprintf(stderr, "player: %d %d\n", x, y);
		display_bitstring(n, m);
		char display_board[105][105] = {};
		int now = 63;
		for ( int i=0; i<n; ++i ) {
			for ( int j=0; j<m; ++j ) {
				if ( global_board[board_idx][i][j] == '#' ) {
					display_board[i][j] = '#';
				}
				else if ( global_board[board_idx][i][j]=='.' 
					or global_board[board_idx][i][j]=='+') {
					if ( (state&(1ULL<<now)) > 0 ) {
						display_board[i][j] = '*';
					}
					else {
						display_board[i][j] = '.';
					}
				}
				else if ( (state&(1ULL<<now)) > 0 ) {
					if ( global_board[board_idx][i][j]=='+' 
						or global_board[board_idx][i][j]=='*' ) {
						display_board[i][j] = '*';
					}
					else {
						display_board[i][j] = '$';
					}
				} 
				else {
					display_board[i][j] = '-';
				}
				now--;
			}
		}
		if ( global_board[board_idx][x][y]=='.' 
			or global_board[board_idx][x][y]=='+' 
			or global_board[board_idx][x][y]=='*' ) {
			display_board[x][y] = '+';
		}
		else {
			display_board[x][y] = '@';
		}
		fprintf(stderr, "step: %d\n", step_cnt);
		if ( rev ) {
			for ( int i=step_cnt-1; i>=0; i-- ) {
				fprintf(stderr, "%c", step[i]);
			}
		}
		else {
			for ( int i=0; i<step_cnt; ++i ) {
				fprintf(stderr, "%c", step[i]);
			}
		}
		fprintf(stderr, "\n");
		for ( int i=0; i<n; ++i ) {
			fprintf(stderr, "%s\n", display_board[i]);
		}
		fprintf(stderr, "================================================\n");
	}
};
typedef board_state STATE;
typedef std::vector<STATE> VECTOR;
typedef std::queue<STATE> QUEUE;
typedef std::unordered_map<ULL,int> HASHMAP;
#endif
