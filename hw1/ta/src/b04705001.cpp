// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file b04705001.cpp
    \brief main file
	\author Yueh-Ting Chen (eopXD)
	\course Theory of Computer Game (TCG)
*/

// C++
#include <cstdio>
#include <ctime>
// STL
#include <vector>
// CTPL
#include "include/ctpl_stl.h"
// eopXD
#include "include/normal_search.hpp" // solve_normal(t)
#include "include/reverse_search.hpp" // solve_reverse(t)
#include "include/bi_direction_search.hpp" // solve_bi_directional(t)

/*void simple_solve () {
	parse_input();
	for ( int i=0; i<total_testcase; ++i ) {
		clock_t start = clock();
		solve_reverse(testdata[i]);
		fprintf(stderr, "spent time: %.3f sec\n", (double)(clock()-start)/CLOCKS_PER_SEC);
	}
}

void multi_solve ( int id, BASIC &t ) {
	clock_t start = clock();	
	solve_reverse(t);
	fprintf(stderr, "spent time: %.3f sec\n", (double)(clock()-start)/CLOCKS_PER_SEC);
}

#define THREAD_LIMIT 10
void multi_thread () {
	fprintf(stderr, "multi thread mode start\n");
	
	parse_input();
	ctpl::thread_pool *pool = new ctpl::thread_pool(2);
	for ( int i=0; i<total_testcase; ++i ) {
		pool->push(multi_solve, std::ref(testdata[i]));
	}
	delete pool;
}*/
int main ()
{
	clock_t begin_of_universe = clock();

/* batch mode */
	//simple_solve();
	//multi_thread();
	//goto THE_END;
/**************/

/* stream mode */
	while ( input(testdata[total_testcase]) ) {
		clock_t start = clock();
		testdata[total_testcase].case_num=total_testcase;

	/* methods to solve */
		//solve_normal(testdata[total_testcase]);
		//solve_reverse(testdata[total_testcase]);
		solve_bi_directional(testdata[total_testcase]);
	/********************/
		
		++total_testcase;
		fprintf(stderr, "spent time: %.3f sec\n", (double)(clock()-start)/CLOCKS_PER_SEC);
		//break;
	}
/**************/

//THE_END:
	//output();
	fprintf(stderr, "total spent time: %.3f sec\n", (double)(clock()-begin_of_universe)/CLOCKS_PER_SEC);
	return (0);
}
/*
Legend:
#: wall
@: player on non-goal
+: player on goal
$: box on non-goal
*: box on goal
.: goal square
-: non-goal square
*/
