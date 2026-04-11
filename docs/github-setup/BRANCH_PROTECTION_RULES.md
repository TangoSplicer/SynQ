# GitHub Branch Protection Rules

This document provides instructions for setting up branch protection rules on the main branch.

---

## Why Branch Protection?

Branch protection rules ensure code quality by:
- Requiring pull request reviews before merging
- Running automated tests before merge
- Preventing force pushes to main
- Maintaining a stable main branch
- Enforcing code standards

---

## How to Set Up Branch Protection

### Step 1: Access Settings

1. Go to your repository on GitHub
2. Click **Settings** (top right)
3. Click **Branches** (left sidebar)
4. Click **Add rule** under "Branch protection rules"

### Step 2: Configure Rule

**Branch name pattern:** `main`

### Step 3: Enable Protections

Check the following boxes:

#### ✅ Require a pull request before merging
- **Require approvals:** 1
- **Dismiss stale pull request approvals when new commits are pushed:** ✅
- **Require review from code owners:** ✅

#### ✅ Require status checks to pass before merging
- **Require branches to be up to date before merging:** ✅
- **Status checks that must pass:**
  - `build` (if using GitHub Actions)
  - `test` (if using GitHub Actions)
  - `lint` (if using GitHub Actions)

#### ✅ Require branches to be up to date before merging
- ✅ Checked

#### ✅ Require code reviews before merging
- **Number of approvals:** 1
- **Dismiss stale pull request approvals:** ✅
- **Require review from code owners:** ✅

#### ✅ Require conversation resolution before merging
- ✅ Checked

#### ✅ Require signed commits
- ✅ Checked (optional, but recommended)

#### ✅ Require linear history
- ✅ Checked (prevents merge commits)

#### ✅ Restrict who can push to matching branches
- ✅ Checked (only maintainers)

#### ✅ Allow force pushes
- ⭕ Do not allow force pushes

#### ✅ Allow deletions
- ⭕ Do not allow deletions

### Step 4: Save

Click **Create** to save the branch protection rule.

---

## Status Checks Configuration

If you're using GitHub Actions, configure these status checks:

### Compiler Tests
```yaml
name: compiler-tests
```

### Backend Tests
```yaml
name: backend-tests
```

### Frontend Tests
```yaml
name: frontend-tests
```

---

## CODEOWNERS File

Create a `.github/CODEOWNERS` file to specify who must review changes:

```
# Global owners
* @TangoSplicer

# Compiler owners
/compiler/ @TangoSplicer

# Backend owners
/backend/ @TangoSplicer

# Frontend owners
/frontend/ @TangoSplicer

# Documentation owners
*.md @TangoSplicer
/docs/ @TangoSplicer
```

---

## Pull Request Workflow

With branch protection enabled, here's the workflow:

1. **Create Branch**
   ```bash
   git checkout -b feature/my-feature
   ```

2. **Make Changes**
   ```bash
   git add .
   git commit -m "Add my feature"
   ```

3. **Push Branch**
   ```bash
   git push origin feature/my-feature
   ```

4. **Create Pull Request**
   - Go to GitHub
   - Click "Compare & pull request"
   - Fill in PR description
   - Click "Create pull request"

5. **Wait for Checks**
   - Status checks run automatically
   - Code review required (1 approval)
   - Conversations must be resolved

6. **Merge**
   - Once all checks pass and PR is approved
   - Click "Merge pull request"
   - Click "Confirm merge"
   - Delete branch (optional)

---

## Best Practices

1. **Keep main stable** - Never commit directly to main
2. **Use descriptive branch names** - `feature/add-quantum-gates`, `fix/compiler-bug`
3. **Write clear PR descriptions** - Explain what and why
4. **Request reviews** - Get feedback before merging
5. **Resolve conversations** - Address all feedback
6. **Keep commits clean** - Use meaningful commit messages
7. **Rebase before merge** - Keep history linear

---

## Troubleshooting

### PR won't merge - "Required status checks not passing"
- Wait for all checks to complete
- Fix any failing tests
- Push fixes to the same branch
- Checks will re-run automatically

### PR won't merge - "Requires code review"
- Request review from a maintainer
- Wait for approval
- Address any feedback
- Re-request review if needed

### PR won't merge - "Branch is out of date"
- Click "Update branch" button
- Or locally:
  ```bash
  git fetch origin
  git rebase origin/main
  git push --force-with-lease origin feature/my-feature
  ```

---

## Advanced Configuration

### Auto-merge
You can enable auto-merge for PRs:

1. Go to PR
2. Click "Auto-merge" dropdown
3. Select merge strategy:
   - Create a merge commit
   - Squash and merge
   - Rebase and merge

### Require dismissal of stale reviews
When new commits are pushed, old reviews are dismissed. Reviewers must re-approve.

### Require status checks from specific apps
You can require specific status checks (e.g., Codecov, SonarQube).

---

## Resources

- [GitHub Branch Protection Docs](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/managing-protected-branches/about-protected-branches)
- [CODEOWNERS Documentation](https://docs.github.com/en/repositories/managing-your-repositorys-settings-and-features/customizing-your-repository/about-code-owners)
- [GitHub Actions Documentation](https://docs.github.com/en/actions)

---

## Summary

| Setting | Value | Purpose |
|---------|-------|---------|
| Require PR reviews | 1 approval | Code quality |
| Require status checks | Compiler, Backend, Frontend | Automated testing |
| Require branches up to date | ✅ | Prevents merge conflicts |
| Require conversation resolution | ✅ | Ensures feedback addressed |
| Require signed commits | ✅ | Security |
| Require linear history | ✅ | Clean history |
| Allow force pushes | ❌ | Prevents accidents |
| Allow deletions | ❌ | Prevents accidents |

---

**Setup time:** 10-15 minutes  
**Ongoing benefit:** Stable, high-quality main branch
