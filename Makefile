CC=g++
LIBS=-lm

clusterer: driver.o KMeansClusterer.o Cluster.o ClusterImage.o
	$(CC) driver.o KMeansClusterer.o Cluster.o ClusterImage.o -o clusterer -std=c++11 $(LIBS)

%.o : %.cpp   
	$(CC) -c $< -o $@ -std=c++11

unitTests: unitTests.o HuffmanTree.o HuffmanNode.o
	$(CC) unitTests.o HuffmanTree.o HuffmanNode.o -o unitTests -std=c++11 $(LIBS)

all:
	make
	make unitTests
   
# deletes all the object code files as well as the previously generated outputs
clean:
	@rm -f *.o
	@rm clusterer
	#@rm unitTests
	
run:
	./clusterer Gradient_Numbers_PPMS -o testoutput -k 20 -bin 2

run-tests:
	./unitTests