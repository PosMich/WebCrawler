/*********************************
 *                               *
 * Howorka Sarah      - fhs33719 *
 * Poschacher Michael - fhs33734 *
 * Slamanig Sebastian - fhs33741 *
 *                               *
 *********************************/

#define nDEBUG

#include <stdio.h>
#include <iostream>
#include <deque>
#include <curl/curl.h>
#include <boost/regex.hpp>
#include <list>

#include "Crawlist.h"
#include "URLQueue.h"

using namespace std;

timespec total_time_start, total_time_end;

unsigned iterations     = 1000;
unsigned num_of_threads = 4;
Crawlist *crawlist = new Crawlist();

static pthread_mutex_t m_iterations = PTHREAD_MUTEX_INITIALIZER;

URLQueue *urls = new URLQueue(4);

boost::regex e("<\\s*A\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"", boost::regex::normal | boost::regbase::icase);

size_t writeToString(void *ptr, size_t size, size_t count, void *stream)
{
    ((string*)stream)->append((char*)ptr, 0, size* count);
    return size* count;
}

double diffTime( timespec* start, timespec* end ) 
{
   return ( end->tv_sec + (double) end->tv_nsec/10000000000 ) 
             - ( start->tv_sec + (double) start->tv_nsec/10000000000 );
}

// BRUM BRUM BRUM
void *workerFunc( void *arg )
{

    cout << "Thread with id: " << pthread_self() << " is running." << endl;

    // unsigned curr_thread = (unsigned) arg;

    timespec time_start, time_end;

    string curr_url, base_url;
    deque<string> raw_urls;

    size_t found;
    unsigned slash_pos = 0;

    CrawlistElement *elem = NULL;
    string data;
    string raw_data;

    CURL *curl_handle = NULL;
    CURLcode code;

    curl_handle = curl_easy_init();
    curl_easy_setopt( curl_handle, CURLOPT_WRITEFUNCTION, writeToString );
    curl_easy_setopt( curl_handle, CURLOPT_WRITEDATA, &data );
    curl_easy_setopt( curl_handle, CURLOPT_FAILONERROR, true );

    while( iterations > 0 )
    {
        pthread_mutex_lock( &m_iterations );
            iterations--;
        pthread_mutex_unlock( &m_iterations );

        curr_url = urls->get();

        if( curr_url == "" )
        {
            cout << "Thread " << pthread_self() << " exits." << endl;
            curl_easy_cleanup( curl_handle );
            pthread_exit(NULL);
        }
            // return 0;

        base_url = curr_url;

        // Schritt 1: "base-URL" rausziehen
        //      alles nach letztem Slash wegsparxXxen

        found = curr_url.find("/");
        slash_pos = 0;

        while( found != string::npos)
        {
            slash_pos = found;
            found = curr_url.find("/", found+1);
        }

        base_url.erase( slash_pos+1, (curr_url.size() - slash_pos) );

#ifdef DEBUG
    cout << pthread_self() << ": base url = " << base_url << endl;
    cout << pthread_self() << ": curr url = " << curr_url << endl;
#endif
        
        curl_easy_setopt( curl_handle, CURLOPT_URL, curr_url.c_str() );

        clock_gettime(CLOCK_MONOTONIC, &time_start);
            code = curl_easy_perform( curl_handle );
        clock_gettime(CLOCK_MONOTONIC, &time_end);
        
#ifdef DEBUG
        cout << pthread_self() << ": URL response-code: " << code << endl;
#endif

        if ( crawlist->exists( curr_url ) )
        {
#ifdef DEBUG
        cout << pthread_self() << ": +++++++++++++++ Element is allready in CrawList." << endl;
#endif
            elem = crawlist->get_element( curr_url );
            elem->increase_counter();
            elem->add_load_time( diffTime( &time_start, &time_end) );

        }
        else
        {

#ifdef DEBUG
        cout << pthread_self() << ": --------------- Element not in CrawList -> add." << endl;
#endif      

            if ( code == 0 )
            {

                elem = new CrawlistElement( curr_url, false );
                elem->add_load_time( diffTime( &time_start, &time_end) );
                elem->increase_counter();
                crawlist->add( elem );
                delete elem;

                boost::regex_split(front_inserter(raw_urls), data, e);

                while(raw_urls.size())
                {

                    raw_data = raw_urls.back();
                    raw_urls.pop_back();

                    // Überprüfung ob absolut/relativ (http:// am Anfang)
                    //      absolut: wenn letzte Stelle keine "/" -> dransparxXxen und in "urls" front pushen
                    //      relativ: an "base-url" dransparxXxen -> in "urls" front pushen

                    if( raw_data.find( "http://" ) == string::npos )
                        raw_data.insert( 0, base_url );

                    urls->append( raw_data );
#ifdef DEBUG
                cout << pthread_self() << ": raw_data: " << raw_data << endl;
#endif

                }

            }
            else
            {
                elem = new CrawlistElement( curr_url, true );
                elem->add_load_time( diffTime( &time_start, &time_end) );
                elem->increase_counter();
                crawlist->add( elem );
                delete elem;
            }
        }
    }
    curl_easy_cleanup( curl_handle );
    return 0;
}

int main( int argc, const char* argv[] )
{
    string start_point;
    curl_global_init( CURL_GLOBAL_ALL );

    /*
        args:
            start-point
            iterations
            threads

    */

    if (argc != 4) {
        cout << "you have to pass the start-point, iterations and num of threads, like 'crawler http://www.google.at 1000 4'" << endl;
        return 0;
    }

    start_point    = argv[1];
    iterations     = atoi(argv[2]);
    num_of_threads = atoi(argv[3]);


#ifdef DEBUG
    cout << "start_point: " << start_point << endl;
    cout << "iterations: " << iterations << endl;
    cout << "threads: " << num_of_threads << endl;
#endif

/*
    start-URL http://multimediatechnology.at/~fhs33741/web-crawler/0a6x0q2qk9ecjr0vbxrs.html
    HTML Parsen: 3 Arten von Links:
        - Anchor #asdf                      - 1. Stelle eine "#"?
        - Relative URL ../home.html | home.html | anderesDir/home.html
            - Relativ und Anchor gleich, weil einfach dransparxXxen

        - Absolute URL http://www.google.at | http://webcar.multimediatechnology.at:8000/home/
            - Überprüfung auf http:// am Anfang
                http://www.google.at                                | mehr als 2 "/"? - Nein: "/" dranhängen und fertig; wenn ja -> weiter
                http://webcar.multimediatechnology.at:8000/         | letztes Zeichen ein "/"? - Ja: fertig; wenn nein -> weiter
                http://webcar.multimediatechnology.at:8000/home     | Teil nach letztem "/" wegsparxXxen!

*/

    pthread_t thread[num_of_threads];

    clock_gettime( CLOCK_MONOTONIC, &total_time_start );

    urls->set_operators( num_of_threads );

    urls->append( start_point );

    cout << "creating threads" << endl;

    for( unsigned i = 0; i < num_of_threads; ++i)
        pthread_create( &thread[i], NULL, workerFunc, NULL );

    for( unsigned i = 0; i < num_of_threads; ++i)
        pthread_join(thread[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &total_time_end);

    cout << "************** CrawList **************" << endl;
    cout << "Total time needed: " << diffTime( &total_time_start, &total_time_end) << endl;
    cout << "Good Links found: " << crawlist->get_good() << endl;
    cout << "Broken Links found: " << crawlist->get_broken() << endl;
    cout << "Total Links found: " << crawlist->get_total() << endl << endl;
    cout << "************** Details **************" << endl;
    crawlist->print();

    delete crawlist;
    delete urls;

    return 0;
}