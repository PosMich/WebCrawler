 7. Erklären Sie einer anderen Gruppe eine zentrale Methode Ihrer Implementierung. Bauen Sie die Verbesserungsvorschläge ein. Zeigen Sie Ihren Code vor und nach dem Review.

  Partner-Gruppe: Lang, Lederer & Schäkermann
  ===========================================

  Methode vor Verbesserung:
  _________________________

  // BRUM BRUM BRUM
  void *workerFunc( void *arg )
  {

      //cout << "Thread with id: " << pthread_self() << " is running." << endl;

      // unsigned curr_thread = (unsigned) arg;

      timespec time_start, time_end;

      string curr_url, base_url;
      deque<string> raw_urls;

      size_t found;
      unsigned slash_pos = 0;

      //CrawlistElement *elem = NULL;
      string data;
      string raw_data;

      CURL *curl_handle = NULL;
      CURLcode code;

      curl_handle = curl_easy_init();
      curl_easy_setopt( curl_handle, CURLOPT_WRITEFUNCTION, writeToString );
      curl_easy_setopt( curl_handle, CURLOPT_WRITEDATA, &data );
      curl_easy_setopt( curl_handle, CURLOPT_FAILONERROR, true );


      pthread_mutex_lock( &m_iterations );
      unsigned my_iterations = iterations;
      pthread_mutex_unlock( &m_iterations );

      while( my_iterations > 0 )
      {
          pthread_mutex_lock( &m_iterations );
              if (iterations > 0)
                  iterations--;
              my_iterations = iterations;
          pthread_mutex_unlock( &m_iterations );

          curr_url = urls->get();

          if( curr_url == "" )
          {
              //cout << "Thread " << pthread_self() << " exits." << endl;
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

              crawlist->add_load_time( curr_url, diffTime( &time_start, &time_end) );

          }
          else
          {

  #ifdef DEBUG
          cout << pthread_self() << ": --------------- Element not in CrawList -> add." << endl;
  #endif

              if ( code == 0 )
              {
  #ifdef DEBUG
                  cout << "create new CrawlistElement" << endl;
  #endif

                  crawlist->add( curr_url, diffTime( &time_start, &time_end), false);

                  pthread_mutex_lock(&m_regex);
                  boost::regex_split(front_inserter(raw_urls), data, e);
                  pthread_mutex_unlock(&m_regex);



                  while(raw_urls.size())
                  {

                      raw_data = raw_urls.back();
                      raw_urls.pop_back();

                      // Überprüfung ob absolut/relativ (http:// am Anfang)
                      //      absolut: wenn letzte Stelle keine "/" -> dransparxXxen und in "urls" front pushen
                      //      relativ: an "base-url" dransparxXxen -> in "urls" front pushen

                      if( raw_data.find( "://" ) == string::npos )
                          raw_data.insert( 0, base_url );

                      urls->append( raw_data );
  #ifdef DEBUG
                  cout << pthread_self() << ": raw_data: " << raw_data << endl;
  #endif

                  }

              }
              else
              {
                  crawlist->add( curr_url, diffTime( &time_start, &time_end), true);
              }
          }
      }
      curl_easy_cleanup( curl_handle );
      return 0;
  }

  Verbesserungsvorschläge:
  ________________________

    1. Kommentare ohne Aussagekraft wie "// BRUM BRUM BRUM" bitte entfernen ;-)

    2. Auskommentierten Code bitte entfernen "// unsigned curr_thread = (unsigned) arg;". Dazu ist das Version Control System da.

    3. deque<string> raw_urls; Hier würde eine normale queue ausreichen.

    4. Warum wird der Parsing-Vorgang mit einem Mutex gelockt? Bitte kommentieren:

      pthread_mutex_lock(&m_regex);
        boost::regex_split(front_inserter(raw_urls), data, e);
        pthread_mutex_unlock(&m_regex);

    5. Relative Links wie "path/to/file.txt" werden nicht korrekt erkannt.

Methode nach Verbesserung:
  __________________________

  void *workerFunc( void *arg )
  {
      timespec time_start, time_end;

      string curr_url, base_url;
      queue<string> raw_urls;

      size_t found;
      unsigned slash_pos = 0;

      string data;
      string raw_data;

      CURL *curl_handle = NULL;
      CURLcode code;

      curl_handle = curl_easy_init();
      curl_easy_setopt( curl_handle, CURLOPT_WRITEFUNCTION, writeToString );
      curl_easy_setopt( curl_handle, CURLOPT_WRITEDATA, &data );
      curl_easy_setopt( curl_handle, CURLOPT_FAILONERROR, true );

      pthread_mutex_lock( &m_iterations );
      unsigned my_iterations = iterations;
      pthread_mutex_unlock( &m_iterations );

      while( my_iterations > 0 )
      {
          pthread_mutex_lock( &m_iterations );
              if (iterations > 0)
                  iterations--;
              my_iterations = iterations;
          pthread_mutex_unlock( &m_iterations );

          curr_url = urls->get();

          if( curr_url == "" )
          {
              curl_easy_cleanup( curl_handle );
              pthread_exit(NULL);
          }

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
              crawlist->add_load_time( curr_url, diffTime( &time_start, &time_end) );
          }
          else
          {
  #ifdef DEBUG
          cout << pthread_self() << ": --------------- Element not in CrawList -> add." << endl;
  #endif
              if ( code == 0 )
              {
  #ifdef DEBUG
                  cout << "create new CrawlistElement" << endl;
  #endif
                  crawlist->add( curr_url, diffTime( &time_start, &time_end), false);

                  // boost::regex_split wird hier gelockt, da es im valgrind Fehlermeldungen produziert hat, als es noch nicht gelocket war.
                  pthread_mutex_lock(&m_regex);
                  boost::regex_split(front_inserter(raw_urls), data, e);
                  pthread_mutex_unlock(&m_regex);

                  while(raw_urls.size())
                  {

                      raw_data = raw_urls.back();
                      raw_urls.pop_back();

                      // Überprüfung ob absolut/relativ (http:// am Anfang)
                      //      absolut: wenn letzte Stelle keine "/" -> dransparxXxen und in "urls" front pushen
                      //      relativ: an "base-url" dransparxXxen -> in "urls" front pushen

                      if( raw_data.find( "://" ) == string::npos )
                          raw_data.insert( 0, base_url );

                      urls->append( raw_data );
  #ifdef DEBUG
                  cout << pthread_self() << ": raw_data: " << raw_data << endl;
  #endif
[23:55:47] Mike Schäkermann:
                  }

              }
              else
              {
                  crawlist->add( curr_url, diffTime( &time_start, &time_end), true);
              }
          }
      }
      curl_easy_cleanup( curl_handle );
      return 0;
  }


8. Reviewen Sie eine zentrale Methode einer anderen Gruppe. Notieren Sie welche Vorschläge Sie geäussert haben. Hinweis/Achten Sie auf: bugs, Synchronisation, Klarheit des Codes, memory leaks, Formatierung, Verständilchkeit, Einfachheit, Kommentare, ...

  Partner-Gruppe: Lang, Lederer & Schäkermann
  ===========================================

  Methode vor Verbesserung:
  _________________________

  void Worker::run()
  {
    CURL ** curls = new CURL *[numberOfConcurrentCurls];
    bool * curlsRunning = new bool[numberOfConcurrentCurls];
    CallbackParameters * parameters = new CallbackParameters[numberOfConcurrentCurls];
    CURLM * curlMulti = curl_multi_init();
    CURLMsg * message;
    int numberOfMessagesLeft;
    int numberOfRunningCurls = 0;

    if (curlMulti)
    {
      for (int c = 0; c < numberOfConcurrentCurls; ++c)
      {
        auto curl = curls[c] = curl_easy_init();
        if (curl)
        {
          // accept redirects
          curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
          // bind callback function to process response
          curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handleResponse);
          // pass crawler to callback function
          parameters[c].crawlerPtr = crawler;
          curl_easy_setopt(curl, CURLOPT_WRITEDATA, &(parameters[c]));
          // no signal needs to be set to 1 in multi-threading contexts
          curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
          // initalize running flag to false
          curlsRunning[c] = false;
        }
      }
    }

    while (keepRunning())
    {
      int numberOfCurlsToAdd = numberOfConcurrentCurls - numberOfRunningCurls;

      for (int c = 0; c < numberOfCurlsToAdd; ++c)
      {
        string URL = crawler->getNextTask();
        if (URL == "")
        {
          usleep(10);
          break;
        }

        unsigned curlIndex = 0;

        for (int d = 0; d < numberOfConcurrentCurls; ++d)
        {
          if (!curlsRunning[d])
          {
            curlsRunning[d] = true;
            curlIndex = d;
            break;
          }
        }

        auto curl = curls[curlIndex];
        auto& curlParameters = parameters[curlIndex];

        if (curl)
        {
          // add curl handle to curl multi handle
          curl_multi_add_handle(curlMulti, curl);
          // set URL
          curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
          // set parameters
          curlParameters.URL = URL;
          curlParameters.timer.start();
        }
      }

      do {
        curl_multi_perform(curlMulti, &numberOfRunningCurls);
      } while (numberOfRunningCurls == numberOfConcurrentCurls);

      while ((message = curl_multi_info_read(curlMulti, &numberOfMessagesLeft)))
      {
        if (message->msg == CURLMSG_DONE)
        {
          unsigned curlIndex = 0;
          auto curl = message->easy_handle;

          for (int c = 0; c < numberOfConcurrentCurls; ++c)
          {
            if (curls[c] == curl)
            {
              curlIndex = c;
              break;
            }
          }

          curlsRunning[curlIndex] = false;

          auto& curlParameters = parameters[curlIndex];
          curlParameters.timer.stop();

          long httpResponseCode = 0;
          curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpResponseCode);
          bool httpErrorOccured = (httpResponseCode >= 400);

          bool linkIsDead = (message->data.result != CURLE_OK || httpErrorOccured);

          crawler->finishTask(curlParameters.URL, linkIsDead, curlParameters.timer.getDeltaTimeInSeconds());

          // remove curl handle from curl multi handle
          curl_multi_remove_handle(curlMulti, curl);
        }
      }
    }

    do {
      curl_multi_perform(curlMulti, &numberOfRunningCurls);
    } while (numberOfRunningCurls > 0);

    for (int c = 0; c < numberOfConcurrentCurls; ++c)
    {
      if (curls[c])
      {
        curl_easy_cleanup(curls[c]);
      }
    }

    if (curlMulti)
    {
      curl_multi_cleanup(curlMulti);
    }

    delete[] curls;
    delete[] parameters;
    delete[] curlsRunning;

    return;
  }

  Verbesserungsvorschläge:
  ________________________

  Kommentare?
  Break nach dem Sleep - macht hier keinen Sinn?
  2x for durch while ersetzt
  Generell bei den Schleifen: Abbruchbedingungen?

  Methode nach Verbesserung:
  __________________________

  void Worker::run()
  {
      CURL ** curls = new CURL *[numberOfConcurrentCurls];
      bool * curlsRunning = new bool[numberOfConcurrentCurls];
      CallbackParameters * parameters = new CallbackParameters[numberOfConcurrentCurls];
      CURLM * curlMulti = curl_multi_init();
      CURLMsg * message;
      int numberOfMessagesLeft;
      int numberOfRunningCurls = 0;

      if (curlMulti)
      {
          for (int c = 0; c < numberOfConcurrentCurls; ++c)
          {
              auto curl = curls[c] = curl_easy_init();
              if (curl)
              {
                  // accept redirects
                  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
                  // bind callback function to process response
                  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handleResponse);
                  // pass crawler to callback function
                  parameters[c].crawlerPtr = crawler;
                  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &(parameters[c]));
                  // no signal needs to be set to 1 in multi-threading contexts
                  curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
                  // initalize running flag to false
                  curlsRunning[c] = false;
              }
          }
      }

      while (keepRunning())
      {
          int numberOfCurlsToAdd = numberOfConcurrentCurls - numberOfRunningCurls;

          // in this loop, idle CURL handles are refilled with new tasks (= URLs)
          for (int c = 0; c < numberOfCurlsToAdd; ++c)
          {
              string URL = crawler->getNextTask();
              if (URL == "")
              {
                  usleep(10);
                  break;
              }

              unsigned curlIndex = 0;

              while ( curlIndex < numberOfConcurrentCurls )
              {
                  if (!curlsRunning[curlIndex])
                  {
                      curlsRunning[curlIndex] = true;
                      break;
                  }
                  ++curlIndex;
              }

              auto curl = curls[curlIndex];
              auto& curlParameters = parameters[curlIndex];

              if (curl)
              {
                  // add curl handle to curl multi handle
                  curl_multi_add_handle(curlMulti, curl);
                  // set URL
                  curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
                  // set parameters
                  curlParameters.URL = URL;
                  curlParameters.timer.start();
              }
          }
          // perform all requests as long as at least one response is returned
          do {
              curl_multi_perform(curlMulti, &numberOfRunningCurls);
          } while (numberOfRunningCurls == numberOfConcurrentCurls);

          while ((message = curl_multi_info_read(curlMulti, &numberOfMessagesLeft)))
          {
              if (message->msg == CURLMSG_DONE)
              {
                  unsigned curlIndex = 0;
                  auto curl = message->easy_handle;

                  while ( curlIndex < numberOfConcurrentCurls )
                  {
                      if (curls[curlIndex] == curl)
                          break;
                      curlIndex++;
                  }

                  curlsRunning[curlIndex] = false;

                  auto& curlParameters = parameters[curlIndex];
                  curlParameters.timer.stop();

                  long httpResponseCode = 0;
                  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpResponseCode);
                  bool httpErrorOccured = (httpResponseCode >= 400);

                  bool linkIsDead = (message->data.result != CURLE_OK || httpErrorOccured);

                  crawler->finishTask(curlParameters.URL, linkIsDead, curlParameters.timer.getDeltaTimeInSeconds());

                  // remove curl handle from curl multi handle
                  curl_multi_remove_handle(curlMulti, curl);
              }
          }
      }

      // perform all requests until they have finished in order to prevent open pipes
      do {
          curl_multi_perform(curlMulti, &numberOfRunningCurls);
      } while (numberOfRunningCurls > 0);

      // clean up everything...
      for (int c = 0; c < numberOfConcurrentCurls; ++c)
      {
          if (curls[c])
          {
              curl_easy_cleanup(curls[c]);
          }
      }

      if (curlMulti)
      {
          curl_multi_cleanup(curlMulti);
      }

      delete[] curls;
      delete[] parameters;
      delete[] curlsRunning;

      return;
  }
