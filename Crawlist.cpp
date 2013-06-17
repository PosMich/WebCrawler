/*********************************
 *                               *
 * Howorka Sarah      - fhs33719 *
 * Poschacher Michael - fhs33734 *
 * Slamanig Sebastian - fhs33741 *
 *                               *
 *********************************/

#include "Crawlist.h"
#include <iostream>

Crawlist::~Crawlist()
{
}

void Crawlist::add( CrawlistElement* element )
{
	crawlist.push_back( *element );
}

bool Crawlist::exists( string url )
{
	for( vector<CrawlistElement>::iterator i = crawlist.begin(); i != crawlist.end(); ++i )
	{
		if( i->get_url() == url )
			return true;
	}
	return false;
}

unsigned Crawlist::get_good()
{
	unsigned good = 0;
	for( vector<CrawlistElement>::iterator i = crawlist.begin(); i != crawlist.end(); ++i )
	 	good += (i->is_broken()) ? 0 : 1;
	return good;
}

unsigned Crawlist::get_broken()
{
	unsigned broken = 0;
	for( vector<CrawlistElement>::iterator i = crawlist.begin(); i != crawlist.end(); ++i )
	 	broken += (i->is_broken()) ? 1 : 0;

	return broken;
}

unsigned Crawlist::get_total()
{
	return crawlist.size();
}

CrawlistElement* Crawlist::get_element( string url )
{
	for( vector<CrawlistElement>::iterator i = crawlist.begin(); i != crawlist.end(); ++i )
		if( i->get_url() == url ) return &(*i);
	
	return NULL;
}

void Crawlist::print()
{
	cout << "URL\tBroken\tCounter\tAVG-load-time\tMAX-load-time" << endl;
	for( vector<CrawlistElement>::iterator i = crawlist.begin(); i != crawlist.end(); ++i )
	{
		cout << i->get_url() << "\t" << ( i->is_broken() ? "broken" : "good" ) << "\t" << i->get_count() << "\t" << i->get_avg_load_time() << "\t" << i->get_max_load_time() << endl;
	}
}