#include <cstdlib>
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;
struct numbers
{
	int fails;
	int succeeded;
};

numbers operator +(numbers a,numbers b){
	numbers c;
	c.fails = a.fails + b.fails;
	c.succeeded = a.succeeded + b.succeeded;
	return c;
}
int nr_of_prunes;
typedef int Coin ;
typedef vector<Coin> Money ;

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

int main()
{
//  note: C++ does not have initializing expressions for vectors, so we use a copy function
//        that copies an array to a vector
    nr_of_prunes = 0;
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
    cout << "nummer of prunes: " << nr_of_prunes << endl;
    return 0;
}
