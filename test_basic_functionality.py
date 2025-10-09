#!/usr/bin/env python3

import subprocess
import os
import sys

def test_build():
    """Test basic build functionality"""
    print("Testing SynQ build system...")
    
    # Change to build directory
    os.chdir("build")
    
    # Try to build the core library
    result = subprocess.run(["make", "synq_lib", "-j2"], 
                          capture_output=True, text=True)
    
    if result.returncode == 0:
        print("✅ Core library built successfully!")
        return True
    else:
        print("❌ Build failed:")
        print("STDOUT:", result.stdout)
        print("STDERR:", result.stderr)
        return False

def test_examples():
    """Test that example files were created"""
    print("\nTesting example files...")
    
    examples = [
        "examples/hello.synq",
        "examples/quantum_demo.synq", 
        "examples/ai_demo.synq",
        "examples/synq_demo.ipynb",
        "examples/advanced_synq_examples.ipynb"
    ]
    
    all_exist = True
    for example in examples:
        if os.path.exists(example):
            print(f"✅ {example} exists")
        else:
            print(f"❌ {example} missing")
            all_exist = False
    
    return all_exist

def test_license():
    """Test that license was updated"""
    print("\nTesting license update...")
    
    # Check main license file
    with open("LICENSE.txt", "r") as f:
        license_content = f.read()
    
    if "MIT License" in license_content:
        print("✅ LICENSE.txt contains MIT License")
    else:
        print("❌ LICENSE.txt does not contain MIT License")
        return False
    
    # Check a sample source file
    with open("src/backend/simulator.cpp", "r") as f:
        source_content = f.read()
    
    if "MIT License" in source_content:
        print("✅ Source files contain MIT License header")
    else:
        print("❌ Source files missing MIT License header")
        return False
    
    return True

def main():
    """Run all tests"""
    print("=" * 60)
    print("SynQ Finalization Test Suite")
    print("=" * 60)
    
    tests = [
        ("Build System", test_build),
        ("Example Files", test_examples), 
        ("License Update", test_license)
    ]
    
    results = {}
    for test_name, test_func in tests:
        print(f"\n--- {test_name} ---")
        try:
            results[test_name] = test_func()
        except Exception as e:
            print(f"❌ {test_name} failed with exception: {e}")
            results[test_name] = False
    
    print("\n" + "=" * 60)
    print("Test Results Summary:")
    print("=" * 60)
    
    for test_name, passed in results.items():
        status = "✅ PASSED" if passed else "❌ FAILED"
        print(f"{test_name}: {status}")
    
    all_passed = all(results.values())
    print(f"\nOverall: {'✅ ALL TESTS PASSED' if all_passed else '❌ SOME TESTS FAILED'}")
    
    return all_passed

if __name__ == "__main__":
    success = main()
    sys.exit(0 if success else 1)