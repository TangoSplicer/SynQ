# GitHub Project Board Setup

This document provides instructions for setting up an automated GitHub Project Board for SynQ.

---

## Project Board Overview

The SynQ project board uses automated workflows to manage issues and pull requests through different stages of development.

**Board Name:** SynQ Development  
**Type:** Table (for better organization)  
**Automation:** Enabled for all columns

---

## Project Board Columns

### 1. **Backlog**
- **Status:** Not started
- **Automation:** Auto-add new issues
- **Purpose:** All new issues start here
- **Action:** Triage and prioritize

### 2. **Ready**
- **Status:** Prioritized and ready
- **Automation:** Manual move from Backlog
- **Purpose:** Issues ready to be worked on
- **Action:** Assign to developers

### 3. **In Progress**
- **Status:** Currently being worked on
- **Automation:** Auto-move when PR created
- **Purpose:** Track active work
- **Action:** Monitor progress

### 4. **In Review**
- **Status:** Code review in progress
- **Automation:** Auto-move when PR marked for review
- **Purpose:** Track code reviews
- **Action:** Review and approve

### 5. **Done**
- **Status:** Completed
- **Automation:** Auto-move when PR merged
- **Purpose:** Track completed work
- **Action:** Celebrate wins!

---

## How to Create the Project Board

### Step 1: Create New Project
1. Go to your repository
2. Click **Projects** tab
3. Click **New project**
4. Name: `SynQ Development`
5. Template: `Table`
6. Click **Create project**

### Step 2: Create Columns
1. Click **+ Add column**
2. Add columns in this order:
   - Backlog
   - Ready
   - In Progress
   - In Review
   - Done

### Step 3: Configure Automation
For each column:
1. Click **...** (three dots)
2. Click **Manage automation**
3. Set up rules:

**Backlog:**
- Auto-add: All new issues
- Auto-remove: When moved to another column

**Ready:**
- Manual management

**In Progress:**
- Auto-add: When assigned
- Auto-remove: When PR created

**In Review:**
- Auto-add: When PR created
- Auto-remove: When PR merged

**Done:**
- Auto-add: When PR merged
- Auto-remove: Never

### Step 4: Add Issues
1. Click **+ Add item**
2. Search for existing issues
3. Or create new issues directly in the board

---

## Workflow Example

1. **Issue Created** → Auto-added to **Backlog**
2. **Issue Triaged** → Manually moved to **Ready**
3. **Developer Assigned** → Manually moved to **In Progress**
4. **PR Created** → Auto-moved to **In Review**
5. **PR Merged** → Auto-moved to **Done**

---

## Project Board Views

You can create multiple views of the same project:

### View 1: By Priority
- Filter: Priority (critical, high, medium, low)
- Sort: Priority descending

### View 2: By Category
- Filter: Category (compiler, backend, frontend, quantum, ml)
- Sort: Category

### View 3: By Effort
- Filter: Effort (easy, medium-effort, hard)
- Sort: Effort ascending

### View 4: By Assignee
- Group by: Assignee
- Sort: Assignee

---

## Tips for Effective Project Management

1. **Triage Weekly** - Review backlog and prioritize
2. **Limit WIP** - Don't have too many items in "In Progress"
3. **Celebrate Wins** - Highlight completed items
4. **Track Velocity** - Monitor how many items move to "Done" each week
5. **Review Regularly** - Check project board daily

---

## Metrics to Track

- **Backlog Size** - How many items waiting?
- **Cycle Time** - How long from "Ready" to "Done"?
- **Velocity** - How many items completed per week?
- **Blocked Items** - How many are blocked?

---

## Integration with GitHub

The project board integrates with:
- ✅ Issues - Auto-sync with issue status
- ✅ Pull Requests - Auto-sync with PR status
- ✅ Milestones - Group by milestone
- ✅ Labels - Filter by labels
- ✅ Assignees - Assign to team members

---

## Automation Rules (Advanced)

You can set up GitHub Actions to automatically manage the project board:

```yaml
name: Auto-manage Project Board

on:
  issues:
    types: [opened, closed]
  pull_request:
    types: [opened, closed, synchronize]

jobs:
  manage-board:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/github-script@v6
        with:
          script: |
            // Auto-add new issues to project board
            // Auto-move issues based on status
            // Auto-close stale issues
```

---

## Next Steps

1. Create the project board (15 minutes)
2. Add existing issues to the board (30 minutes)
3. Set up automation rules (15 minutes)
4. Start using the board for all new work (ongoing)

---

## Resources

- [GitHub Project Board Docs](https://docs.github.com/en/issues/planning-and-tracking-with-projects)
- [GitHub Automation](https://docs.github.com/en/issues/planning-and-tracking-with-projects/automating-your-project)
- [Best Practices](https://github.blog/2021-04-05-github-issues-projects-a-fresh-take-on-projects/)
