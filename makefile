CPPFLAGS= -Wall -Wextra -pedantic -g

all:
	g++ -std=c++17 $(CPPFLAGS) Test_Interpolate.cpp -o Test_Interpolate

run:
	./Test_Interpolate

checkmem:
	valgrind --leak-check=yes -v ./Test_Interpolate

clean:
	rm Test_Interpolate
#
# all:
# 	g++ -std=c++17 $(CPPFLAGS) testI.cpp -o testI
#
# run:
# 	./testI
#
# checkmem:
# 	valgrind --leak-check=yes -v ./testI
#
# clean:
# 	rm testI
