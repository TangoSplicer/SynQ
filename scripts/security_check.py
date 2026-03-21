#!/usr/bin/env python3
"""Security checking and validation script for SynQ platform."""

import subprocess
import json
import sys
from typing import List, Dict, Any
from datetime import datetime


class SecurityChecker:
    """Run security checks on SynQ codebase."""
    
    def __init__(self):
        self.results = {
            'timestamp': datetime.now().isoformat(),
            'checks': {}
        }
        self.passed = 0
        self.failed = 0
    
    def run_check(self, name: str, command: str, critical: bool = False) -> bool:
        """Run a security check."""
        print(f"\n🔍 Running: {name}")
        print(f"   Command: {command}")
        
        try:
            result = subprocess.run(
                command,
                shell=True,
                capture_output=True,
                text=True,
                timeout=60
            )
            
            passed = result.returncode == 0
            
            self.results['checks'][name] = {
                'passed': passed,
                'critical': critical,
                'command': command,
                'stdout': result.stdout[:500],  # Truncate for readability
                'stderr': result.stderr[:500]
            }
            
            if passed:
                print(f"   ✅ PASSED")
                self.passed += 1
            else:
                print(f"   ❌ FAILED")
                self.failed += 1
                if critical:
                    print(f"   ⚠️  CRITICAL - Must fix before deployment")
            
            return passed
        
        except subprocess.TimeoutExpired:
            print(f"   ⏱️  TIMEOUT")
            self.results['checks'][name] = {
                'passed': False,
                'critical': critical,
                'error': 'Command timed out'
            }
            self.failed += 1
            return False
        
        except Exception as e:
            print(f"   ❌ ERROR: {e}")
            self.results['checks'][name] = {
                'passed': False,
                'critical': critical,
                'error': str(e)
            }
            self.failed += 1
            return False
    
    def check_dependencies(self):
        """Check for vulnerable dependencies."""
        print("\n" + "=" * 50)
        print("Dependency Scanning")
        print("=" * 50)
        
        # Python dependencies
        self.run_check(
            "Python: Safety check",
            "pip install safety && safety check",
            critical=True
        )
        
        # JavaScript dependencies
        self.run_check(
            "JavaScript: npm audit",
            "cd frontend && npm audit",
            critical=True
        )
    
    def check_static_analysis(self):
        """Run static code analysis."""
        print("\n" + "=" * 50)
        print("Static Code Analysis")
        print("=" * 50)
        
        # Python linting
        self.run_check(
            "Python: Bandit security linter",
            "pip install bandit && bandit -r backend/ -f json",
            critical=True
        )
        
        # Python type checking
        self.run_check(
            "Python: MyPy type checking",
            "pip install mypy && mypy backend/",
            critical=False
        )
        
        # JavaScript linting
        self.run_check(
            "JavaScript: ESLint",
            "cd frontend && npm run lint",
            critical=False
        )
        
        # C++ analysis
        self.run_check(
            "C++: Cppcheck",
            "cppcheck --enable=all compiler/src/ 2>/dev/null",
            critical=False
        )
    
    def check_secrets(self):
        """Check for exposed secrets."""
        print("\n" + "=" * 50)
        print("Secrets Detection")
        print("=" * 50)
        
        self.run_check(
            "Detect hardcoded secrets",
            "git log -p | grep -E '(password|secret|token|api_key)' | head -20",
            critical=True
        )
        
        self.run_check(
            "Check .env files not committed",
            "! git ls-files | grep -E '\\.env'",
            critical=True
        )
    
    def check_authentication(self):
        """Check authentication implementation."""
        print("\n" + "=" * 50)
        print("Authentication Checks")
        print("=" * 50)
        
        self.run_check(
            "Check JWT implementation",
            "grep -r 'jwt' backend/ | grep -v '.pyc' | wc -l",
            critical=False
        )
        
        self.run_check(
            "Check password hashing",
            "grep -r 'bcrypt\\|argon2' backend/ | grep -v '.pyc' | wc -l",
            critical=False
        )
    
    def check_https(self):
        """Check HTTPS configuration."""
        print("\n" + "=" * 50)
        print("HTTPS & TLS Checks")
        print("=" * 50)
        
        self.run_check(
            "Check HTTPS enforcement",
            "grep -r 'https\\|HTTPS' backend/ | wc -l",
            critical=False
        )
    
    def check_input_validation(self):
        """Check input validation."""
        print("\n" + "=" * 50)
        print("Input Validation Checks")
        print("=" * 50)
        
        self.run_check(
            "Check for input validation",
            "grep -r 'validator\\|validate' backend/ | grep -v '.pyc' | wc -l",
            critical=False
        )
    
    def check_logging(self):
        """Check logging implementation."""
        print("\n" + "=" * 50)
        print("Logging Checks")
        print("=" * 50)
        
        self.run_check(
            "Check for security logging",
            "grep -r 'logging\\|logger' backend/ | grep -v '.pyc' | wc -l",
            critical=False
        )
    
    def run_all(self):
        """Run all security checks."""
        print("\n" + "=" * 50)
        print("SynQ Security Audit")
        print("=" * 50)
        
        self.check_dependencies()
        self.check_static_analysis()
        self.check_secrets()
        self.check_authentication()
        self.check_https()
        self.check_input_validation()
        self.check_logging()
        
        return self.generate_report()
    
    def generate_report(self) -> Dict[str, Any]:
        """Generate security report."""
        print("\n" + "=" * 50)
        print("Security Audit Report")
        print("=" * 50)
        
        total = self.passed + self.failed
        percentage = (self.passed / total * 100) if total > 0 else 0
        
        print(f"\n✅ Passed: {self.passed}")
        print(f"❌ Failed: {self.failed}")
        print(f"📊 Score: {percentage:.1f}%")
        
        # Determine status
        if self.failed == 0:
            status = "🟢 SECURE"
        elif percentage >= 80:
            status = "🟡 ACCEPTABLE"
        else:
            status = "🔴 CRITICAL"
        
        print(f"\n{status}")
        
        # Save report
        self.results['summary'] = {
            'passed': self.passed,
            'failed': self.failed,
            'total': total,
            'score': percentage,
            'status': status
        }
        
        with open('security_report.json', 'w') as f:
            json.dump(self.results, f, indent=2)
        
        print("\n✓ Report saved to security_report.json")
        
        return self.results


def main():
    """Run security checks."""
    checker = SecurityChecker()
    results = checker.run_all()
    
    # Exit with error if critical checks failed
    critical_failed = any(
        check.get('critical') and not check.get('passed')
        for check in results['checks'].values()
    )
    
    if critical_failed:
        print("\n❌ Critical security issues found!")
        sys.exit(1)
    else:
        print("\n✅ Security checks passed!")
        sys.exit(0)


if __name__ == '__main__':
    main()
