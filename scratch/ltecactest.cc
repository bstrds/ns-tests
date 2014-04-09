/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
 *
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
 *
 */

/* This is a minimal LTE-only simulation (no EPC) */

#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/mobility-module.h>
#include <ns3/lte-module.h>
#include "ns3/point-to-point-helper.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("LteCacTest");
int
main (int argc, char *argv[])
{	
	uint16_t ueNum = 15;
	uint16_t ueLimit = 10;
	
	/* This will instantiate some common objects (e.g., the Channel object) and 
	 * provide the methods to add eNBs and UEs and configure them.
	 */
	Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();

	// Creating Node Objects for the eNB(s) and UEs (at this point they are empty)
	NodeContainer enbNodes;
	enbNodes.Create (1);
	NodeContainer ueNodes;
	ueNodes.Create (ueNum);
	
	// Configure the Mobility model for all the nodes. 
	MobilityHelper mobility;
	mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	mobility.Install (enbNodes);
	
	mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
		"MinX", DoubleValue (0.0),
		"MinY", DoubleValue (0.0),
		"DeltaX", DoubleValue (5.0),
		"DeltaY", DoubleValue (10.0),
		"GridWidth", UintegerValue (3),
		"LayoutType", StringValue ("RowFirst"));
	mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
		"Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
	mobility.Install (ueNodes);
	
	// Installing an LTE protocol stack on the eNB(s).
	NetDeviceContainer enbDevs;
	enbDevs = lteHelper->InstallEnbDevice (enbNodes);
	
	// Installing an LTE protocol stack on the UEs.
	NetDeviceContainer ueDevs;
	ueDevs = lteHelper->InstallUeDevice (ueNodes);
	
	/* Attach the UEs to an eNB. This will configure each UE according to the 
	 * eNB configuration, and create an RRC connection between them:
	 */
	 
	// First way. 
	//lteHelper->Attach (ueDevs, enbDevs.Get (0)); 
	
	// Second way.
	/*for (NetDeviceContainer::Iterator i = ueDevs.Begin (); i != ueDevs.End () ; ++i) 
    {		
		lteHelper->Attach (*i, enbDevs.Get (0));
    }*/
    
    // Third way.
    for (int i = 0; i < ueNum; i++)
    {
		lteHelper->Attach (ueDevs.Get (i), enbDevs.Get (0));
	}
	
	// Activate a data radio bearer between each UE and the eNB it is attached to:
	enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
	EpsBearer bearer (q);
	
	// First way.
	//lteHelper->ActivateDataRadioBearer (ueDevs, bearer); 
	
	// Second way. 
	/*for (NetDeviceContainer::Iterator i = ueDevs.Begin (); i != ueDevs.End () ; ++i)
    {
		lteHelper->ActivateDataRadioBearer (*i, bearer);
    }*/
    
    // Third way.
	for (int i = 0; i < ueLimit; i++)
	{
		lteHelper->ActivateDataRadioBearer (ueDevs.Get (i), bearer);
	}
	
	lteHelper->EnableRlcTraces ();
	
	// Set stop time :
	Simulator::Stop (Seconds (10.0));
	
	Simulator::Run ();
	
	Simulator::Destroy ();
	
	return 0;
}
