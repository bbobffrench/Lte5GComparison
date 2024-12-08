#include "NrNetwork.h"

using namespace std;
using namespace ns3;

NrNetwork::NrNetwork(uint16_t port, uint16_t ueCount, TypeId socketType){
	gnbNode.Create(1);
	ueNodes.Create(ueCount);
	
	MobilityHelper gnbMobilityH;
	Ptr<ListPositionAllocator> gnbPos = CreateObject<ListPositionAllocator>();
	gnbPos->Add(Vector(0.0, 0.0, 40.0));
	gnbMobilityH.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	gnbMobilityH.SetPositionAllocator(gnbPos);
	gnbMobilityH.Install(gnbNode);

	MobilityHelper ueMobilityH;
	Ptr<ListPositionAllocator> uePos = CreateObject<ListPositionAllocator>();
	for(uint16_t i; i < ueCount; i++)
		uePos->Add(Vector(100, 100, 0.0));
	ueMobilityH.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	ueMobilityH.SetPositionAllocator(uePos);
	ueMobilityH.Install(ueNodes);
	
	epcH = CreateObject<NrPointToPointEpcHelper>();
	beamformingH = CreateObject<IdealBeamformingHelper>();
	nrH = CreateObject<NrHelper>();
	nrH->SetBeamformingHelper(beamformingH);
	nrH->SetEpcHelper(epcH);
	
	CcBwpCreator::SimpleOperationBandConf bandConf;
	band = ccBwpCreator.CreateOperationBandContiguousCc(bandConf);
	
	channelH = CreateObject<NrChannelHelper>();
	channelH->ConfigureFactories("UMa", "Default", "ThreeGpp");
	channelH->AssignChannelsToBands({band});
	allBwps = CcBwpCreator::GetAllBwps({band});
	
	NetDeviceContainer gnbDev = nrH->InstallGnbDevice(gnbNode, allBwps);
	NetDeviceContainer ueDevs = nrH->InstallUeDevice(ueNodes, allBwps);
	
	nrH->GetGnbPhy(gnbDev.Get(0), 0)->SetTxPower(30);

	DynamicCast<NrGnbNetDevice>(gnbDev.Get(0))->UpdateConfig();
	for (auto it = ueDevs.Begin(); it != ueDevs.End(); ++it)
        	DynamicCast<NrUeNetDevice>(*it)->UpdateConfig();

	// Create the remote host, and install the IP stack on it
	remoteHostNode.Create(1);
	InternetStackHelper internetH;
	internetH.Install(remoteHostNode);

	// Create P2P links between the packet gateway and the remote host
	PointToPointHelper p2pH;
	p2pH.SetDeviceAttribute("DataRate", DataRateValue(DataRate("100Gb/s")));
	p2pH.SetChannelAttribute("Delay", TimeValue(MilliSeconds(10)));
	p2pH.SetDeviceAttribute("Mtu", UintegerValue(1500));
	internetDev = p2pH.Install(epcH->GetPgwNode(), remoteHostNode.Get(0));

	// Assign IP addresses
	Ipv4AddressHelper ipv4H;
	ipv4H.SetBase("1.0.0.0", "255.0.0.0");
	internetIface = ipv4H.Assign(internetDev);

	// Create routes from the PGW to the remote hosts
	Ipv4StaticRoutingHelper routingH;
	Ptr<Ipv4> remoteHostIpv4 = remoteHostNode.Get(0)->GetObject<Ipv4>();
	Ptr<Ipv4StaticRouting> remoteHostRouting = routingH.GetStaticRouting(remoteHostIpv4);
	remoteHostRouting->AddNetworkRouteTo(Ipv4Address("7.0.0.0"), Ipv4Mask("255.0.0.0"), 1);

	// Install the IP stack on the UEs
	internetH.Install(ueNodes);
	ueIfaces = epcH->AssignUeIpv4Address(NetDeviceContainer(ueDevs));
	
	// Create a route from the gNB to the UE
	for(uint16_t i = 0; i < ueCount; i++){
		Ptr<Ipv4> ipv4 = ueNodes.Get(i)->GetObject<Ipv4>();
		Ptr<Ipv4StaticRouting> routing = routingH.GetStaticRouting(ipv4);
		routing->SetDefaultRoute(epcH->GetUeDefaultGatewayAddress(), 1);
	}

	// Attach the UEs with the gNB
	for(uint16_t i; i < ueCount; i++)
		nrH->AttachToGnb(ueDevs.Get(i), gnbDev.Get(0));

	// Create sockets
	for(uint16_t i; i < ueCount; i++){
		// Get the socket address for the current UE
		Ipv4Address ipv4Address = ueIfaces.GetAddress(i);
		InetSocketAddress socketAddress(ipv4Address, port);
		ueAddrs.push_back(socketAddress);

		// Create the UE socket
		Ptr<Socket> socket = Socket::CreateSocket(ueNodes.Get(i), socketType);
		socket->Bind(socketAddress);
		ueSockets.push_back(socket);

	}
	remoteHostSocket = Socket::CreateSocket(remoteHostNode.Get(0), socketType);
}
