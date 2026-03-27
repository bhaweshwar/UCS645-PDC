CXX = mpic++
CXXFLAGS = -O2

TARGETS = daxpy broadcast dot_product_dist primes perfect

all: $(TARGETS)

daxpy: daxpy.cpp
	$(CXX) $(CXXFLAGS) daxpy.cpp -o daxpy

broadcast: broadcast.cpp
	$(CXX) $(CXXFLAGS) broadcast.cpp -o broadcast

dot_product_dist: distributed_dot.cpp
	$(CXX) $(CXXFLAGS) distributed_dot.cpp -o dot_product_dist

primes: prime_master_slave.cpp
	$(CXX) $(CXXFLAGS) prime_master_slave.cpp -o primes

perfect: perfect_numbers.cpp
	$(CXX) $(CXXFLAGS) perfect_numbers.cpp -o perfect

clean:
	rm -f $(TARGETS)
