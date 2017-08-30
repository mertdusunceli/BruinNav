#include "provided.h"
#include <string>
#include <iostream>
#include "MyMap.h"
#include "support.h"

using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
    MyMap<string,GeoCoord> attractGeo;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    StreetSegment segm;
    GeoCoord g;
    string n;
    
    for (int i = 0; i < ml.getNumSegments(); i++)
    {
        ml.getSegment(i, segm);
        for (int i = 0; i < segm.attractions.size(); i++)
        {
            n = (segm.attractions[i].name);
            g = segm.attractions[i].geocoordinates;
            attractGeo.associate(lower(n), g);
        }
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    //case insensitive
    if (attractGeo.find(lower(attraction)) == nullptr)
        return false;
    else
    {
        gc = *attractGeo.find(lower(attraction));
        return true;
    }
}

// ******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
