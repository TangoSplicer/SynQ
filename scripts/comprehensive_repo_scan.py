#!/usr/bin/env python3
"""
SynQ Comprehensive Repository Scan

Performs full diagnostic analysis:
1. Code quality analysis
2. Security vulnerability scanning
3. Test coverage analysis
4. Documentation completeness
5. Dependency analysis
6. Performance analysis
7. Architecture analysis
8. Integration analysis
"""

import os
import json
import re
from datetime import datetime
from typing import Dict, List, Tuple
from pathlib import Path


class ComprehensiveRepoScan:
    """Comprehensive repository diagnostic scanner"""
    
    def __init__(self):
        self.timestamp = datetime.now().isoformat()
        self.repo_path = '/tmp/synq_phase12'
        self.issues = []
        self.warnings = []
        self.errors = []
        self.critical_issues = []
        self.results = {
            'scan_timestamp': self.timestamp,
            'repo_path': self.repo_path,
            'issues': [],
            'warnings': [],
            'errors': [],
            'critical_issues': [],
            'summary': {}
        }
    
    # ========================================================================
    # SCAN 1: CODE QUALITY ANALYSIS
    # ========================================================================
    
    def scan_code_quality(self) -> Dict:
        """Scan code quality issues"""
        print("\n[SCAN 1] Analyzing code quality...")
        
        code_quality = {
            'status': 'scanning',
            'findings': []
        }
        
        # Check for common code quality issues
        py_files = list(Path(self.repo_path).rglob('*.py'))
        
        for py_file in py_files:
            try:
                with open(py_file, 'r') as f:
                    content = f.read()
                    lines = content.split('\n')
                    
                    # Check for long lines (>120 chars)
                    for i, line in enumerate(lines, 1):
                        if len(line) > 120:
                            self.warnings.append(f"Long line ({len(line)} chars) in {py_file}:{i}")
                    
                    # Check for TODO comments
                    for i, line in enumerate(lines, 1):
                        if 'TODO' in line or 'FIXME' in line:
                            self.warnings.append(f"TODO/FIXME found in {py_file}:{i}: {line.strip()}")
                    
                    # Check for hardcoded credentials
                    if re.search(r'(password|secret|api_key|token)\s*=\s*["\']', content, re.IGNORECASE):
                        self.critical_issues.append(f"Potential hardcoded credentials in {py_file}")
                    
                    # Check for print statements (should use logging)
                    for i, line in enumerate(lines, 1):
                        if re.match(r'\s*print\(', line):
                            self.warnings.append(f"Print statement (use logging) in {py_file}:{i}")
            except Exception as e:
                self.errors.append(f"Error scanning {py_file}: {str(e)}")
        
        code_quality['findings'] = {
            'files_scanned': len(py_files),
            'warnings': len([w for w in self.warnings if py_file.name in w]),
            'critical_issues': len(self.critical_issues)
        }
        code_quality['status'] = 'complete'
        
        print(f"✓ Code quality scan complete ({len(py_files)} files)")
        return code_quality
    
    # ========================================================================
    # SCAN 2: SECURITY VULNERABILITY SCANNING
    # ========================================================================
    
    def scan_security_vulnerabilities(self) -> Dict:
        """Scan for security vulnerabilities"""
        print("\n[SCAN 2] Scanning for security vulnerabilities...")
        
        security = {
            'status': 'scanning',
            'vulnerabilities': []
        }
        
        # Check for common security issues
        all_files = list(Path(self.repo_path).rglob('*'))
        
        for file_path in all_files:
            if file_path.is_file():
                try:
                    # Skip binary files
                    if file_path.suffix in ['.pyc', '.o', '.so', '.dll']:
                        continue
                    
                    with open(file_path, 'r', errors='ignore') as f:
                        content = f.read()
                        
                        # Check for SQL injection vulnerabilities
                        if 'sql' in file_path.name.lower() or 'db' in file_path.name.lower():
                            if re.search(r'execute\s*\(\s*["\'].*\{.*\}', content):
                                self.critical_issues.append(f"Potential SQL injection in {file_path}")
                        
                        # Check for hardcoded URLs
                        if re.search(r'https?://[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}', content):
                            if 'example.com' not in content and 'localhost' not in content:
                                self.warnings.append(f"Hardcoded URL found in {file_path}")
                        
                        # Check for insecure random
                        if 'random.' in content and 'secrets.' not in content:
                            if 'import random' in content:
                                self.warnings.append(f"Using random instead of secrets in {file_path}")
                        
                        # Check for eval/exec
                        if re.search(r'\b(eval|exec)\s*\(', content):
                            self.critical_issues.append(f"Dangerous eval/exec in {file_path}")
                        
                        # Check for pickle
                        if 'pickle.load' in content:
                            self.warnings.append(f"Unsafe pickle.load in {file_path}")
                
                except Exception as e:
                    pass
        
        security['vulnerabilities'] = {
            'critical': len(self.critical_issues),
            'warnings': len(self.warnings)
        }
        security['status'] = 'complete'
        
        print(f"✓ Security scan complete")
        return security
    
    # ========================================================================
    # SCAN 3: TEST COVERAGE ANALYSIS
    # ========================================================================
    
    def scan_test_coverage(self) -> Dict:
        """Analyze test coverage"""
        print("\n[SCAN 3] Analyzing test coverage...")
        
        test_coverage = {
            'status': 'scanning',
            'findings': {}
        }
        
        # Check for test files
        test_files = list(Path(self.repo_path).rglob('test_*.py')) + \
                     list(Path(self.repo_path).rglob('*_test.py'))
        
        # Check for source files without tests
        src_files = list(Path(self.repo_path).rglob('*.py'))
        src_files = [f for f in src_files if 'test' not in f.name and 'script' not in f.name]
        
        # Estimate coverage
        test_count = len(test_files)
        src_count = len(src_files)
        
        if src_count > 0:
            coverage_ratio = (test_count / src_count) * 100
        else:
            coverage_ratio = 0
        
        test_coverage['findings'] = {
            'test_files': test_count,
            'source_files': src_count,
            'estimated_coverage_ratio': f"{coverage_ratio:.1f}%",
            'missing_tests': []
        }
        
        # Check for untested modules
        for src_file in src_files:
            test_file = src_file.parent / f"test_{src_file.name}"
            if not test_file.exists():
                test_coverage['findings']['missing_tests'].append(str(src_file))
        
        test_coverage['status'] = 'complete'
        
        print(f"✓ Test coverage analysis complete ({test_count} test files)")
        return test_coverage
    
    # ========================================================================
    # SCAN 4: DOCUMENTATION COMPLETENESS
    # ========================================================================
    
    def scan_documentation(self) -> Dict:
        """Scan documentation completeness"""
        print("\n[SCAN 4] Scanning documentation completeness...")
        
        documentation = {
            'status': 'scanning',
            'findings': {}
        }
        
        # Check for documentation files
        doc_files = list(Path(self.repo_path).rglob('*.md'))
        readme_exists = (Path(self.repo_path) / 'README.md').exists()
        
        # Check for docstrings in Python files
        py_files = list(Path(self.repo_path).rglob('*.py'))
        undocumented_functions = 0
        
        for py_file in py_files:
            try:
                with open(py_file, 'r') as f:
                    content = f.read()
                    
                    # Count functions without docstrings
                    functions = re.findall(r'def\s+(\w+)\s*\(', content)
                    docstrings = re.findall(r'def\s+\w+\s*\(.*?\):\s+"""', content)
                    
                    if len(functions) > len(docstrings):
                        undocumented_functions += (len(functions) - len(docstrings))
            except:
                pass
        
        documentation['findings'] = {
            'readme_exists': readme_exists,
            'doc_files': len(doc_files),
            'undocumented_functions': undocumented_functions,
            'doc_files_list': [f.name for f in doc_files[:10]]
        }
        
        if not readme_exists:
            self.warnings.append("README.md not found in root")
        
        if undocumented_functions > 10:
            self.warnings.append(f"Many undocumented functions: {undocumented_functions}")
        
        documentation['status'] = 'complete'
        
        print(f"✓ Documentation scan complete ({len(doc_files)} doc files)")
        return documentation
    
    # ========================================================================
    # SCAN 5: DEPENDENCY ANALYSIS
    # ========================================================================
    
    def scan_dependencies(self) -> Dict:
        """Analyze dependencies"""
        print("\n[SCAN 5] Analyzing dependencies...")
        
        dependencies = {
            'status': 'scanning',
            'findings': {}
        }
        
        # Check for requirements files
        requirements_files = [
            Path(self.repo_path) / 'requirements.txt',
            Path(self.repo_path) / 'package.json',
            Path(self.repo_path) / 'Cargo.toml',
            Path(self.repo_path) / 'go.mod'
        ]
        
        found_requirements = []
        for req_file in requirements_files:
            if req_file.exists():
                found_requirements.append(req_file.name)
        
        # Check for dependency vulnerabilities
        if (Path(self.repo_path) / 'requirements.txt').exists():
            try:
                with open(Path(self.repo_path) / 'requirements.txt', 'r') as f:
                    deps = f.readlines()
                    
                    # Check for pinned versions
                    unpinned = [d for d in deps if '==' not in d and d.strip()]
                    
                    if unpinned:
                        self.warnings.append(f"Unpinned dependencies: {len(unpinned)}")
            except:
                pass
        
        dependencies['findings'] = {
            'requirements_files': found_requirements,
            'count': len(found_requirements)
        }
        
        if not found_requirements:
            self.warnings.append("No requirements/dependency files found")
        
        dependencies['status'] = 'complete'
        
        print(f"✓ Dependency analysis complete")
        return dependencies
    
    # ========================================================================
    # SCAN 6: PERFORMANCE ANALYSIS
    # ========================================================================
    
    def scan_performance(self) -> Dict:
        """Analyze performance issues"""
        print("\n[SCAN 6] Analyzing performance issues...")
        
        performance = {
            'status': 'scanning',
            'findings': {}
        }
        
        # Check for common performance issues
        py_files = list(Path(self.repo_path).rglob('*.py'))
        
        performance_issues = []
        
        for py_file in py_files:
            try:
                with open(py_file, 'r') as f:
                    content = f.read()
                    lines = content.split('\n')
                    
                    # Check for nested loops
                    for i, line in enumerate(lines):
                        if 'for' in line and i < len(lines) - 1:
                            if 'for' in lines[i+1]:
                                performance_issues.append(f"Nested loops in {py_file}:{i+1}")
                    
                    # Check for inefficient string operations
                    if '+=' in content and 'str' in content:
                        performance_issues.append(f"String concatenation in loop in {py_file}")
                    
                    # Check for N+1 queries
                    if 'for' in content and 'query' in content.lower():
                        performance_issues.append(f"Potential N+1 query in {py_file}")
            except:
                pass
        
        performance['findings'] = {
            'potential_issues': len(performance_issues),
            'issues': performance_issues[:5]
        }
        
        performance['status'] = 'complete'
        
        print(f"✓ Performance analysis complete")
        return performance
    
    # ========================================================================
    # SCAN 7: ARCHITECTURE ANALYSIS
    # ========================================================================
    
    def scan_architecture(self) -> Dict:
        """Analyze architecture"""
        print("\n[SCAN 7] Analyzing architecture...")
        
        architecture = {
            'status': 'scanning',
            'findings': {}
        }
        
        # Check directory structure
        dirs = []
        for item in Path(self.repo_path).iterdir():
            if item.is_dir() and not item.name.startswith('.'):
                dirs.append(item.name)
        
        # Check for circular imports
        py_files = list(Path(self.repo_path).rglob('*.py'))
        imports = {}
        
        for py_file in py_files:
            try:
                with open(py_file, 'r') as f:
                    content = f.read()
                    import_lines = re.findall(r'from\s+(\S+)\s+import|import\s+(\S+)', content)
                    imports[py_file.name] = import_lines
            except:
                pass
        
        architecture['findings'] = {
            'directories': dirs,
            'directory_count': len(dirs),
            'python_files': len(py_files),
            'structure_issues': []
        }
        
        # Check for missing __init__.py files
        for dir_path in Path(self.repo_path).rglob('*'):
            if dir_path.is_dir() and not dir_path.name.startswith('.'):
                py_files_in_dir = list(dir_path.glob('*.py'))
                if py_files_in_dir and not (dir_path / '__init__.py').exists():
                    if 'test' not in dir_path.name:
                        self.warnings.append(f"Missing __init__.py in {dir_path}")
        
        architecture['status'] = 'complete'
        
        print(f"✓ Architecture analysis complete")
        return architecture
    
    # ========================================================================
    # SCAN 8: INTEGRATION ANALYSIS
    # ========================================================================
    
    def scan_integration(self) -> Dict:
        """Analyze integration points"""
        print("\n[SCAN 8] Analyzing integration points...")
        
        integration = {
            'status': 'scanning',
            'findings': {}
        }
        
        # Check for API endpoints
        py_files = list(Path(self.repo_path).rglob('*.py'))
        api_endpoints = []
        external_apis = []
        
        for py_file in py_files:
            try:
                with open(py_file, 'r') as f:
                    content = f.read()
                    
                    # Check for Flask/FastAPI routes
                    if '@app.route' in content or '@router.' in content:
                        routes = re.findall(r'@(?:app|router)\.(?:get|post|put|delete)\(["\']([^"\']+)["\']', content)
                        api_endpoints.extend(routes)
                    
                    # Check for external API calls
                    if 'requests.' in content or 'urllib' in content:
                        external_apis.append(py_file.name)
            except:
                pass
        
        integration['findings'] = {
            'api_endpoints': len(api_endpoints),
            'external_integrations': len(external_apis),
            'endpoints_sample': api_endpoints[:5],
            'integration_files': external_apis[:5]
        }
        
        integration['status'] = 'complete'
        
        print(f"✓ Integration analysis complete")
        return integration
    
    # ========================================================================
    # MAIN EXECUTION
    # ========================================================================
    
    def run_all_scans(self) -> Dict:
        """Run all scans"""
        print("=" * 70)
        print("SynQ COMPREHENSIVE REPOSITORY SCAN")
        print("=" * 70)
        print(f"Repository: {self.repo_path}")
        print(f"Scan started: {self.timestamp}")
        
        # Run all scans
        scans = {
            'code_quality': self.scan_code_quality(),
            'security': self.scan_security_vulnerabilities(),
            'test_coverage': self.scan_test_coverage(),
            'documentation': self.scan_documentation(),
            'dependencies': self.scan_dependencies(),
            'performance': self.scan_performance(),
            'architecture': self.scan_architecture(),
            'integration': self.scan_integration()
        }
        
        # Compile results
        self.results['scans'] = scans
        self.results['issues_found'] = len(self.issues)
        self.results['warnings_found'] = len(self.warnings)
        self.results['errors_found'] = len(self.errors)
        self.results['critical_issues_found'] = len(self.critical_issues)
        
        self.results['summary'] = {
            'total_issues': len(self.issues) + len(self.warnings) + len(self.errors),
            'critical_issues': len(self.critical_issues),
            'warnings': len(self.warnings),
            'errors': len(self.errors),
            'severity': self._calculate_severity()
        }
        
        print("\n" + "=" * 70)
        print("SCAN COMPLETE")
        print("=" * 70)
        
        return self.results
    
    def _calculate_severity(self) -> str:
        """Calculate overall severity"""
        if len(self.critical_issues) > 0:
            return "CRITICAL"
        elif len(self.errors) > 5:
            return "HIGH"
        elif len(self.warnings) > 20:
            return "MEDIUM"
        else:
            return "LOW"
    
    def generate_report(self) -> str:
        """Generate comprehensive scan report"""
        report = []
        report.append("=" * 70)
        report.append("SynQ COMPREHENSIVE REPOSITORY SCAN REPORT")
        report.append("=" * 70)
        report.append(f"\nScan Timestamp: {self.timestamp}")
        report.append(f"Repository: {self.repo_path}")
        
        report.append("\n" + "-" * 70)
        report.append("SCAN SUMMARY")
        report.append("-" * 70)
        report.append(f"Total Issues Found: {self.results['summary']['total_issues']}")
        report.append(f"Critical Issues: {len(self.critical_issues)}")
        report.append(f"Errors: {len(self.errors)}")
        report.append(f"Warnings: {len(self.warnings)}")
        report.append(f"Overall Severity: {self.results['summary']['severity']}")
        
        if self.critical_issues:
            report.append("\n" + "-" * 70)
            report.append("🚨 CRITICAL ISSUES (MUST FIX)")
            report.append("-" * 70)
            for issue in self.critical_issues[:10]:
                report.append(f"  ❌ {issue}")
        
        if self.errors:
            report.append("\n" + "-" * 70)
            report.append("⚠️  ERRORS (SHOULD FIX)")
            report.append("-" * 70)
            for error in self.errors[:10]:
                report.append(f"  ⚠️  {error}")
        
        if self.warnings:
            report.append("\n" + "-" * 70)
            report.append("⚡ WARNINGS (NICE TO FIX)")
            report.append("-" * 70)
            for warning in self.warnings[:10]:
                report.append(f"  ℹ️  {warning}")
        
        report.append("\n" + "-" * 70)
        report.append("SCAN RESULTS BY CATEGORY")
        report.append("-" * 70)
        
        for scan_name, scan_result in self.results['scans'].items():
            report.append(f"\n{scan_name.upper()}: {scan_result['status']}")
            if 'findings' in scan_result:
                for key, value in scan_result['findings'].items():
                    if isinstance(value, (int, str, bool)):
                        report.append(f"  - {key}: {value}")
        
        report.append("\n" + "=" * 70)
        report.append("RECOMMENDATIONS")
        report.append("=" * 70)
        
        if self.critical_issues:
            report.append("\n1. IMMEDIATE: Fix all critical issues")
            report.append("   These could cause security vulnerabilities or runtime failures")
        
        if self.errors:
            report.append("\n2. HIGH PRIORITY: Address all errors")
            report.append("   These indicate code quality or compatibility issues")
        
        if self.warnings:
            report.append("\n3. MEDIUM PRIORITY: Review warnings")
            report.append("   These suggest improvements to code quality and maintainability")
        
        report.append("\n" + "=" * 70)
        
        return "\n".join(report)


def main():
    """Main execution"""
    scanner = ComprehensiveRepoScan()
    results = scanner.run_all_scans()
    
    # Print report
    print(scanner.generate_report())
    
    # Save results
    with open('/tmp/synq_phase12/comprehensive_scan_results.json', 'w') as f:
        json.dump(results, f, indent=2)
    
    print("\nDetailed results saved to: comprehensive_scan_results.json")
    
    # Print summary
    print("\n" + "=" * 70)
    print("QUICK SUMMARY")
    print("=" * 70)
    print(f"Critical Issues: {len(scanner.critical_issues)}")
    print(f"Errors: {len(scanner.errors)}")
    print(f"Warnings: {len(scanner.warnings)}")
    print(f"Overall Severity: {results['summary']['severity']}")
    print("=" * 70)


if __name__ == "__main__":
    main()
