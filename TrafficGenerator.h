#ifndef _TRAFFIC_GENERATOR_H_
#define _TRAFFIC_GENERATOR_H_

#include <ns3/core-module.h>
#include <ns3/network-module.h>

class TrafficGenerator{
private:
	uint64_t packetCount;

	// For throughput, latency, and packet loss calculations
	double meanDelay = 0;
	uint64_t sentPackets = 0;
	uint64_t receivedPackets = 0;
	uint64_t receivedBytes = 0;
	ns3::Time lastReceive;

	ns3::Ptr<ns3::Socket> srcSocket;
	std::vector<ns3::Ptr<ns3::Socket>> destSockets;
	std::vector<ns3::InetSocketAddress> destAddrs;


	std::function<double()> GetPacketInterval;
	std::function<uint64_t()> GetPacketSize;

	void SendPacket();
	void ReceivePacket(ns3::Ptr<ns3::Socket> socket);
public:
	TrafficGenerator(
		uint64_t packetCount,
		ns3::Ptr<ns3::Socket> srcSocket,
		std::vector<ns3::Ptr<ns3::Socket>> destSockets,
		std::vector<ns3::InetSocketAddress> destAddrs
	);

	void SetPacketIntervalFunction(std::function<double()> func){
		GetPacketInterval = func;
	}

	void SetPacketSizeFunction(std::function<uint64_t()> func){
		GetPacketSize = func;
	}

	void RunSimulation();
};

#endif
