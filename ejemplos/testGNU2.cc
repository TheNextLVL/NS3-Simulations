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
//       n0    n1
//       |     |
//       =======
//         LAN
//
// - UDP flows from n0 to n1

#include <fstream>
#include "ns3/core-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/csma-module.h"
#include "ns3/gnuplot.h"
#include "ns3/internet-module.h"
#include <iomanip>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace ns3;

void ThroughputMonitor (FlowMonitorHelper *fmhelper, Ptr<FlowMonitor> flowMon,Gnuplot2dDataset DataSet);
void LatencyMonitor (FlowMonitorHelper *fmhelper, Ptr<FlowMonitor> flowMon);

NS_LOG_COMPONENT_DEFINE ("UdpclientServerExample");

class Simulation1 {
public:
	//Inicio de simulacion
	Simulation1();
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
	Gnuplot 	gnuplot;
	Gnuplot2dDataset dataset;

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
	// Serveraddress
	Address serverAddress;

	/// crea los nodos
	void CreateNodes();
	/// Instala internet en los nodos
	void InstallInternetStack();
	/// Instala las aplicaciones
	void InstallApplication();
	/// Configura GnuPlot
	void ConfigureGNU();
};

Simulation1::Simulation1(){
	m_startTime = 0.0;
	m_totalTime = 20.0;
	m_packetSize = 1024;
	m_maxPacketCount = 640;//320;
	lat = 2.0;
	rate = 5000000; // Data rate in bps
	interval = 0.01;//0.05;
	m_nnodes = 2;

	fileNameWithNoExtension = "FlowVSThroughput_";
	graphicsFileName        = fileNameWithNoExtension + ".png";
	plotFileName            = fileNameWithNoExtension + ".plt";
	plotTitle               = "Flow vs Throughput";
	dataTitle               = "Throughput";
}

void Simulation1::Configure(int argc, char *argv[]){
	LogComponentEnable("UdpClient", LOG_LEVEL_INFO);
	LogComponentEnable("UdpServer", LOG_LEVEL_INFO);

	bool useV6 = false;
	CommandLine cmd;
	cmd.AddValue("useIpv6", "Use Ipv6", useV6);
	cmd.Parse (argc, argv);

	NS_LOG_DEBUG ("Simulation Configure COMPLETE");

}

void Simulation1::CreateNodes(){

	NS_LOG_INFO ("Create nodes");

	// Creamos los nodos
	nodes.Create(m_nnodes);

	NS_LOG_INFO ("Create channels");
	CsmaHelper csma;
	csma.SetChannelAttribute ("DataRate", DataRateValue (DataRate (5000000)));
	csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
	csma.SetDeviceAttribute ("Mtu", UintegerValue (1400));
	NetDeviceContainer d = csma.Install(nodes);

}

void Simulation1::InstallInternetStack(){
	InternetStackHelper internetStack;
	internetStack.Install (nodes);

	Ipv4AddressHelper address;

	NS_LOG_INFO ("Assign IP Addresses.");
	address.SetBase ("10.1.1.0", "255.255.255.0");
	int0 = address.Assign (dev0);
	serverAddress = Address(int0.GetAddress(1));

	//Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
}

void Simulation1::InstallApplication(){
	NS_LOG_INFO ("Create Applications.");
	uint16_t port = 4000; // Need different port numbers to ensure there is no conflict

	// Udp Server
	UdpServerHelper server(port);
	ApplicationContainer apps = server.Install(nodes.Get(1));
	apps.Start(Seconds(1.0));
	apps.Stop(Seconds(10.0));

	// Udp Client
	uint32_t MaxPacketSize = 1024;
	Time interPacketInterval = Seconds (0.05);
	uint32_t maxPacketCount = 320;
	UdpClientHelper client (serverAddress, port);
	client.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
	client.SetAttribute ("Interval", TimeValue (interPacketInterval));
	client.SetAttribute ("PacketSize", UintegerValue (MaxPacketSize));
	apps = client.Install (nodes.Get (0));
	apps.Start (Seconds (2.0));
	apps.Stop (Seconds (10.0));
}

void Simulation1::ConfigureGNU(){
    // Set the filename and title
    gnuplot.SetOutputFilename(graphicsFileName);
    gnuplot.SetTitle (plotTitle);

    // Make the graphics file, which the plot file will be when it
    // is used with Gnuplot, be a PNG file.
    gnuplot.SetTerminal ("png");

    // Set the labels for each axis.
    gnuplot.SetLegend ("Time", "Throughput");

   dataset.SetTitle (dataTitle);
   dataset.SetStyle (Gnuplot2dDataset::LINES_POINTS);
}

int Simulation1::Run(){
	ConfigureGNU();
	CreateNodes();
	InstallInternetStack();
	InstallApplication();

	//flowMonitor declaration
	FlowMonitorHelper flowmon;
	Ptr<FlowMonitor> monitor = flowmon.InstallAll();
	// Llamamos a la funcion flowmonitor
	ThroughputMonitor(&flowmon, monitor, dataset);

	NS_LOG_INFO("Run Simulation.");
	Simulator::Stop (Seconds(m_totalTime));
	Simulator::Run ();

	LatencyMonitor(&flowmon, monitor);

	monitor->CheckForLostPackets();
	monitor->SerializeToXmlFile("testGNU.flowmn", true,true);

	//Gnuplot ...continued
	gnuplot.AddDataset (dataset);
	// Open the plot file.
	std::ofstream plotFile (plotFileName.c_str());
	// Write the plot file.
	gnuplot.GenerateOutput (plotFile);
	// Close the plot file.
	plotFile.close ();

	Simulator::Destroy ();

	NS_LOG_INFO("Done.");

	return 0;
}

void ThroughputMonitor (FlowMonitorHelper *fmhelper, Ptr<FlowMonitor> flowMon,Gnuplot2dDataset DataSet){
	Simulator::Schedule(Seconds(1),&ThroughputMonitor,fmhelper,flowMon, DataSet);

	double localThrou = 0;
	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (fmhelper->GetClassifier ());
	  std::map<FlowId, FlowMonitor::FlowStats> stats = flowMon->GetFlowStats ();
	  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
	    {
		  Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
	      if ((t.sourceAddress=="10.1.1.1" && t.destinationAddress == "10.1.2.2"))
	      {
	          std::cout << "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")" << std::endl;
	          std::cout << "  Tx Bytes:   " << i->second.txBytes << std::endl;
	          std::cout << "  Rx Bytes:   " << i->second.rxBytes << std::endl;
	          std::cout << "  Duration:   " << (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) << std::endl;
	      	  std::cout << "  Last Received Packet:    " << i->second.timeLastRxPacket.GetSeconds()<<" seconds" << std::endl;
	          std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/1024  << " Mbps"<< std::endl;
	          localThrou =(i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds()-i->second.timeFirstTxPacket.GetSeconds())/1024/1024);
	          // Actualizamos datos de Gnu
	          DataSet.Add((double)Simulator::Now().GetSeconds(),(double) localThrou);
	          std::cout<<"---------------------------------------------------------------------------"<<std::endl;
	      }
	     }
	  flowMon->SerializeToXmlFile("ThroughputMonitor.xml",true,true);
}

void LatencyMonitor(FlowMonitorHelper *fmhelper, Ptr<FlowMonitor> flowMon){
	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (fmhelper->GetClassifier ());
	  std::map<FlowId, FlowMonitor::FlowStats> stats = flowMon->GetFlowStats ();
	  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
	    {
		  Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
	      if ((t.sourceAddress=="10.1.1.1" && t.destinationAddress == "10.1.2.2"))
	      {
	          std::cout<<"---------------------------------------------------------------------------"<<std::endl;
	    	  std::cout << "Latency:    " << i-> second.delaySum.GetMilliSeconds()/i->second.rxPackets << "ms"<<std::endl;
	          std::cout<<"---------------------------------------------------------------------------"<<std::endl;
	      }
	     }
}

int main (int argc, char *argv[])
{
  Simulation1 s;
  s.Configure (argc, argv);
  return s.Run ();
}

