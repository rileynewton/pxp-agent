#ifndef SRC_AGENT_MODULES_ECHO_H_
#define SRC_AGENT_MODULES_ECHO_H_

#include "src/agent/module.h"

namespace Cthun {
namespace Agent {
namespace Modules {

class Echo : public Cthun::Agent::Module {
  public:
    Echo();
    void call_action(std::string action_name,
                     const Json::Value& request,
                     const Json::Value& input,
                     Json::Value& output);
    void call_delayed_action(std::string action_name,
                             const Json::Value& request,
                             const Json::Value& input,
                             Json::Value& output,
                             std::string job_id) {}
};

}  // namespace Modules
}  // namespace Agent
}  // namespace Cthun

#endif  // SRC_AGENT_MODULES_ECHO_H_
