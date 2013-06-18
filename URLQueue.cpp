/*********************************
 *                               *
 * Howorka Sarah      - fhs33719 *
 * Poschacher Michael - fhs33734 *
 * Slamanig Sebastian - fhs33741 *
 *                               *
 *********************************/

#include "URLQueue.h"
#include <iostream>

using namespace std;

void URLQueue::set_operators( unsigned v_num_of_threads )
{
    operators = v_num_of_threads;
}

void URLQueue::append(const string &x)
{
    pthread_mutex_lock(&mutex);
    q.push_back(x.c_str());
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

unsigned int URLQueue::size()
{
    pthread_mutex_lock(&mutex);
    unsigned int s = q.size();
    pthread_mutex_unlock(&mutex);
    return s;
}

string URLQueue::get()
{
    while (!terminate)
    {
        pthread_mutex_lock(&mutex);

        string x = "";

        if (q.empty())
        {
            if( operators > 0)
                operators--;

            if( operators == 0 )
            {
                // cout << "Queue empty, nothing to do." << endl;
                pthread_cond_broadcast( &cond );
                pthread_mutex_unlock(&mutex);
                return "";
            }
            else
            {
                if( !pthread_cond_wait(&cond, &mutex))
                {
                    if( q.empty() )
                    {
                        pthread_mutex_unlock(&mutex);
                        continue;
                    }
                    else
                    {
                        operators++;
                        // cout << "*************************************************** increase operators." << endl;
                    }
                }
                else cout << "wait() failed." << endl;
            }


        }

        x = q.front();
        q.erase(q.begin());
        pthread_mutex_unlock(&mutex);
        return x;

    }
    return "";
}