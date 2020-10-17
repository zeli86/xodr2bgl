
#include "types.hpp"
#include "RoadLaneSectionRow.hpp"
#include "helper.hpp"
#include <iomanip>
#include <sstream>
#include <string>

cRoadNetVertexData::cRoadNetVertexData(const int32_t no)
{
    _vertex_id = no;
}
std::ostream& operator<<(std::ostream& os, const cRoadNetVertexData& data)
{
    printer::tuple(os, "no") << " := ";
    printer::tuple(os, data._vertex_id) << ' ';
    return os;
}

cRoadNetEdgeData::cRoadNetEdgeData(const int32_t no, const cRoadLaneSectionRow& row)
{
    using namespace std;
    _id = no;
    _row_id = row._id;
    _road_id = row._road;
    _lane_id = row._lane;
    _lane_section_id = row._section;
    _no_sections = row._no_sections;
    _length = row._length;
    stringstream aux;
    aux << fixed << printer::tupleX(_road_id, _lane_id, _lane_section_id, _length);
    _name = aux.str();
}

std::ostream& operator<<(std::ostream& os, const cRoadNetEdgeData& data)
{
    os << printer::tupleX("road", "lane", "sct") << " := "
       << printer::tupleX(data._road_id, data._lane_id, data._lane_section_id) << ' ';
    return os;
}
