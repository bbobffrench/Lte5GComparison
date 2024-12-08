#include "TrafficGenerator.h"

using namespace std;
using namespace ns3;

void
TrafficGenerator::SendPacket(){
	if(sentPackets >= (packetCount * destSockets.size())){
		Simulator::Schedule(MilliSeconds(100), &Simulator::Stop);
		return;
	}

	Ptr<Packet> packet = Create<Packet>(GetPacketSize());
	TimestampTag tag;
	tag.SetTimestamp(Simulator::Now());
	packet->AddPacketTag(tag);

	for(uint16_t i = 0; i < destSockets.size(); i++)
		srcSocket->SendTo(packet, 0, destAddrs[i]);
	sentPackets+= destSockets.size();

	Simulator::Schedule(Seconds(GetPacketInterval()), &TrafficGenerator::SendPacket, this);
}

void
TrafficGenerator::ReceivePacket(Ptr<Socket> socket){
	Ptr<Packet> packet = socket->Recv();
	receivedBytes += packet->GetSize();
	receivedPackets++;

	TimestampTag tag;
	packet->PeekPacketTag(tag);
	Time sendTime = tag.GetTimestamp();
	lastReceive = Simulator::Now();
	double delay = lastReceive.GetMilliSeconds() - sendTime.GetMilliSeconds();
	meanDelay = (meanDelay + delay) / (double)2;
}

TrafficGenerator::TrafficGenerator(
	uint64_t packetCount,
	Ptr<Socket> srcSocket,
	vector<Ptr<Socket>> destSockets,
	vector<InetSocketAddress> destAddrs
){
	this->packetCount = packetCount;
	this->srcSocket = srcSocket;
	this->destSockets = destSockets;
	this->destAddrs = destAddrs;

	for(Ptr<Socket> socket : destSockets)
		socket->SetRecvCallback(MakeCallback(&TrafficGenerator::ReceivePacket, this));
}

void
TrafficGenerator::RunSimulation(){
	cout << "Running simulation..." << endl;
	
	Simulator::Schedule(MilliSeconds(50), &TrafficGenerator::SendPacket, this);
	Simulator::Run();
	Simulator::Destroy();

	Time totalTime = lastReceive - MilliSeconds(50);
	uint32_t throughput = (receivedBytes * 8) / totalTime.GetSeconds();
	double packetLoss = ((sentPackets - receivedPackets) / (double)sentPackets) * 100;

	cout << "Throughput:\t" << throughput << " bps" << endl;
	cout << "Latency (ms):\t" << meanDelay << " ms" << endl;
	cout << "Packet loss:\t" << packetLoss << "%" << endl;
}
