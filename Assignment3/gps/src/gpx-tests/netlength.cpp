#include <boost/test/unit_test.hpp>

#include "logs.h"
#include "types.h"
#include "route.h"

using namespace GPS;

BOOST_AUTO_TEST_SUITE(Route_netlength)

const bool isFileName = true;

BOOST_AUTO_TEST_CASE( CountCorrectly ){
    Route route = Route(LogFiles::GPXRoutesDir + "AFKPU.gpx", isFileName);
    BOOST_CHECK_EQUAL( route.netLength(), >0 );
}

BOOST_AUTO_TEST_CASE( OnePositionOnly ){
    Route route = Route(LogFiles::GPXRoutesDir + "Q.gpx", isFileName);
    BOOST_CHECK_EQUAL( route.netLength(), 0 );
}

BOOST_AUTO_TEST_CASE( PositionsAreInSameLocation ){
    const metres granularity = 10000000000;
    Route route = Route(LogFiles::GPXRoutesDir + "AFKPU.gpx", isFileName, granularity);
    BOOST_CHECK_EQUAL( route.netLength(), 0 );
}


BOOST_AUTO_TEST_SUITE_END()
