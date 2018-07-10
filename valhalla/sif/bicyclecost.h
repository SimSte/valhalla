#ifndef VALHALLA_SIF_BICYCLECOST_H_
#define VALHALLA_SIF_BICYCLECOST_H_

#include <cstdint>
#include <valhalla/baldr/rapidjson_utils.h>
#include <valhalla/proto/directions_options.pb.h>
#include <valhalla/sif/dynamiccost.h>

namespace valhalla {
namespace sif {

/**
 * Parses the bicycle cost options from json and stores values in pbf.
 */
void ParseBicycleCostOptions(const rapidjson::Document& doc,
                             const std::string& costing_options_key,
                             odin::CostingOptions* pbf_costing_options);

/**
 * Create a bicyclecost
 * @param  config  Property tree with configuration / options.
 */
cost_ptr_t CreateBicycleCost(const boost::property_tree::ptree& config);

} // namespace sif
} // namespace valhalla

#endif // VALHALLA_SIF_BICYCLECOST_H_
