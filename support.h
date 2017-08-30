//
//  support.h
//  navi
//
//  Created by Mert Dusunceli on 3/14/17.
//  Copyright © 2017 Mert Dusunceli. All rights reserved.
//

#ifndef support_h
#define support_h

#include "provided.h"
#include "MyMap.h"

bool operator==(const GeoCoord &a, const GeoCoord &b);
bool operator>(const GeoCoord &a, const GeoCoord &b);
bool operator<(const GeoCoord &a, const GeoCoord &b);
std::string lower(std::string l);
std::string directionOfLine(const GeoSegment& gs);

#endif /* support_h */
