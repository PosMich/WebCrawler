/*********************************
 *                               *
 * Howorka Sarah      - fhs33719 *
 * Poschacher Michael - fhs33734 *
 * Slamanig Sebastian - fhs33741 *
 *                               *
 *********************************/
#ifndef URLQUEUE_H
#define URLQUEUE_H

#include <deque>
#include <pthread.h>
#include <string>
#include <iostream>

using namespace std;

class URLQueue {
    public:
        URLQueue( unsigned v_num_of_threads ) :mutex(PTHREAD_MUTEX_INITIALIZER), cond(PTHREAD_COND_INITIALIZER), terminate(false), q(), operators( v_num_of_threads ) {}
        ~URLQueue(){}
        void append(const string &x);
        string get();
        unsigned int size();
        void set_operators( unsigned v_num_of_threads );

    private:
        pthread_mutex_t mutex;
        pthread_cond_t cond;
        bool terminate;
        deque<string> q;
        unsigned operators;
};

#endif