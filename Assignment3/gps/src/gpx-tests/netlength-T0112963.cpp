#include <boost/test/unit_test.hpp>

#include "logs.h"
#include "types.h"
#include "route.h"

using namespace GPS;

//Check whether function works
//Check whether function count net length correctly with different number of positions
//Check whether function count net length correctly if positions are less than granularity
//Check whether function count net length not equal to total length

BOOST_AUTO_TEST_SUITE(Route_netlength)

const bool isFileName = false; // All GPX data in this suite is provided as strings.

//check function isn't work or not
BOOST_AUTO_TEST_CASE( FunctionWorkCheck ){
    const std::string gpxData =
      "<gpx><rte><name>test</name><rtept lat=\"0\" lon=\"0\"></rtept><rtept lat=\"1\" lon=\"1\"></rtept></rte></gpx>";
    Route route = Route(gpxData, isFileName);
    BOOST_REQUIRE( route.netLength() > 0 );
}

//check returned value isn't equal to 0 if there is no any position
BOOST_AUTO_TEST_CASE( NoPosition ){
    const std::string gpxData =
      "<gpx><rte><name>test</name><rtept lat=\"0\" lon=\"0\"></rtept></rte></gpx>";
    Route route = Route(gpxData, isFileName);
    BOOST_CHECK_EQUAL( route.netLength(), 0 );
}

//check returned value isn't equal to 0 if there are only one position
BOOST_AUTO_TEST_CASE( OnePosition ){
    const std::string gpxData =
      "<gpx><rte><name>test</name><rtept lat=\"1\" lon=\"1\"></rtept></rte></gpx>";
    Route route = Route(gpxData, isFileName);
    BOOST_CHECK_EQUAL( route.netLength(), 0 );
}

//check returned value isn't correct
BOOST_AUTO_TEST_CASE( TwoPositions ){
    const std::string gpxData =
      "<gpx><rte><name>test</name><rtept lat=\"0\" lon=\"0\"></rtept><rtept lat=\"0.1\" lon=\"0.1\"></rtept></rte></gpx>";
    Route route = Route(gpxData, isFileName);
    BOOST_CHECK_EQUAL( route.netLength(), 15725.355061691958 );
}

//check whether function select the two positions to count net length if there are more than two positions correctly
BOOST_AUTO_TEST_CASE( ThreePositions ){
    const std::string gpxData =
      "<gpx><rte><name>test</name><rtept lat=\"0\" lon=\"0\"></rtept><rtept lat=\"0.2\" lon=\"0.2\"></rtept><rtept lat=\"0.1\" lon=\"0.1\"></rtept></rte></gpx>";
    Route route = Route(gpxData, isFileName);
    BOOST_CHECK_EQUAL( route.netLength(), 15725.355061691958 );
}

//check whether net length euqal to 0 if first position is equal to last position
BOOST_AUTO_TEST_CASE( FirstPositionEqualToLastPosition ){
    const std::string gpxData =
      "<gpx><rte><name>test</name><rtept lat=\"0\" lon=\"0\"></rtept><rtept lat=\"0.2\" lon=\"0.2\"></rtept><rtept lat=\"0\" lon=\"0\"></rtept></rte></gpx>";
    Route route = Route(gpxData, isFileName);
    BOOST_CHECK_EQUAL( route.netLength(), 0 );
}

//check whether net length equal 0 if all positions are too close
BOOST_AUTO_TEST_CASE( AllPositionLessThanGranularity ){
    const metres granularity = 10000000;
    const std::string gpxData =
      "<gpx><rte><name>test</name><rtept lat=\"1\" lon=\"1\"></rtept></rte></gpx>";
    Route route = Route(gpxData, isFileName, granularity);
    BOOST_CHECK_EQUAL( route.netLength(), 0 );//discard all position
}

//check whether net length count correclty if some positions are too close
BOOST_AUTO_TEST_CASE( SomePositionLessThanGranularity ){
    const metres granularity = 5;
    const std::string gpxData =
      "<gpx><rte><name>test</name><rtept lat=\"0.00001\" lon=\"0.00001\"></rtept><rtept lat=\"0.00002\" lon=\"0.00002\"></rtept><rtept lat=\"1\" lon=\"1\"></rtept></rte></gpx>";
    Route route = Route(gpxData, isFileName, granularity);
    BOOST_CHECK_EQUAL( route.netLength(), 157248.02593813941 ); //discard second position
}

//check whether net length count correclty if no position is too close
BOOST_AUTO_TEST_CASE( NoPositionLessThanGranularity ){
    const metres granularity = 0;
    const std::string gpxData =
      "<gpx><rte><name>test</name><rtept lat=\"1\" lon=\"1\"></rtept><rtept lat=\"0.5\" lon=\"0.5\"></rtept><rtept lat=\"0\" lon=\"0\"></rtept></rte></gpx>";
    Route route = Route(gpxData, isFileName, granularity);
    BOOST_CHECK_EQUAL( route.netLength(), 157249.59847404022 );//no discard any position
}

//check if total length equal to net length if there is only one position
BOOST_AUTO_TEST_CASE( TotalLength_VS_NetLength_OnePosition ){
    const std::string gpxData =
      "<gpx><rte><name>test</name><rtept lat=\"0\" lon=\"1\"></rtept></rte></gpx>";
    Route route = Route(gpxData, isFileName);
    BOOST_CHECK( route.netLength() == route.netLength() );
}

//check if total length equal to net length if there are two positions
BOOST_AUTO_TEST_CASE( TotalLength_VS_NetLength_twoPositions ){
    const std::string gpxData =
      "<gpx><rte><name>test</name><rtept lat=\"0\" lon=\"1\"></rtept><rtept lat=\"1\" lon=\"1\"></rtept></rte></gpx>";
    Route route = Route(gpxData, isFileName);
    BOOST_CHECK( route.netLength() == route.netLength() );
}

//check if total length equal to net length if there are three positions in line
BOOST_AUTO_TEST_CASE( TotalLength_VS_NetLength_threePositions_InLine ){
    const std::string gpxData =
      "<gpx><rte><name>test</name><rtept lat=\"0\" lon=\"0\"></rtept><rtept lat=\"1\" lon=\"1\"></rtept><rtept lat=\"2\" lon=\"2\"></rtept></rte></gpx>";
    Route route = Route(gpxData, isFileName);
    BOOST_CHECK( route.netLength() == route.netLength() );
}


//check if total length is not equal to net length if three positions are not in line
BOOST_AUTO_TEST_CASE( TotalLength_VS_NetLength_triangle ){
    const std::string gpxData =
      "<gpx><rte><name>test</name><rtept lat=\"1\" lon=\"1\"></rtept><rtept lat=\"2\" lon=\"1\"></rtept><rtept lat=\"2\" lon=\"2\"></rtept></rte></gpx>";
    Route route = Route(gpxData, isFileName);
    BOOST_CHECK( route.netLength() != route.totalLength() );
}

BOOST_AUTO_TEST_SUITE_END()

