#include "test.h"

#include <vector>
#include <iostream>
#include <fstream>

#include <valhalla/baldr/graphid.h>
#include <valhalla/baldr/edgeinfo.h>
#include "mjolnir/edgeinfobuilder.h"

using namespace std;
using namespace valhalla::baldr;
using namespace valhalla::mjolnir;

namespace {

void TryWriteRead(const EdgeInfoBuilder& eibuilder) {
  // write EdgeInfoBuilder to binary file
  std::ofstream out_file("EdgeInfoBuilder_TestWriteRead.gph",
                         std::ios::out | std::ios::binary | std::ios::ate);
  if (out_file.is_open()) {
    eibuilder.SerializeToOstream(out_file);
    out_file.close();
  } else {
    throw runtime_error("Failed to open file for writing");
  }

  // read EdgeInfo from binary file
  streampos size;
  char * memblock;

  std::ifstream in_file("EdgeInfoBuilder_TestWriteRead.gph",
                        std::ios::in | std::ios::binary | std::ios::ate);
  if (in_file.is_open()) {
    size = in_file.tellg();
    memblock = new char[size];
    in_file.seekg(0, ios::beg);
    in_file.read(memblock, size);
    in_file.close();
  }

  // validate the read in fields to the original EdgeInfoBuilder
  EdgeInfo* ei = new EdgeInfo(memblock);
  ei->ToOstream();

  if (!(eibuilder.name_count() == ei->name_count()))
    throw runtime_error("WriteRead:name_count test failed");
  if (!(eibuilder.shape_count() == ei->shape_count()))
    throw runtime_error("WriteRead:shape_count test failed");
  if (!(eibuilder.exit_sign_count() == ei->exit_sign_count()))
    throw runtime_error("WriteRead:exit_sign_count test failed");
  for (uint32_t x = 0, n = ei->name_count(); x < n; ++x) {
    std::cout << x << ":eibuilder.GetStreetNameOffset="
              << eibuilder.GetStreetNameOffset(x) << std::endl;
    std::cout << x << ":ei->GetStreetNameOffset=" << ei->GetStreetNameOffset(x)
              << std::endl;
    if (!(eibuilder.GetStreetNameOffset(x) == ei->GetStreetNameOffset(x)))
      throw runtime_error("WriteRead:GetStreetNameOffset test failed");
  }
  for (uint32_t x = 0, n = ei->shape_count(); x < n; ++x) {
    std::cout << x << ":eibuilder.GetShapePoint="
              << eibuilder.GetShapePoint(x).lat() << ","
              << eibuilder.GetShapePoint(x).lng() << std::endl;
    std::cout << x << ":ei->GetShapePoint=" << ei->GetShapePoint(x).lat() << ","
              << ei->GetShapePoint(x).lng() << std::endl;
    if (!(eibuilder.GetShapePoint(x) == ei->GetShapePoint(x)))
      throw runtime_error("WriteRead:GetStreetNameOffset test failed");
  }
  // TODO exit sign test

  delete ei;
  delete[] memblock;
}

void TestWriteRead() {
  EdgeInfoBuilder eibuilder;
  std::vector<uint32_t> street_name_offset_list;
  street_name_offset_list.push_back(963);
  street_name_offset_list.push_back(957);
  street_name_offset_list.push_back(862);
  eibuilder.set_street_name_offset_list(street_name_offset_list);
  std::vector<PointLL> shape;
  shape.push_back(PointLL(-76.3002, 40.0433));
  shape.push_back(PointLL(-76.3036, 40.043));
  eibuilder.set_shape(shape);
  // TODO: exit
  TryWriteRead(eibuilder);
}

}

int main() {
  test::suite suite("edgeinfobuilder");

  // Write to file and read into EdgeInfo
  suite.test(TEST_CASE(TestWriteRead));

  return suite.tear_down();
}
