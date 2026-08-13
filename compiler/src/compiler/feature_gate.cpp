// Copyright (c) 2025 SynQ Contributors

#include "feature_gate.h"

#include <utility>

namespace synq::compiler {

bool FeatureRegistry::register_feature(FeatureGate feature) {
    if (feature.name.empty() || features_.find(feature.name) != features_.end()) {
        return false;
    }

    const std::string name = feature.name;
    features_.emplace(name, std::move(feature));
    enabled_features_.emplace(name, false);
    return true;
}

const FeatureGate* FeatureRegistry::find(const std::string& name) const {
    const auto entry = features_.find(name);
    return entry == features_.end() ? nullptr : &entry->second;
}

FeatureGateState FeatureRegistry::state(const std::string& name) const {
    const FeatureGate* feature = find(name);
    if (feature == nullptr) {
        return FeatureGateState::Unknown;
    }
    if (feature->stage == FeatureStage::Stable) {
        return FeatureGateState::Stable;
    }
    const auto enabled = enabled_features_.find(name);
    return enabled != enabled_features_.end() && enabled->second
        ? FeatureGateState::Enabled
        : FeatureGateState::Disabled;
}

bool FeatureRegistry::enable(const std::string& name) {
    const FeatureGate* feature = find(name);
    if (feature == nullptr) {
        return false;
    }
    if (feature->stage != FeatureStage::Stable) {
        enabled_features_[name] = true;
    }
    return true;
}

bool FeatureRegistry::is_enabled(const std::string& name) const {
    const FeatureGateState feature_state = state(name);
    return feature_state == FeatureGateState::Enabled || feature_state == FeatureGateState::Stable;
}

const char* feature_stage_name(FeatureStage stage) {
    switch (stage) {
        case FeatureStage::Alpha:
            return "alpha";
        case FeatureStage::Beta:
            return "beta";
        case FeatureStage::Stable:
            return "stable";
    }
    return "unknown";
}

FeatureRegistry make_default_feature_registry() {
    FeatureRegistry registry;
    registry.register_feature({
        "parameterized-quantum-gates",
        FeatureStage::Alpha,
        "Enables the recovery profile's bounded literal-angle quantum gate syntax.",
        "synq#language-foundation",
    });
    return registry;
}

}  // namespace synq::compiler
