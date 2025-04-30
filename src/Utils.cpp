#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace PolygonalLibrary
{
    bool ImportMesh(PolygonalMesh& mesh)
    {
        if (!ImportCell0Ds(mesh))
            return false;

        if (!ImportCell1Ds(mesh))
            return false;

        if (!ImportCell2Ds(mesh))
            return false;

        return true;
    }

    // ***************************************************************************
    bool ImportCell0Ds(PolygonalMesh& mesh)
    {
        ifstream file("./Cell0Ds.csv");

        if (file.fail())
            return false;

        list<string> listLines;

        string line;
        while (getline(file, line)) {
            istringstream ss(line);
            string separator;
            string no_separator;

            while (getline(ss, separator, ';')) {
                if (!no_separator.empty())
                    no_separator += ' '; 
                no_separator += separator;
            }
            listLines.push_back(no_separator);
        }
        file.close();

        listLines.pop_front();

        mesh.NumCell0Ds = listLines.size();

        if (mesh.NumCell0Ds == 0)
        {
            cerr << "There is no cell 0D" << endl;
            return false;
        }

        mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
        mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);

        for (const string& line : listLines)
        {
            istringstream converter(line);

            unsigned int id;
            unsigned int marker;
            Vector2d coord;

            converter >> id >> marker >> mesh.Cell0DsCoordinates(0, id) >> mesh.Cell0DsCoordinates(1, id);

            mesh.Cell0DsId.push_back(id);

            
            if (marker != 0)
            {
                const auto it = mesh.MarkerCell0Ds.find(marker);
                if (it == mesh.MarkerCell0Ds.end())
                {
                    mesh.MarkerCell0Ds.insert({ marker, { id } });
                }
                else
                {
                    it->second.push_back(id);
                }
            }
        }

        return true;
    }

    bool ImportCell1Ds(PolygonalMesh& mesh)
    {
        ifstream file("./Cell1Ds.csv");

        if (file.fail())
            return false;

        list<string> listLines;

        string line;
        while (getline(file, line)) {
            istringstream ss(line);
            string separator;
            string no_separator;

            while (getline(ss, separator, ';')) {
                if (!no_separator.empty())
                    no_separator += ' '; 
                no_separator += separator;
            }
            listLines.push_back(no_separator);
        }
        file.close();

       
        listLines.pop_front();

        mesh.NumCell1Ds = listLines.size();

        if (mesh.NumCell1Ds == 0)
        {
            cerr << "There is no cell 1D" << endl;
            return false;
        }

        mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
        mesh.Cell1DsExtrema = Eigen::MatrixXi(2, mesh.NumCell1Ds);

        for (const string& line : listLines)
        {
            istringstream converter(line);

            unsigned int id;
            unsigned int marker;
            Vector2i vertices;

            converter >> id >> marker >> mesh.Cell1DsExtrema(0, id) >> mesh.Cell1DsExtrema(1, id);
            mesh.Cell1DsId.push_back(id);

           
            if (marker != 0)
            {
                const auto it = mesh.MarkerCell1Ds.find(marker);
                if (it == mesh.MarkerCell1Ds.end())
                {
                    mesh.MarkerCell1Ds.insert({ marker, { id } });
                }
                else
                {
                    it->second.push_back(id);
                }
            }
        }

        return true; 
    }

    bool ImportCell2Ds(PolygonalMesh& mesh)
    {
        ifstream file("./Cell2Ds.csv");

        if (file.fail())
            return false;

        list<string> listLines;

        string line;
        while (getline(file, line)) {
            istringstream ss(line);
            string separator;
            string no_separator;

            while (getline(ss, separator, ';')) {
                if (!no_separator.empty())
                    no_separator += ' '; 
                no_separator += separator;
            }
            listLines.push_back(no_separator);
        }
        file.close();

       
        listLines.pop_front();

        mesh.NumCell2Ds = listLines.size();

        if (mesh.NumCell2Ds == 0)
        {
            cerr << "There is no cell 2D" << endl;
            return false;
        }

        mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
        mesh.NumVertices2Ds.reserve(mesh.NumCell2Ds);
        mesh.NumEdges2Ds.reserve(mesh.NumCell2Ds);
        for (const string& line : listLines)
        {
            istringstream converter(line);

            unsigned int id;
            unsigned int marker;
            unsigned int numVertices;
            unsigned int numEdges;
            converter >> id >> marker >> numVertices;
            mesh.Cell2DsId.push_back(id);
            mesh.NumVertices2Ds.push_back(numVertices);
            std::vector<unsigned int> coord_v(numVertices);
            for (unsigned int i = 0; i < numVertices; i++)
            {
                converter >> coord_v[i];
            }
            mesh.Cell2DsVertices.push_back(coord_v);
            converter >> numEdges;
            std::vector<unsigned int> coord_e(numEdges);
            for (unsigned int i = 0; i < numEdges; i++)
            {
                converter >> coord_e[i]; 
            }
            mesh.Cell2DsEdges.push_back(coord_e);

            
            if (marker !=0)
            {
                const auto it = mesh.MarkerCell2Ds.find(marker);
                if (it == mesh.MarkerCell2Ds.end())
                {
                    mesh.MarkerCell2Ds.insert({ marker, { id } });
                }
                else
                {
                    it->second.push_back(id);
                }
            }
        }

        return true; 
    }
}
// ***************************************************************************
