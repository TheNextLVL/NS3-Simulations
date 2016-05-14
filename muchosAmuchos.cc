/**
* @file topology2.cc
* @brief NS3 Simulation of Complex Network, using Gnuplot to plot the stadistics of the network.
* @Author Adolfo, Cristina, Elena y Tinu.
* @date 11/05/2016
*/

#include "ns3/point-to-point-module.h"
#include "ns3/core-module.h"
#include "ns3/global-route-manager.h"
#include "ns3/bridge-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/csma-helper.h"
#include <fstream>
#include "PlotGNU.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Topologia v2");

/**
@brief  Clase que contiene toda la simulacion
*/
class Simulacion {
public:
	//Contructor
	Simulacion();
	// Configuracion de la siumacion desde los argumentos de la linea de comandos
	void Configure(int argc, char ** argv);
	// Iniciar la simulacion
	int Run();

private:
	//Declaracion de variables
	double		m_startTime;
	double		m_stopTime;
	uint16_t	m_packetSize;
	uint32_t    m_maxPacketCount;
	std::string		m_2delay;
	std::string		m_5delay;
	std::string		m_10delay;
	std::string 	m_100rate;
	std::string		m_200rate;
	std::string		m_500rate;

	double 		m_interval;

	/* Contenedores de las redes */
	NodeContainer p2pNodes;
	
	NodeContainer nc_r3s0;
	NodeContainer nc_r3s1;
	NodeContainer nc_r3s2;
	
	NodeContainer nc_r6r3;
	NodeContainer nc_r6r5;
	NodeContainer nc_r6r7;
	NodeContainer nc_r5v4;
	
	NodeContainer nc_r7s8;
	NodeContainer nc_r7s9;
	NodeContainer nc_r7s10;
	NodeContainer nc_r7s11;

	//contiene los ordenadores que estan conectados a ellos
	NodeContainer nc_sw0;
	NodeContainer nc_sw1;
	NodeContainer nc_ap2;
	NodeContainer nc_sw8;
	NodeContainer nc_sw9;
	NodeContainer nc_sw10;
	NodeContainer nc_ap11;

	/* Internet Stack */
	InternetStackHelper istack;

	/* Interface Container*/
	Ipv4InterfaceContainer ic_r3s0;
	Ipv4InterfaceContainer ic_r3s1;
	Ipv4InterfaceContainer ic_r3s2;

	Ipv4InterfaceContainer ic_r6r3;
	Ipv4InterfaceContainer ic_r6r5;
	Ipv4InterfaceContainer ic_r6r7;
	Ipv4InterfaceContainer ic_r5v4;

	Ipv4InterfaceContainer ic_r7s8;
	Ipv4InterfaceContainer ic_r7s9;
	Ipv4InterfaceContainer ic_r7s10;
	Ipv4InterfaceContainer ic_r7s11;

	Ipv4InterfaceContainer ic_sw0;
	Ipv4InterfaceContainer ic_sw1;

	Ipv4InterfaceContainer ic_sw8;
	Ipv4InterfaceContainer ic_sw9;
	Ipv4InterfaceContainer ic_sw10;

	/* Declaraci�n de las funciones */

	void createTopology();	/// crea los nodos

	void installInternetStack(); /// Instala internet en los nodos

	void installAplications(); /// Instala las aplicaciones
};


/**
@brief  Configura los parametros de la simulacion -- NO TERMINADO
*/
Simulacion::Simulacion() {
	// NO TERMINADO
	m_startTime = 1.0;
	m_stopTime = 10.0;
	m_packetSize = 1024;
	m_maxPacketCount = 350;
	m_2delay = "2ms";
	m_5delay = "5ms";
	m_10delay = "10ms";
	m_100rate = "100Mbps";
	m_200rate = "200Mbps";
	m_500rate = "500Mbps";
	m_interval = 0.2;

	NS_LOG_DEBUG("Contruccion Simulacion COMPLETE");
}


/**
@brief  Configuracion de la siumacion desde los argumentos de la linea de comandos
*/
void Simulacion::Configure(int argc, char ** argv) {

	CommandLine cmd;
	cmd.Parse(argc, argv);

	NS_LOG_DEBUG("Simulacion Configure COMPLETE");
}

/**
@brief Crea y configura los nodos.
*/
void Simulacion::createTopology() {

	NS_LOG_INFO("Creating the nodes");
	// Creacion de nodos p2p y enlaces p2p entre ellos
	p2pNodes.Create(12);

	nc_r3s0 = NodeContainer(p2pNodes.Get(3), p2pNodes.Get(0));
	nc_r3s1 = NodeContainer(p2pNodes.Get(3), p2pNodes.Get(1));
	nc_r3s2 = NodeContainer(p2pNodes.Get(3), p2pNodes.Get(2));

	nc_r6r3 = NodeContainer(p2pNodes.Get(6), p2pNodes.Get(3));
	nc_r6r5 = NodeContainer(p2pNodes.Get(6), p2pNodes.Get(5));
	nc_r6r7 = NodeContainer(p2pNodes.Get(6), p2pNodes.Get(7));
	nc_r5v4 = NodeContainer(p2pNodes.Get(5), p2pNodes.Get(4));

	nc_r7s8 = NodeContainer(p2pNodes.Get(7), p2pNodes.Get(8));
	nc_r7s9 = NodeContainer(p2pNodes.Get(7), p2pNodes.Get(9));
	nc_r7s10 = NodeContainer(p2pNodes.Get(7), p2pNodes.Get(10));
	nc_r7s11 = NodeContainer(p2pNodes.Get(7), p2pNodes.Get(11));

	// Creacion de nodos csma
	// hay que repetirlo para cada switch
	nc_sw0.Add(p2pNodes.Get(0));
	nc_sw0.Create(6);

	nc_sw1.Add(p2pNodes.Get(1));
	nc_sw1.Create(10);

	nc_ap11.Add(p2pNodes.Get(2));
	nc_ap11.Add(p2pNodes.Get(11));

	nc_sw8.Add(p2pNodes.Get(8));
	nc_sw8.Create(10);

	nc_sw9.Add(p2pNodes.Get(9));
	nc_sw9.Create(10);

	nc_sw10.Add(p2pNodes.Get(10));
	nc_sw10.Create(5);



}

void Simulacion::installInternetStack() {

	// Instalamos internet en todos los nodos p2p y csma
	// *NOTA: Solo 1 internet por dispositivo!!!. Primero todos csma y luego resto p2p (p2p son menos...)
	for (int n = 3; n <8 ;n++)
		istack.Install(p2pNodes.Get(n));
	
	istack.Install(nc_sw0);
	istack.Install(nc_sw1);
	istack.Install(nc_ap2);
	istack.Install(nc_sw8);
	istack.Install(nc_sw9);	
	istack.Install(nc_sw10);
	istack.Install(nc_ap11);

	// Creamos los canales ente las redes p2p y asignamos distintos atributos
	PointToPointHelper p2p_rs;
	PointToPointHelper p2p_rr;
	PointToPointHelper p2p_rv;

	p2p_rs.SetDeviceAttribute("DataRate", StringValue(m_100rate));
	p2p_rs.SetChannelAttribute("Delay", StringValue(m_2delay));

	p2p_rr.SetDeviceAttribute("DataRate", StringValue(m_200rate));
	p2p_rr.SetChannelAttribute("Delay", StringValue(m_5delay));

	p2p_rv.SetDeviceAttribute("DataRate", StringValue(m_500rate));
	p2p_rv.SetChannelAttribute("Delay", StringValue(m_10delay));

	NetDeviceContainer ndc_r3s0 = p2p_rs.Install(nc_r3s0);
	NetDeviceContainer ndc_r3s1 = p2p_rs.Install(nc_r3s1);
	NetDeviceContainer ndc_r3s2 = p2p_rs.Install(nc_r3s2);

	NetDeviceContainer ndc_r6r3 = p2p_rr.Install(nc_r6r3);
	NetDeviceContainer ndc_r6r5 = p2p_rr.Install(nc_r6r5);
	NetDeviceContainer ndc_r6r7 = p2p_rr.Install(nc_r6r7);

	NetDeviceContainer ndc_r5v4 = p2p_rv.Install(nc_r5v4);

	NetDeviceContainer ndc_r7s8 = p2p_rs.Install(nc_r7s8);
	NetDeviceContainer ndc_r7s9 = p2p_rs.Install(nc_r7s9);
	NetDeviceContainer ndc_r7s10 = p2p_rs.Install(nc_r7s10);
	NetDeviceContainer ndc_r7s11 = p2p_rs.Install(nc_r7s11);

	// Creamos los enlaces CSMA y asignamos atributos
	CsmaHelper csma;

	csma.SetChannelAttribute("DataRate", StringValue(m_100rate));
	csma.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));

	NetDeviceContainer ndc_sw0 = csma.Install(nc_sw0);
	NetDeviceContainer ndc_sw1 = csma.Install(nc_sw1);

	NetDeviceContainer ndc_sw8 = csma.Install(nc_sw8);
	NetDeviceContainer ndc_sw9 = csma.Install(nc_sw9);
	NetDeviceContainer ndc_sw10 = csma.Install(nc_sw10);


	// Asignar direcciones IP
	Ipv4AddressHelper ipv4;
	std::cout << "INCIO IP! :D" << std::endl;
	ipv4.SetBase("10.10.0.0", "255.255.255.0");
	ic_r3s0 = ipv4.Assign(ndc_r3s0);
	ipv4.SetBase("10.11.0.0", "255.255.255.0");
	ic_r3s1 = ipv4.Assign(ndc_r3s1);
	ipv4.SetBase("10.12.0.0", "255.255.255.0");
	ic_r3s2 = ipv4.Assign(ndc_r3s2);

	std::cout << "FIN BLOQUE 1" << std::endl;

	ipv4.SetBase("10.30.0.0", "255.255.255.0");
	ic_r6r3 = ipv4.Assign(ndc_r6r3);
	ipv4.SetBase("10.50.0.0", "255.255.255.0");
	ic_r6r5 = ipv4.Assign(ndc_r6r5);
	ipv4.SetBase("10.40.0.0", "255.255.255.0");
	ic_r6r7 = ipv4.Assign(ndc_r6r7);
	ipv4.SetBase("10.60.0.0", "255.255.255.0");
	ic_r5v4 = ipv4.Assign(ndc_r5v4);

	ipv4.SetBase("10.20.0.0", "255.255.255.0");
	ic_r7s8 = ipv4.Assign(ndc_r7s8);
	ipv4.SetBase("10.21.0.0", "255.255.255.0");
	ic_r7s9 = ipv4.Assign(ndc_r7s9);
	ipv4.SetBase("10.22.0.0", "255.255.255.0");
	ic_r7s10 = ipv4.Assign(ndc_r7s10);
	ipv4.SetBase("10.23.0.0", "255.255.255.0");
	ic_r7s11 = ipv4.Assign(ndc_r7s11);

	ipv4.SetBase("10.0.10.0", "255.255.255.0");
	ic_sw0 = ipv4.Assign(ndc_sw0);
	ipv4.SetBase("10.0.11.0", "255.255.255.0");
	ic_sw1 = ipv4.Assign(ndc_sw1);

	ipv4.SetBase("10.0.20.0", "255.255.255.0");
	ic_sw8 = ipv4.Assign(ndc_sw8);
	ipv4.SetBase("10.0.21.0", "255.255.255.0");
	ic_sw9 = ipv4.Assign(ndc_sw9);
	ipv4.SetBase("10.0.22.0", "255.255.255.0");
	ic_sw10 = ipv4.Assign(ndc_sw10);

	// Habilitando ruting statico
	Ipv4GlobalRoutingHelper::PopulateRoutingTables();

}

void Simulacion::installAplications() { //muchos a muchos
	//Servidores: pc's unidos a sw9 y sw2
	//Clientes: pc's unidos a sw1 y sw8
	
	NS_LOG_INFO("Create Applications.");

	uint16_t port1 = 8000;
	uint16_t f;
	uint32_t MaxPacketSize = 1024;
	Time interPacketInterval = Seconds(0.01);
	uint32_t maxPacketCount = 1;

	UdpServerHelper server1(port1);
	ApplicationContainer serverApp = server1.Install(nc_sw9); //instalamos 1 servidor en cada pc de sw9
	serverApp.Start(Seconds(1.0));
	serverApp.Stop(Seconds(4.0));

	for(f=0; f<10; f++){
		uint16_t x=f;
		UdpClientHelper f(ic_sw9.GetAddress(x), port1); //ip del pc servidor
		f.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
		f.SetAttribute("Interval", TimeValue(interPacketInterval));
		f.SetAttribute("PacketSize", UintegerValue(MaxPacketSize));

		ApplicationContainer clientApp = f.Install(nc_sw1); //instalamos cliente en cada pc de sw1

	}

	UdpServerHelper server2(port1);
	ApplicationContainer serverApp = server2.Install(nc_sw10); //instalamos 1 servidor en cada pc de sw10
	serverApp.Start(Seconds(1.0));
	serverApp.Stop(Seconds(4.0));

	for(f=0; f<5; f++){
		uint16_t x=f;
		UdpClientHelper f(ic_sw10.GetAddress(x), port1); //ip del pc servidor
		f.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
		f.SetAttribute("Interval", TimeValue(interPacketInterval));
		f.SetAttribute("PacketSize", UintegerValue(MaxPacketSize));

		ApplicationContainer clientApp = f.Install(nc_sw8); //instalamos 1 cliente en cada pc de sw8
	}
	
	//ejemplo muchos clientes a un servidor
	/*UdpServerHelper server2(port1);
	ApplicationContainer serverApp = server2.Install(nc_sw10.Get(5)); //instalamos 1 servidor en uno de los pc's de sw10
	serverApp.Start(Seconds(1.0));
	serverApp.Stop(Seconds(4.0));
	
	UdpClientHelper a(ic_sw10.GetAddress(5), port1); //ip del pc servidor
	a.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
	a.SetAttribute("Interval", TimeValue(interPacketInterval));
	a.SetAttribute("PacketSize", UintegerValue(MaxPacketSize));
	
	for(f=0; f<5; f++){
		ApplicationContainer clientApp = a.Install(nc_sw8); //instalamos 1 cliente en cada pc de sw8
	}*/

}

int Simulacion::Run() {
	std::cout << "INICIO TOPOLOGIA" << std::endl;
	createTopology();
	std::cout << "INICIO INTERNET" << std::endl;
	installInternetStack();
	std::cout << "INICIO APLICACIONES" << std::endl;
	installAplications();

	FlowMonitorHelper flowmon;
	Ptr<FlowMonitor> monitor = flowmon.InstallAll();

	std::cout << "INICIO SIMULACION" << std::endl;
	NS_LOG_INFO("Run Simulation");
	Simulator::Stop(Seconds(5.0));
	Simulator::Run();

	std::cout << "FIN SIMULACION" << std::endl;
	monitor->CheckForLostPackets();

	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
	std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();
	for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin(); i != stats.end(); ++i)
	{
		Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);
		if (true) //(t.sourceAddress == "10.0.2.3")
		{
			std::cout << "Flow " << i->first << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
			std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
			std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
			std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) / 1024 / 1024 << " Mbps\n";
		}
	}

	monitor->SerializeToXmlFile("topology2.flowmon", true, true);

	Simulator::Destroy();

	NS_LOG_INFO("Done :D");
	return 0;
}

int main(int argc, char *argv[])
{
	LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
	LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
	Simulacion s;
	s.Configure(argc, argv);
	return s.Run();
}
