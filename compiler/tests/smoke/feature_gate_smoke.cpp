#include <iostream>
#include <string>

#include "compiler/feature_gate.h"

namespace {

bool require(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << "\n";
        return false;
    }
    return true;
}

bool registry_records_and_controls_feature_stages() {
    synq::compiler::FeatureRegistry registry;
    if (!require(registry.register_feature({"alpha-synthesis", synq::compiler::FeatureStage::Alpha,
                                            "Experimental synthesis path.", "synq#42"}),
                 "registry accepts a new alpha feature")) return false;
    if (!require(registry.register_feature({"stable-core", synq::compiler::FeatureStage::Stable,
                                            "Stable core feature.", "synq#1"}),
                 "registry accepts a new stable feature")) return false;
    if (!require(!registry.register_feature({"alpha-synthesis", synq::compiler::FeatureStage::Beta,
                                             "Duplicate feature.", "synq#43"}),
                 "registry rejects a duplicate feature")) return false;
    if (!require(registry.state("alpha-synthesis") == synq::compiler::FeatureGateState::Disabled,
                 "alpha feature starts disabled")) return false;
    if (!require(!registry.is_enabled("alpha-synthesis"), "disabled alpha feature is unavailable")) return false;
    if (!require(registry.enable("alpha-synthesis"), "registry enables a registered alpha feature")) return false;
    if (!require(registry.state("alpha-synthesis") == synq::compiler::FeatureGateState::Enabled,
                 "enabled alpha feature reports enabled")) return false;
    if (!require(registry.is_enabled("alpha-synthesis"), "enabled alpha feature is available")) return false;
    if (!require(registry.state("stable-core") == synq::compiler::FeatureGateState::Stable,
                 "stable feature reports stable")) return false;
    if (!require(registry.is_enabled("stable-core"), "stable feature is available without opt-in")) return false;
    if (!require(!registry.enable("missing-feature"), "registry rejects an unknown feature")) return false;
    if (!require(registry.state("missing-feature") == synq::compiler::FeatureGateState::Unknown,
                 "unknown feature reports unknown")) return false;
    return require(std::string(synq::compiler::feature_stage_name(synq::compiler::FeatureStage::Beta)) == "beta",
                   "feature stage labels are stable and readable");
}

}  // namespace

int main() {
    if (!registry_records_and_controls_feature_stages()) return 1;
    std::cout << "SynQ feature gate smoke test passed\n";
    return 0;
}
