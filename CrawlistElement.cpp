/*********************************
 *                               *
 * Howorka Sarah      - fhs33719 *
 * Poschacher Michael - fhs33734 *
 * Slamanig Sebastian - fhs33741 *
 *                               *
 *********************************/

#include "CrawlistElement.h"

CrawlistElement::CrawlistElement( string v_url, bool v_broken ) : 
	url( v_url ), broken( v_broken ), visit_counter( 0 ), load_time( 0.0 )
{
}

CrawlistElement::~CrawlistElement()
{
}

void CrawlistElement::increase_counter()
{
	visit_counter++;
}

unsigned CrawlistElement::get_count()
{
	return visit_counter;
}

string CrawlistElement::get_url()
{
	return url;
}

void CrawlistElement::add_load_time( double v_load_time )
{
	load_time.push_back( v_load_time );
}

double CrawlistElement::get_avg_load_time()
{
	
	list<double>::iterator i;
	double total_time = 0.0;

	for( i = load_time.begin(); i != load_time.end(); ++i )
		total_time += *i;

	return total_time / load_time.size();

}

double CrawlistElement::get_max_load_time()
{
	list<double>::iterator i;
	double max_time = 0.0;

	for( i = load_time.begin(); i != load_time.end(); ++i )
		max_time = ( *i > max_time ) ? *i : max_time;

	return max_time;
}

bool CrawlistElement::is_broken()
{
	return broken;
}