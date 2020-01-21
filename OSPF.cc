#include <iostream>
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ospf-tag.h"
#include "ns3/global-router-interface.h"
#include "ns3/ipv4-routing-table-entry.h"
#include "ns3/ipv4-ospf-routing-helper.h"
#include "ns3/ipv4-ospf-routing.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/stats-module.h"
#include "ns3/rip.h"
#include "ns3/ripng-header.h"
#include "ns3/log.h"
#include "ns3/conf-loader.h"




using namespace ns3;
NS_LOG_COMPONENT_DEFINE("RIP_OSPF_Routing");
int main (int argc, char *argv[]){
int simtim=40;
bool verbose = false;
bool printRoutingTables = false;
bool showPings = false;
std::string SplitHorizon("poisonReverse");
int val1 = 0;
//trace st;

CommandLine cmd;
cmd.AddValue("verbose","turn on log component",verbose);
cmd.AddValue("printRoutingTables","Print routing tables at 10,20,30 and 40 seconds",printRoutingTables);
cmd.AddValue("showPings","Show Pings reception",showPings);
cmd.AddValue("splitHorizonStrategy","Split Horizon Strategy to use(NoSplitHorizon,SplitHorizon,PoisonReverse)",SplitHorizon);
cmd.Parse(argc,argv);
double min = 0.0;
double max = 20.0;
Ptr <UniformRandomVariable> x = CreateObject<UniformRandomVariable>();
x-> SetAttribute ("Min", DoubleValue(min));
x-> SetAttribute ("Max", DoubleValue(max));
if(verbose){
	LogComponentEnableAll(LogLevel(LOG_PREFIX_TIME | LOG_PREFIX_NODE));
	LogComponentEnable ("RipRouting", LOG_LEVEL_INFO);
	LogComponentEnable ("Rip", LOG_LEVEL_ALL);
	LogComponentEnable ("Ipv4Interface", LOG_LEVEL_ALL);
	LogComponentEnable ("Icmpv4L4Protocol", LOG_LEVEL_ALL);
	LogComponentEnable ("Ipv4L3Protocol", LOG_LEVEL_ALL);
	LogComponentEnable ("ArpCache", LOG_LEVEL_ALL);
	LogComponentEnable ("V4Ping", LOG_LEVEL_ALL);
}
if (SplitHorizon == "NoSplitHorizon"){
	Config::SetDefault ("ns3::Rip::SplitHorizon",EnumValue(RipNg::NO_SPLIT_HORIZON));
}
else if(SplitHorizon == "SplitHorizon"){
	Config::SetDefault("ns3::Rip::SplitHorizon",EnumValue(RipNg::NO_SPLIT_HORIZON));
}
else{
	Config::SetDefault("ns3::Rip::SplitHorizon",EnumValue(RipNg::NO_SPLIT_HORIZON));
}
NS_LOG_INFO("create nodes.");
Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

Ipv4OSPFRoutingHelper createnode;
Ipv4ListRoutingHelper nodeList;
nodeList.Add(createnode,10);

Ptr<Node>src = CreateObject<Node>();
Names::Add("Source",src);
Ptr<Node> dst = CreateObject<Node>();
Names::Add("Destination",dst);
Ptr<Node> r1 = CreateObject<Node>();
Names::Add("Router-1",r1);
Ptr<Node> r2 = CreateObject<Node>();
Names::Add("Router-2",r2);
Ptr<Node> r3 = CreateObject<Node>();
Names::Add("Router-3",r3);
Ptr<Node> r4 = CreateObject<Node>();
Names::Add("Router-4",r4);
Ptr<Node> r5 = CreateObject<Node>();
Names::Add("Router-5",r5);
Ptr<Node> r6 = CreateObject<Node>();
Names::Add("Router-6",r6);
Ptr<Node> r7 = CreateObject<Node>();
Names::Add("Router-7",r7);
Ptr<Node> r8 = CreateObject<Node>();
Names::Add("Router-8",r8);
nodeList.Create(src);
nodeList.Create(dst);
nodeList.Create(r1);
/*
createnode.Create(src);
createnode.Create(dst);
createnode.Create(r1);
createnode.Create(r2);
createnode.Create(r3);
createnode.Create(r4);
createnode.Create(r5);
createnode.Create(r6);
createnode.Create(r7);
createnode.Create(r8);
//createnode.Create(src);
*/
double value = x -> GetValue();
NodeContainer net1 (src,r1);
NodeContainer net2 (r1,r2);
NodeContainer net3 (r1,r3);
NodeContainer net4 (r3,r4);
NodeContainer net5 (r2,r4);
NodeContainer net6 (r4,dst);
NodeContainer net7 (r2,r7);
NodeContainer net8 (r7,r8);
NodeContainer net9 (r8,r6);
NodeContainer net10 (r4,r5);
NodeContainer net11 (r5,r6);
NodeContainer net12 (r5,r7);
NodeContainer net13 (r4,r7);
NodeContainer net14 (r7,r6);
NodeContainer routers (r1,r2,r3,r4);
NodeContainer nodes (src,dst);
NS_LOG_INFO("Create Channels.");

CsmaHelper csma;
csma.SetChannelAttribute("DataRate", DataRateValue(5000000));
csma.SetChannelAttribute("Delay", TimeValue(MilliSeconds(2)));
NetDeviceContainer ndc1 = csma.Install(net1);
NetDeviceContainer ndc2 = csma.Install(net2);
NetDeviceContainer ndc3 = csma.Install(net3);
NetDeviceContainer ndc4 = csma.Install(net4);
NetDeviceContainer ndc5 = csma.Install(net5);
NetDeviceContainer ndc6 = csma.Install(net6);
NetDeviceContainer ndc7 = csma.Install(net7);
NetDeviceContainer ndc8 = csma.Install(net8);
NetDeviceContainer ndc9 = csma.Install(net9);
NetDeviceContainer ndc10 = csma.Install(net10);
NetDeviceContainer ndc11 = csma.Install(net11);
NetDeviceContainer ndc12 = csma.Install(net12);
NetDeviceContainer ndc13 = csma.Install(net13);
NetDeviceContainer ndc14 = csma.Install(net14);
val1= x->GetValue();
NS_LOG_INFO ("Create Ipv4 and Routing");
RipHelper ripRouting;
ripRouting.ExcludeInterface(r1,1);
ripRouting.ExcludeInterface(r4,3);
ripRouting.SetInterfaceMetric(r3,3,10);
ripRouting.SetInterfaceMetric(r4,1,10);
Ipv4ListRoutingHelper listRH;
listRH.Add(ripRouting,0);
InternetStackHelper internet;
internet.SetIpv6StackInstall(false);
internet.SetRoutingHelper(listRH);
internet.Install(routers);
InternetStackHelper internetNodes;
internetNodes.SetIpv6StackInstall(false);
internetNodes.Install(nodes);
NS_LOG_INFO("Assign Ipv4 Addresses");
Ipv4AddressHelper ipv4;
ipv4.SetBase(Ipv4Address("10.0.0.0"), Ipv4Mask("255.255.255.0"));
Ipv4InterfaceContainer ic1= ipv4.Assign(ndc1);
ipv4.SetBase(Ipv4Address("10.0.1.0"), Ipv4Mask("255.255.255.0"));
Ipv4InterfaceContainer ic2= ipv4.Assign(ndc2);
ipv4.SetBase(Ipv4Address("10.0.2.0"), Ipv4Mask("255.255.255.0"));
Ipv4InterfaceContainer ic3= ipv4.Assign(ndc3);
ipv4.SetBase(Ipv4Address("10.0.3.0"), Ipv4Mask("255.255.255.0"));
Ipv4InterfaceContainer ic4= ipv4.Assign(ndc4);
ipv4.SetBase(Ipv4Address("10.0.4.0"), Ipv4Mask("255.255.255.0"));
Ipv4InterfaceContainer ic5= ipv4.Assign(ndc5);
ipv4.SetBase(Ipv4Address("10.0.5.0"), Ipv4Mask("255.255.255.0"));
Ipv4InterfaceContainer ic6= ipv4.Assign(ndc6);
//st.BER(val1,simtim);
cout << val1;
cout << simtim ;
Ptr<Ipv4StaticRouting> staticRouting;
staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting>(src->GetObject <Ipv4>() -> GetRoutingProtocol());
staticRouting ->SetDefaultRoute("10.0.0.2",1);
staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting>(dst->GetObject <Ipv4>() -> GetRoutingProtocol());
staticRouting ->SetDefaultRoute("10.0.6.1",1);
//st.Delay(value,simtim);
cout << value;
cout << simtim ;
if(printRoutingTables){
	RipHelper routingHelper;
	Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> (&std::cout);
	routingHelper.PrintRoutingTableAt(Seconds (10.0), r1, routingStream);
	routingHelper.PrintRoutingTableAt(Seconds (10.0), r2, routingStream);
	routingHelper.PrintRoutingTableAt(Seconds (10.0), r3, routingStream);
	routingHelper.PrintRoutingTableAt(Seconds (10.0), r4, routingStream);
	routingHelper.PrintRoutingTableAt(Seconds (20.0), r1, routingStream);
	routingHelper.PrintRoutingTableAt(Seconds (20.0), r2, routingStream);
	routingHelper.PrintRoutingTableAt(Seconds (20.0), r3, routingStream);
	routingHelper.PrintRoutingTableAt(Seconds (20.0), r4, routingStream);
	routingHelper.PrintRoutingTableAt(Seconds (30.0), r1, routingStream);
	routingHelper.PrintRoutingTableAt(Seconds (30.0), r2, routingStream);
	routingHelper.PrintRoutingTableAt(Seconds (30.0), r3, routingStream);
	routingHelper.PrintRoutingTableAt(Seconds (30.0), r4, routingStream);
	routingHelper.PrintRoutingTableAt(Seconds (40.0), r1, routingStream);
	routingHelper.PrintRoutingTableAt(Seconds (40.0), r2, routingStream);
	routingHelper.PrintRoutingTableAt(Seconds (40.0), r3, routingStream);
	routingHelper.PrintRoutingTableAt(Seconds (40.0), r4, routingStream);
}
NS_LOG_INFO("Create Applications.");
uint32_t packetSize = 1024;
uint32_t bandwidth = 1000;
Time interPacketInterval = Seconds(1.0);
V4PingHelper ping("10.0.6.2");
ping.SetAttribute("Interval", TimeValue (interPacketInterval));
ping.SetAttribute("Size", UintegerValue (packetSize));
ping.SetAttribute("Bandwidth", UintegerValue (bandwidth));
if(showPings){
	ping.SetAttribute("Verbose", BooleanValue (true));
}
ApplicationContainer apps = ping.Install(src);
apps.Start(Seconds(1.0));
apps.Stop(Seconds (simtim));
AsciiTraceHelper ascii;
csma.EnableAsciiAll(ascii.CreateFileStream("RIP-OSPF-Routing.tr"));

NS_LOG_INFO("Run Simulation");
Simulator :: Stop(Seconds(41.0));
Simulator::Run();
Simulator::Destroy();
NS_LOG_INFO ("Done.");
//system("gnuplot 'delay.plt'");
//system("gnuplot 'BER.plt'");
}
