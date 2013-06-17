# Howorka Sarah      - fhs33719
# Poschacher Michael - fhs33734
# Slamanig Sebastian - fhs33741

CXXFLAGS = -g -Wall -Weffc++ -std=c++0x
LDFLAGS = -lrt -pthread -lcurl -lboost_regex
CXX = g++
OUT = WebCrawler.out

all: webcrawler

webcrawler: *.cpp
	$(CXX) *.cpp $(CXXFLAGS) $(LDFLAGS) -o $(OUT)

mem: webcrawler
	valgrind --tool=memcheck --leak-check=full --show-reachable=yes ./$(OUT) http://multimediatechnology.at/~fhs33741/web-crawler/09bvqb9g90jemhx4s2y0.html 10 1

hel: webcrawler
	valgrind --tool=helgrind ./$(OUT) http://multimediatechnology.at/~fhs33741/web-crawler/09bvqb9g90jemhx4s2y0.html 10 4

clean:
	rm $(OUT) out/* -rf

run1:
	./$(OUT)  http://multimediatechnology.at/~fhs33741/web-crawler/09bvqb9g90jemhx4s2y0.html 10000 1

run2:
	./$(OUT)  http://multimediatechnology.at/~fhs33741/web-crawler/09bvqb9g90jemhx4s2y0.html 10000 2

run4:
	./$(OUT)  http://multimediatechnology.at/~fhs33741/web-crawler/09bvqb9g90jemhx4s2y0.html 10000 4

run8:
	./$(OUT)  http://multimediatechnology.at/~fhs33741/web-crawler/09bvqb9g90jemhx4s2y0.html 10000 8

fail1:
	./$(OUT)  http://multimediatechnology.at/~fhs33741/webcrawler-fail/4cnaa0fpw4ri16uszl6g.html 10000 1

fail2:
	./$(OUT)  http://multimediatechnology.at/~fhs33741/webcrawler-fail/4cnaa0fpw4ri16uszl6g.html 10000 2

fail4:
	./$(OUT)  http://multimediatechnology.at/~fhs33741/webcrawler-fail/4cnaa0fpw4ri16uszl6g.html 10000 4

fail8:
	./$(OUT)  http://multimediatechnology.at/~fhs33741/webcrawler-fail/4cnaa0fpw4ri16uszl6g.html 10000 8

google:
	./$(OUT)  http://www.google.at 1000 8