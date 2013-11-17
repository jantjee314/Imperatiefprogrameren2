/*********************************************************************
*   Voorbeeldraamwerk voor opdracht 1 IPC015.
*   Inhoud:
*   - de type definities voor de muziek DB;
*   - ordeningsrelaties op Track;
*   - inlezen van Nummers.txt bestand;
*   - functies en datastructuren uit hoorcollege #1 IPC015:
*   - sorteer-algoritmen insertion_sort, selection_sort, bubble_sort
*     (gebaseerd op MuziekDBs)
*   - main die Nummers.txt inleest en een sorteer-algoritme aanroept.
**********************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cassert>
#define NDEBUG
using namespace std;


/*********************************************************************
*   MuziekDB type definities en globale vector <El>& MuziekDB:
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
bool SORT_BY_TIME = true;
typedef Track El ;
vector <El> MuziekDB;
int count_comparisons = 0;
/************************************************************************
*   Ordenings-relaties op Track:
*      definieer zelf < en == voor Length en Track, dan volgen
*      de overige ordeningen (>, <=, >=, >) daar vanzelf uit.
************************************************************************/
bool operator<(const Track& a, const Track& b)
{
	count_comparisons ++;
	if(SORT_BY_TIME)
	{
		if(a.time.minutes == b.time.minutes)
		{
			return a.time.seconds < b.time.seconds;
		}
		return a.time.minutes < b.time.minutes;
	}
	else
	{
		if(a.artist == b.artist)
		{
			if(a.cd == b.cd)
			{
				if(a.year == b.year)
				{
					return a.track < b.track;
				}
				return a.year < b.year;
			}
			return a.cd < b.cd;
		}
		return a.artist < b.artist;
	}
}

bool operator==(const Track& a, const Track& b)
{
	count_comparisons ++;
	if(SORT_BY_TIME)
	{
		return (a.time.minutes == b.time.minutes && a.time.seconds == b.time.seconds);
	}
    return (a.artist == b.artist && a.title == b.title && a.year == b.year && a.track == b.track);
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

void laad_in (istream& infile, vector <El>& MuziekDB)
{
    Track track;
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
    MuziekDB.push_back(track);
}

ostream& operator<< (ostream& out, const Track track)
{
    out << track.artist << " " << track.cd << " [" << track.track << "] (" << track.time << ")" ;
    return out ;
}

int lees_liedjes(ifstream& infile, vector <El>& MuziekDB)
{
    do {
        laad_in(infile, MuziekDB) ;
    }
    while (infile);
    return MuziekDB.size() ;
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
	int aantal = lees_liedjes (nummersDBS, MuziekDB);
	nummersDBS.close();
	return aantal;
}

void toon_MuziekDB (vector <El>& MuziekDB, int aantalLiedjes)
{
    for (int i = 0 ; i < aantalLiedjes; i++)
        cout << i+1 << ". " << MuziekDB[i] << endl ;
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

bool is_sorted (vector <El>& MuziekDB, Slice s)
{
//	pre-condition:
	assert (valid_slice(s)) ;	// ...and s.to < size of MuziekDB
//	post-condition:
//	result is true if	MuziekDB[s.from]   <= MuziekDB[s.from+1]
//						MuziekDB[s.from+1] <= MuziekDB[s.from+2]
//						...
//						MuziekDB[s.to-1]   <= MuziekDB[s.to]
	bool sorted = true ;
	for (int i = s.from; i < s.to && sorted; i++)
		if (MuziekDB[i] > MuziekDB[i+1])
			sorted = false ;
	return sorted ;
}

int find_position ( vector <El>& MuziekDB, Slice s, El y )
{
//	pre-condition:
	assert (valid_slice(s) && is_sorted(MuziekDB,s)) ;    // ...and s.to < size of MuziekDB
//	post-condition: s.from <= result <= s.to+1
	for ( int i = s.from ; i <= s.to ; i++ )
		if ( y <= MuziekDB [i] )
			return i ;
	return s.to+1;
}

void shift_right ( vector <El>& MuziekDB, Slice s )
{
//	pre-condition:
	assert (valid_slice (s)) ;	// ... and s.to < size (MuziekDB) - 1
//	post-condition:  MuziekDB[s.from+1]	= 	old MuziekDB[s.from]
//			    	 MuziekDB[s.from+2]	= 	old MuziekDB[s.from+1]
//						...
//			    	 MuziekDB[s.to+1]		= 	old MuziekDB[s.to]
	for ( int i = s.to+1 ; i > s.from ; i-- )
		MuziekDB [i]  = MuziekDB [i-1] ;
}

void insert ( vector <El>& MuziekDB, int& length, El y )
{
	const int pos = find_position (MuziekDB, mkSlice (0, length-1), y) ;
	if (pos < length)
		shift_right ( MuziekDB, mkSlice (pos, length-1) ) ;
	MuziekDB [pos] = y ;
	length++;
}

void swap (vector <El>& MuziekDB, int  i, int  j )
{
//	pre-condition:
	assert ( i >= 0 && j >= 0 ) ;	// ... and i < size of MuziekDB
						            // ... and j < size of MuziekDB
// Post-condition: MuziekDB[i] = old MuziekDB[j] and MuziekDB[j] = old MuziekDB[i]
	const El help = MuziekDB [i];
	MuziekDB [i] = MuziekDB [j] ;
	MuziekDB [j] = help;
}

/************************************************************************
*   MuziekDB-based sorteer-algoritmen uit hoorcollege #1 IPC015:
************************************************************************/
//	MuziekDB-based insertion sort:
void insertion_sort ( vector <El>& MuziekDB, int length )
{
	int sorted = 1 ;
    while ( sorted < length )
    {   insert ( MuziekDB, sorted, MuziekDB[sorted] ) ;
    }
}

//	MuziekDB-based selection sort:
int smallest_value_at ( vector <El>& MuziekDB, Slice s )
{
//	pre-condition:
	assert (valid_slice (s)) ;	// ... and s.to < size (s)
//	Post-condition: s.from <= result <= s.to and MuziekDB[result] is
//	the minimum value of MuziekDB[s.from] .. MuziekDB[s.to]
	int  smallest_at = s.from ;
	for ( int index = s.from+1 ; index <= s.to ; index++ )
		if ( MuziekDB [index] < MuziekDB [smallest_at] )
			smallest_at = index ;
	return smallest_at ;
}

void selection_sort ( vector <El>& MuziekDB, int length )
{	for ( int unsorted = 0 ; unsorted < length ; unsorted++ )
	{   const int k = smallest_value_at (MuziekDB, mkSlice (unsorted, length-1));
	    swap ( MuziekDB, unsorted, k ) ;
	}
}

//	MuziekDB-based bubble sort:
bool bubble ( vector <El>& MuziekDB, Slice s )
{
//	pre-condition:
	assert (valid_slice(s));	// ... and s.to < size(MuziekDB)
//	Post-condition:
//	maximum of MuziekDB[s.from]..MuziekDB[s.to] is at MuziekDB[s.to]
//	if result is true then sorted( MuziekDB, s )
	bool is_sorted = true ;
	for ( int i = s.from ; i < s.to ; i++)
	    if ( MuziekDB [i] > MuziekDB [i+1])
		{	swap ( MuziekDB, i, i+1 ) ;
			is_sorted = false ;
		}
	return is_sorted ;
}

void bubble_sort ( vector <El>& MuziekDB, int length )
{
    while ( !bubble ( MuziekDB, mkSlice (0, length-1 ) ) )
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
        case Insertion: insertion_sort(MuziekDB,aantalLiedjes) ; break ;
        case Selection: selection_sort(MuziekDB,aantalLiedjes) ; break ;
        case Bubble:    bubble_sort   (MuziekDB,aantalLiedjes) ; break ;
        default:        cout << "Huh?" << endl ;
    }
    cout << "Bestand gesorteerd." << endl;
    toon_MuziekDB (MuziekDB,aantalLiedjes) ;
    cout << count_comparisons << endl;
	return 0;
}
