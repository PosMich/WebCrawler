WebCrawler
==========

WebCrawler, Sarah Howorka, Sebastian Slamanig, Michael Poschacher

Gruppeneinteilung
  * Sarah Howorka
  * Sebastian Slamanig
  * Michael Poschacher

Ablaufdiagramm Sequentiell:
> https://docs.google.com/drawings/d/1BWavdSnTYTv6N3DJz3Ode9d6aj1_aQ41bECQg5-uQU8/edit?usp=sharing

Ablaufdiagramm Parallel:
> https://docs.google.com/drawings/d/1cDzH89FdXP-niBBwqgEPWQ5d6InezPrxQR8ezgCpkyM/edit?usp=sharing

UML:
> https://docs.google.com/drawings/d/1nzph4sblU7sypvj6ONd3OxU6J87OWP6uYrzlwdLuyEI/edit?usp=sharing


Falls es Probleme gibt beim Öffnen der Links, bitte Mail an michael.poschacher@gmail.com



Ausführen:
----------
	./WebCrawler.out url iterations threads

	z.B: http://multimediatechnology.at/~fhs33741/web-crawler/09bvqb9g90jemhx4s2y0.html 10000 4

Builden:
--------
	make

Valgrind:
---------
	make mem

Helgrind:
---------
	make hel

Testfälle:
----------
	Startseite:
		http://multimediatechnology.at/~fhs33741/web-crawler/09bvqb9g90jemhx4s2y0.html
	Iterationen:
		10000

	Links:
		Good:   100
		Broken: 672

	make run1 --> 1 thread 
	make run2 --> 2 threads 
	make run4 --> 4 threads  
	make run8 --> 8 threads


	Startseite:
		http://multimediatechnology.at/~fhs33741/webcrawler-fail/4cnaa0fpw4ri16uszl6g.html
	Iterationen:
		10000
	
	Links:
		Good:	10
		Broken: 59

	make fail1 --> 1 thread
	make fail2 --> 2 threads
	make fail4 --> 4 threads
	make fail8 --> 8 threads

Einschränkungen:
----------------
	Cookies, Redirects werden nicht unterstützt.

	Auf Grund von Memory Leaks von curl ist SSL deaktiviert ( CURL_GLOBAL_NOTHING durch CURL_GLOBAL_ALL ersetzen aktiviert zwar ssl support, aber auch memory leaks  bzw. reachable memory )

Ausgaben:
---------
	Es wird nach Ablauf der Iterationen bzw. wenn alle Urls besucht wurden eine Statistik mit folgenden Werten ausgegeben:
	URL BROKEN? VISIT_COUNTER AVG_LOADTIME MAX_LOADTIME

	Zusammengeschönte Ausgabe: siehe web-crawler.ods bzw. web-crawler.pdf
