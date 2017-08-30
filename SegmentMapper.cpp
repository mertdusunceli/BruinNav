
#include "provided.h"
#include <vector>
#include <iostream>
#include "MyMap.h"
#include "support.h"
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
    MyMap<GeoCoord,vector<StreetSegment>> segmap;

};

SegmentMapperImpl::SegmentMapperImpl()      
{
    
}

SegmentMapperImpl::~SegmentMapperImpl()
{
    
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    StreetSegment segm;
    GeoCoord g;
    vector<StreetSegment> hehe;
    
    for (int i = 0; i < ml.getNumSegments(); i++)
    {
        ml.getSegment(i, segm);                 //start coordinate
        g = segm.segment.start;
        if (segmap.find(g) == nullptr)
        {
            hehe.push_back(segm);
            segmap.associate(g, hehe);
            hehe.pop_back();
        }
        else
        {
            segmap.find(g)->push_back(segm);
        }
        
        g = segm.segment.end;               //end coordinate
        if (segmap.find(g) == nullptr)
        {
            hehe.push_back(segm);
            segmap.associate(g, hehe);
            hehe.pop_back();
        }
        else
        {
            segmap.find(g)->push_back(segm);
        }
        
        for (int i = 0; i < segm.attractions.size(); i++)   //attractions
        {
            g = segm.attractions[i].geocoordinates;
            
            if (segmap.find(g) == nullptr)
            {
                hehe.push_back(segm);
                segmap.associate(g, hehe);
                hehe.pop_back();
            }
            else
            {
                segmap.find(g)->push_back(segm);
            }
            
        }
        
    }
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    vector<StreetSegment> segments;
    
    if (segmap.find(gc) == nullptr)
        return segments;
    else
    {
        segments = *segmap.find(gc);
        return segments;
    }
}

// ******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
