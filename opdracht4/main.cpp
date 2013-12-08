#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include <algorithm>


using namespace std;
struct numbers
{
	int fails;
	int succeeded;
};
struct Product
{
    double prijs;
    string naam;
    int aantal;
};
struct Lijst
{
    double budget;
    vector <string> wensen;
};
struct giftsolution
{
    double budget_over;
    vector <string> producten;
};
struct person
{
    string naam;
    Lijst wenslijst;
    int som_presents;
};
numbers operator +(numbers a,numbers b){
	numbers c;
	c.fails = a.fails + b.fails;
	c.succeeded = a.succeeded + b.succeeded;
	return c;
}
bool operator <(person a, person b){
    return a.naam<b.naam;
}
int nr_of_prunes;
vector <Product> Giftshop;
typedef int Coin ;
typedef vector<Coin> Money ;

/************************************************************************
*   Inlezen van giftstore.txt bestand:
************************************************************************/

void lees_lijstje (string bestandnaam, Lijst& wenslijst)
{
    string dummy;
    ifstream infile (bestandnaam.c_str());
    if (!infile)
    {
        cout << "Kon '" << bestandnaam << "' niet openen." << endl;
    }
    cout << "Lees '" << bestandnaam << "' in." << endl;
    infile >> wenslijst.budget;
    do {
        getline(infile,dummy);
        wenslijst.wensen.push_back(dummy);
    }
    while (infile);
    infile.close();
}

void laad_in (istream& infile, vector <Product>& giftstore)
{
    Product product;
    string dummy;
    infile >> product.prijs;
    getline(infile,product.naam);
    product.aantal = 2;
    giftstore.push_back(product);
}

int lees_gifts(ifstream& infile, vector <Product>& giftstore)
{
    do {
        laad_in(infile, giftstore) ;
    }
    while (infile);
    return giftstore.size() ;
}

int lees_giftstore (string bestandnaam)
{
    ifstream infile (bestandnaam.c_str());
    if (!infile)
    {
        cout << "Kon '" << bestandnaam << "' niet openen." << endl;
        return -1;
    }
    cout << "Lees '" << bestandnaam << "' in." << endl;
	int aantal = lees_gifts (infile, Giftshop);
	infile.close();
	return aantal;
}

void show_gifts(vector<Product>& giftstore)
{
    for(unsigned int i=0; i<giftstore.size();i++)
    {
        cout << "Prijs: " << giftstore[i].prijs << " Naam: " << giftstore[i].naam << endl;
    }
}

void show_lijst(Lijst& wenslijst)
{
    cout << "Budget: " << wenslijst.budget << endl;
    for(unsigned int i = 1; i < wenslijst.wensen.size();i++)
        cout << "Product: " << wenslijst.wensen[i] << endl;
}

int size (Money& money)
{// pre-condition:
    assert (true) ;
//  post-condition: result is the number of elements in money
    return static_cast<int> (money.size()) ;
}

bool real_money (Money& money)
{// pre-condition:
    assert (true) ;
//  post-condition: result is true only if all elements in money are positive (> 0)
    bool real = true ;
    for (int i = 0 ; i < size(money) && real ; i++)
        if (money[i] <= 0)
            real = false ;
    return real ;
}
void show_solution(Money& s){
	cout << "Een oplossing is:";
	for(int i=0;i<s.size();i++){
		cout << s[i] << " ";
	}
	cout << endl;
}

numbers numbers_fail()
{
	numbers a;
	a.fails = 1;
	a.succeeded = 0;
	return a;
}

numbers numbers_succes()
{
	numbers a;
	a.fails = 0;
	a.succeeded = 1;
	return a;
}
bool smallest_coin_larger(Money& m,int c,int index)
{
	nr_of_prunes++;
	for(int i=index; i<m.size();i++){
		if(m[index] <= c){ return false; }
	}
	return true;
}

bool sum_smaller(Money& m, int c, int index=0)
{
	nr_of_prunes++;
	int sum = 0;
	while(index < m.size())
	{
		sum += m[index];
		index++;
	}
	return sum < c;
}

numbers solutions (Money& s,Money& m, int c, int target )
{// pre-condition:
    assert (real_money(m) && 0 <= c && c <= size(m)) ;
//  post-condition:
//  result is the number of ways of matching target exactly with the coins in m[c] .. m[size(m)-1]
	if	    ( target == 0 )		{
		show_solution(s);
		numbers a = numbers_succes();
		return a;
	}
	else if ( target < 0 )		{
		numbers b = numbers_fail();
		return b;
	}
	else if ( c >= size(m) )	{
		numbers c = numbers_fail();
		return c;
	}
	else {
		if(smallest_coin_larger(m,target,c))
		{
			numbers c = numbers_fail();
			return c;
		}
		if(sum_smaller(m,target,c)){
			numbers c = numbers_fail();
			return c;
		}
		s.push_back(m[c]);
		numbers with_c    = solutions ( s, m, c+1, target-m[c] );
		s.pop_back();
		numbers without_c = solutions ( s, m, c+1, target );
		return with_c + without_c;
	}
}

void copy_money (Coin array [], int length, Money& m)
{// pre-condition:
    assert (length >= 0 && size(m) == 0) ;
//  post-condition: all values in array[0] .. array[length-1] are copied to m
    for (int i=0 ; i < length ; i++)
        m.push_back(array[i]) ;
}
double value_gift(string naam)
{
    for(unsigned int i=0;i<Giftshop.size();i++)
    {
        if(Giftshop[i].naam.find(naam) != -1)
        {
            if(Giftshop[i].aantal > 0)
            {
                return Giftshop[i].prijs;
            }
        }
    }
    return -1;
}
void show_solution_gift(giftsolution& s)
{
    cout << s.producten.size() << "Budget over: " << s.budget_over << endl;
    for(unsigned int i=0;i<s.producten.size();i++)
    {
        cout << "Product: " << s.producten[i]<<endl;
    }
}
giftsolution gifts (giftsolution s,Lijst& m, int c)
{// pre-condition:
    assert (0 <= c && c <= m.wensen.size()) ;
//  post-condition:
//  result is the number of ways of matching target exactly with the coins in m[c] .. m[size(m)-1]
	if ( c >= m.wensen.size() )	{
		return s;
	}
	else {
        double price = value_gift(m.wensen[c]);
        if(price != -1 && price < s.budget_over)
        {
            s.producten.push_back(m.wensen[c]);
            s.budget_over = s.budget_over - price;
            giftsolution with_c    = gifts ( s, m, c+1);
            s.producten.pop_back();
            s.budget_over = s.budget_over + price;
            giftsolution without_c = gifts ( s, m, c+1);
            if(with_c.budget_over < without_c.budget_over){
                return with_c;
            }
            return without_c;
        }
        return gifts(s,m,c+1);
	}
}
void update_product(string naam)
{
    for(unsigned int i=0;i<Giftshop.size();i++)
    {
        if(Giftshop[i].naam.find(naam) != -1)
        {
            Giftshop[i].aantal -= 1;
        }
    }
}
void update_giftshop(giftsolution s)
{
    assert(true);
    for(unsigned int i=0;i<s.producten.size();i++)
    {
        update_product(s.producten[i]);
    }
}
void reset_giftshop()
{
    assert(true);
    for(unsigned int i=0;i<Giftshop.size();i++)
    {
        Giftshop[i].aantal = 2;
    }
}
void most_presents_for_person(vector <person> p)
{
    do{
        reset_giftshop();
        for(int i = 0; i < p.size();i++)
        {
            giftsolution s;
            s.budget_over = p[i].wenslijst.budget;
            giftsolution f = gifts(s,p[i].wenslijst,0);
            p[i].som_presents += f.producten.size();
            update_giftshop(f);
        }
    }
    while(next_permutation(p.begin(),p.end()));

    for(int i=0;i<p.size();i++)
    {
        cout << "Persoon: " << p[i].naam << " Cadeatjes: " << p[i].som_presents << endl;
    }
}
void add_person(string naam, vector <person>& persons)
{
    person p;
    p.naam = naam;
    Lijst wenslijst;
    naam.append(".txt");
    lees_lijstje(naam,wenslijst);
    p.wenslijst = wenslijst;
    p.som_presents = 0;
    persons.push_back(p);
}
int main()
{
    int aantal = lees_giftstore("giftstore.txt");
    vector <person> persons;
    add_person("Andrew",persons);
    add_person("Belle",persons);
    add_person("Chris",persons);
    add_person("Desiree",persons);
    add_person("Edward",persons);
    add_person("Fabienne",persons);
    most_presents_for_person(persons);
    /*
    show_gifts(Giftshop);
    Lijst wenslijst;
    lees_lijstje("Chris.txt",wenslijst);
    show_lijst(wenslijst);
    giftsolution s;
    s.budget_over = wenslijst.budget;
    giftsolution sr;
    sr= gifts(s,wenslijst,0);
    show_solution_gift(sr);
    */
//  note: C++ does not have initializing expressions for vectors, so we use a copy function
//        that copies an array to a vector
    /*nr_of_prunes = 0;
	Coin coins0 [] = {} ;
    Coin coins1 [] = {2,2,2,5,10,10,20} ;
    Coin coins2 [] = {20,10,10,5,2,2,2} ;
    Coin coins3 [] = {20,50,1000,1000,2000} ;
    Money m0, m1, m2, m3 ;
    copy_money(coins0, 0, m0) ;
    copy_money(coins1, 7, m1) ;
    copy_money(coins2, 7, m2) ;
    copy_money(coins3, 5, m3) ;
//  the test coin collections:
    Money tests [] = {m0, m1, m2, m3} ;
//  the test target values:
    const int targets [] = {0,1,42,abs (5-12-2013)} ;
    Money s;
    numbers num = solutions(s,tests[3],0,3050);
    cout << "nummer of succes: " << num.succeeded << endl;
    cout << "nummer of failed: " << num.fails << endl;
    cout << "nummer of prunes: " << nr_of_prunes << endl;*/
    return 0;
}
