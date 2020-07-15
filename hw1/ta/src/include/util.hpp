// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file util.hpp
    \brief utility functions
	\author Yueh-Ting Chen (eopXD)
	\course Theory of Computer Game (TCG)
*/
#ifndef UTIL_HPP
#define UTIL_HPP
// C++
#include <cstdio>
// STL
#include <algorithm>
// eopXD
#include "global.hpp"

/* clearing STL containers */
template <class CONTAINER> void clear ( CONTAINER &jizz ) {
	CONTAINER empty;
	std::swap(jizz, empty);
}

/* input */ 
bool input ( BASIC &t ) {
	int res = scanf("%d%d",&t.n, &t.m);
	if ( res != 2 ) {
		return (0);
	}
	t.board_idx = board_cnt;
	for ( int i=0; i<t.n; ++i ) {
		scanf("%s", global_board[t.board_idx][i]);
	}
	board_cnt = (board_cnt+1)%15;
	return (1);
}
void parse_input () {
	while ( input(testdata[total_testcase]) ) {
		testdata[total_testcase].case_num = total_testcase;
		++total_testcase;
	}
}

/* out of bound */ 
bool out ( int n, int m, int x, int y ) { 
	return (x<0 or y<0 or x>=n or y>=m); 
}

/* output solution to stdout */
void output () {
	sort(ans.begin(), ans.end());
	for ( int i=0; i<total_testcase; ++i ) {
//		fprintf(stderr, "case %d\n", ans[i].case_num);
		fprintf(stdout, "%d\n", ans[i].total_step);
		for ( int j=0; j<ans[i].total_step; ++j ) {
			fprintf(stdout, "%c", ans[i].step[j]);
		}fprintf(stdout, "\n");
	}
}

#endif