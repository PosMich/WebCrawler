/*********************************
 *                               *
 * Howorka Sarah      - fhs33719 *
 * Poschacher Michael - fhs33734 *
 * Slamanig Sebastian - fhs33741 *
 *                               *
 *********************************/

#include "Crawlist.h"
#include <iostream>

Crawlist::Crawlist(): m(PTHREAD_MUTEX_INITIALIZER), crawlist()
{
}

Crawlist::~Crawlist()
{
}

void Crawlist::add( string url, double time, bool broken)
{
    pthread_mutex_lock(&m);
    for( vector<CrawlistElement>::iterator i = crawlist.begin(); i != crawlist.end(); ++i )
    {
        if( i->get_url() == url )
        {
            i->add_load_time( time );
            i->increase_counter();
            pthread_mutex_unlock(&m);
            return;
        }
    }

    CrawlistElement* elem = new CrawlistElement( url, broken );
    elem->add_load_time( time );
    elem->increase_counter();
    crawlist.push_back( *elem );
    delete elem;
	pthread_mutex_unlock(&m);

    return;
}

bool Crawlist::exists( string url )
{
    pthread_mutex_lock(&m);
    bool found = false;
	for( vector<CrawlistElement>::iterator i = crawlist.begin(); i != crawlist.end() && !found; ++i )
	{
		if( i->get_url() == url)
			found = true;
	}
    pthread_mutex_unlock(&m);
	return found;
}

unsigned Crawlist::get_good()
{
    pthread_mutex_lock(&m);
	unsigned good = 0;
	for( vector<CrawlistElement>::iterator i = crawlist.begin(); i != crawlist.end(); ++i )
	 	good += (i->is_broken()) ? 0 : 1;
    pthread_mutex_unlock(&m);
	return good;
}

unsigned Crawlist::get_broken()
{
    pthread_mutex_lock(&m);
	unsigned broken = 0;
	for( vector<CrawlistElement>::iterator i = crawlist.begin(); i != crawlist.end(); ++i )
	 	broken += (i->is_broken()) ? 1 : 0;

    pthread_mutex_unlock(&m);
	return broken;
}

unsigned Crawlist::get_total()
{
    pthread_mutex_lock(&m);
	unsigned s =crawlist.size();
    pthread_mutex_unlock(&m);
    return s;
}

void Crawlist::incr_counter( string url )
{
    pthread_mutex_lock(&m);
    bool found = false;
    for( vector<CrawlistElement>::iterator i = crawlist.begin(); i != crawlist.end() && !found; ++i )
    {
        if (i->get_url() == url)
        {
            found = true;
            i->increase_counter();
        }
    }
    pthread_mutex_unlock(&m);
}

void Crawlist::add_load_time( string url, double time )
{
    pthread_mutex_lock(&m);
    bool found = false;
    for( vector<CrawlistElement>::iterator i = crawlist.begin(); i != crawlist.end() && !found; ++i )
    {
        if (i->get_url() == url)
        {
            found = true;
            i->increase_counter();
            i->add_load_time( time );
        }
    }
    pthread_mutex_unlock(&m);
}

/*
CrawlistElement Crawlist::get_element( string url )
{
    pthread_mutex_lock(&m);
	CrawlistElement* elem = NULL;
	for( vector<CrawlistElement>::iterator i = crawlist.begin(); i != crawlist.end() && elem != NULL; ++i )
		if( i->get_url() == url ) elem = &(*i);

    pthread_mutex_unlock(&m);
	return *elem;
}
*/

void Crawlist::print()
{
    pthread_mutex_lock(&m);
	cout << "URL\tBroken\tCounter\tAVG-load-time\tMAX-load-time" << endl;
	for( vector<CrawlistElement>::iterator i = crawlist.begin(); i != crawlist.end(); ++i )
	{
		cout << i->get_url() << "\t" << ( i->is_broken() ? "broken" : "good" ) << "\t" << i->get_count() << "\t" << i->get_avg_load_time() << "\t" << i->get_max_load_time() << endl;
	}
    pthread_mutex_unlock(&m);
}