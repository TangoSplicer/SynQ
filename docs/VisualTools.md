// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

✅ File: synq/docs/VisualTools.md

# SynQ Visual Tools Guide

## Overview

SynQ’s visual tools allow developers to observe, debug, and present hybrid quantum-classical-AI workflows through rich, interactive visualizations. These tools integrate seamlessly with REPL, debugger, notebooks, and plugin pipelines.

---

## 1. Enabling Visual Mode

Visual mode overlays visual outputs on REPL and runtime tools.

```synq
:set visual true

Or via config:

{
  "visual_mode": true
}


---

2. Built-in Visual Features

Tool	Description

Circuit Renderer	Draws quantum circuits in SVG or WebGL
State Explorer	Shows real-time qubit amplitudes
Memory Heatmap	Visualizes variable usage and growth
Gate-by-Gate Animation	Animates circuit progression
Entanglement Graph	Renders qubit links and correlations
Plugin Dashboards	Dynamic plugin output interfaces



---

3. Visual Plugins

SynQ supports modular visual plugins:

visual_circuit_svg

qpu_live_sim

ai_prompt_tree

trace_overlay

optimizer_convergence_plot


Load with:

:plugin load visual_circuit_svg

Or:

synq plugin load my_visual_plugin/


---

4. Visualizing Circuits

qc = QuantumCircuit()
qc.add(H, 0)
qc.add(CX, 0, 1)

render(qc)

You can also export:

render(qc, output="circuit.svg")


---

5. Visualizing Quantum State

qreg = QRegister(2)
plot_statevector(qreg)

This opens:

Real/Imaginary bar plots

Bloch spheres

Interactive sliders for gate impact



---

6. Memory Visualization

Enable:

:mem

With visual mode on, shows:

Allocation bars

Type usage graphs

Quantum-classical correlation lines



---

7. AI Tree Visualizations

Visualize multi-agent interaction trees:

render(ai.trace_tree())

Each node contains:

Prompt content

Model used

Output preview

Cost estimate (token usage)



---

8. Debugging Views

When debugging with visual mode:

Circuits animate per instruction

Highlighted lines correspond to REPL step

Collapses (e.g., measurement) show particle-like transitions



---

9. Export Formats

Visual outputs can be exported to:

Format	Use Case

.svg	Static diagram export
.png	For documentation/media
.html	Embedded dashboard
.json	Export for later rerender
.mp4	Animated circuit playback



---

10. Custom Visualizations

You can write your own plugin with:

register_visual_output("MyView", MyRenderFunction);

Use render("MyView") inside SynQ.


---

Tips

Combine :trace + visual for best debugging insight

Use render(...) inside notebooks for inline circuit display

All visual plugins support metadata overlays (e.g., gate labels, cost)



---

Troubleshooting

Issue	Fix

"No visual renderer found"	Ensure visual_mode=true and plugin is loaded
Empty output	Check if circuit/state is initialized
Crash on animation	Disable GPU mode in .synqrc (for headless)



---

Support

Documentation: synqhub.org/docs/visual

Or launch REPL:

synq repl
:set visual true


---

© 2025 SynQ Project — See the Unseen. Visualize the Quantum-Classical Frontier.

---


