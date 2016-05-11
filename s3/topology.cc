/**
* @file topology.cc
* @brief NS3 Simulation of Complex Network, using Gnuplot to plot the stadistics of the network.
* @Author Adolfo, Cristina, Elena y Tinu.
* @date 11/05/2016
*/

#include "ns3/core-module.h"
#include "ns3/global-route-manager.h"
#include "ns3/bridge-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"

/* ESTOS INCLIDE NO EXISTEN... 
#include "ns3/simulator-module.h" 
#include "ns3/node-module.h"
#include "ns3/common-module.h"
#include "ns3/helper-module.h"
#include "ns3/contrib-module.h" 
*/

#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/gnuplot.h"
#include "ns3/flow-monitor-module.h"
#include <ns3/flow-monitor-helper.h>
#include "ns3/csma-helper.h"

using namespace ns3;
using namespace std;
NS_LOG_COMPONENT_DEFINE ("Topologia");

class Simulation {
public:
	//Contructor
	Simulation();
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
	double 		latency;
	uint64_t 	rate;
	double 		interval;


	// Declaración de las funciones
	void CreateTopology();	/// crea los nodos

	void InstallInternetStack(); /// Instala internet en los nodos

	void InstallAplications(); /// Instala las aplicaciones


};

/**
@brief Crea y configura los nodos.
*/
void Simulation::CreateTopology() {

	NS_LOG_INFO("Creating the nodes");
	NodeContainer router;
	router.Create(4);
	NodeContainer switches;
	switches.Create(8);
	NodeContainer server;
	server.Create(2);
	NodeContainer printer;
	printer.Create(2);
	NodeContainer admin;
	admin.Create(1);
	NodeContainer wirelessPC;
	wirelessPC.Create(7);
	NodeContainer pcT0;
	pcT0.Create(5);
	NodeContainer pcT1;
	pcT1.Create(5);
	NodeContainer pcT2;
	pcT2.Create(5);
	NodeContainer pcT3;
	pcT3.Create(5);
	NodeContainer pcT4;
	pcT4.Create(5);
	NodeContainer pcT5;
	pcT5.Create(5);
	NodeContainer ap;
	ap.Create(3);

	NS_LOG_INFO("Creating the links");
	CsmaHelper csma_bridge_0;
	csma_bridge_0.SetChannelAttribute("DataRate", DataRateValue(100000000));
	csma_bridge_0.SetChannelAttribute("Delay", TimeValue(MilliSeconds(10000)));
	CsmaHelper csma_bridge_1;
	csma_bridge_1.SetChannelAttribute("DataRate", DataRateValue(100000000));
	csma_bridge_1.SetChannelAttribute("Delay", TimeValue(MilliSeconds(10000)));
	CsmaHelper csma_bridge_2;
	csma_bridge_2.SetChannelAttribute("DataRate", DataRateValue(100000000));
	csma_bridge_2.SetChannelAttribute("Delay", TimeValue(MilliSeconds(10000)));
	CsmaHelper csma_bridge_3;
	csma_bridge_3.SetChannelAttribute("DataRate", DataRateValue(100000000));
	csma_bridge_3.SetChannelAttribute("Delay", TimeValue(MilliSeconds(10000)));
	CsmaHelper csma_bridge_4;
	csma_bridge_4.SetChannelAttribute("DataRate", DataRateValue(100000000));
	csma_bridge_4.SetChannelAttribute("Delay", TimeValue(MilliSeconds(10000)));
	CsmaHelper csma_bridge_5;
	csma_bridge_5.SetChannelAttribute("DataRate", DataRateValue(100000000));
	csma_bridge_5.SetChannelAttribute("Delay", TimeValue(MilliSeconds(10000)));
	CsmaHelper csma_bridge_6;
	csma_bridge_6.SetChannelAttribute("DataRate", DataRateValue(100000000));
	csma_bridge_6.SetChannelAttribute("Delay", TimeValue(MilliSeconds(10000)));
	CsmaHelper csma_bridge_7;
	csma_bridge_7.SetChannelAttribute("DataRate", DataRateValue(100000000));
	csma_bridge_7.SetChannelAttribute("Delay", TimeValue(MilliSeconds(10000)));

	YansWifiPhyHelper wifiPhy_ap_0 = YansWifiPhyHelper::Default();
	YansWifiChannelHelper wifiChannel_ap_0 = YansWifiChannelHelper::Default();
	wifiPhy_ap_0.SetChannel(wifiChannel_ap_0.Create());
	YansWifiPhyHelper wifiPhy_ap_1 = YansWifiPhyHelper::Default();
	YansWifiChannelHelper wifiChannel_ap_1 = YansWifiChannelHelper::Default();
	wifiPhy_ap_1.SetChannel(wifiChannel_ap_1.Create());
	YansWifiPhyHelper wifiPhy_ap_2 = YansWifiPhyHelper::Default();
	YansWifiChannelHelper wifiChannel_ap_2 = YansWifiChannelHelper::Default();
	wifiPhy_ap_2.SetChannel(wifiChannel_ap_2.Create());

	CsmaHelper csma_r2_r3;
	csma_r2_r3.SetChannelAttribute("DataRate", DataRateValue(100000000));
	csma_r2_r3.SetChannelAttribute("Delay", TimeValue(MilliSeconds(10000)));
	CsmaHelper csma_r0_r2;
	csma_r0_r2.SetChannelAttribute("DataRate", DataRateValue(100000000));
	csma_r0_r2.SetChannelAttribute("Delay", TimeValue(MilliSeconds(10000)));
	CsmaHelper csma_r1_r2;
	csma_r1_r2.SetChannelAttribute("DataRate", DataRateValue(100000000));
	csma_r1_r2.SetChannelAttribute("Delay", TimeValue(MilliSeconds(10000)));


	/* Build link net device container. */
	NodeContainer all_bridge_0; //lo conectado al switch 0
	all_bridge_0.Add(router.Get(3));
	all_bridge_0.Add(server.Get(0));
	all_bridge_0.Add(server.Get(1));
	NetDeviceContainer terminalDevices_bridge_0; //interfaces de router3, server0 y server1
	NetDeviceContainer BridgeDevices_bridge_0; //interfaces del switch
	for (int i = 0; i < 3; i++) //crea el enlace al switch para todos los dispositivos
	{
		NetDeviceContainer link = csma_bridge_0.Install(NodeContainer(all_bridge_0.Get(i), switches.Get(0))); //instala los nodos en los extremos de los enlaces
		terminalDevices_bridge_0.Add(link.Get(0)); //une el nodo a su netDevice
		BridgeDevices_bridge_0.Add(link.Get(1)); //une el nodo a su netDevice
	}
	BridgeHelper switch0; //crea el switch
	switch0.Install(switches.Get(0), BridgeDevices_bridge_0);
	NetDeviceContainer ndc_bridge_0 = terminalDevices_bridge_0; //contiene toda las interfaces conectadas al switch

	NodeContainer all_bridge_1; //lo conectado al switch 1
	all_bridge_1.Add(router.Get(0));
	NetDeviceContainer terminalDevices_bridge_1; //interfaz de router0
	NetDeviceContainer BridgeDevices_bridge_1; //interfaces del switch
	for (int i = 0; i < 1; i++)
	{
		NetDeviceContainer link = csma_bridge_1.Install(NodeContainer(all_bridge_1.Get(i), switches.Get(1)));
		terminalDevices_bridge_1.Add(link.Get(0));
		BridgeDevices_bridge_1.Add(link.Get(1));
	}
	BridgeHelper switch1;
	switch1.Install(switches.Get(1), BridgeDevices_bridge_1);
	NetDeviceContainer ndc_bridge_1 = terminalDevices_bridge_1;

	NodeContainer all_bridge_2; //lo conectado al switch 2
	all_bridge_2.Add(router.Get(0));
	all_bridge_2.Add(printer.Get(0));
	all_bridge_2.Add(pcT0.Get(0));
	all_bridge_2.Add(pcT0.Get(1));
	all_bridge_2.Add(pcT0.Get(2));
	all_bridge_2.Add(pcT0.Get(3));
	all_bridge_2.Add(pcT0.Get(4));
	NetDeviceContainer terminalDevices_bridge_2; //interfaz de router0, printer0 y groupT4
	NetDeviceContainer BridgeDevices_bridge_2; //interfaces del switch
	for (int i = 0; i < 7; i++)
	{
		NetDeviceContainer link = csma_bridge_2.Install(NodeContainer(all_bridge_2.Get(i), switches.Get(2)));
		terminalDevices_bridge_2.Add(link.Get(0));
		BridgeDevices_bridge_2.Add(link.Get(1));
	}
	BridgeHelper switch2;
	switch2.Install(switches.Get(2), BridgeDevices_bridge_2);
	NetDeviceContainer ndc_bridge_2 = terminalDevices_bridge_2;

	NodeContainer all_bridge_3; //lo conectado al switch 3
	all_bridge_3.Add(router.Get(0));
	all_bridge_3.Add(admin);
	all_bridge_3.Add(pcT1.Get(0));
	all_bridge_3.Add(pcT1.Get(1));
	all_bridge_3.Add(pcT1.Get(2));
	all_bridge_3.Add(pcT1.Get(3));
	all_bridge_3.Add(pcT1.Get(4));
	NetDeviceContainer terminalDevices_bridge_3;
	NetDeviceContainer BridgeDevices_bridge_3;
	for (int i = 0; i < 7; i++)
	{
		NetDeviceContainer link = csma_bridge_3.Install(NodeContainer(all_bridge_3.Get(i), switches.Get(3)));
		terminalDevices_bridge_3.Add(link.Get(0));
		BridgeDevices_bridge_3.Add(link.Get(1));
	}
	BridgeHelper switch3;
	switch3.Install(switches.Get(3), BridgeDevices_bridge_3);
	NetDeviceContainer ndc_bridge_3 = terminalDevices_bridge_3;

	NodeContainer all_bridge_4; //lo conectado al switch 4
	all_bridge_4.Add(router.Get(1));
	all_bridge_4.Add(printer.Get(1));
	all_bridge_4.Add(pcT2.Get(0));
	all_bridge_4.Add(pcT2.Get(1));
	all_bridge_4.Add(pcT2.Get(2));
	all_bridge_4.Add(pcT2.Get(3));
	all_bridge_4.Add(pcT2.Get(4));
	NetDeviceContainer terminalDevices_bridge_4;
	NetDeviceContainer BridgeDevices_bridge_4;
	for (int i = 0; i < 7; i++)
	{
		NetDeviceContainer link = csma_bridge_4.Install(NodeContainer(all_bridge_4.Get(i), switches.Get(4)));
		terminalDevices_bridge_4.Add(link.Get(0));
		BridgeDevices_bridge_4.Add(link.Get(1));
	}
	BridgeHelper switch4;
	switch4.Install(switches.Get(4), BridgeDevices_bridge_4);
	NetDeviceContainer ndc_bridge_4 = terminalDevices_bridge_4;

	NodeContainer all_bridge_5; //todo lo conectado al switch 5
	all_bridge_5.Add(router.Get(1));
	NetDeviceContainer terminalDevices_bridge_5;
	NetDeviceContainer BridgeDevices_bridge_5;
	for (int i = 0; i < 1; i++)
	{
		NetDeviceContainer link = csma_bridge_5.Install(NodeContainer(all_bridge_5.Get(i), switches.Get(5)));
		terminalDevices_bridge_5.Add(link.Get(0));
		BridgeDevices_bridge_5.Add(link.Get(1));
	}
	BridgeHelper switch5;
	switch5.Install(switches.Get(5), BridgeDevices_bridge_5);
	NetDeviceContainer ndc_bridge_5 = terminalDevices_bridge_5;

	NodeContainer all_bridge_6; //todo lo conectado al switch 6
	all_bridge_6.Add(router.Get(1));
	all_bridge_6.Add(pcT5.Get(0));
	all_bridge_6.Add(pcT5.Get(1));
	all_bridge_6.Add(pcT5.Get(2));
	all_bridge_6.Add(pcT5.Get(3));
	all_bridge_6.Add(pcT5.Get(4));
	NetDeviceContainer terminalDevices_bridge_6;
	NetDeviceContainer BridgeDevices_bridge_6;
	for (int i = 0; i < 6; i++)
	{
		NetDeviceContainer link = csma_bridge_6.Install(NodeContainer(all_bridge_6.Get(i), switches.Get(6)));
		terminalDevices_bridge_6.Add(link.Get(0));
		BridgeDevices_bridge_6.Add(link.Get(1));
	}
	BridgeHelper switch6;
	switch6.Install(switches.Get(6), BridgeDevices_bridge_6);
	NetDeviceContainer ndc_bridge_6 = terminalDevices_bridge_6;

	NodeContainer all_bridge_7; //todo lo conectado al switch 7
	all_bridge_7.Add(router.Get(1));
	all_bridge_7.Add(pcT3.Get(0));
	all_bridge_7.Add(pcT3.Get(1));
	all_bridge_7.Add(pcT3.Get(2));
	all_bridge_7.Add(pcT3.Get(3));
	all_bridge_7.Add(pcT3.Get(4));
	all_bridge_7.Add(pcT4.Get(0));
	all_bridge_7.Add(pcT4.Get(1));
	all_bridge_7.Add(pcT4.Get(2));
	all_bridge_7.Add(pcT4.Get(3));
	all_bridge_7.Add(pcT4.Get(4));
	NetDeviceContainer terminalDevices_bridge_7;
	NetDeviceContainer BridgeDevices_bridge_7;
	for (int i = 0; i < 11; i++)
	{
		NetDeviceContainer link = csma_bridge_7.Install(NodeContainer(all_bridge_7.Get(i), switches.Get(7)));
		terminalDevices_bridge_7.Add(link.Get(0));
		BridgeDevices_bridge_7.Add(link.Get(1));
	}
	BridgeHelper switch7;
	switch7.Install(switches.Get(7), BridgeDevices_bridge_7);
	NetDeviceContainer ndc_bridge_7 = terminalDevices_bridge_7;


	NodeContainer all_ap_0;
	all_ap_0.Add(wirelessPC.Get(0));
	all_ap_0.Add(wirelessPC.Get(1));
	NetDeviceContainer ndc_ap_0;
	Ssid ssid_ap_0 = Ssid("wifi-default-0");
	WifiHelper wifi_ap_0 = WifiHelper::Default();
	NqosWifiMacHelper wifiMac_ap_0 = NqosWifiMacHelper::Default();
	wifi_ap_0.SetRemoteStationManager("ns3::ArfWifiManager");
	wifiMac_ap_0.SetType("ns3::NqapWifiMac",
		"Ssid", SsidValue(ssid_ap_0),
		"BeaconGeneration", BooleanValue(true),
		"BeaconInterval", TimeValue(Seconds(2.5)));
	ndc_ap_0.Add(wifi_ap_0.Install(wifiPhy_ap_0, wifiMac_ap_0, ap.Get(0))); //instala wifi en el netDevice de ap0
	wifiMac_ap_0.SetType("ns3::NqstaWifiMac",
		"Ssid", SsidValue(ssid_ap_0),
		"ActiveProbing", BooleanValue(false));
	ndc_ap_0.Add(wifi_ap_0.Install(wifiPhy_ap_0, wifiMac_ap_0, all_ap_0)); //instala wifi en los netDevices de s0 y s1
	MobilityHelper mobility_ap_0;
	mobility_ap_0.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobility_ap_0.Install(ap.Get(0));
	mobility_ap_0.Install(all_ap_0);

	NodeContainer all_ap_1;
	all_ap_1.Add(wirelessPC.Get(2));
	all_ap_1.Add(wirelessPC.Get(3));
	NetDeviceContainer ndc_ap_1;
	Ssid ssid_ap_1 = Ssid("wifi-default-1");
	WifiHelper wifi_ap_1 = WifiHelper::Default();
	NqosWifiMacHelper wifiMac_ap_1 = NqosWifiMacHelper::Default();
	wifi_ap_1.SetRemoteStationManager("ns3::ArfWifiManager");
	wifiMac_ap_1.SetType("ns3::NqapWifiMac",
		"Ssid", SsidValue(ssid_ap_1),
		"BeaconGeneration", BooleanValue(true),
		"BeaconInterval", TimeValue(Seconds(2.5)));
	ndc_ap_1.Add(wifi_ap_1.Install(wifiPhy_ap_1, wifiMac_ap_1, ap.Get(1)));
	wifiMac_ap_1.SetType("ns3::NqstaWifiMac",
		"Ssid", SsidValue(ssid_ap_1),
		"ActiveProbing", BooleanValue(false));
	ndc_ap_1.Add(wifi_ap_1.Install(wifiPhy_ap_1, wifiMac_ap_1, all_ap_1));
	MobilityHelper mobility_ap_1;
	mobility_ap_1.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobility_ap_1.Install(ap.Get(1));
	mobility_ap_1.Install(all_ap_1);

	NodeContainer all_ap_2;
	all_ap_2.Add(wirelessPC.Get(4));
	all_ap_2.Add(wirelessPC.Get(5));
	all_ap_2.Add(wirelessPC.Get(6));
	NetDeviceContainer ndc_ap_2;
	Ssid ssid_ap_2 = Ssid("wifi-default-2");
	WifiHelper wifi_ap_2 = WifiHelper::Default();
	NqosWifiMacHelper wifiMac_ap_2 = NqosWifiMacHelper::Default();
	wifi_ap_2.SetRemoteStationManager("ns3::ArfWifiManager");
	wifiMac_ap_2.SetType("ns3::NqapWifiMac",
		"Ssid", SsidValue(ssid_ap_2),
		"BeaconGeneration", BooleanValue(true),
		"BeaconInterval", TimeValue(Seconds(2.5)));
	ndc_ap_2.Add(wifi_ap_2.Install(wifiPhy_ap_2, wifiMac_ap_2, ap.Get(2)));
	wifiMac_ap_2.SetType("ns3::NqstaWifiMac",
		"Ssid", SsidValue(ssid_ap_2),
		"ActiveProbing", BooleanValue(false));
	ndc_ap_2.Add(wifi_ap_2.Install(wifiPhy_ap_2, wifiMac_ap_2, all_ap_2));
	MobilityHelper mobility_ap_2;
	mobility_ap_2.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobility_ap_2.Install(ap.Get(2));
	mobility_ap_2.Install(all_ap_2);


	NodeContainer all_r2_r3;
	all_r2_r3.Add(router.Get(2));
	all_r2_r3.Add(router.Get(3));
	NetDeviceContainer ndc_r2_r3 = csma_r2_r3.Install(all_r2_r3);

	NodeContainer all_r0_r2;
	all_r0_r2.Add(router.Get(0));
	all_r0_r2.Add(router.Get(2));
	NetDeviceContainer ndc_r0_r2 = csma_r0_r2.Install(all_r0_r2);

	NodeContainer all_r1_r2;
	all_r1_r2.Add(router.Get(1));
	all_r1_r2.Add(router.Get(2));
	NetDeviceContainer ndc_r1_r2 = csma_r1_r2.Install(all_r1_r2);


	/* Install the IP stack. */
	InternetStackHelper internetStackH;
	internetStackH.Install(router.Get(0));
	internetStackH.Install(router.Get(1));
	internetStackH.Install(router.Get(2));
	internetStackH.Install(router.Get(3));
	internetStackH.Install(switches.Get(0));
	internetStackH.Install(switches.Get(1));
	internetStackH.Install(switches.Get(2));
	internetStackH.Install(switches.Get(3));
	internetStackH.Install(switches.Get(4));
	internetStackH.Install(switches.Get(5));
	internetStackH.Install(switches.Get(6));
	internetStackH.Install(switches.Get(7));
	internetStackH.Install(server.Get(0));
	internetStackH.Install(server.Get(1));
	internetStackH.Install(printer.Get(0));
	internetStackH.Install(printer.Get(1));
	internetStackH.Install(admin.Get(0));
	internetStackH.Install(wirelessPC.Get(0));
	internetStackH.Install(wirelessPC.Get(1));
	internetStackH.Install(wirelessPC.Get(2));
	internetStackH.Install(wirelessPC.Get(3));
	internetStackH.Install(wirelessPC.Get(4));
	internetStackH.Install(wirelessPC.Get(5));
	internetStackH.Install(wirelessPC.Get(6));
	internetStackH.Install(pcT0.Get(0));
	internetStackH.Install(pcT0.Get(1));
	internetStackH.Install(pcT0.Get(2));
	internetStackH.Install(pcT0.Get(3));
	internetStackH.Install(pcT0.Get(4));
	internetStackH.Install(pcT1.Get(0));
	internetStackH.Install(pcT1.Get(1));
	internetStackH.Install(pcT1.Get(2));
	internetStackH.Install(pcT1.Get(3));
	internetStackH.Install(pcT1.Get(4));
	internetStackH.Install(pcT2.Get(0));
	internetStackH.Install(pcT2.Get(1));
	internetStackH.Install(pcT2.Get(2));
	internetStackH.Install(pcT2.Get(3));
	internetStackH.Install(pcT2.Get(4));
	internetStackH.Install(pcT3.Get(0));
	internetStackH.Install(pcT3.Get(1));
	internetStackH.Install(pcT3.Get(2));
	internetStackH.Install(pcT3.Get(3));
	internetStackH.Install(pcT3.Get(4));
	internetStackH.Install(pcT4.Get(0));
	internetStackH.Install(pcT4.Get(1));
	internetStackH.Install(pcT4.Get(2));
	internetStackH.Install(pcT4.Get(3));
	internetStackH.Install(pcT4.Get(4));
	internetStackH.Install(pcT5.Get(0));
	internetStackH.Install(pcT5.Get(1));
	internetStackH.Install(pcT5.Get(2));
	internetStackH.Install(pcT5.Get(3));
	internetStackH.Install(pcT5.Get(4));
	internetStackH.Install(ap.Get(0));
	internetStackH.Install(ap.Get(1));
	internetStackH.Install(ap.Get(2));


	/* IP assign. */
	Ipv4AddressHelper ipv4;
	ipv4.SetBase("10.0.0.0", "255.255.255.0");
	Ipv4InterfaceContainer iface_ndc_bridge_0 = ipv4.Assign(ndc_bridge_0);
	ipv4.SetBase("10.0.1.0", "255.255.255.0");
	Ipv4InterfaceContainer iface_ndc_bridge_1 = ipv4.Assign(ndc_bridge_1);
	ipv4.SetBase("10.0.2.0", "255.255.255.0");
	Ipv4InterfaceContainer iface_ndc_bridge_2 = ipv4.Assign(ndc_bridge_2);
	ipv4.SetBase("10.0.3.0", "255.255.255.0");
	Ipv4InterfaceContainer iface_ndc_bridge_3 = ipv4.Assign(ndc_bridge_3);
	ipv4.SetBase("10.0.4.0", "255.255.255.0");
	Ipv4InterfaceContainer iface_ndc_bridge_4 = ipv4.Assign(ndc_bridge_4);
	ipv4.SetBase("10.0.5.0", "255.255.255.0");
	Ipv4InterfaceContainer iface_ndc_bridge_5 = ipv4.Assign(ndc_bridge_5);
	ipv4.SetBase("10.0.8.0", "255.255.255.0");
	Ipv4InterfaceContainer iface_ndc_bridge_6 = ipv4.Assign(ndc_bridge_6);
	ipv4.SetBase("10.0.9.0", "255.255.255.0");
	Ipv4InterfaceContainer iface_ndc_bridge_7 = ipv4.Assign(ndc_bridge_7);
	ipv4.SetBase("10.0.17.0", "255.255.255.0");
	Ipv4InterfaceContainer iface_ndc_ap_0 = ipv4.Assign(ndc_ap_0);
	ipv4.SetBase("10.0.6.0", "255.255.255.0");
	Ipv4InterfaceContainer iface_ndc_ap_1 = ipv4.Assign(ndc_ap_1);
	ipv4.SetBase("10.0.7.0", "255.255.255.0");
	Ipv4InterfaceContainer iface_ndc_ap_2 = ipv4.Assign(ndc_ap_2);
	ipv4.SetBase("10.0.12.0", "255.255.255.0");
	Ipv4InterfaceContainer iface_ndc_hub_4 = ipv4.Assign(ndc_r2_r3);
	ipv4.SetBase("10.0.14.0", "255.255.255.0");
	Ipv4InterfaceContainer iface_ndc_hub_5 = ipv4.Assign(ndc_r0_r2);
	ipv4.SetBase("10.0.15.0", "255.255.255.0");
	Ipv4InterfaceContainer iface_ndc_hub_6 = ipv4.Assign(ndc_r1_r2);

}

/**
@brief Instala los protocolos necesarios para tener red entre los nodos, también asigna las ips.
*/
void Simulation::InstallInternetStack() {
	// pendiente...
}

/**
@brief Instala las distintas aplicaciones que reciven/generan los datos entre los distintos nodos.
*/
void Simulation::InstallAplications() {
	// pendiente...
}


/**
@brief Clase que crea un grafica en formato .plt
*/
class PlotGNU {
public:
	//contructor
	PlotGNU(string filename,string title,string xlabel,string ylabel);
	void addData(double x, double y);
	void generateOutput();

private:
	Gnuplot 	gnuplot;
	Gnuplot2dDataset dataset;
	string plotFileName;
};

/**
@brief Constructor de la clase PlotGNU
@param filename Nombre del archivo a guardar (sin extensión)
@param title Titulo de la gráfica
@param xlabel Nombre del eje X
@param ylabel Nombre del eje Y
*/
PlotGNU::PlotGNU(string filename, string title, string xlabel, string ylabel) {
	plotFileName = filename;
	// Set the filename of and extension for the file 
	gnuplot.SetOutputFilename(plotFileName + ".png");
	gnuplot.SetTerminal("png");
	
	// Set the title and labels
	gnuplot.SetTitle(title);
	gnuplot.SetLegend(xlabel, ylabel);

	// Customize the form to show data
	dataset.SetTitle("data");
	dataset.SetStyle(Gnuplot2dDataset::LINES_POINTS);

	// Add the dataset to gnuplot
	gnuplot.AddDataset(dataset);
}


/**
  @brief Añade al dataset el valor y en la posicion x.
  @param x Valor eje horizontal.
  @param y Valor eje vertical.
*/
void PlotGNU::addData(double x, double y) {
	dataset.Add(x, y);
}


/**
  @brief Crea y guarda la información del dataset en un archivo .plt
*/
void PlotGNU::generateOutput() {
	// Creamos el archivo para guardar los datos
	std::ofstream plotFile((plotFileName +".plt").c_str());

	// Guardamos los datos al archivo
	gnuplot.GenerateOutput(plotFile);

	// Cerramos el archivo
	plotFile.close();
}

void ThroughputMonitor(FlowMonitorHelper *fmhelper, Ptr<FlowMonitor> flowMon, Gnuplot2dDataset DataSet) {
	// NO ACABADO
	Simulator::Schedule(Seconds(1), &ThroughputMonitor, fmhelper, flowMon, DataSet);

	double localThrou = 0;
	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(fmhelper->GetClassifier());
	std::map<FlowId, FlowMonitor::FlowStats> stats = flowMon->GetFlowStats();
	for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin(); i != stats.end(); ++i)
	{
		Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);
		if ((t.sourceAddress == "10.1.1.1" && t.destinationAddress == "10.1.2.2"))
		{
			std::cout << "Flow " << i->first << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")" << std::endl;
			std::cout << "  Tx Bytes:   " << i->second.txBytes << std::endl;
			std::cout << "  Rx Bytes:   " << i->second.rxBytes << std::endl;
			std::cout << "  Duration:   " << (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) << std::endl;
			std::cout << "  Last Received Packet:    " << i->second.timeLastRxPacket.GetSeconds() << " seconds" << std::endl;
			std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) / 1024 / 1024 << " Mbps" << std::endl;
			localThrou = (i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) / 1024 / 1024);
			// Actualizamos datos de Gnu
			DataSet.Add((double)Simulator::Now().GetSeconds(), (double)localThrou);
			std::cout << "---------------------------------------------------------------------------" << std::endl;
		}
	}
	flowMon->SerializeToXmlFile("ThroughputMonitor.xml", true, true);
}

int main(int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);

  Simulation s;


  /* Generate Route. */
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  /* Generate Application. */

  /* Simulation. */
  /* Pcap output. */
  /* Stop the simulation after x seconds. */
  uint32_t stopTime = 1;
  Simulator::Stop (Seconds (stopTime));
  /* Start and clean simulation. */
  Simulator::Run ();
  Simulator::Destroy ();
}
