# GitHub Labels Configuration

This document lists all recommended GitHub labels for organizing issues, pull requests, and discussions.

## How to Add Labels

1. Go to your repository on GitHub
2. Click **Issues** → **Labels**
3. Click **New label**
4. Copy the label name, description, and color from below
5. Click **Create label**

---

## Label Categories

### Priority Labels (Red)
- **critical** - `#d73a49` - Urgent issue affecting production
- **high** - `#ff6b6b` - Important issue, should be addressed soon
- **medium** - `#ffa94d` - Normal priority issue
- **low** - `#85e89d` - Nice to have, can wait

### Type Labels (Blue)
- **bug** - `#0075ca` - Something isn't working
- **enhancement** - `#a2eeef` - New feature or improvement
- **documentation** - `#0366d6` - Documentation improvements
- **question** - `#d876e3` - Question or discussion
- **discussion** - `#9e10f0` - Discussion topic

### Status Labels (Purple)
- **in-progress** - `#6f42c1` - Currently being worked on
- **blocked** - `#5a6c75` - Blocked by another issue
- **waiting-feedback** - `#b60205` - Waiting for user feedback
- **needs-review** - `#fbca04` - Needs code review
- **ready-to-merge** - `#0e8a16` - Ready to merge

### Category Labels (Green)
- **compiler** - `#0e8a16` - Compiler-related
- **backend** - `#1f6feb` - Backend/API-related
- **frontend** - `#7057ff` - Frontend-related
- **quantum** - `#00ff00` - Quantum computing features
- **ml** - `#cccccc` - Machine learning features
- **performance** - `#ffd700` - Performance optimization
- **security** - `#ff0000` - Security issues

### Effort Labels (Orange)
- **good-first-issue** - `#7057ff` - Good for new contributors
- **help-wanted** - `#008672` - Help wanted from community
- **easy** - `#90ee90` - Easy to implement
- **medium-effort** - `#ffa500` - Medium effort required
- **hard** - `#ff4500` - Difficult to implement

### Status Labels (Gray)
- **duplicate** - `#cccccc` - Duplicate of another issue
- **invalid** - `#e4e669` - Invalid or not applicable
- **wontfix** - `#ffffff` - Will not be fixed
- **on-hold** - `#cccccc` - On hold pending decision

---

## Quick Reference

| Label | Color | Purpose |
|-------|-------|---------|
| critical | Red | Urgent, production-affecting |
| bug | Blue | Bug report |
| enhancement | Cyan | Feature request |
| documentation | Blue | Docs improvement |
| in-progress | Purple | Being worked on |
| blocked | Purple | Blocked by another issue |
| compiler | Green | Compiler-related |
| quantum | Green | Quantum features |
| good-first-issue | Purple | New contributor friendly |
| help-wanted | Teal | Community help needed |

---

## Using Labels Effectively

1. **Always add a Priority** - critical, high, medium, or low
2. **Always add a Type** - bug, enhancement, documentation, question, discussion
3. **Add Category** - compiler, backend, frontend, quantum, ml, performance, security
4. **Add Status** - in-progress, blocked, waiting-feedback, needs-review, ready-to-merge
5. **Add Effort** - good-first-issue, easy, medium-effort, hard

**Example Issue:**
- Priority: `high`
- Type: `bug`
- Category: `compiler`
- Status: `in-progress`
- Effort: `medium-effort`

---

## Automation

You can use GitHub Actions to automatically add labels based on:
- File changes (e.g., `compiler/` changes → add `compiler` label)
- Keywords in title (e.g., "performance" → add `performance` label)
- Author (e.g., first-time contributor → add `good-first-issue`)

See `.github/workflows/auto-label.yml` for automation setup.
