# make CXX=clang++
# make CXX=g++-10
# valgrind --tool=helgrind ./a.out

atomic_race_v:
	$(CXX) -fsanitize=thread -g -O0 atomic_race.cpp

atomic_race_clean:
	$(CXX) -g -O0 atomic_race.cpp -lpthread