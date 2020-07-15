// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file bi_direction_search.hpp
    \brief Bi-directional search 
	\author Yueh-Ting Chen (eopXD)
	\course Theory of Computer Game (TCG)
*/
#ifndef BI_DIR_SEARCH_HPP
#define BI_DIR_SEARCH_HPP
// C++
#include <cstdio>
// eopXD
#include "global.hpp"
#include "action.hpp"
#include "build.hpp"

// calls build_bi_dir(), then do bi-directional search
void solve_bi_directional ( BASIC &t) { // bi-directional search
// discuss
// since we only obtain hashmaps of the state visited
// we would need to "regain" half of the solution when 2 ends meet
// we more likely to favor the way with smaller branching factor
// therefore we would like to meet at half way of the reverse search
// and then begin another reverse search on the middle we meet
	build(t);
	int ans_cnt = 0;
	char ans_step[500];
	
	
	STATE now, middle;
	
	for ( int i=0; i<4; ++i ) {
	// the for-loop act as a layered-approach which may save some time
		while ( !t.qFront.empty() or !t.qRev.empty() ) {
			if ( (t.qRev.empty() 
				or SZ(t.walkedFront)<=SZ(t.walkedRev)) 
				and !t.qFront.empty() ) { // search front
				now = t.qFront.front(); t.qFront.pop();
				//fprintf(stderr, "front search\n");
				//now.display(0);
				/**********************  NOTE-ABLE  **********************/
				/* may be better to just avoid break here! */
				/*
				if ( walkedRev.find(now.state) != walkedRev.end() ) { // meet
				// less favor to break here
					middle = now;
					flag_meet = 0;
					break;
				}
				*/
				for ( int i=0; i<4; ++i ) {
					tway(t.n, t.m, t.board_idx, now, i, t.qFront, 
						t.walkedFront, 0);
				}
				for ( int i=0; i<4; ++i ) {
					drag(t.n, t.m, t.board_idx, now, i, t.qFront, 
						t.walkedFront, 0);
				}
			}
			else { // search reverse
				now = t.qRev.front(); t.qRev.pop();
				//fprintf(stderr, "reverse search\n");
				//now.display(0);
				if ( t.walkedFront.find(now.state) != t.walkedFront.end() ) {
				// more favorable to break here
					middle = now;
					//flag_meet = 1;
					break;
				}
				for ( int i=0; i<4; ++i ) {
					tway(t.n, t.m, t.board_idx, now, i, t.qRev, t.walkedRev, 1);
				}
				for ( int i=0; i<4; ++i ) {
					drag(t.n, t.m, t.board_idx, now, i, t.qRev, t.walkedRev, 1);
				}
			}
		}
		/* debug
		if ( flag_meet == -1 ) {
			fprintf(stderr, "does not meet OAO\n");
			exit(1);
		}
		middle.display(1);*/
		
		fprintf(stderr, "front-searched state: %d\n", SZ(t.walkedFront));
		fprintf(stderr, "reverse-searched state: %d\n", SZ(t.walkedRev));
		
		initialize(t);
		for ( int i=0; i<middle.step_cnt; ++i ) {
			ans_step[ans_cnt] = middle.step[i];
			++ans_cnt;
		}
		middle.step_cnt = 0;		

		t.qFront.push(t.starting);
		t.qRev.push(middle);
	}

	// complete the rest of the search
	while ( !t.qRev.empty() ) {
		now = t.qRev.front(); t.qRev.pop();
		if ( now.state == t.starting.state ) { // found starting position
			for ( int i=0; i<now.step_cnt; ++i ) {
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
	}
	fprintf(stderr, "front-searched state: %d\n", SZ(t.walkedFront));
	fprintf(stderr, "reverse-searched state: %d\n", SZ(t.walkedRev));

	for ( int i=0; i<ans_cnt/2; ++i ) {
		std::swap(ans_step[i], ans_step[ans_cnt-1-i]);
	}

// output answer
	fprintf(stdout, "%d\n", ans_cnt);
	for ( int i=0; i<ans_cnt; ++i ) {
		fprintf(stdout, "%c", ans_step[i]);
	} fprintf(stdout, "\n");

	ans.push_back(SOLUTION(t.case_num, ans_cnt, ans_step));
	fprintf(stderr, "case %d done\n", t.case_num);
}
#endif