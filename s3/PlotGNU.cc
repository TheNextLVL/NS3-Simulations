/**
* @file plotGNU.cc
* @brief NS3 Simulation of Complex Network, using Gnuplot to plot the stadistics of the network.
* @Author Adolfo, Cristina, Elena y Tinu.
* @date 11/05/2016
*/

#include "PlotGNU.h"
using namespace ns3;

/**
@brief Constructor de la clase PlotGNU
@param filename Nombre del archivo a guardar (sin extensión)
@param title Titulo de la grafica
@param xlabel Nombre del eje X
@param ylabel Nombre del eje Y
*/
PlotGNU::PlotGNU(std::string filename, std::string title, std::string xlabel, std::string ylabel) {
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
@brief Crea y guarda la informacion del dataset en un archivo .plt
*/
void PlotGNU::generateOutput() {
	// Creamos el archivo para guardar los datos
	std::ofstream plotFile((plotFileName + ".plt").c_str());

	// Guardamos los datos al archivo
	gnuplot.GenerateOutput(plotFile);

	// Cerramos el archivo
	plotFile.close();
}