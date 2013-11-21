#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>

using namespace std;

int size (vector<char>& r)
{// pre-condition:
    assert (true) ;
//  post-condition: result is the number of elements in r
    return static_cast<int>(r.size()) ;
}

int add_without_dup (char x, vector<char>& r)
{// pre-condition:
    assert (true) ;
//  post-condition: x is added to the end of r if it is not yet present in r
//                  the result is the number of comparisons performed in this function
    int i = 0 ;
    while ( i < size(r) && r[i] != x )
        i++ ;
    if ( i == size(r))
        r.push_back (x) ;
    return i ;
}
/**
De complexiteit van remove dup is nu in de vorm van O(n^2) omdat 2 keer geloopt wordt over alle elementen eerst hier en dan in add_without_dup over alle niet duplicate elementen
Dus als er niet veel duplicates zijn zit je aan n^2.

De manier om deze functie O(n) te krijgen is als de functie gesorteerd is dan kun je gewoon loopen over de elementen en bij het k-de element kijken of het hetzelfde is als
het k-1-de element als dit zo is voeg niet toe aan je dest vector anders wel.

**/
int remove_dup (vector<char>& source, vector<char>& dest)
{// pre-condition:
    assert (size (dest) == 0) ;
//  post-condition: dest is a copy of source without duplicate elements
//                  the result is the number of comparisons performed in this function
    int nr_of_comparisons = 0 ;
    for (int i = 0 ; i < size (source) ; i++)
        nr_of_comparisons += add_without_dup (source[i], dest) ;
    return nr_of_comparisons ;
}

int remove_sort_dup (vector<char>& source, vector<char>& dest)
{// pre-condition:
 assert (size (dest) == 0 && is_sorted (source)) ;
// post-condition: dest is a copy of source without duplicate elements
// result is the number of comparisons performed in this function
    for(int i = 0 ; i < size(source); i++)
    {
        if(i>0)
        {
            if(source[i] != source[i-1])
            {
                dest.push_back(sourche[i]);
            }
        }
        else
        {
            dest.push_back(source[i]);
        }
    }
}


void fill (string txt, vector<char>& r)
{// pre-condition:
    assert (size(r) == 0) ;
//  post-condition: r contains all characters of txt
    for (int i=0; i<txt.size(); i++ )
        r.push_back(txt[i]) ;
}

void show (vector<char>& r)
{// pre-condition:
    assert (true) ;
//  post-condition: all elements of r are displayed (from index 0 to index size(r)-1)
    for (int i=0; i<size(r); i++)
        cout << r[i] ;
}

int main ()
{
    vector<char> source;
    fill ("Alice was beginning to get very tired of sitting by her sister on the bank",source) ;
    vector<char> dest;
    int nr_of_comparisons = remove_dup (source,dest) ;
    cout << "The source is: " << endl ;
    show (source) ;
    cout << endl << "The dest is: " << endl ;
    show (dest) ;
    cout << endl << "The number of comparisons is: " << nr_of_comparisons << endl ;
    return 0;
}
