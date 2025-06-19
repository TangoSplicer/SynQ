// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// qclassical_pipeline.synq

@doc "Defines a quantum-classical hybrid execution pipeline."
@macro pipeline(stage1::str, stage2::str, stage3::str) {
  pipeline::execute([stage1, stage2, stage3])
}