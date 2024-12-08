#ifndef _LTE_NETWORK_H_
#define _LTE_NETWORK_H_

#include <ns3/core-module.h>
#include <ns3/internet-module.h>
#include <ns3/applications-module.h>
#include <ns3/antenna-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/mobility-module.h>
#include <ns3/propagation-module.h>
#include <ns3/spectrum-module.h>
#include <ns3/lte-module.h>

class LteNetwork{
private:
	ns3::Ptr<ns3::LteHelper> lteH;
	ns3::Ptr<ns3::PointToPointEpcHelper> epcH;

	ns3::NodeContainer remoteHostNode;
	ns3::NodeContainer enbNode;
	ns3::NodeContainer ueNodes;

	ns3::NetDeviceContainer internetDev;
	ns3::NetDeviceContainer enbDev;
	ns3::NetDeviceContainer ueDevs;

	ns3::Ipv4InterfaceContainer internetIface;
	ns3::Ipv4InterfaceContainer ueIfaces;

	std::vector<ns3::Ptr<ns3::Socket>> ueSockets;
	std::vector<ns3::InetSocketAddress> ueAddrs;
	ns3::Ptr<ns3::Socket> remoteHostSocket;
public:
	LteNetwork(uint16_t port, uint16_t ueCount, ns3::TypeId socketType);

	std::vector<ns3::Ptr<ns3::Socket>> GetUeSockets(){ return ueSockets; }
	std::vector<ns3::InetSocketAddress> GetUeAddrs(){ return ueAddrs; }
	ns3::Ptr<ns3::Socket> GetRemoteHostSocket(){ return remoteHostSocket; }
};

#endif
