#include "src/agent/modules/ping.h"
#include "src/common/log.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <valijson/constraints/concrete_constraints.hpp>

#include <ctime>
#include <string>
#include <sstream>

LOG_DECLARE_NAMESPACE("agent.modules.ping");

namespace Cthun {
namespace Agent {
namespace Modules {

Ping::Ping() {
    module_name = "ping";

    valijson::constraints::TypeConstraint json_type_object {
        valijson::constraints::TypeConstraint::kObject };

    valijson::constraints::TypeConstraint json_type_string {
        valijson::constraints::TypeConstraint::kString };

    valijson::Schema input_schema;
    valijson::constraints::PropertiesConstraint::PropertySchemaMap properties;
    valijson::constraints::PropertiesConstraint::PropertySchemaMap pattern_properties;

    properties["sender_timestamp"].addConstraint(json_type_string);
    input_schema.addConstraint(new valijson::constraints::PropertiesConstraint(
                              properties,
                              pattern_properties));

    valijson::Schema output_schema;
    output_schema.addConstraint(json_type_object);

    actions["ping"] = Action { input_schema, output_schema, "interactive" };
}

void Ping::ping_action(const Json::Value &request, const Json::Value& input, Json::Value& output) {
    int sender_timestamp;
    std::istringstream(input["sender_timestamp"].asString()) >> sender_timestamp;

    boost::posix_time::ptime current_date_microseconds = boost::posix_time::microsec_clock::local_time();
    auto current_date_milliseconds =
        current_date_microseconds.time_of_day().total_milliseconds();
    auto time_to_agent = current_date_milliseconds - sender_timestamp;

    Json::Value result;
    result["sender_timestamp"] = input["sender_timestamp"];
    result["time_to_agent"] = std::to_string(time_to_agent);
    result["agent_timestamp"] = std::to_string(current_date_milliseconds);
    output = result;
}

void Ping::call_action(std::string action_name,
                       const Json::Value& request,
                       const Json::Value& input,
                       Json::Value& output) {
    ping_action(request, input, output);
}

}  // namespace Modules
}  // namespace Agent
}  // namespace Cthun
