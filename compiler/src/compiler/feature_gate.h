// Copyright (c) 2025 SynQ Contributors
//
// Experimental-feature registry for the recovered compiler profile. The
// registry records language feature status; it is not a permissions or runtime
// security system.
#ifndef SYNQ_COMPILER_FEATURE_GATE_H
#define SYNQ_COMPILER_FEATURE_GATE_H

#include <map>
#include <string>

namespace synq::compiler {

enum class FeatureStage {
    Alpha,
    Beta,
    Stable,
};

struct FeatureGate {
    std::string name;
    FeatureStage stage;
    std::string description;
    std::string tracking_issue;
};

enum class FeatureGateState {
    Unknown,
    Disabled,
    Enabled,
    Stable,
};

class FeatureRegistry {
public:
    // Returns false when the name is empty or already registered.
    bool register_feature(FeatureGate feature);

    const FeatureGate* find(const std::string& name) const;
    FeatureGateState state(const std::string& name) const;

    // Enables a registered alpha/beta feature. Stable features are available
    // without an opt-in and return true. Unknown names return false.
    bool enable(const std::string& name);
    bool is_enabled(const std::string& name) const;

private:
    std::map<std::string, FeatureGate> features_;
    std::map<std::string, bool> enabled_features_;
};

const char* feature_stage_name(FeatureStage stage);

// Contains only features implemented and tested by the recovered compiler
// profile. Additions must have parser/semantic enforcement and smoke coverage.
FeatureRegistry make_default_feature_registry();

}  // namespace synq::compiler

#endif
