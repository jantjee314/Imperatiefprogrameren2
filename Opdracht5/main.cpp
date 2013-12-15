#include <cstdlib>
#include <iostream>
#include <vector>
//#define NDEBUG
#include <cassert>

using namespace std;

// data structures:
const int width  = 4;                    // width  > 0
const int height = 4;                    // height > 0
typedef int Cell;                        // content of slide: not empty: 1 <= value < width*height, empty: value = width*height
const Cell empty = width*height;         // the empty cell has value width*height
struct Pos
{   int col;                             // column index (0 <= col < width)
    int row;                             // row    index (0 <= row < height)
};
struct Puzzle
{   Cell board [width][height];          // the slides of the puzzle
    Pos empty;                           // the coordinate of the empty cell
};
/**
 * 1.3
 * 2: Breadth-first/depth-first = 16 / 15  steps /  vector-size: 40 / 4
 * 3: Breadth-first/depth-first = 128 / 247 steps / vector-size: 272 / 8
 * 4: Breadth-first/depth-first  = 30902 /109746 steps / vector-size : 63040 / 15
 *
 */
const char cr = static_cast<char>(13);   // the carriage-return ASCII code

// test-cases:
// puzzle p1 is immediately ready
const Puzzle p1 = {{{ 1, 5, 9,13}
                   ,{ 2, 6,10,14}
                   ,{ 3, 7,11,15}
                   ,{ 4, 8,12,16}
                   }
                  ,{3,3}
                  } ;
// puzzle p2 is solvable in 3 moves
const Puzzle p2 = {{{ 1, 5, 9,16}
                   ,{ 2, 6,10,13}
                   ,{ 3, 7,11,14}
                   ,{ 4, 8,12,15}
                   }
                  ,{0,3}
                  } ;
// puzzle p3 is solvable in 6 moves
const Puzzle p3 = {{{16, 1, 9,13}
                   ,{ 2, 5, 6,10}
                   ,{ 3, 7,11,14}
                   ,{ 4, 8,12,15}
                   }
                  ,{0,0}
                  } ;
// puzzle p4 is solvable in 13 moves
const Puzzle p4 = {{{ 2, 1, 9,13}
                   ,{ 5, 7, 6,10}
                   ,{ 8,16,11,14}
                   ,{ 3, 4,12,15}
                   }
                  ,{2,1}
                  } ;
Puzzle challenge = p2 ;

bool operator!= (const Pos& p1, const Pos& p2)
{// Pre-condition:
    assert (true) ;
//  Post-condition: result is true only if p1 is different from p2
    return p1.col != p2.col || p1.row != p2.row;
}

bool valid_pos (Pos a)
{// Pre-condition:
    assert (true) ;
//  Post-condition: result is true only if 0 <= a.col < width and 0 <= a.row < height
    return 0 <= a.col && a.col < width && 0 <= a.row && a.row < height;
}

bool puzzle_correctly_filled (Puzzle& p)
{// Pre-condition:
    assert (true) ;
//  Post-condition: result is true only if all cells of p.board have a value between 1 and width*height,
//                  and each value occurs exactly once
    int value [width*height] ;
    for (int i=0 ; i < width*height; i++) value[i] = 0 ;
    for (int col=0; col < width; col++)
        for (int row=0; row < height; row++)
            value[p.board[col][row]-1]++ ;
    bool ok = true ;
    for (int i=0 ; i < width*height && ok; i++)
        ok = value[i] == 1 ;
    return ok ;
}

bool puzzle_has_correct_empty (Puzzle& p)
{// Pre-condition:
    assert (valid_pos(p.empty)) ;
//  Post-condition: result is true only if p.board contains value empty at the coordinates p.empty
    return p.board[p.empty.col][p.empty.row] == empty ;
}

//  decision whether a puzzle has a solution is based on:
//  http://mathworld.wolfram.com/15Puzzle.html
bool puzzle_has_solution (Puzzle& p)
{// Pre-condition:
    assert (puzzle_correctly_filled(p)) ;
//  Post-condition: result is true only if p has a solution
    Cell elements [width*height] ;
    for (int row=0; row < height; row++)
        for (int col=0; col < width ; col++)
            elements[row*width+col] = p.board[col][row] ;
    int occurrences [width*height] ;
    for (int i=0 ; i<width*height; i++)
        occurrences[i] = 0 ;
    for (int elem=0 ; elem < width*height; elem++)
    {
        if (elements[elem] == empty)
            occurrences[elem] = elem / width + 1;    // count row-number of empty cell (counting from 1)
        else
        {
            int nr_smaller = 0 ;
            for (int later=elem+1 ; later < width*height; later++)
                if (elements[later] < elements[elem])
                    nr_smaller++ ;
            occurrences[elem] = nr_smaller ;
        }
    }
    int sum = 0 ;
    for (int i=0 ; i<width*height; i++)
        sum += occurrences[i] ;
    return sum % 2 == 0 ;
}

bool operator== (const Puzzle& p1, const Puzzle& p2)
{// Pre-condition:
    assert (true) ;
//  Post-condition: result is true only if p1 is equal to p2
    if (p1.empty != p2.empty)
        return false;
    for (int col = 0; col < width; col++)
        for (int row = 0; row < height; row++)
            if (p1.board[col][row] != p2.board[col][row])   // found counter-example
               return false;
    return true;      // no counter-example, so equal
}

bool go_north (Puzzle& p)
{// Pre-condition:
    assert (puzzle_has_correct_empty(p)) ;
//  Post-condition: result is true only if the empty cell is not at the top
    return (p.empty.row > 0);
}

Pos north (Puzzle& p)
{// Pre-condition:
    assert (go_north(p)) ;
//  Post-condition: the result is the position of the empty slide, when moved on row to the top
    Pos x = p.empty;
    x.row--;
    return x;
}

bool go_south (Puzzle& p)
{// Pre-condition:
    assert (puzzle_has_correct_empty(p)) ;
//  Post-condition: result is true only if the empty cell is not at the bottom
    return (p.empty.row < height-1);
}

Pos south (Puzzle& p)
{// Pre-condition:
    assert (go_south(p)) ;
//  Post-condition: the result is the position of the empty slide, when moved on row to the bottom
    Pos x = p.empty;
    x.row++;
    return x;
}

bool go_west (Puzzle& p)
{// Pre-condition:
    assert (puzzle_has_correct_empty(p)) ;
//  Post-condition: result is true only if the empty cell is not at the left
    return (p.empty.col > 0);
}

Pos west (Puzzle& p)
{// Pre-condition:
    assert (go_west(p)) ;
//  Post-condition: the result is the position of the empty slide, when moved on column to the left
    Pos x = p.empty;
    x.col--;
    return x;
}

bool go_east (Puzzle& p)
{// Pre-condition:
    assert (puzzle_has_correct_empty(p)) ;
//  Post-condition: result is true only if the empty cell is not at the right
    return (p.empty.col < width-1);
}

Pos east (Puzzle& p)
{// Pre-condition:
    assert (go_east(p)) ;
//  Post-condition: the result is the position of the empty slide, when moved on row to the right
    Pos x = p.empty;
    x.col++;
    return x;
}

int log (int base, int x)
{// Pre-condition:
    assert (base >= 1);
//  Post-condition: result is the base logarithm of x
    if (abs (x) < base)
        return 0;
    else
        return 1 + log (base, x/base);
}

const int max_cellwidth = log (10,height*width) + 1;  // width of a cell

void show_cell (Cell c)
{// Pre-condition:
    assert (1 <= c && c <= width*height);
//  Post-condition: the cell value c is displayed between [ ]
    int cellwidth = 0;
    if (c != empty)
        cellwidth = log(10,c) + 1;
    cout << "[";
    for (int i=max_cellwidth-cellwidth; i > 0; i--)
        cout << " ";
    if (c != empty)
        cout << c;
    cout << "]";
}

bool is_neighbour (Pos a, Pos b)
{// Pre-condition:
    assert (true) ;
//  Post-condition: result is true only if a and b differ in only the column by one or the row by one,
//                  but not both
    return (  (a.row == b.row && abs (a.col - b.col) == 1)
           || (a.col == b.col && abs (a.row - b.row) == 1)
           );
}

void move (Puzzle p, Pos next, Puzzle& next_p)
{// Pre-condition:
    assert (puzzle_has_correct_empty(p) && valid_pos (next) && is_neighbour (p.empty,next)) ;
//  Post-condition: next_p is the same puzzle as p, except that the empty cell has traded places with next
    next_p.empty = next;
    for (int col = 0; col < width; col++)
        for (int row = 0; row < height; row++)
            next_p.board[col][row] = p.board[col][row];
    next_p.board[p.empty.col][p.empty.row] = p.board[next.col][next.row];
    next_p.board[next.col][next.row] = empty;
}

void show_puzzle (Puzzle& p)
{// Pre-condition:
    assert (true) ;
//  Post-condition: puzzle p is displayed row-by-row, column-by-column
    cout << endl;
    for (int row=0; row < height; row++)
    {   for (int col=0; col < width; col++)
            show_cell (p.board[col][row]);
        cout << endl;
    }
}

//  puzzle_ready (p)
//     levert true op als de achtereenvolgende kolommen oplopend geordend zijn.
bool puzzle_ready (Puzzle& p)
{// Pre-condition:
    assert (puzzle_correctly_filled(p)) ;
//  Post-condition: result is true only if the slides in this puzzle are ordered from 1..empty
//                  when read from left-to-right, top-to-bottom
    int previous = 0;
    for (int row=0; row < height; row++)
        for (int col=0; col < width; col++)
        {   if (p.board[col][row] <= previous)  // found counter-example, so not ready
               return false;
            previous = p.board[col][row];
        }
    return true;                                // no counter-example found, so ready
}

typedef vector<Puzzle> Solution;                // a solution is a sequence of puzzles

int size (Solution& s)
{// Pre-condition:
    assert (true) ;
//  Post-condition: result is the number of elements in s
    return static_cast<int> (s.size());
}

bool puzzle_present (Puzzle& p, Solution& s)
{// Pre-condition:
    assert (true) ;
//  Post-condition: for some 0 <= i < size(s): s[i] == p
    for (int i = 0; i < size(s); i++)
        if (s[i] == p)
           return true;
    return false;
}

void show (Solution& s)
{// Pre-condition:
    assert (true) ;
//  Post-condition: the last 10 (if present) elements of s are displayed.
    const int max_nr_of_shown_steps = 10 ;
    const int n = size(s);
    cout << endl << "showing last " << min(max_nr_of_shown_steps,n) << " puzzles: " << endl ;
    for (int i=max(0,n-max_nr_of_shown_steps); i < n; i++)
    {
        cout << "step " << i << ":\n";
        show_puzzle (s[i]);
    }
}

void copy_shorter_solution (Solution& s, Solution& shortest)
{
    const int shorter = size (s);
    const int longer  = size (shortest);
//  Pre-condition:
    assert (longer == 0 || shorter < longer) ;
//  Post-condition: s is copied to shortest
    if (longer == 0)                                                      // a shortest solution has not yet been found
        for (int i = 0; i < shorter; i++)
            shortest.push_back(s[i]);
    else
    {
        for (int i = 0; i < longer-shorter; i++) shortest.pop_back();     // make shortest equally long as s
        for (int i = 0; i < shorter; i++)
            shortest[i] = s[i] ;                                          // copy solution to shortest
    }
}


//--------------------------------------------------------
//
//	depth-first search version:
//
//--------------------------------------------------------

int nr_of_solve_calls = 0 ;           // keep track of the number of calls to solve
int max_vector_size   = 0 ;           // keep track of the max depth of solution vector

//  mutually recursive function, so declare signature ahead
void depth_first (Solution& s, Solution& shortest, int maximum);

void tries (Solution& s, Pos next, Solution& shortest, int maximum)
{// Pre-condition:
    assert (size(s) > 0 && valid_pos(next) && maximum >= 0) ;
//  Post-condition: all attempts that:
//                   - start with s, and
//                   - in which the empty cell of the last puzzle of s has been moved to next, and
//                   - that end with a puzzle in the final state, and
//                   - the length does not exceed max+1
//                  are displayed
//                  shortest contains (one of) the shortest of such solutions
    Puzzle p;
    move (s[size(s)-1], next, p); // move the empty cell
    if (!puzzle_present(p,s))
    {
        s.push_back (p);
        depth_first (s,shortest,maximum);
        s.pop_back();
    }
}

void depth_first (Solution& s, Solution& shortest, int maximum)
{// Pre-condition:
    assert (size(s) > 0 && maximum >= 0) ;
//  Post-condition: all attempts that:
//                   - start with s, and
//                   - that end with a puzzle in the final state, and
//                   - the length does not exceed max+1
//                  are displayed
//                  shortest contains (one of) the shortest of such solutions
    const int n = size(s);
    nr_of_solve_calls++ ;                            // keep track of dimension time
    max_vector_size = max (max_vector_size, n) ;     // keep track of dimension space
    Puzzle p = s[n-1];
    if (n > maximum)
        return;
    if (size(shortest) != 0 && n >= size(shortest))  // solution s can not become any better than current shortest
        return;                                      // abandon search
    if (puzzle_ready (p))                            // found shorter or first solution
    {
        cout << "\n\nsolution found of " << n-1 << " steps.\n\n";
        show (s);
        copy_shorter_solution (s, shortest);         // remember newer better solution
    }
    else
    {
        cout << cr << "nr_of_solve_calls = " << nr_of_solve_calls << ", max_vector_size = " << max_vector_size ;
        if (go_south (p))  tries (s, south(p), shortest, maximum);
        if (go_east  (p))  tries (s, east (p), shortest, maximum);
        if (go_north (p))  tries (s, north(p), shortest, maximum);
        if (go_west  (p))  tries (s, west (p), shortest, maximum);
    }
}


//--------------------------------------------------------
//
//	breadth-first search version:
//
//--------------------------------------------------------
struct Candidate
{
	Puzzle candidate ;
	int parent ;
};
typedef vector<Candidate> Attempts ;

int size (Attempts& a)
{// Pre-condition:
    assert (true) ;
//  Post-condition: result is length of a
	return static_cast<int> (a.size()) ;
}

bool puzzle_present (Attempts& a, int c, Puzzle p)
{// Pre-condition:
    assert (size(a) > 0 && 0 <= c && c < size(a)) ;
//  Post-condition: result is true only if a[c] or one of its predecessors up until a[0] is equal to p
    for(int i = 0; i<=c; i++)
    {
    	if(a[i].candidate == p)
    		return true;
    }
    return false;
}

void show_path (Attempts& a, int last)
{// Pre-condition:
    assert (size(a) > 0 && 0 <= last && last < size(a)) ;
//  Post-condition: a[last] and all of its predecessors up until a[0] is displayed in reverse order
    if(last > 0)
    	show_path(a,last-1);
    show_puzzle(a[last].candidate);

}

int depth (Attempts& a, int from)
{// Pre-condition:
    assert (size(a) > 0 && 0 <= from && from < size(a)) ;
//  Post-condition: result is the number of predecessors of a[from] plus one
	if (from == 0)
		return 0 ;
	else
		return 1 + depth (a, a[from].parent) ;
}

void tries (Attempts& a, int c, Pos next)
{// Pre-condition:
    assert (size(a) > 0 && 0 <= c && c < size(a) && valid_pos(next)) ;
//  Post-condition: the puzzle created by moving the empty slide of a[c] to next is added to the end of a if it is
//                  not present as one of its predecessors
	Puzzle p = a[c].candidate ;
	Puzzle newp ;
	move (p, next, newp) ;
	Candidate newc = {newp,c} ;
	if (!puzzle_present(a,c,newp))
		a.push_back (newc) ;
}

void breadth_first (Puzzle start)
{// Pre-condition:
    assert (puzzle_correctly_filled(start)) ;
//  Post-condition: the shortest possible solution starting with start is displayed
	vector<Candidate> a ;
	Candidate first = {start,0} ;
	a.push_back(first) ;
	int c = 0 ;
	bool found = false ;
	while (c < size(a) && !found)
	{
		cout << cr << c << " attempts at " << depth(a,c) << " search depth" ;
		Puzzle p = a[c].candidate ;
		if (puzzle_ready (p))
		{
			cout << endl << "solution found of " << depth (a, c) << " steps." << a.size() << endl ;
			//show_path (a, c) ;
			found = true ;
		}
		else
		{
            if (go_north (p)) 	tries (a, c, north(p)) ;
            if (go_west  (p)) 	tries (a, c, west (p)) ;
            if (go_south (p)) 	tries (a, c, south(p)) ;
            if (go_east  (p)) 	tries (a, c, east (p)) ;
            c++ ;
		}
	}
}

int enter_max_depth ()
{// Pre-condition:
    assert (true) ;
//  Post-condition: the result is the first positive value (> 0) that is entered by the user
    int max_depth = -1 ;
    while (max_depth <= 0)
    {
        cout << "Give max nr of steps: " ;
        cin  >> max_depth ;
    }
    return max_depth ;
}

int main ()
{
    show_puzzle (challenge);
    if (!puzzle_has_solution (challenge))
        cout << "This puzzle has no solution at all." << endl ;
    else
    {
        cout << "Start solving: " << endl;
        Solution s, shortest;
        s.push_back (challenge);
    //    depth_first (s,shortest,enter_max_depth ()) ;
        breadth_first (challenge) ;
        cout << endl << "final nr_of_solve_calls = " << nr_of_solve_calls << ", final max_vector_size = " << max_vector_size << endl ;
        system("PAUSE");
        return EXIT_SUCCESS;
    }
}
