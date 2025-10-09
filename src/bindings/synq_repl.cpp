// MIT License
// 
// Copyright (c) 2025 SynQ Contributors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../compiler/repl.h"

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