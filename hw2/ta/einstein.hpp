// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file einstein.hpp
    \brief fast implementation, heavy bit operation.
    \author Yueh-Ting Chen (eopXD)
    \course Theory of Computer Game (TCG)
*/

#ifndef EINSTEIN_HPP
#define EINSTEIN_HPP

#include<array>
#include<algorithm>
#include<ostream>
#include<random>
#include<stdexcept>
#include<string>
#include<tuple>
#include<type_traits>
#include<utility>
#include<vector>

#include <fstream>
extern std::fstream flog;

template<int n_sides> class Die final{
    static_assert(n_sides>=2, "A die should contain at least 2 sides.\n");
private:
    static std::uniform_int_distribution<int> distribution;
    uint_fast64_t const _seed;
    mutable std::mt19937_64 random_generator;
public:
    // a die shouldn't be copied
    Die(Die const&) = delete;
    Die &operator =(Die const&) = delete;
    explicit Die(uint_fast64_t const &seed = std::random_device{}()): _seed(seed), random_generator(seed){}
    // make a roll
    int operator ()() const noexcept{
        return distribution(random_generator);
    }
    uint_fast64_t seed() const noexcept{
        return _seed;
    }
};
// the distribution of an n-sided die, U[1, n]
template<int n_sides> std::uniform_int_distribution<int> Die<n_sides>::distribution(1, n_sides);
using Coin = Die<2>;
using Die6 = Die<6>;
std::array<std::string, 7> const Die6_icon{{"", "⚀", "⚁", "⚂", "⚃", "⚄", "⚅"}};

// the color of a cube or a player
enum class Color{red, blue, other, draw};
// cast a scoped enum to its underlying type
template<typename E> typename std::underlying_type<E>::type constexpr Enum2int(E enumerator) noexcept{
    return static_cast<typename std::underlying_type<E>::type>(enumerator);
}
std::ostream &operator <<(std::ostream &os, Color const &c){
    return os << "rb_x"[Enum2int(c)];
}

std::array<std::string, 8> const Cube_icon{{"  ", "１", "２", "３", "４", "５", "６", "？"}};
// the class of cubes
class Cube final{
private:
    //  0:  red 1;  1:  red 2;  2:  red 3;  3:  red 4;  4:  red 5;  5:  red 6;
    //  6: blue 1;  7: blue 2;  8: blue 3;  9: blue 4; 10(a): blue 5; 11(b): blue 6;
    // 12(c):  red ?; 13(d): blue ?; 14(e): otherwise
    int datum = 14;
    explicit Cube(int const &x) noexcept: datum(x){}
public:
    Cube() noexcept = default;
    Cube(Color c, int no=0) noexcept{
        if(c == Color::other){
            datum = 14;
        }else if(no<1 || no>6){
            datum = 12+Enum2int(c);
        }else if(c == Color::red){
            datum = no-1;
        }else datum = no+5;
    }
    // whether (*this) is a valid cube
    explicit operator bool() const noexcept{
        return datum <= 11;
    }
    bool operator ==(Cube const &b) const noexcept{
        return datum == b.datum;
    }
    bool operator !=(Cube const &b) const noexcept{
        return datum != b.datum;
    }
    Color color() const noexcept{
        if(datum >= 12){
            return static_cast<Color>(datum&3);
        }
        return static_cast<Color>(datum>=6);
    }
    // if (*this) is not valid, return 0
    int no() const noexcept{
        if(datum >= 12){
            return datum==14? 0: 7;
        }
        return (datum<=5)? (datum+1): (datum-5);
    }
    friend class Board;
};
std::ostream &operator <<(std::ostream &os, Cube const &c){
    if(c.color() == Color::other){
        return os << "--";
    }
    os << c.color();
    if(c.no() == 7){
        return os << '?';
    }
    return os << c.no();
}
// the class of squares aka positions on the board
class Square final{
private:
    // 36: not-a-square
    int datum = 36;
    explicit Square(int const &x) noexcept: datum(x){}
public:
    Square() noexcept = default;
    Square(int const &x, int const &y) noexcept{
        if(x<1 || x>6 || y<1 || y>6){
            datum = 36;
        }else datum = 6*(x-1)+(y-1);
    }
    // whether (*this) is a valid square
    explicit operator bool() const noexcept{
        return datum != 36;
    }
    bool operator ==(Square const &b) const noexcept{
        return datum == b.datum;
    }
    bool operator !=(Square const &b) const noexcept{
        return datum != b.datum;
    }
    // if (*this) is not valid, return 0
    int x() const noexcept{
        if(datum == 36){
            return 0;
        }
        return (datum/6+1);
    }
    int y() const noexcept{
        if(datum == 36){
            return 0;
        }
        return (datum%6+1);
    }
    friend class Board;
};
std::ostream &operator <<(std::ostream &os, Square const &p){
    if(p) return os << '(' << p.x() << ", " << p.y() << ')';
    return os << "--";
}

// the class recording a board
class Board{
//private:
public:
    using ull = unsigned long long;
    static std::array<int, 6> const init_pos_i;
    /* 5x5 version (LSB) */
    // a board is represented in 3 64-bit unsigned integers
    // std::get<0>(data): the Cube on Square 0-15
    // std::get<1>(data): the Cube on Square 16-24, the number of red/blue cubes, and the winner
    // std::get<2>(data): the Position of Cube 0-11, turn(), is_even_turn() (each position needs 5 bits)
    //std::tuple<ull, ull, ull> data{0xedeeeceeecceeccc, 0x200dddeeddee, 0xce739ce739ce739};
    
    /* 6x6 version (LSB) */
    // a board is represented in 5 64-bit unsigned integers
    // std::get<0>(data): the Cube on Square 0-15
    // std::get<1>(data): the Cube on Square 16-31
    // std::get<2>(data): the Cube on Square 32~35, the number of red/blue cubes, and the winner
    // std::get<3>(data): the Position of Cube 0~9
    // std::get<4>(data): the Position of Cube 10~11, turn(), is_even_turn()
    std::tuple<ull, ull, ull, ull, ull> data{0xeeeceeeecceeeccc, 0xeeddeeeedeeeeeee, 0x200ddde, 0x924924924924924, 0x924};

    //int get_cube_i(int const &pos) const noexcept{
    //    return ((pos>>4&1)? std::get<1>(data): std::get<0>(data)) >> ((pos&15)<<2) & 15;
    //}
    int get_cube_i ( int const &pos ) const noexcept { // get cube of the position
        if ( pos >= 32 ) {
            ull r = std::get<2>(data);
            return ((r >> ((pos&15)<<2))&15);
        } else if ( pos >= 16 ) {
            ull r = std::get<1>(data);
            return ((r >> ((pos&15)<<2))&15);
        } else {
            ull r = std::get<0>(data);
            return ((r >> ((pos&15)<<2))&15);
        }
    }
    //int get_pos_i(int const &cube) const noexcept{
    //    return std::get<2>(data) >> (5*cube) & 31;
    //}
    int get_pos_i ( int const &cube ) const noexcept {
        if ( cube>9 ) {
            return ((std::get<4>(data)>>(6*(cube-10)))&63);
        }
        else {
            return ((std::get<3>(data)>>(6*cube))&63);
        }
        return ((std::get<4>(data)>>(6*cube-10*(cube>9))&63));
    }
    //int get_num_i(int const &color) const noexcept{
    //    return std::get<1>(data) >> ((9+color)<<2) & 15;
    //}
    int get_num_i ( int const &color ) const noexcept { // get number of cubes for color
        return ((std::get<2>(data) >> ((4+color)<<2)) & 15);
    }
    //int get_ply_i() const noexcept{
    //    return std::get<2>(data) >> 60;
    //}
    int get_ply_i () const noexcept { // get whose turn it is
        return (std::get<4>(data)>>12);
    }
    //int get_winner_i() const noexcept{
    //    return std::get<1>(data) >> 44 & 15;
    //}
    int get_winner_i () const noexcept { // get winner status
        return (std::get<2>(data)>>24&15);
    }
    //void assign_cube_i(int const &pos, int const &cube) noexcept{ 
    //    ull &r = (pos>>4&1)? std::get<1>(data): std::get<0>(data);
    //     r = (r & ~(15ull<<((pos&15)<<2))) | (static_cast<ull>(cube)<<((pos&15)<<2));
    //}
    void assign_cube_i ( int const &pos, int const &cube ) noexcept { // assign cube to given position
        if ( pos >= 32 ) {
            ull &r = std::get<2>(data);
            r = (r & ~(15ull<<((pos&15)<<2))) | (static_cast<ull>(cube)<<((pos&15)<<2));
        } else if ( pos >= 16 ) {
            ull &r = std::get<1>(data);
            r = (r & ~(15ull<<((pos&15)<<2))) | (static_cast<ull>(cube)<<((pos&15)<<2));
        } else {
            ull &r = std::get<0>(data);
            r = (r & ~(15ull<<((pos&15)<<2))) | (static_cast<ull>(cube)<<((pos&15)<<2));
        }
    }
    //void assign_pos_i(int const &cube, int const &pos) noexcept{
    //    std::get<2>(data) = (std::get<2>(data) & ~(31ull<<(5*cube))) | (static_cast<ull>(pos)<<(5*cube));
    //}
    void assign_pos_i ( int const &cube, int const &pos ) noexcept {  // assign position to cube

        ull &r = (cube > 9) ? std::get<4>(data) : std::get<3>(data);
        r = (r & ~( 63ull << 6*(cube-10*(cube>9)))) | (static_cast<ull>(pos)<<6*(cube-10*(cube>9)));
    }
    //void increment_num_i(int const &color, int const &dn) noexcept{
    //    std::get<1>(data) = (std::get<1>(data) & ~(15ull<<((9+color)<<2))) | (static_cast<ull>(get_num_i(color)+dn)<<((9+color)<<2));
    //}
    void increment_num_i ( int const &color, int const &dn ) noexcept { // add number of cube for player of the color
        std::get<2>(data) = (std::get<2>(data) & ~(15ull<<((4+color)<<2))) | (static_cast<ull>(get_num_i(color)+dn)<<((4+color)<<2));
    }
    //void chg_ply_i() noexcept{
    //    std::get<2>(data) = (std::get<2>(data) + (1ull<<60)) & ((1ull<<62)-1);
    //}
    void chg_ply_i () noexcept { // flip turn() bit
        ull &r = std::get<4>(data);
        r = (r+(1ull<<12)) & ((1ull<<14)-1);
    }
    //void assign_winner_i(int const &color) noexcept{
    //    std::get<1>(data) = (std::get<1>(data) & ~(15ull<<44)) | (static_cast<ull>(color)<<44);
    //}
    void assign_winner_i ( int const &color ) noexcept {
        std::get<2>(data) = (std::get<2>(data) & ~(15ull<<24)) | (static_cast<ull>(color)<<24);
    }
    //int new_pos_i(int const &cube, int const &dir) const noexcept{
    //    if(get_winner_i()!=2 || cube>=12 || dir<1 || dir>3) return 25;
    //    int p = get_pos_i(cube);
    //    if(p == 25) return 25;
    //    int p2 = Square((p/5+1)+dx[cube>=6][dir], (p%5+1)+dy[cube>=6][dir]).datum;
    //    if(p2 == 25) return 25;
    //    int c2 = get_cube_i(p2);
    //    if(c2<=11 && (c2>=6)==(cube>=6)) return 25; // don't allow eating pieces of own
    //    return p2;
    //}
    int new_pos_i ( int const &cube, int const &dir ) const noexcept {
        if ( get_winner_i()!=2 or cube>=12 or dir<1 or dir>3 ) {
            return (36);
        }
        int p = get_pos_i(cube);
        if ( p == 36 ) {
            return (36);
        }
        int p2 = Square((p/6+1)+dx[cube>=6][dir], (p%6+1)+dy[cube>=6][dir]).datum;
        if ( p2 == 36 ) return (36);
        // allow to eat peices of own
        return (p2);
    }

    
public:
    static std::array<std::array<Square, 6>, 2> const init_pos;
    static std::array<std::array<int, 4>, 2> const dx, dy;
    static bool check_arrangement(std::string const &s) noexcept{
        if(s.size() != 6ull){
            return false;
        }
        unsigned char used = 0;
        for(int i=0; i<=5; ++i){
            if(s[i]<'1' || s[i]>'6' || used>>(s[i]-'1')&1){
                return false;
            }
            used &= 1<<(s[i]-'1');
        }
        return true;
    }
    Board() noexcept = default;
    Board(std::string const &red, std::string const &blue){
        if(!check_arrangement(red) || !check_arrangement(blue)){
            throw std::runtime_error("Board::Board(): invalid initial board arrangement");
        }
        for(int i=0; i<=5; ++i){ // red
            assign_cube_i(init_pos_i[i], red[i]-'1');
            assign_pos_i(red[i]-'1', init_pos_i[i]);
        }
        for(int i=0; i<=5; ++i){ // blue
            assign_cube_i(35-init_pos_i[i], 6+blue[i]-'1');
            assign_pos_i(6+blue[i]-'1', 35-init_pos_i[i]);
        }
        increment_num_i(0, 6);
        increment_num_i(1, 6);
    }
    Board(std::string const &arr): Board(arr, arr){}
    virtual ~Board() = default;
    // find the cube on a given square
    Cube operator ()(Square const &p) const noexcept{ // revieces Square() from below
        if(!p) {
            return {};
        }else {
            return static_cast<Cube>(get_cube_i(p.datum));
        }
    }
    Cube operator ()(int const &x, int const &y) const noexcept{ // send to Cube operator ()
        return (*this)(Square(x, y));
    }
    // find the position of a given cube
    Square operator ()(Cube const &c) const noexcept{
        if(!c) return {};
        return static_cast<Square>(get_pos_i(c.datum));
    }
    Square operator ()(Color const &c, int const &no) const noexcept{
        return (*this)(Cube(c, no));
    }
    Square operator ()(int const &no) const noexcept{
        return (*this)(Cube(turn(), no));
    }
    // get the number of cubes in color c
    int num_cubes(Color const &c) const{
        if(c == Color::other){
            throw std::runtime_error("Board::num_cube(): the Color can't be Color::other");
        }
        return get_num_i(Enum2int(c));
    }
    // return whose turn the current board is
    Color turn() const noexcept{
        return Color(get_ply_i()&1);
    }
    // return whether the current board is in an even turn
    bool is_even_turn() const noexcept{
        return get_ply_i()>>1;
    }
    // get the winner; if no one have won, return Color::other
    Color winner() const noexcept{
        return Color(get_winner_i());
    }
    // return the new position of cube no if the direction dir is valid
    Square new_pos(int const &no, int const &dir) const noexcept{
        if(no<1 || no>6 || dir<1 || dir>3) return {};
        int ply = get_ply_i();
        // can move any pieces
        //if(ply>>1){ // even turn
        //    if(no&1) return {};
        //}else{ // odd turn
        //    if(!(no&1)) return {};
        //}
        return Square(new_pos_i((ply&1? 6: 0)+no-1, dir));
    }
    // get all valid moves
    // each move is represented in (no, dir)
    // (0, 0): pass
    std::vector<std::pair<int, int>> move_list() const noexcept{
        if(winner() != Color::other){
            return {};
        }
        std::vector<std::pair<int, int>> ml;
        for(int no=1; no<=6; ++no) for(int dir=1; dir<=3; ++dir) if(new_pos(no, dir)){
            ml.emplace_back(no, dir);
        }
        if(ml.empty()){
            ml.emplace_back(0, 0);
        }
        return ml;
    }
    bool check_move(int const &no, int const &dir) const noexcept{
        auto ml = move_list();
        return std::find(ml.begin(), ml.end(), std::make_pair(no, dir)) != ml.end();
    }
    // make a move
    virtual void do_move(int const &no, int const &dir){
        if(!check_move(no, dir)){
            flog << "invalid move" << std::endl;
            throw std::runtime_error("board::do_move(): the game is over, or the cube/direction is invalid");
        }
        if(no==0 && dir==0){
            chg_ply_i();
            return;
        }
        int ply = get_ply_i();
        int p = ((*this)(no)).datum, p2 = new_pos(no, dir).datum;
        int c = ((ply&1)? 6: 0)+no-1, c2 = get_cube_i(p2);
        if(c2 <= 11){
            assign_pos_i(c2, 36);
            increment_num_i(c2>=6, -1);
        }
        assign_cube_i(p2, c);
        assign_cube_i(p, 14);
        assign_pos_i(c, p2);
        chg_ply_i();
        /* check win condition */
        if ( c <= 5 ) { // from NorthWest Corner
            if ( !get_num_i(1) ) {
                assign_winner_i(0);
            }
            else if ( p2==35 ) { //
                int c3 = get_cube_i(0);
                if ( c3>=6 and c3<=11 ) {
                    if ( c < c3-6 ) {
                        assign_winner_i(0);
                    } else if ( c > c3-6 ) {
                        assign_winner_i(1);
                    } else {
                        assign_winner_i(3);
                    }
                }
            }
        }
        else { // from SouthEast Corner
            if ( !get_num_i(0) ) {
                assign_winner_i(1);
            }
            else if ( p2 == 0 ) {
                int c3 = get_cube_i(35);
                if ( c3 < 6  ) {
                    if ( c-6 < c3 ) {
                        assign_winner_i(1);
                    } else if ( c-6 > c3 ) {
                        assign_winner_i(0);
                    } else {
                        assign_winner_i(3);
                    }
                }
            }
        }
    }
};
std::array<int, 6> const Board::init_pos_i{{0, 1, 2, 6, 7, 12}};
std::array<std::array<int, 4>, 2> const Board::dx{{{0, 1, 0, 1}, {0, -1, 0, -1}}}, Board::dy{{{0, 0, 1, 1}, {0, 0, -1, -1}}};
std::ostream &operator <<(std::ostream &os, Board const &board){
    for(int i=1; i<=6; ++i){
        for(int j=1; j<=6; ++j){
            os << board(i, j) << " \n"[j==6];
        }
    }
    return os;
}

// Board_gui = Board + GUI
// In most cases you don't need to know how it works.
class Board_gui final: public Board{
private:
    // bool init = true;
    Square hlpos;
    int hldir = 0;
public:
    template<typename... T> Board_gui(T&&... args): Board(std::forward<T>(args)...){}
    Square highlight_pos() const noexcept{
        return hlpos;
    }
    int highlight_dir() const noexcept{
        return hldir;
    }
    void highlight(Square const &p = {}) noexcept{
        if(!p || (*this)(p).color()==Color::other){
            hlpos = {};
        }else hlpos = p;
        hldir = 0;
    }
    void highlight(int const &x, int const &y) noexcept{
        highlight(Square(x, y));
    }
    void highlight(Cube const &c) noexcept{
        highlight((*this)(c));
    }
    void highlight(Color const &c, int const &no) noexcept{
        highlight((*this)(c, no));
    }
    void highlight(int const &no) noexcept{
        highlight((*this)(no));
    }
    void reset_hldir() noexcept{
        hldir = 0;
    }
    void set_hldir(int const &dir){
        if(!hlpos || !(*this)(hlpos)){
            throw std::runtime_error("Board_gui::set_hldir(): highlighted square not set or invalid");
        }
        int no = (*this)(hlpos).no();
        if(!check_move(no, dir)){
            throw std::runtime_error("Board_gui::set_hldir(): invalid direction");
        }
        hldir = dir;
    }
    void do_move(int const &no, int const &dir) override{
        Board::do_move(no, dir);
        hlpos = {};
        hldir = 0;
    }
};
std::ostream &operator <<(std::ostream &os, Board_gui const &board){
    os << "\033[m";
    Square ph = board.highlight_pos(), pd;
    int player = Enum2int(board(ph).color());
    int dir = board.highlight_dir();
    if(dir){
        pd = {ph.x()+Board::dx[player][dir], ph.y()+Board::dy[player][dir]};
    }

    for(int i=1; i<=6; ++i){
        for(int j=1; j<=6; ++j){
            Square pij(i, j);
            if(pij == pd){
                os << (player? "\033[1;33;46m": "\033[1;32;45m") << "＊";
            }else{
                if(board(i, j).color() == Color::red){
                    os << (pij==ph? "\033[30;42m": "\033[1;41m");
                }else if(board(i, j).color() == Color::blue){
                    os << (pij==ph? "\033[30;43m": "\033[1;44m");
                }else os << "\033[47m";
                os << Cube_icon[board(i, j).no()];
            }
            os << "\033[m";
        }
        os << '\n';
    }
    return os;
}
#endif
