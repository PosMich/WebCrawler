/*********************************
 *                               *
 * Howorka Sarah      - fhs33719 *
 * Poschacher Michael - fhs33734 *
 * Slamanig Sebastian - fhs33741 *
 *                               *
 *********************************/
#ifndef CRAWLIST_H
#define CRAWLIST_H

#include "CrawlistElement.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

class Crawlist
{
	public:
		Crawlist();
		~Crawlist();

		void add( string url, double time, bool broken );

		bool exists( string url );

		unsigned get_good();
		unsigned get_broken();
		unsigned get_total();

		//CrawlistElement get_element( string url );
		void incr_counter( string url );
		void add_load_time( string url, double time );

		void print();

	private:
        pthread_mutex_t m;
		vector<CrawlistElement> crawlist;
};

#endif