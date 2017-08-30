#include "provided.h"
#include <string>
#include <vector>
#include <iostream> 
#include <queue>
#include "MyMap.h"
#include "support.h"

using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
    
private:
    MapLoader mappy;
    AttractionMapper am;                
    SegmentMapper sm;

    struct nextPoint
    {
        double distance;
        GeoCoord coordinate;
       
    };
    
    struct compare
    {
        bool operator()(const nextPoint &a, const nextPoint &b)
        {
            if (a.distance > b.distance)
                return true;
            else
                return false;
        }
    };
    
    vector<StreetSegment> getStreetName(GeoCoord geo) const
    {
        return sm.getSegments(geo);
    }

    string setDirection(GeoSegment geo1, GeoSegment geo2) const
    {
        double travelAngle = angleBetween2Lines(geo1, geo2);
        
        if (travelAngle < 180)
            return "left";
        else
            return "right";

    }

};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    if (!mappy.load(mapFile))
    {
        return false;
    }
    
    am.init(mappy);
    sm.init(mappy);

    return true;
}



NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    for (int i = 0; i < directions.size(); i++)         //clear vector in case there's something in it
    {
        directions.pop_back();
    }
    
    GeoCoord startCoord;
    GeoCoord endCoord;
    
    if (!am.getGeoCoord(start, startCoord))
    {
        return NAV_BAD_SOURCE;
    }
    
    if (!am.getGeoCoord(end, endCoord))
    {
        return NAV_BAD_DESTINATION;
    }
    
    string endStreet = sm.getSegments(endCoord)[0].streetName;
    
    priority_queue<nextPoint, vector<nextPoint>, compare> pq;
    
    nextPoint prevP;
    prevP.distance = distanceEarthMiles(startCoord, endCoord);
    prevP.coordinate = startCoord;
    pq.push(prevP);
    
    nextPoint nextP;
    MyMap<GeoCoord, GeoCoord> locationOfPreviousPoint;

    bool found = false;

    while (!pq.empty())
    {
        prevP = pq.top();
        pq.pop();
        
        for (int i = 0; i < sm.getSegments(prevP.coordinate).size(); i++)
        {
            StreetSegment street = sm.getSegments(prevP.coordinate)[i];
            if (street.streetName == endStreet)
            {
                for (int j = 0; j < street.attractions.size(); j++)
                {
                    if (street.attractions[j].geocoordinates == endCoord) //found!
                    {
                        locationOfPreviousPoint.associate(endCoord, prevP.coordinate);
                        size_t poppy = pq.size();
                        for (int k = 0; k < poppy; k++)
                        {
                            pq.pop();
                        }
                        found = true;
                        break;
                    }
                }
            }
            if (found)
            {
                break;
            }
            if (!(prevP.coordinate == sm.getSegments(prevP.coordinate)[i].segment.start))
            {
                nextP.distance = distanceEarthMiles(sm.getSegments(prevP.coordinate)[i].segment.start, endCoord);
                nextP.coordinate = sm.getSegments(prevP.coordinate)[i].segment.start;
                if (locationOfPreviousPoint.find(nextP.coordinate) == nullptr)
                {
                    locationOfPreviousPoint.associate(nextP.coordinate, prevP.coordinate);
                    pq.push(nextP);
                }
            }
            
            if (!(prevP.coordinate == sm.getSegments(prevP.coordinate)[i].segment.end))
            {
                nextP.distance = distanceEarthMiles(sm.getSegments(prevP.coordinate)[i].segment.end, endCoord);
                nextP.coordinate = sm.getSegments(prevP.coordinate)[i].segment.end;
                if (locationOfPreviousPoint.find(nextP.coordinate) == nullptr)
                {
                    locationOfPreviousPoint.associate(nextP.coordinate, prevP.coordinate);
                    pq.push(nextP);
                }
            }
            

        }
    }
    
    if (!found)
    {
        return NAV_NO_ROUTE; //no route has been found
    }
    
    vector<GeoCoord> finalPoints;
    finalPoints.push_back(endCoord);
    
    GeoCoord a = *locationOfPreviousPoint.find(endCoord);
    finalPoints.push_back(a);
    
    while (!(a == startCoord))
    {
        a = *locationOfPreviousPoint.find(a);
        finalPoints.push_back(a);
    }

    GeoCoord first;
    GeoCoord second;
    
    GeoSegment geoSeg;
    string sname;
    vector<StreetSegment> finalStreet;
    StreetSegment ss;
    
    int size = finalPoints.size();
    
    for (int i = 0; i < size-1; i++)
    {
        if (i == 0)
        {
            first = finalPoints.back();
            finalPoints.pop_back();
        }
        else
        {
            first = second;
        }
        second = finalPoints.back();
        finalPoints.pop_back();
        
        geoSeg.start = first;
        geoSeg.end = second;
        
        for (int i = 0; i < getStreetName(first).size(); i++)
        {
            for (int j = 0; j < getStreetName(second).size(); j++)
            {
                if (getStreetName(first)[i].streetName == getStreetName(second)[j].streetName)
                    sname = getStreetName(first)[i].streetName;
            }
        }
        
        ss.streetName = sname;
        ss.segment = geoSeg;
        finalStreet.push_back(ss);
    }
    
    vector<StreetSegment> ready;
    
    while (!finalStreet.empty())
    {
        ready.push_back(finalStreet.back());
        finalStreet.pop_back();
    }

    
    StreetSegment almost1;
    StreetSegment almost2;
    bool firstOne = true;
    
    // angle between lines
    
    if (ready.size() == 1)
    {
        almost1 = ready.back();
        ready.pop_back();
        double distance4 = distanceEarthMiles(almost1.segment.start, almost1.segment.end);
        NavSegment e(directionOfLine(almost1.segment), almost1.streetName, distance4, almost1.segment);
        directions.push_back(e);
    }
    
    else if (ready.size() > 1)
    {
        while (!ready.empty())
        {
            if (firstOne)
            {
                almost1 = ready.back();
                ready.pop_back();
                firstOne = false;
            }
            else
            {
                almost1 = almost2;
            }
            almost2 = ready.back();
            ready.pop_back();
            if (almost1.streetName != almost2.streetName)
            {
                // first proceed on the line and then turn
                double distance1 = distanceEarthMiles(almost1.segment.start, almost1.segment.end);
                NavSegment a(directionOfLine(almost1.segment), almost1.streetName, distance1, almost1.segment);
                directions.push_back(a);
                
                NavSegment b(setDirection(almost1.segment, almost2.segment), almost2.streetName);
                directions.push_back(b);
            }
            else
            {
                // proceed
                double distance2 = distanceEarthMiles(almost1.segment.start, almost1.segment.end);
                NavSegment c(directionOfLine(almost1.segment), almost1.streetName, distance2, almost1.segment);
                directions.push_back(c);
                
            }

        }
        double distance3 = distanceEarthMiles(almost2.segment.start, almost2.segment.end);
        NavSegment d(directionOfLine(almost2.segment), almost2.streetName, distance3, almost2.segment);
        directions.push_back(d);
        
    }
    

     return NAV_SUCCESS; //a path found
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}

