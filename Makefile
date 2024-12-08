NS3_INCLUDE_DIR := /home/bob/bin/ns-3-dev/build/include
NS3_LIB_DIR     := /home/bob/bin/ns-3-dev/build/lib

CXX      := g++
CXXFLAGS := -std=c++20 -I$(NS3_INCLUDE_DIR)
LDFLAGS  := -L$(NS3_LIB_DIR) -lns3-dev-core-default -lns3-dev-network-default  \
            -lns3-dev-internet-default -lns3-dev-applications-default          \
	    -lns3-dev-point-to-point-default -lns3-dev-mobility-default        \
	    -lns3-dev-lte-default -lns3-dev-nr-default                         \
	    -lns3-dev-antenna-default -lns3-dev-propagation-default

TARGET := trafficComparison
OBJS   := TrafficGenerator.o LteNetwork.o NrNetwork.o TrafficComparison.o

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

TrafficGenerator.o: TrafficGenerator.cpp TrafficGenerator.h
	$(CXX) -c TrafficGenerator.cpp $(CXXFLAGS)

LteNetwork.o: LteNetwork.cpp LteNetwork.h
	$(CXX) -c LteNetwork.cpp $(CXXFLAGS)

NrNetwork.o: NrNetwork.cpp NrNetwork.h
	$(CXX) -c NrNetwork.cpp $(CXXFLAGS)

TrafficComparison.o: TrafficGenerator.h LteNetwork.h NrNetwork.h TrafficComparison.cpp
	$(CXX) -c TrafficComparison.cpp $(CXXFLAGS)

.PHONY: clean
clean:
	rm -rf $(OBJS) $(TARGET)
