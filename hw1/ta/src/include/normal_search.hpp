// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file normal_search.hpp
    \brief normal BFS
	\author Yueh-Ting Chen (eopXD)
	\course Theory of Computer Game (TCG)
*/
#ifndef NORMAL_HPP
#define NORMAL_HPP
// C++
#include <cstdio>
// eopXD
#include "global.hpp"
#include "action.hpp"
#include "build.hpp"

void solve_normal ( BASIC &t ) {
	build(t);
	int ans_cnt = 0;
	char ans_step[500];
	
	STATE now;
	while ( !t.qFront.empty() ) {
		now = t.qFront.front(); t.qFront.pop();
		//now.display(t.n, t.m, t.board_idx, 0);
		
		if ( t.goal_state.find(now.state) != t.goal_state.end() ) {
			for ( int i=0; i<now.step_cnt; ++i ) {
				ans_step[ans_cnt] = now.step[i];
				++ans_cnt;
			}
			break;
		}
		for ( int i=0; i<4; ++i ) {
			tway(t.n, t.m, t.board_idx, now, i, t.qFront, t.walkedFront, 0);
		}
		for ( int i=0; i<4; ++i ) {
			drag(t.n, t.m, t.board_idx, now, i, t.qFront, t.walkedFront, 0);
		}
		//getchar();
	}
	fprintf(stderr, "front-searched state: %d\n", SZ(t.walkedFront));

// output answer
	fprintf(stdout, "%d\n", ans_cnt);
	for ( int i=0; i<ans_cnt; ++i ) {
		fprintf(stdout, "%c", ans_step[i]);
	} fprintf(stdout, "\n");

	ans.push_back(SOLUTION(t.case_num, ans_cnt, ans_step));
	fprintf(stderr, "case %d done\n", t.case_num);
}
#endif