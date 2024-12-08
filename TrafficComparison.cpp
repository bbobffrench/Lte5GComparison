#include "TrafficGenerator.h"
#include "LteNetwork.h"
#include "NrNetwork.h"

using namespace std;

void
LteVoip(uint16_t ueCount, uint32_t packetCount){
	uint16_t port = 5060;
	LteNetwork network(port, ueCount, ns3::UdpSocketFactory::GetTypeId());
	TrafficGenerator traffic(
		packetCount,
		network.GetRemoteHostSocket(),
		network.GetUeSockets(),
		network.GetUeAddrs()
	);
	traffic.SetPacketIntervalFunction([]() -> double { return 20e-3; });
	traffic.SetPacketSizeFunction([]() -> uint16_t { return 172; });
	traffic.RunSimulation();
}

void
NrVoip(uint16_t ueCount, uint32_t packetCount){
	uint16_t port = 5060;
	NrNetwork network(port, ueCount, ns3::UdpSocketFactory::GetTypeId());
	TrafficGenerator traffic(
		packetCount,
		network.GetRemoteHostSocket(),
		network.GetUeSockets(),
		network.GetUeAddrs()
	);
	traffic.SetPacketIntervalFunction([]() -> double { return 20e-3; });
	traffic.SetPacketSizeFunction([]() -> uint16_t { return 172; });
	traffic.RunSimulation();
}

void
LteVideostream(uint16_t ueCount, uint32_t packetCount){
	uint16_t port = 5060;
	LteNetwork network(port, ueCount, ns3::UdpSocketFactory::GetTypeId());
	TrafficGenerator traffic(
		packetCount,
		network.GetRemoteHostSocket(),
		network.GetUeSockets(),
		network.GetUeAddrs()
	);
	traffic.SetPacketIntervalFunction([]() -> double { return 15e-3; });
	traffic.SetPacketSizeFunction([]() -> uint16_t { return 1500; });
	traffic.RunSimulation();
}

void
NrVideostream(uint16_t ueCount, uint32_t packetCount){
	uint16_t port = 5060;
	NrNetwork network(port, ueCount, ns3::UdpSocketFactory::GetTypeId());
	TrafficGenerator traffic(
		packetCount,
		network.GetRemoteHostSocket(),
		network.GetUeSockets(),
		network.GetUeAddrs()
	);
	traffic.SetPacketIntervalFunction([]() -> double { return 15e-3; });
	traffic.SetPacketSizeFunction([]() -> uint16_t { return 1500; });
	traffic.RunSimulation();
}

void
LteIot(uint16_t ueCount, uint32_t packetCount){
	uint16_t port = 5060;
	LteNetwork network(port, ueCount, ns3::TcpSocketFactory::GetTypeId());
	TrafficGenerator traffic(
		packetCount,
		network.GetRemoteHostSocket(),
		network.GetUeSockets(),
		network.GetUeAddrs()
	);
	traffic.SetPacketIntervalFunction([]() -> double { return 1; });
	traffic.SetPacketSizeFunction([]() -> uint16_t { return 50; });
	traffic.RunSimulation();
}

void
NrIot(uint16_t ueCount, uint32_t packetCount){
	uint16_t port = 5060;
	NrNetwork network(port, ueCount, ns3::TcpSocketFactory::GetTypeId());
	TrafficGenerator traffic(
		packetCount,
		network.GetRemoteHostSocket(),
		network.GetUeSockets(),
		network.GetUeAddrs()
	);
	traffic.SetPacketIntervalFunction([]() -> double { return 1; });
	traffic.SetPacketSizeFunction([]() -> uint16_t { return 50; });
	traffic.RunSimulation();
}


int
main(int argc, char **argv){
	if(argc < 4){
		cout << "Usage: trafficComparison [ueCount] [packetCount]" << endl;
		return 1;
	}
	uint16_t ueCount = stoi(argv[1]);
	uint32_t packetCount = stoi(argv[2]);

	cout << "Simulation Type:\t" << argv[3] << endl;
	cout << "UE Count:\t\t" << ueCount << endl;
	cout << "Packets sent per UE:\t" << packetCount << endl << endl;

	string simulationType = argv[3];
	if(simulationType == "VoIP"){
		cout << "--- 5G Results ---" << endl;
		NrVoip(ueCount, packetCount);
		cout << endl << "--- LTE results ---" << endl;
		LteVoip(ueCount, packetCount);
	}
	else if(simulationType == "Video"){
		cout << "--- 5G Results ---" << endl;
		NrVideostream(ueCount, packetCount);
		cout << endl << "--- LTE results ---" << endl;
		LteVideostream(ueCount, packetCount);
	}
	else if(simulationType == "IoT"){
		cout << "--- 5G Results ---" << endl;
		NrVideostream(ueCount, packetCount);
		cout << endl << "--- LTE results ---" << endl;
		LteVideostream(ueCount, packetCount);
	}
	else cout << "Invalid simulation type, not running" << endl;

	return 0;
}
