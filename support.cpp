//
//  support.cpp
//  navi
//
//  Created by Mert Dusunceli on 3/14/17.
//  Copyright © 2017 Mert Dusunceli. All rights reserved.
//

#include <iostream>
#include <string>
#include "support.h"

bool operator==(const GeoCoord &a, const GeoCoord &b)
{
    if (a.latitude == b.latitude && a.longitude == b.longitude)
        return true;
    return false;
}

bool operator>(const GeoCoord &a, const GeoCoord &b)
{
    if (a.latitude > b.latitude)
        return true;
    else if (a.latitude == b.latitude && a.longitude > b.longitude)
        return true;
    else
        return false;
}

bool operator<(const GeoCoord &a, const GeoCoord &b)
{
    if (a.latitude < b.latitude)
        return true;
    else if (a.latitude == b.latitude && a.longitude < b.longitude)
        return true;
    else
        return false;
}

std::string lower(std::string l)
{
    std::string f;
    for (int i = 0; i < l.size(); i++)
    {
        f += tolower(l[i]);
    }
    return f;
}

std::string directionOfLine(const GeoSegment& gs)
{
    double travelAngle = angleOfLine(gs);
    
    std::string direction;
    
    if (0 <= travelAngle && travelAngle <= 22.5)
        direction = "east";
    else if (22.5 < travelAngle && travelAngle <= 67.5)
        direction = "northeast";
    else if (67.5 < travelAngle && travelAngle <= 112.5)
        direction = "north";
    else if (112.5 < travelAngle && travelAngle <= 157.5)
        direction = "northwest";
    else if (157.5 < travelAngle && travelAngle <= 202.5)
        direction = "west";
    else if (202.5 < travelAngle && travelAngle <= 247.5)
        direction = "southwest";
    else if (247.5 < travelAngle && travelAngle <= 292.5)
        direction = "south";
    else if (292.5 < travelAngle && travelAngle <= 337.5)
        direction = "southeast";
    else if (337.5 < travelAngle && travelAngle < 360)
    {
        direction = "east";
    }
    return direction;
    
}

