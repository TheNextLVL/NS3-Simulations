/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */



/*
	LAB Assignment #1
	1. Create a simple topology of two nodes (Node1, Node2)
	separated by a point-to-point link.

	2. Setup a UdpClient on one Node1 and a UdpServer on Node2.
	Let it be of a fixed data rate Rate1.

	3. Start the client application, and measure end to end throughput
	whilst varying the latency of the link.

	4. Now add another client application to Node1 and a server instance to Node2.
	What do you need to configure to ensure that there is no conflict?

	5. Repeat step 3 with the extra client and server application instances.
	Show screenshots of pcap traces which indicate that delivery is made to the appropriate server instance.

	Solution by: Konstantinos Katsaros (K.Katsaros@surrey.ac.uk)
	based on udp-client-server.cc
*/

// Network topology
//
//       n1 ------ n2
//            p2p
//
// - UDP flows from n1 to n2

#include <fstream>
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/gnuplot.h"
#include <iomanip>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace ns3;

void ThroughputMonitor (FlowMonitorHelper *fmhelper, Ptr<FlowMonitor> flowMon);

NS_LOG_COMPONENT_DEFINE ("Lab1");

class Simulation0 {
public:
	//Inicio de simulacion
	Simulation0();
	// Configuracion de la siumacion desde los argumentos de la linea de comandos
	void Configure(int argc, char ** argv);
	// Iniciar la simulacion
	int Run();

private:
	//Declaracion de variables
	double		m_startTime;
	double 		m_totalTime;
//	double		m_packetInterval;
	uint16_t	m_packetSize;
	uint32_t    m_maxPacketCount;
//	uint32_t	m_nIfaces; // No necesario ?
	uint32_t	m_nnodes;
	double 		lat;
	uint64_t 	rate;
	double 		interval;
	double		mtu;

	//Parametros gnu
	std::string fileNameWithNoExtension;
	std::string graphicsFileName;
	std::string plotFileName;
	std::string plotTitle;
	std::string dataTitle;

	// Contenedor de las redes (nodos)
	NodeContainer nodes;
	// contenedor de los dispositivos
	NetDeviceContainer dev0;
	NetDeviceContainer dev1;
	// Contenedor de las interfaces
	Ipv4InterfaceContainer int0;
	Ipv4InterfaceContainer int1;

	/// crea los nodos
	void CreateNodes();
	/// Instala internet en los nodos
	void InstallInternetStack();
	/// Instala las aplicaciones
	void InstallApplication();
};

Simulation0::Simulation0(){
	m_startTime = 0.0;
	m_totalTime = 11.0;
	m_packetSize = 1024;
	m_maxPacketCount = 640;//320;
	mtu = 1400;
	lat = 2.0;
	rate = 100000; // Data rate in bps --> 10Mbps
	interval = 0.05;
	m_nnodes = 3;
}

void Simulation0::Configure(int argc, char *argv[]){
	CommandLine cmd;
	cmd.AddValue ("latency", "P2P link Latency in miliseconds", lat);
	cmd.AddValue ("rate", "P2P data rate in bps", rate);
	cmd.AddValue ("interval", "UDP client packet interval", interval);

	cmd.Parse (argc, argv);

	NS_LOG_DEBUG ("Simulation Configure COMPLETE");

}

void Simulation0::CreateNodes(){

	NS_LOG_INFO ("Create " << m_nnodes << " nodes");

	// Creamos los nodos
	nodes.Create(m_nnodes);

	NS_LOG_INFO ("Create channels.");

	PointToPointHelper p2p;
	p2p.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (lat)));
	p2p.SetDeviceAttribute ("DataRate", DataRateValue (DataRate (rate)));
	p2p.SetDeviceAttribute ("Mtu", UintegerValue (mtu));

	dev0 = p2p.Install (nodes.Get(0), nodes.Get(1));
	dev1 = p2p.Install (nodes.Get(1), nodes.Get(2));

	//
	// Tracing
	//
	AsciiTraceHelper ascii;
	p2p.EnableAscii(ascii.CreateFileStream ("lab-1.tr"), dev0);
	p2p.EnablePcap("lab-1", dev0, false);
}

void Simulation0::InstallInternetStack(){
	InternetStackHelper internetStack;
	internetStack.Install (nodes);
	Ipv4AddressHelper address;

	NS_LOG_INFO ("Assign IP Addresses.");
	address.SetBase ("10.1.1.0", "255.255.255.0");
	int0 = address.Assign (dev0);

	address.SetBase ("10.1.2.0", "255.255.255.0");
	int1 = address.Assign (dev1);


	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
}

void Simulation0::InstallApplication(){
	NS_LOG_INFO ("Create Applications.");
	uint16_t port1 = 8000; // Need different port numbers to ensure there is no conflict
	uint16_t port2 = 8001;

	//
	// SERVER
	//

	UdpServerHelper server1 (port1);
	UdpServerHelper server2 (port2);

	ApplicationContainer apps;
	apps = server1.Install (nodes.Get (2));
	apps = server2.Install (nodes.Get (2));

	apps.Start (Seconds (m_startTime));
	apps.Stop (Seconds (m_totalTime));

	//
	// CLIENT
	//
	Time interPacketInterval = Seconds (interval);
	UdpClientHelper client1 (int0.GetAddress (1), port1);
	UdpClientHelper client2 (int1.GetAddress (1), port2);

	client1.SetAttribute ("MaxPackets", UintegerValue (m_maxPacketCount));
	client1.SetAttribute ("Interval", TimeValue (interPacketInterval));
	client1.SetAttribute ("PacketSize", UintegerValue (m_packetSize));

	client2.SetAttribute ("MaxPackets", UintegerValue (m_maxPacketCount));
	client2.SetAttribute ("Interval", TimeValue (interPacketInterval));
	client2.SetAttribute ("PacketSize", UintegerValue (m_packetSize));

	apps = client1.Install (nodes.Get (0));
	apps = client2.Install (nodes.Get (0));

	apps.Start (Seconds (m_startTime));
	apps.Stop (Seconds (m_totalTime));

}

int Simulation0::Run(){
	CreateNodes();
	InstallInternetStack();
	InstallApplication();

	//flowMonitor declaration
	FlowMonitorHelper flowmon;
	Ptr<FlowMonitor> monitor = flowmon.InstallAll();
	// Llamamos a la funcion flowmonitor

	NS_LOG_INFO("Run Simulation.");
	Simulator::Stop (Seconds(m_totalTime));
	Simulator::Run ();

	monitor->CheckForLostPackets();
	ThroughputMonitor(&flowmon, monitor);
	monitor->SerializeToXmlFile("testGNU.flowmn", true,true);

	Simulator::Destroy ();

	return 0;
}


int main (int argc, char *argv[])
{
  Simulation0 s;
  s.Configure (argc, argv);
  return s.Run ();
}

void ThroughputMonitor (FlowMonitorHelper *fmhelper, Ptr<FlowMonitor> flowMon){
	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (fmhelper->GetClassifier ());
	  std::map<FlowId, FlowMonitor::FlowStats> stats = flowMon->GetFlowStats ();
	  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
	    {
		  Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
	      if ((t.sourceAddress=="10.1.1.1" && t.destinationAddress == "10.1.2.2"))
	      {
	          std::cout<<"---------------------------------------------------------------------------"<<std::endl;
	          std::cout << "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")" << std::endl;
	          std::cout << "  Bandwith:   " << 100000/1000000 <<"Mbps" << std::endl;
	          std::cout << "  Tx Bytes:   " << i->second.txBytes << std::endl;
	          std::cout << "  Rx Bytes:   " << i->second.rxBytes << std::endl;
	          std::cout << "  Duration:   " << (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) << " s"<< std::endl;
	          std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/1024  << " Mbps"<< std::endl;
	          std::cout << "  Latency:    " << i->second.delaySum.GetMilliSeconds()/i->second.rxPackets << " ms" << std::endl;
	          std::cout << "  Jitter:     " << i->second.jitterSum.GetMilliSeconds()/i->second.rxPackets << " ms" << std::endl;
	          std::cout << "  Accuracy:   " << (i->second.txPackets - i->second.rxPackets)/100 << " %" << std::endl;
	          std::cout << "  Message Size:" << 1400/8 << " bytes" << std::endl;
	          std::cout << "  Payload:    " << (1400/8) - (8 + 20 + 6) << " bytes" << std::endl;
	          std::cout<<"---------------------------------------------------------------------------"<<std::endl;
	      }
	     }

	  flowMon->SerializeToXmlFile("ThroughputMonitor.xml",true,true);

}
