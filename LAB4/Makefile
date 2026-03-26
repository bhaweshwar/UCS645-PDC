CXX = mpic++
CXXFLAGS = -O2

TARGETS = ring array_sum max_min dot_product

all: $(TARGETS)

ring: ring.cpp
	$(CXX) $(CXXFLAGS) ring.cpp -o ring

array_sum: array_sum.cpp
	$(CXX) $(CXXFLAGS) array_sum.cpp -o array_sum

max_min: max_min.cpp
	$(CXX) $(CXXFLAGS) max_min.cpp -o max_min

dot_product: dot_product.cpp
	$(CXX) $(CXXFLAGS) dot_product.cpp -o dot_product

clean:
	rm -f $(TARGETS)
