## Sokoban Solver

ASSETS:

- `global.hpp`: global variables declaration

DOMAIN: 

- `board_state.hpp`: structure for saving current board state
- `action.hpp`: `drag()` and `pull()`

PREPROCESS:

- `build.hpp`: build for candidates of searching and terminal positions of 
	search (both front search and reverse search)

SOLVE METHODS:

- `normal_search.hpp`
- `reverse_search.hpp`
- `bi_directional_search.hpp`

UTILITY:

- `util.hpp`: utility functions (ex: `input()`, `out()`, `clear()`)

MULTI-THREADING:

- `ctpl_stl.h`: from [here](https://github.com/vit-vit/CTPL)

