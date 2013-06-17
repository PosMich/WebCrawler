/*********************************
 *                               *
 * Howorka Sarah      - fhs33719 *
 * Poschacher Michael - fhs33734 *
 * Slamanig Sebastian - fhs33741 *
 *                               *
 *********************************/
#ifndef CLELEMENT_H
#define CLELEMENT_H

#include <list>
#include <string>

using namespace std;

class CrawlistElement {

public:
	CrawlistElement( string v_url, bool v_broken );
	~CrawlistElement();

	void increase_counter();
	unsigned get_count();

	string get_url();

	void add_load_time( double v_load_time );
	double get_avg_load_time();
	double get_max_load_time();
	bool is_broken();

private:
	string url;
	bool broken;
	unsigned visit_counter;
	list<double> load_time;

};

#endif