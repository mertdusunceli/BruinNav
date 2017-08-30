#include "provided.h"
#include <string>
#include <fstream>
#include <iostream> 

using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
    
private:
    vector<StreetSegment> street;
    
    vector<string> processCoord(string s)
    {
        string a;
        bool ended = false;
        vector<string> points;
        
        for (int i = 0; i < s.size(); i++)
        {
            while (s[i] != ',' && s[i] != ' ' && i < s.size())
            {
                ended = false; 
                a += s[i];
                i++;
            }
            if (ended == false && (s[i] == ',' || s[i] == ' ' || i == s.size()))
            {
                ended = true;
                points.push_back(a);
                a = "";
            }
          
        }
        
        return points;
    }
    
    vector<string> processAttraction(string s)
    {
        string a;
        bool ended = false;
        bool coord = false;
        vector<string> attractions;
        
        for (int i = 0; i < s.size(); i++)
        {
            while (s[i] != '|' && ended == false)
            {
                a += s[i];
                i++;
            }
            if (s[i] == '|')
            {
                attractions.push_back(a);
                a = "";
                ended = true;
                i++;
            }
            
            while (s[i] != ',' && s[i] != ' ' && i < s.size())
            {
                coord = false;
                a += s[i];
                i++;
            }
            if (coord == false && (s[i] == ',' || s[i] == ' ' || i == s.size()))
            {
                coord = true;
                attractions.push_back(a);
                a = "";
            }
            
            
        }
        
        return attractions;
    }
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
    ifstream infile(mapFile);    // infile is a name of our choosing
    if ( ! infile )		        // Did opening the file fail?
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }
    
    string s;
    int count = 0;
    int vectorCount = 0;

    StreetSegment prototype;
    vector<string> points;
    vector<string> attractions;

    while (getline(infile, s))
    {
        if (count == 0)
        {
            
            prototype.streetName = s;
            count++;
        }
        else if (count == 1)
        {
            points = processCoord(s);
            string coord[4];
            coord[3] = points.back();
            points.pop_back();
            coord[2] = points.back();
            points.pop_back();
            coord[1] = points.back();
            points.pop_back();
            coord[0] = points.back();
            points.pop_back();
            GeoCoord starting(coord[0], coord[1]);
            GeoCoord ending(coord[2], coord[3]);
            prototype.segment = GeoSegment(starting,ending);
            count++;
        }
        else if (count == 2)
        {
            vectorCount = stoi(s);
            if (vectorCount == 0)
            {
                count = 0;
                street.push_back(prototype);
            }
            else
            {
                count++;
            }
        }
        else if (count == 3)
        {
            int attractCount = vectorCount;
            while (vectorCount != 0)
            {
                attractions = processAttraction(s);
                
                string longi = attractions.back();
                attractions.pop_back();
                string latit = attractions.back();
                attractions.pop_back();
                string name = attractions.back();
                attractions.pop_back();

                Attraction attract;
                attract.name = name;
                attract.geocoordinates = GeoCoord(latit,longi);
                
                prototype.attractions.push_back(attract);
                vectorCount--;
                if (vectorCount != 0)
                {
                    getline(infile, s);
                }

            }
            street.push_back(prototype);
            for (int i = 0; i < attractCount; i++)
            {
                prototype.attractions.pop_back();
            }
            count = 0;
            
        }

    }
    return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
    return street.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    if (segNum < 0 || segNum >= street.size())
        return false;
    else
        seg = street[segNum];
    return true;
}


//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
