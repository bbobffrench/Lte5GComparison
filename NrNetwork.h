#ifndef _NR_NETWORK_H_
#define _NR_NETWORK_H_

#include <ns3/core-module.h>
#include <ns3/internet-module.h>
#include <ns3/applications-module.h>
#include <ns3/antenna-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/mobility-module.h>
#include <ns3/propagation-module.h>
#include <ns3/spectrum-module.h>
#include <ns3/nr-module.h>

class NrNetwork{
private:
	ns3::Ptr<ns3::NrHelper> nrH;
	ns3::Ptr<ns3::NrPointToPointEpcHelper> epcH;
	ns3::Ptr<ns3::IdealBeamformingHelper> beamformingH;

	ns3::NodeContainer remoteHostNode;
	ns3::NodeContainer gnbNode;
	ns3::NodeContainer ueNodes;

	ns3::CcBwpCreator ccBwpCreator;
	ns3::OperationBandInfo band;
	ns3::BandwidthPartInfoPtrVector allBwps;
	ns3::Ptr<ns3::NrChannelHelper> channelH;

	ns3::NetDeviceContainer internetDev;
	ns3::NetDeviceContainer gnbDev;
	ns3::NetDeviceContainer ueDevs;

	ns3::Ipv4InterfaceContainer internetIface;
	ns3::Ipv4InterfaceContainer ueIfaces;

	std::vector<ns3::Ptr<ns3::Socket>> ueSockets;
	std::vector<ns3::InetSocketAddress> ueAddrs;
	ns3::Ptr<ns3::Socket> remoteHostSocket;
public:
	NrNetwork(uint16_t port, uint16_t ueCount, ns3::TypeId socketType);

	std::vector<ns3::Ptr<ns3::Socket>> GetUeSockets(){ return ueSockets; }
	std::vector<ns3::InetSocketAddress> GetUeAddrs(){ return ueAddrs; }
	ns3::Ptr<ns3::Socket> GetRemoteHostSocket(){ return remoteHostSocket; }
};

#endif
