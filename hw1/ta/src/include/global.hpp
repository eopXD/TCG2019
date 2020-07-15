// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file global.hpp
    \brief global variables (testdata input/output) and static things
	\author Yueh-Ting Chen (eopXD)
	\course Theory of Computer Game (TCG)
*/
#ifndef GLOBAL_HPP
#define GLOBAL_HPP
// eopXD
#include "basic.hpp"


// global variables
#define SZ(x) ((int)((x).size()))

// movement
static int dx[] = {1, -1, 0, 0};
static int dy[] = {0, 0, 1, -1};
// output characters
static char walk[2][6] = {"durl", "udlr"};
static char push_box[2][6] = {"DURL", "^v<>"};
static char drag_box[2][6] = {"v^><", "UDLR"};
//static char rev_walk[] = "udlr";
//static char drag_box[] = "UDLR";

int total_testcase;
BASIC testdata[20];
SOLUTION_VEC ans;
#endif