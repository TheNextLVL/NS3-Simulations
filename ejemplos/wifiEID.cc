/*
 * Wifi-EID.cc
 *
 *  Created on: 16 févr. 2010
 *      Author: user
 */

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"
#include "ns3/EID_server_helper.h"
#include "ns3/EID_client_helper.h"
#include <fstream>



using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("WifiEIDexample");

  int
main (int argc, char *argv[])
{
  LogComponentEnable("EID_client_application", LOG_LEVEL_INFO);
  LogComponentEnable("EID_server_application", LOG_LEVEL_INFO);


   NodeContainer wifiStaNodes01, wifiStaNodes02,wifiStaNodes03;
   wifiStaNodes01.Create (1);
   wifiStaNodes02.Create (1);
   wifiStaNodes03.Create(1);
   NodeContainer wifistatsNode =NodeContainer (wifiStaNodes01.Get (0), wifiStaNodes02.Get (0), wifiStaNodes03.Get (0));

   NodeContainer wifiApNode;
   wifiApNode.Create (1);


    YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
    YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
    phy.SetChannel (channel.Create ());

    WifiHelper wifi = WifiHelper::Default ();
    wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

    NqosWifiMacHelper mac = NqosWifiMacHelper::Default ();

    Ssid ssid = Ssid ("ns-3-ssid");
     mac.SetType ("ns3::StaWifiMac", "Ssid", SsidValue (ssid), "ActiveProbing", BooleanValue (false));

    NetDeviceContainer stasDevices = wifi.Install (phy, mac, wifistatsNode);
    /* NetDeviceContainer staDevices01, staDevices02; staDevices03;
    staDevices01 = wifi.Install (phy, mac, wifiStaNodes01);
    staDevices02 = wifi.Install (phy, mac, wifiStaNodes02);
    staDevices03 = wifi.Install (phy, mac, wifiStaNodes03);*/
    mac.SetType ("ns3::ApWifiMac", "Ssid", SsidValue (ssid));

   	/*mac.SetType ("ns3::ApWifiMac",
      "Ssid", SsidValue (ssid),
      "BeaconGeneration", BooleanValue (true),
      "BeaconInterval", TimeValue (Seconds (2.5)));
    */
    NetDeviceContainer apDevices;
    apDevices = wifi.Install (phy, mac, wifiApNode);

    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAllocAp01 = CreateObject<ListPositionAllocator> ();
    Ptr<ListPositionAllocator> positionAllocSta01 = CreateObject<ListPositionAllocator> ();
    Ptr<ListPositionAllocator> positionAllocSta02 = CreateObject<ListPositionAllocator> ();
    Ptr<ListPositionAllocator> positionAllocSta03 = CreateObject<ListPositionAllocator> ();

    positionAllocAp01->Add (Vector (100, 100, 0.0));
    mobility.SetPositionAllocator (positionAllocAp01);

    // setup the AP.
      mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
      			 "Mode", StringValue ("Time"),
    			 "Time", StringValue ("2s"),
    			 "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=0]"),
    			 "Bounds", RectangleValue (Rectangle (0, 400.0, 0.0, 400.0)));
      mobility.Install (wifiApNode.Get (0));

      positionAllocSta01->Add (Vector (70, 130, 0.0));
      mobility.SetPositionAllocator (positionAllocSta01);

      mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
          		 "Mode", StringValue ("Time"),
      			 "Time", StringValue ("2s"),
      			 "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=0]"),
      			 "Bounds", RectangleValue (Rectangle (0, 400.0, 0.0, 400.0)));
      mobility.Install (wifiStaNodes01.Get (0));

      positionAllocSta02->Add (Vector (130, 70, 0.0));
        mobility.SetPositionAllocator (positionAllocSta02);

        mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
          			 "Mode", StringValue ("Time"),
      			 "Time", StringValue ("2s"),
      			 "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=0]"),
      			 "Bounds", RectangleValue (Rectangle (0, 400.0, 0.0, 400.0)));
        mobility.Install (wifiStaNodes02.Get (0));

        positionAllocSta03->Add (Vector (70, 80, 0.0));
          mobility.SetPositionAllocator (positionAllocSta03);

          mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
            			 "Mode", StringValue ("Time"),
        			 "Time", StringValue ("2s"),
        			 "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=0]"),
        			 "Bounds", RectangleValue (Rectangle (0, 400.0, 0.0, 400.0)));
          mobility.Install (wifiStaNodes03.Get (0));


   InternetStackHelper stack;
   //static Ipv4GlobalRoutingHelper globalRouting;

   //stack.SetRoutingHelper (globalRouting);

   stack.Install (wifistatsNode);
   stack.Install (wifiApNode);

   Ipv4AddressHelper address;

   address.SetBase ("10.1.1.0", "255.255.255.0");
   Ipv4InterfaceContainer stasInterfaces;
   stasInterfaces = address.Assign (stasDevices);
   Ipv4InterfaceContainer apInterfaces;
   apInterfaces= address.Assign (apDevices);


    Ptr<Node> b= (wifiStaNodes03.Get (0));
    Ptr<NetDevice> device = b->GetDevice(0);
    Address MACaddress = device->GetAddress();
    Mac48Address* macad;
    Mac48Address Mac_address = macad->ConvertFrom (MACaddress);
    std::cout << "Macaddressdest " << Mac_address<< " \n ";
    EID_address EID_addr= EID_address("10.1.1.3",Mac_address);
    EID_address* addr=&EID_addr;
    Address dest= addr->ConvertTo();


    Ptr<Node> a= wifiStaNodes01.Get (0);
    Ptr<NetDevice> device0 = a->GetDevice(0);
    Address MACaddress0 = device0->GetAddress();
    Mac48Address* mac0;
    Mac48Address Mac_address0 = mac0->ConvertFrom (MACaddress0);
    std::cout << "Macaddresssrc " << Mac_address0 << " \n ";
    EID_address EID_addr0= EID_address("10.1.1.1",Mac_address0);
    EID_address* addr0=&EID_addr0;
    Address src= addr0->ConvertTo();


      //  uint16_t port = 9;   // Discard port (RFC 863)

        EID_server_helper EID_server("ns3::UdpSocketFactory",
                                        Address (InetSocketAddress ("10.1.1.3", 1045)));

          ApplicationContainer apps1 = EID_server.Install (wifiStaNodes03.Get (0));
          apps1.Start (Seconds (0.0));
          apps1.Stop (Seconds (11.0));


    EID_client_helper EID_client = EID_client_helper ("ns3::UdpSocketFactory", dest, 1045);
    EID_client.SetAttribute ("OnTime", RandomVariableValue (ConstantVariable (1)));
    EID_client.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable (0)));
    EID_client.SetAttribute ("DataRate", DataRateValue (DataRate (6000)));
    EID_client.SetAttribute ("PacketSize", UintegerValue (1200));

    ApplicationContainer apps = EID_client.Install (wifiStaNodes01.Get (0));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (10.0));

//    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

   // PacketSinkHelper sink ("ns3::UdpSocketFactory",
            //                 Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
     // apps = sink.Install (nodes.Get (1));
     // apps.Start (Seconds (0.0));



   // apps = EID_server.Install (b);
    //apps.Start (Seconds (0.0));
    //apps.Stop (Seconds (11.0));

     NS_LOG_INFO ("Run Simulation.");
     Simulator::Stop (Seconds (100.0));
      Simulator::Run ();
      Simulator::Destroy ();

    NS_LOG_INFO ("Done.");
  }

