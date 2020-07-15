// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file reverse_search.hpp
    \brief reverse search, using "pull" as action reduces branching 
     factor significantly
	\author Yueh-Ting Chen (eopXD)
	\course Theory of Computer Game (TCG)
*/
#ifndef REVERSE_HPP
#define REVERSE_HPP
// C++
#include <cstdio>
// eopXD
#include "global.hpp"
#include "action.hpp"
#include "build.hpp"

void solve_reverse ( BASIC &t ) {
	build(t);
	int ans_cnt = 0;
	char ans_step[500];

	STATE now;
	while ( !t.qRev.empty() ) {
		now = t.qRev.front(); t.qRev.pop();
		//now.display(t.n, t.m, t.board_idx, 1);
		if ( now.state == t.starting.state ) {
			for ( int i=now.step_cnt-1; i>=0; i-- ) {
				ans_step[ans_cnt] = now.step[i];
				++ans_cnt;
			}
			break;
		}
		for ( int i=0; i<4; ++i ) {
			tway(t.n, t.m, t.board_idx, now, i, t.qRev, t.walkedRev, 1);
		}
		for ( int i=0; i<4; ++i ) {
			drag(t.n, t.m, t.board_idx, now, i, t.qRev, t.walkedRev, 1);
		}
		//getchar();
	}
	fprintf(stderr, "reverse-searched state: %d\n", SZ(t.walkedRev));

// output answer
	fprintf(stdout, "%d\n", ans_cnt);
	for ( int i=0; i<ans_cnt; ++i ) {
		fprintf(stdout, "%c", ans_step[i]);
	} fprintf(stdout, "\n");

	ans.push_back(SOLUTION(t.case_num, ans_cnt, ans_step));
	fprintf(stderr, "case %d done\n", t.case_num);
	
}
#endif
