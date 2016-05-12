#ifndef PLOTGNU_H
#define PLOTGNU_H
/**
* @file plotGNU.h
* @brief NS3 Simulation of Complex Network, using Gnuplot to plot the stadistics of the network.
* @Author Adolfo, Cristina, Elena y Tinu.
* @date 11/05/2016
*/
#include "ns3/gnuplot.h"
#include <fstream>

using namespace ns3;

/**
@brief Clase que crea un grafica en formato .plt
*/
class PlotGNU {
public:
	//contructor
	PlotGNU(std::string filename, std::string title, std::string xlabel, std::string ylabel);
	void addData(double x, double y);
	void generateOutput();

private:
	Gnuplot 	gnuplot;
	Gnuplot2dDataset dataset;
	std::string plotFileName;
};

#endif