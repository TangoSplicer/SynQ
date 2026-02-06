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
# File: tools/html_report.py

import os

def parse_coverage(file_path):
    with open(file_path, "r") as f:
        lines = f.readlines()

    sections = {"Line Coverage": [], "Function Calls": []}
    current = None

    for line in lines:
        line = line.strip()
        if "[Line Coverage]" in line:
            current = "Line Coverage"
        elif "[Function Calls]" in line:
            current = "Function Calls"
        elif current and line:
            sections[current].append(line)

    return sections

def generate_html_report(sections, output_path):
    html = """
    <html>
    <head>
        <title>SynQ Coverage Report</title>
        <style>
            body { font-family: monospace; background: #fdfdfd; padding: 2em; }
            h1 { color: #222; }
            .section { margin-top: 2em; }
            .line { background: #e9e9e9; margin: 0.25em 0; padding: 0.5em; border-left: 4px solid #5a5; }
        </style>
    </head>
    <body>
        <h1>📈 SynQ HTML Coverage Report</h1>
    """

    for title, items in sections.items():
        html += f'<div class="section"><h2>{title}</h2>'
        for item in items:
            html += f'<div class="line">{item}</div>'
        html += "</div>"

    html += "</body></html>"

    with open(output_path, "w") as f:
        f.write(html)

if __name__ == "__main__":
    txt_path = "coverage/coverage_report.txt"
    html_path = "coverage/coverage_report.html"

    if not os.path.exists(txt_path):
        print("❌ coverage_report.txt not found.")
    else:
        sections = parse_coverage(txt_path)
        generate_html_report(sections, html_path)
        print(f"✅ HTML report generated: {html_path}")