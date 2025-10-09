#!/usr/bin/env python3
"""
Update license headers in SynQ source files from commercial to MIT license.
"""

import os
import re
import glob

# The old commercial license header pattern
OLD_LICENSE_PATTERN = r'// SynQ Commercial Attribution License v1\.0\s*// © 2025 SynQ Contributors\. All rights reserved\.\s*// This file is part of the SynQ programming ecosystem\.\s*'

# The new MIT license header
NEW_LICENSE_HEADER = """// MIT License
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
// SOFTWARE."""

def update_file(filepath):
    """Update license header in a single file."""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Check if file has the old license
        if re.search(OLD_LICENSE_PATTERN, content):
            # Replace the old license with new one
            updated_content = re.sub(OLD_LICENSE_PATTERN, NEW_LICENSE_HEADER + '\n', content)
            
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(updated_content)
            
            print(f"✅ Updated: {filepath}")
            return True
        else:
            print(f"⏭️  Skipped: {filepath} (no old license found)")
            return False
            
    except Exception as e:
        print(f"❌ Error updating {filepath}: {e}")
        return False

def main():
    """Main function to update all files."""
    updated_count = 0
    skipped_count = 0
    error_count = 0
    
    # File extensions to process
    extensions = ['.cpp', '.h', '.hpp', '.c', '.cc', '.synq', '.py', '.md', '.txt']
    
    # Walk through the directory tree
    for root, dirs, files in os.walk('.'):
        # Skip build directories and hidden directories
        dirs[:] = [d for d in dirs if not d.startswith('.') and d not in ['build', '__pycache__']]
        
        for file in files:
            # Check if file has one of our target extensions
            if any(file.endswith(ext) for ext in extensions):
                filepath = os.path.join(root, file)
                
                # Skip certain files
                if 'LICENSE.txt' in filepath or 'update_license.py' in filepath:
                    continue
                
                if update_file(filepath):
                    updated_count += 1
                else:
                    skipped_count += 1
    
    print(f"\n📊 Summary:")
    print(f"   ✅ Updated: {updated_count} files")
    print(f"   ⏭️  Skipped: {skipped_count} files")
    print(f"   ❌ Errors: {error_count} files")

if __name__ == '__main__':
    main()