// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../src/compiler/repl.h"

namespace py = pybind11;
using namespace synq;

PYBIND11_MODULE(synq_repl, m) {
    m.doc() = "SynQ REPL plugin bindings";

    py::class_<REPL>(m, "REPL")
        .def(py::init<>())
        .def("execute", &REPL::execute, "Execute a SynQ DSL command")
        .def("list_plugins", &REPL::list_plugins, "List all registered plugins")
        .def("register_plugin", &REPL::register_plugin,
            py::arg("name"), py::arg("version"), py::arg("desc"), py::arg("handler"),
            "Register a plugin into the system")
        .def("invoke_plugin", &REPL::invoke_plugin,
            py::arg("name"), py::arg("payload"),
            "Invoke a registered plugin");
}