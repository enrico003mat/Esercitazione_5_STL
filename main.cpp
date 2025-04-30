#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"
using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;
int main()
{
	PolygonalMesh mesh;
	if(!ImportMesh(mesh))
    {
        cerr << "file not found" << endl;
        return 1;
	}
	 
	 Gedim::UCDUtilities utilities;
    {
        vector<Gedim::UCDProperty<double>> cell0Ds_properties(1);

        cell0Ds_properties[0].Label = "Marker";
        cell0Ds_properties[0].UnitLabel = "-";
        cell0Ds_properties[0].NumComponents = 1;

        vector<double> cell0Ds_marker(mesh.NumCell0Ds, 0.0);
        for(const auto &m : mesh.MarkerCell0Ds)
            for(const unsigned int id: m.second)
                cell0Ds_marker.at(id) = m.first;

        cell0Ds_properties[0].Data = cell0Ds_marker.data();

        utilities.ExportPoints("./Cell0Ds.inp",
                               mesh.Cell0DsCoordinates,
                               cell0Ds_properties);
    }

    {

        vector<Gedim::UCDProperty<double>> cell1Ds_properties(1);

        cell1Ds_properties[0].Label = "Marker";
        cell1Ds_properties[0].UnitLabel = "-";
        cell1Ds_properties[0].NumComponents = 1;

        vector<double> cell1Ds_marker(mesh.NumCell1Ds, 0.0);
        for(const auto &m : mesh.MarkerCell1Ds)
            for(const unsigned int id: m.second)
                cell1Ds_marker.at(id) = m.first;

        cell1Ds_properties[0].Data = cell1Ds_marker.data();

        utilities.ExportSegments("./Cell1Ds.inp",
                                 mesh.Cell0DsCoordinates,
                                 mesh.Cell1DsExtrema,
                                 {},
                                 cell1Ds_properties);
    }
	
	//non zero length edge
	unsigned int n=mesh.NumCell1Ds;
	bool t= false;
	for(unsigned int i=0;i<n;i++)
	{	
		
		int a = mesh.Cell1DsExtrema(0, i);
		int b = mesh.Cell1DsExtrema(1, i);
		double x1=mesh.Cell0DsCoordinates(0,a);
		double x2=mesh.Cell0DsCoordinates(0,b);
		double y1=mesh.Cell0DsCoordinates(1,a);
		double y2=mesh.Cell0DsCoordinates(1,b);
		double l = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
		if (l <= 1e-12)  
		{
			t = true;
		}
	}
	if(t)
	{ 
		cout<<"zero length"<<endl;
	}
	//non zero area
	unsigned int m=mesh.NumCell2Ds;
	bool t2=false;
	for(unsigned int i=0;i<m;i++)
	{
		unsigned int num_v=mesh.NumVertices2Ds[i];
		double A=0.0;
		for(unsigned int j=0;j<num_v;j++)
		{
			int ind_pi=mesh.Cell2DsVertices[i][j];
			int ind_pi1;
			if(j==num_v-1)
			{
				ind_pi1=mesh.Cell2DsVertices[i][0];
			}
			else
			{
				ind_pi1=mesh.Cell2DsVertices[i][j+1];
			}
			double xi=mesh.Cell0DsCoordinates(0,ind_pi);
			double xi1=mesh.Cell0DsCoordinates(0,ind_pi1);
			double yi=mesh.Cell0DsCoordinates(1,ind_pi);
			double yi1=mesh.Cell0DsCoordinates(1,ind_pi1);
			A+=xi*yi1-xi1*yi;
			
		}
		A=A/2.0;
		if(A<=1e-12)
		{
			t2=true;
		}
		
	}
	if(t2)
	{ 
		cout<<"zero Area"<<endl;
	}
	
    // test marker Cell0Ds
	for (const auto& entrance : mesh.MarkerCell0Ds) 
	{
		cout << "Marker Cell0Ds " << entrance.first << " : ";
		for (const auto& id : entrance.second) 
		{
			cout << id << " ";
		}
		cout << endl;
	}
	// test marker Cell1Ds
	for (const auto& entry : mesh.MarkerCell1Ds) 
	{
		cout << "Marker Cell1Ds " << entry.first << " : ";
		for (const auto& id : entry.second) 
		{
			cout << id << " ";
		}
		cout << endl;
	}
	// test marker Cell2Ds
	for (const auto& entry: mesh.MarkerCell2Ds) 
	{
		cout << "Marker Cell2Ds " << entry.first << " : ";
		for (const auto& id : entry.second) 
		{
			cout << id << " ";
		}
		cout << endl;
	}


	
    return 0;
}
