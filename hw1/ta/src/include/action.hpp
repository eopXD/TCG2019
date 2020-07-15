// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file action.hpp
    \brief drag() and tway() action, takes STATE and performs action and
     updates QUEUE and HASHMAP
	\author Yueh-Ting Chen (eopXD)
	\course Theory of Computer Game (TCG)
*/
#ifndef ACTION_HPP
#define ACTION_HPP

#include "global.hpp"
#include "util.hpp"
#include "board_state.hpp"

/* action: tway, drag */
// 0 = move fail
// 1 = move success

// try push, legal state will be pushed into QUEUE and add to HASHMAP
bool tway ( int n, int m, int board_idx, STATE &now, int dir, QUEUE &q, 
	HASHMAP &hmap, bool rev ) { // direction for dx[], dy[]
	bool box_moved = 0, ret = 0;
	int x, y, xx, yy, xxx, yyy;
	now.player(&x, &y); // current player
	xx = x+dx[dir]; // potential box position (player position after tway)
	yy = y+dy[dir];
	if ( out(n, m, xx, yy) or global_board[board_idx][xx][yy]=='#' ) { 
	// out of bound or meet wall
		return (0);
	}
	if ( now.occupied(m, xx, yy) ) { // exist box on (xx, yy)
		xxx = xx+dx[dir]; // box position after tway
		yyy = yy+dy[dir];
		if ( out(n, m, xxx, yyy) or now.occupied(m, xxx, yyy) or 
			global_board[board_idx][xxx][yyy]=='#' ) {
		// out of bound or there is another box or meet wall
			return (0);
		}
		// move box from (xx, yy) to (xxx, yyy)
		now.move_box(m, xx, yy, xxx, yyy);
		now.step[now.step_cnt] = push_box[rev][dir];
		box_moved = 1;
	}
	else {
		now.step[now.step_cnt] = walk[rev][dir];
	}
	++now.step_cnt;
	now.move_player(xx, yy);
	if ( hmap.find(now.state) == hmap.end() ) {
		q.push(now);
		hmap[now.state] = 1;
		ret = 1;
	} 
	now.step_cnt--;
	now.move_player(x, y);
	if ( box_moved ) {
		now.move_box(m, xxx, yyy, xx, yy);
	}
	return (ret);
}

bool drag ( int n, int m, int board_idx, STATE &now, int dir, QUEUE &q, 
	HASHMAP &hmap, bool rev ) { // direction for dx[], dy[]
	//puts("start drag!!!!!");
	int x, y; // current player (box position after drag)
	now.player(&x, &y);
	int xx = x+dx[dir]; // player position after drag
	int yy = y+dy[dir]; 
	if ( out(n, m, xx, yy) or now.occupied(m, xx, yy) or 
		global_board[board_idx][xx][yy]=='#' ) {
	// out of bound or there is another box or meet wall
		return (0);
	}
	// simply walking
	now.move_player(xx, yy);
	now.step[now.step_cnt] = walk[rev][dir];
	++now.step_cnt;
	if ( hmap.find(now.state) == hmap.end() ) {
		q.push(now);
		hmap[now.state] = 1;
	}
	now.step_cnt--;
	// check if box drag-able
	int _x = x-dx[dir]; // potential box position
	int _y = y-dy[dir];
	if ( !out(n, m, _x, _y) and now.occupied(m, _x, _y) ) {
	//	puts("drag!");
		now.step[now.step_cnt] = drag_box[rev][dir];
		++now.step_cnt;
		now.move_box(m, _x, _y, x, y);
		if ( hmap.find(now.state) == hmap.end() ) {
			q.push(now);
			hmap[now.state] = 1;
		}
		now.step_cnt--;
		now.move_box(m, x, y, _x, _y);
	}
	now.move_player(x, y);
	
	return (1);
}

#endif