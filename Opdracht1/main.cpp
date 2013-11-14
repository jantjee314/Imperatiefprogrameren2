/*********************************************************************
*   Voorbeeldraamwerk voor opdracht 1 IPC015.
*   Inhoud:
*   - de type definities voor de muziek DB;
*   - ordeningsrelaties op Track;
*   - inlezen van Nummers.txt bestand;
*   - functies en datastructuren uit hoorcollege #1 IPC015:
*   - sorteer-algoritmen insertion_sort, selection_sort, bubble_sort
*     (gebaseerd op arrays)
*   - main die Nummers.txt inleest en een sorteer-algoritme aanroept.
**********************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cassert>

using namespace std;


/*********************************************************************
*   MuziekDB type definities en globale array liedjes:
**********************************************************************/
struct Length
{
	int minutes;							// #minuten  (0..)
	int seconds;							// #seconden (0..59)
};
struct Track
{
	string artist;                          // naam van uitvoerende artiest
	string cd;                              // titel van cd
	int    year;                            // jaar van uitgave
	int    track;							// nummer van track
	string title;                           // titel van track
	string tags;                            // tags van track
	Length time;							// lengte van track
	string country;                         // land van artiest
};

const int MAX_AANTAL_LIEDJES = 5000;
typedef Track El ;
typedef Track MuziekDB [MAX_AANTAL_LIEDJES];
MuziekDB liedjes;

/************************************************************************
*   Ordenings-relaties op Track:
*      definieer zelf < en == voor Length en Track, dan volgen
*      de overige ordeningen (>, <=, >=, >) daar vanzelf uit.
************************************************************************/
bool operator<(const Track& a, const Track& b)
{
    return a.artist < b.artist ;    /*implementeer een correcte <  ordening op Track waarden */
}

bool operator==(const Track& a, const Track& b)
{
    return a.artist == b.artist ;   /* implementeer een correcte == ordening op Track waarden */
}

//	afgeleide ordeningen of Track waarden:
bool operator>(const Track& a, const Track& b)
{
	return b < a ;
}

bool operator<=(const Track& a, const Track& b)
{
	return !(b < a) ;
}

bool operator>=(const Track& a, const Track& b)
{
	return b <= a ;
}


/************************************************************************
*   Inlezen van Nummers.txt bestand:
************************************************************************/
istream& operator>> (istream& in, Length& lengte)
{// Preconditie:
    assert (true) ;
/*  Postconditie:
    de waarde van lengte is ingelezen uit in: eerst minuten, daarna ':', daarna seconden.
*/
    char colon ;
    in >> lengte.minutes >> colon >> lengte.seconds ;
    return in ;
}

ostream& operator<< (ostream& out, const Length lengte)
{
    out << lengte.minutes << ':' ;
    if (lengte.seconds < 10)
        out << '0' ;
    out << lengte.seconds ;
    return out ;
}

istream& operator>> (istream& infile, Track& track)
{
    string dummy;
    getline(infile,track.artist);       // artist
    getline(infile,track.cd);           // cd
    infile >> track.year ;              // year
    getline(infile, dummy) ;            // verwijder t/m newline
    infile >> track.track ;             // track nr
    getline(infile, dummy) ;            // verwijder t/m newline
    getline(infile,track.title);        // track title
    getline(infile,track.tags);         // track tags
    infile >> track.time ;              // track time
    getline(infile, dummy) ;            // verwijder t/m newline
    getline(infile,track.country);      // country
    getline(infile, dummy) ;            // verwijder t/m newline
    return infile ;
}

ostream& operator<< (ostream& out, const Track track)
{
    out << track.artist << " " << track.cd << " [" << track.track << "] (" << track.time << ")" ;
    return out ;
}

int lees_liedjes(ifstream& infile, MuziekDB liedjes)
{
    int aantal_ingelezen_liedjes = 0 ;
    do {
        infile >> liedjes[aantal_ingelezen_liedjes++] ;
    }
    while (infile && aantal_ingelezen_liedjes < MAX_AANTAL_LIEDJES);
    return aantal_ingelezen_liedjes ;
}

int lees_bestand (string bestandnaam)
{
    ifstream nummersDBS (bestandnaam.c_str());
    if (!nummersDBS)
    {
        cout << "Kon '" << bestandnaam << "' niet openen." << endl;
        return -1;
    }
    cout << "Lees '" << bestandnaam << "' in." << endl;
	int aantal = lees_liedjes (nummersDBS, liedjes);
	nummersDBS.close();
	return aantal;
}

void toon_MuziekDB (MuziekDB liedjes, int aantalLiedjes)
{
    for (int i = 0 ; i < aantalLiedjes; i++)
        cout << i+1 << ". " << liedjes[i] << endl ;
}

/************************************************************************
*   functies en datastructuren uit hoorcollege #1 IPC015:
************************************************************************/
struct Slice
{
	int from ; 	// 0    <= from
	int to ; 	// from <= to
} ;

Slice mkSlice (int from, int to)
{
//	pre-condition:
	assert (0 <= from && from <= to);
	Slice s = { from, to } ;
	return s ;
}

bool valid_slice (Slice s)
{
	return 0 <= s.from && s.from <= s.to ;
}

bool is_sorted (El array [], Slice s)
{
//	pre-condition:
	assert (valid_slice(s)) ;	// ...and s.to < size of array
//	post-condition:
//	result is true if	array[s.from]   <= array[s.from+1]
//						array[s.from+1] <= array[s.from+2]
//						...
//						array[s.to-1]   <= array[s.to]
	bool sorted = true ;
	for (int i = s.from; i < s.to && sorted; i++)
		if (array[i] > array[i+1])
			sorted = false ;
	return sorted ;
}

int find_position ( El array [], Slice s, El y )
{
//	pre-condition:
	assert (valid_slice(s) && is_sorted(array,s)) ;    // ...and s.to < size of array
//	post-condition: s.from <= result <= s.to+1
	for ( int i = s.from ; i <= s.to ; i++ )
		if ( y <= array [i] )
			return i ;
	return s.to+1;
}

void shift_right ( El array [], Slice s )
{
//	pre-condition:
	assert (valid_slice (s)) ;	// ... and s.to < size (array) - 1
//	post-condition:  array[s.from+1]	= 	old array[s.from]
//			    	 array[s.from+2]	= 	old array[s.from+1]
//						...
//			    	 array[s.to+1]		= 	old array[s.to]
	for ( int i = s.to+1 ; i > s.from ; i-- )
		array [i]  = array [i-1] ;
}

void insert ( El array [], int& length, El y )
{
	const int pos = find_position (array, mkSlice (0, length-1), y) ;
	if (pos < length)
		shift_right ( array, mkSlice (pos, length-1) ) ;
	array [pos] = y ;
	length++;
}

void swap (El array [], int  i, int  j )
{
//	pre-condition:
	assert ( i >= 0 && j >= 0 ) ;	// ... and i < size of array
						            // ... and j < size of array
// Post-condition: array[i] = old array[j] and array[j] = old array[i]
	const El help = array [i];
	array [i] = array [j] ;
	array [j] = help;
}

/************************************************************************
*   array-based sorteer-algoritmen uit hoorcollege #1 IPC015:
************************************************************************/
//	array-based insertion sort:
void insertion_sort ( El array [], int length )
{
	int sorted = 1 ;
    while ( sorted < length )
    {   insert ( array, sorted, array[sorted] ) ;
    }
}

//	array-based selection sort:
int smallest_value_at ( El array [], Slice s )
{
//	pre-condition:
	assert (valid_slice (s)) ;	// ... and s.to < size (s)
//	Post-condition: s.from <= result <= s.to and array[result] is
//	the minimum value of array[s.from] .. array[s.to]
	int  smallest_at = s.from ;
	for ( int index = s.from+1 ; index <= s.to ; index++ )
		if ( array [index] < array [smallest_at] )
			smallest_at = index ;
	return smallest_at ;
}

void selection_sort ( El array [], int length )
{	for ( int unsorted = 0 ; unsorted < length ; unsorted++ )
	{   const int k = smallest_value_at (array, mkSlice (unsorted, length-1));
	    swap ( array, unsorted, k ) ;
	}
}

//	array-based bubble sort:
bool bubble ( El array [], Slice s )
{
//	pre-condition:
	assert (valid_slice(s));	// ... and s.to < size(array)
//	Post-condition:
//	maximum of array[s.from]..array[s.to] is at array[s.to]
//	if result is true then sorted( array, s )
	bool is_sorted = true ;
	for ( int i = s.from ; i < s.to ; i++)
	    if ( array [i] > array [i+1])
		{	swap ( array, i, i+1 ) ;
			is_sorted = false ;
		}
	return is_sorted ;
}

void bubble_sort ( El array [], int length )
{
    while ( !bubble ( array, mkSlice (0, length-1 ) ) )
        length-- ;
}


/************************************************************************
*   de hoofdstructuur van het programma:
*       - lees bestand in
*       - sorteer bestand met insertion_sort / selection_sort / bubble_sort
*                          en 'normale' volgorde van Tracks
*                          en oplopend naar track-lengte
************************************************************************/
enum Sorteermethode {Insertion,Selection,Bubble,AantalMethoden};
string sorteermethode [] = {"insertion", "selection", "bubble"} ;

Sorteermethode selecteer ()
{
    cout << "Kies een sorteermethode: " << endl ;
    for (int m = 0; m < AantalMethoden; m++)
        cout << m+1 << ": " << sorteermethode[m] << " sort" << endl ;
    int keuze ;
    cin >> keuze ;
    return (Sorteermethode)(keuze-1) ;
}

int main()
{
	int aantalLiedjes = lees_bestand ("Nummers.txt");
	if (aantalLiedjes < 0)
	{
        cout << "Bestand inlezen mislukt. Programma termineert." << endl;
	    return aantalLiedjes;
    }
    Sorteermethode m = selecteer () ;
    cout << "Sorteren bestand met " << sorteermethode[m] << " sort" << endl;
    switch (m)
    {
        case Insertion: insertion_sort(liedjes,aantalLiedjes) ; break ;
        case Selection: selection_sort(liedjes,aantalLiedjes) ; break ;
        case Bubble:    bubble_sort   (liedjes,aantalLiedjes) ; break ;
        default:        cout << "Huh?" << endl ;
    }
    cout << "Bestand gesorteerd." << endl;
    toon_MuziekDB (liedjes,aantalLiedjes) ;
	return 0;
}
