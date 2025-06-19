// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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