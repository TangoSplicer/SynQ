# Codecov Integration Setup

This document describes the Codecov integration for the SynQ project.

## Overview

Codecov is integrated to track code coverage across all components:
- **Backend** (Python/FastAPI)
- **Frontend** (React/TypeScript)
- **Compiler** (C++)

## Configuration

### codecov.yml

The `codecov.yml` file configures Codecov behavior:

```yaml
coverage:
  precision: 2
  range: "70...100"

flags:
  backend:
    paths: [backend]
  frontend:
    paths: [frontend]
  compiler:
    paths: [compiler]

status:
  project:
    default:
      target: 85
      threshold: 5%
```

**Key Settings:**
- **Target Coverage:** 85%
- **Threshold:** 5% (allows up to 5% drop)
- **Precision:** 2 decimal places
- **Flags:** Separate tracking for each component

## GitHub Integration

### Codecov GitHub App

1. **Install Codecov App**
   - Go to: https://github.com/apps/codecov
   - Click "Install"
   - Select the TangoSplicer organization
   - Authorize for the SynQ repository

2. **Automatic Checks**
   - Codecov automatically comments on PRs
   - Shows coverage change
   - Blocks merge if coverage drops below threshold

### Status Checks

Codecov provides three status checks:

1. **Project Coverage**
   - Overall coverage across entire project
   - Target: 85%
   - Threshold: 5%

2. **Patch Coverage**
   - Coverage of changed files only
   - Target: 80%
   - Threshold: 5%

3. **Changes Coverage**
   - Coverage of new/modified code
   - Target: 80%
   - Threshold: 5%

## CI/CD Integration

### Compiler CI/CD

```yaml
- name: Upload coverage to Codecov
  uses: codecov/codecov-action@v3
  with:
    files: ./compiler/build/coverage.xml
    flags: compiler
    name: compiler-coverage
  continue-on-error: true
```

### Backend CI/CD

```yaml
- name: Upload coverage to Codecov
  uses: codecov/codecov-action@v3
  with:
    files: ./backend/coverage/coverage-final.json
    flags: backend
    name: backend-coverage
  continue-on-error: true
```

### Frontend CI/CD

```yaml
- name: Upload coverage to Codecov
  uses: codecov/codecov-action@v3
  with:
    files: ./frontend/coverage/coverage-final.json
    flags: frontend
    name: frontend-coverage
  continue-on-error: true
```

## Coverage Badges

### README Badges

Add these badges to README.md:

```markdown
[![codecov](https://codecov.io/gh/TangoSplicer/SynQ/branch/main/graph/badge.svg)](https://codecov.io/gh/TangoSplicer/SynQ)
[![Backend Coverage](https://codecov.io/gh/TangoSplicer/SynQ/branch/main/graph/badge.svg?flag=backend)](https://codecov.io/gh/TangoSplicer/SynQ)
[![Frontend Coverage](https://codecov.io/gh/TangoSplicer/SynQ/branch/main/graph/badge.svg?flag=frontend)](https://codecov.io/gh/TangoSplicer/SynQ)
[![Compiler Coverage](https://codecov.io/gh/TangoSplicer/SynQ/branch/main/graph/badge.svg?flag=compiler)](https://codecov.io/gh/TangoSplicer/SynQ)
```

## Viewing Coverage

### Codecov Dashboard

1. **Visit Codecov**
   - Go to: https://codecov.io/gh/TangoSplicer/SynQ

2. **View Coverage**
   - Overall coverage percentage
   - Coverage by file
   - Coverage trends over time
   - Coverage by component (flags)

### PR Comments

Codecov automatically comments on pull requests with:
- Overall coverage change
- File-by-file coverage
- Coverage comparison to base branch
- Suggestions for improving coverage

## Coverage Targets

### Current Coverage

| Component | Current | Target | Status |
|-----------|---------|--------|--------|
| Backend | 78% | 85% | 🟡 7% gap |
| Frontend | 72% | 85% | 🟡 13% gap |
| Compiler | 82% | 85% | 🟢 On track |
| Overall | 77% | 85% | 🟡 8% gap |

### Improvement Plan

1. **Phase 1 (Week 1):** Add 100+ tests to backend and frontend
2. **Phase 2 (Week 2):** Improve compiler coverage to 90%
3. **Phase 3 (Week 3):** Reach 85% overall coverage
4. **Phase 4 (Week 4):** Maintain and improve coverage

## Troubleshooting

### Coverage Not Showing

**Issue:** Codecov not showing coverage reports

**Solutions:**
1. Ensure `codecov/codecov-action@v3` is in CI/CD workflow
2. Verify coverage files are being generated
3. Check Codecov app is installed and authorized
4. Verify repository is public or Codecov token is configured

### Coverage Dropped

**Issue:** Coverage dropped below threshold

**Solutions:**
1. Review PR changes and add tests
2. Check if new code is covered
3. Update codecov.yml threshold if necessary
4. Merge and re-run CI/CD

### Token Issues

**Issue:** Codecov token not recognized

**Solutions:**
1. For public repos: Token not required
2. For private repos: Add `CODECOV_TOKEN` secret to GitHub
3. Verify token has correct permissions
4. Check token expiration

## Best Practices

1. **Always Run Tests Locally**
   ```bash
   pytest --cov=src --cov-report=html  # Backend
   npm run test -- --coverage          # Frontend
   ctest --verbose                     # Compiler
   ```

2. **Review Coverage Before Committing**
   - Check which files have low coverage
   - Add tests for critical paths
   - Aim for 80%+ coverage per file

3. **Monitor Coverage Trends**
   - Check Codecov dashboard weekly
   - Review coverage by component
   - Identify files needing improvement

4. **Set Coverage Goals**
   - Critical code: 90%+
   - Important code: 80%+
   - General code: 70%+

## Resources

- [Codecov Documentation](https://docs.codecov.io/)
- [Codecov GitHub Action](https://github.com/codecov/codecov-action)
- [Coverage.py Documentation](https://coverage.readthedocs.io/)
- [Jest Coverage Documentation](https://jestjs.io/docs/coverage)

---

**Last Updated:** March 2026
**Maintained By:** SynQ Development Team
