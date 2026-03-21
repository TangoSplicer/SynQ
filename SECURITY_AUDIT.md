# SynQ Security Audit & Hardening Guide

## Executive Summary

This document provides a comprehensive security audit framework and hardening guide for the SynQ platform. It covers vulnerability assessment, remediation, compliance, and best practices across all components.

## Security Assessment Framework

### 1. Vulnerability Scanning

#### Dependency Scanning

**Backend (Python):**

```bash
# Check for known vulnerabilities
pip install safety
safety check

# Or use pip-audit
pip install pip-audit
pip-audit

# Check for outdated packages
pip list --outdated
```

**Frontend (JavaScript):**

```bash
# Check npm dependencies
npm audit

# Fix vulnerabilities
npm audit fix

# Check for outdated packages
npm outdated
```

**Compiler (C++):**

```bash
# Check for known vulnerabilities in dependencies
# Use OWASP Dependency-Check
dependency-check --project "SynQ" --scan .

# Or Snyk
snyk test
```

#### Static Code Analysis

**Backend:**

```bash
# Bandit (security linter for Python)
pip install bandit
bandit -r backend/

# Semgrep (pattern-based scanning)
pip install semgrep
semgrep --config=p/security-audit backend/

# Pylint with security plugins
pip install pylint
pylint --load-plugins=pylint_django backend/
```

**Frontend:**

```bash
# ESLint with security plugin
npm install --save-dev eslint-plugin-security
npx eslint --ext .ts,.tsx frontend/

# SonarQube for comprehensive analysis
docker run -d --name sonarqube -p 9000:9000 sonarqube:latest
```

**Compiler:**

```bash
# Clang Static Analyzer
scan-build cmake --build .

# Cppcheck
cppcheck --enable=all compiler/src/

# AddressSanitizer (runtime)
cmake -DCMAKE_CXX_FLAGS="-fsanitize=address" ..
```

### 2. Authentication & Authorization

#### Backend API Security

**JWT Token Validation:**

```python
from functools import wraps
from flask import request, jsonify
import jwt

def require_auth(f):
    @wraps(f)
    def decorated(*args, **kwargs):
        token = request.headers.get('Authorization', '').replace('Bearer ', '')
        
        if not token:
            return jsonify({'error': 'Missing token'}), 401
        
        try:
            payload = jwt.decode(token, 'SECRET_KEY', algorithms=['HS256'])
            request.user_id = payload['user_id']
        except jwt.InvalidTokenError:
            return jsonify({'error': 'Invalid token'}), 401
        
        return f(*args, **kwargs)
    return decorated

@app.route('/api/protected')
@require_auth
def protected_route():
    return {'user_id': request.user_id}
```

**Rate Limiting:**

```python
from flask_limiter import Limiter
from flask_limiter.util import get_remote_address

limiter = Limiter(
    app,
    key_func=get_remote_address,
    default_limits=["200 per day", "50 per hour"]
)

@app.route('/api/login', methods=['POST'])
@limiter.limit("5 per minute")
def login():
    # Login logic
    pass
```

**CORS Configuration:**

```python
from flask_cors import CORS

CORS(app, resources={
    r"/api/*": {
        "origins": ["https://synq.dev"],
        "methods": ["GET", "POST", "PUT", "DELETE"],
        "allow_headers": ["Content-Type", "Authorization"],
        "max_age": 3600
    }
})
```

#### Frontend Security

**Secure Storage:**

```typescript
// ❌ Bad: Store token in localStorage
localStorage.setItem('token', token);

// ✅ Good: Store in httpOnly cookie
document.cookie = `token=${token}; HttpOnly; Secure; SameSite=Strict`;

// ✅ Better: Use secure session storage
sessionStorage.setItem('token', token);
```

**XSS Prevention:**

```typescript
// ❌ Bad: Direct HTML injection
element.innerHTML = userInput;

// ✅ Good: Use React (auto-escapes)
<div>{userInput}</div>

// ✅ Good: Manual escaping
function escapeHtml(text: string): string {
  const map: {[key: string]: string} = {
    '&': '&amp;',
    '<': '&lt;',
    '>': '&gt;',
    '"': '&quot;',
    "'": '&#039;'
  };
  return text.replace(/[&<>"']/g, m => map[m]);
}
```

### 3. Data Protection

#### Encryption

**At Rest:**

```python
from cryptography.fernet import Fernet

key = Fernet.generate_key()
cipher = Fernet(key)

# Encrypt sensitive data
encrypted = cipher.encrypt(b"sensitive_data")

# Decrypt when needed
decrypted = cipher.decrypt(encrypted)
```

**In Transit:**

```python
# Force HTTPS
@app.before_request
def enforce_https():
    if not request.is_secure and not app.debug:
        return redirect(request.url.replace('http://', 'https://'), code=301)
```

**Database:**

```python
class User(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    email = db.Column(db.String(255), unique=True, index=True)
    password = db.Column(db.String(255))  # Hashed with bcrypt
    api_key = db.Column(db.String(255))   # Encrypted
    
    def set_password(self, password: str):
        import bcrypt
        self.password = bcrypt.hashpw(
            password.encode('utf-8'),
            bcrypt.gensalt(rounds=12)
        )
    
    def check_password(self, password: str) -> bool:
        import bcrypt
        return bcrypt.checkpw(
            password.encode('utf-8'),
            self.password
        )
```

#### Secrets Management

**Environment Variables:**

```bash
# .env (NEVER commit to git)
DATABASE_URL=postgresql://user:pass@localhost/synq
JWT_SECRET=your-secret-key-here
API_KEY=your-api-key-here

# .env.example (safe to commit)
DATABASE_URL=postgresql://user:pass@localhost/synq
JWT_SECRET=your-secret-key-here
API_KEY=your-api-key-here
```

**GitHub Secrets:**

```yaml
# .github/workflows/deploy.yml
env:
  DATABASE_URL: ${{ secrets.DATABASE_URL }}
  JWT_SECRET: ${{ secrets.JWT_SECRET }}
  API_KEY: ${{ secrets.API_KEY }}
```

### 4. Input Validation & Sanitization

**Backend:**

```python
from pydantic import BaseModel, validator

class CircuitCreate(BaseModel):
    name: str
    qubits: int
    
    @validator('name')
    def name_must_not_be_empty(cls, v):
        if not v or len(v.strip()) == 0:
            raise ValueError('Name cannot be empty')
        if len(v) > 255:
            raise ValueError('Name too long')
        return v
    
    @validator('qubits')
    def qubits_must_be_positive(cls, v):
        if v < 1 or v > 100:
            raise ValueError('Qubits must be between 1 and 100')
        return v
```

**Frontend:**

```typescript
// Input validation
function validateEmail(email: string): boolean {
  const regex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
  return regex.test(email);
}

function validatePassword(password: string): {
  valid: boolean;
  errors: string[];
} {
  const errors: string[] = [];
  
  if (password.length < 12) errors.push('Min 12 characters');
  if (!/[A-Z]/.test(password)) errors.push('Need uppercase');
  if (!/[0-9]/.test(password)) errors.push('Need number');
  if (!/[!@#$%^&*]/.test(password)) errors.push('Need special char');
  
  return {
    valid: errors.length === 0,
    errors
  };
}
```

### 5. API Security

**API Key Management:**

```python
import secrets

def generate_api_key() -> str:
    """Generate a cryptographically secure API key."""
    return secrets.token_urlsafe(32)

@app.route('/api/keys', methods=['POST'])
@require_auth
def create_api_key():
    key = generate_api_key()
    
    # Hash before storing
    import hashlib
    hashed = hashlib.sha256(key.encode()).hexdigest()
    
    db.session.add(APIKey(
        user_id=request.user_id,
        key_hash=hashed
    ))
    db.session.commit()
    
    return {'key': key}  # Only return once
```

**Request Signing:**

```python
import hmac
import hashlib

def sign_request(data: dict, secret: str) -> str:
    """Sign request with HMAC."""
    message = json.dumps(data, sort_keys=True)
    signature = hmac.new(
        secret.encode(),
        message.encode(),
        hashlib.sha256
    ).hexdigest()
    return signature

def verify_signature(data: dict, signature: str, secret: str) -> bool:
    """Verify request signature."""
    expected = sign_request(data, secret)
    return hmac.compare_digest(signature, expected)
```

### 6. Logging & Monitoring

**Security Logging:**

```python
import logging

security_logger = logging.getLogger('security')

@app.before_request
def log_request():
    security_logger.info(
        f"Request: {request.method} {request.path} "
        f"from {request.remote_addr} "
        f"User: {getattr(request, 'user_id', 'anonymous')}"
    )

@app.route('/api/login', methods=['POST'])
def login():
    email = request.json.get('email')
    
    try:
        user = User.query.filter_by(email=email).first()
        if not user or not user.check_password(request.json.get('password')):
            security_logger.warning(f"Failed login attempt for {email}")
            return {'error': 'Invalid credentials'}, 401
        
        security_logger.info(f"Successful login for {email}")
        return {'token': generate_token(user)}
    except Exception as e:
        security_logger.error(f"Login error: {e}")
        return {'error': 'Internal error'}, 500
```

**Alert Rules:**

```yaml
# Prometheus alerts
- alert: HighFailedLoginAttempts
  expr: rate(failed_logins_total[5m]) > 10
  for: 5m
  annotations:
    summary: "High failed login attempts detected"

- alert: SuspiciousAPIActivity
  expr: rate(api_errors_total[5m]) > 100
  for: 5m
  annotations:
    summary: "Suspicious API activity detected"
```

## Security Checklist

### Pre-Deployment

- [ ] All dependencies scanned for vulnerabilities
- [ ] Static code analysis completed (no critical issues)
- [ ] SAST/DAST testing completed
- [ ] Secrets not committed to repository
- [ ] All API endpoints authenticated
- [ ] Rate limiting configured
- [ ] CORS properly configured
- [ ] HTTPS enforced
- [ ] Input validation implemented
- [ ] Output encoding implemented
- [ ] Logging configured
- [ ] Monitoring alerts configured

### Post-Deployment

- [ ] Security headers configured (CSP, X-Frame-Options, etc.)
- [ ] WAF rules configured
- [ ] DDoS protection enabled
- [ ] Regular security audits scheduled
- [ ] Incident response plan documented
- [ ] Backup and recovery tested
- [ ] Access control reviewed
- [ ] Audit logs retained

## Compliance

### OWASP Top 10

| Vulnerability | Status | Mitigation |
|---------------|--------|-----------|
| Injection | ✅ | Parameterized queries, input validation |
| Broken Auth | ✅ | JWT, rate limiting, MFA |
| Sensitive Data | ✅ | Encryption, HTTPS, secrets management |
| XML External | ✅ | Disable external entities |
| Broken Access | ✅ | Role-based access control |
| Security Misc | ✅ | Security headers, CSP |
| Injection | ✅ | Input validation, escaping |
| Insecure Deserialization | ✅ | Avoid untrusted serialization |
| Using Components | ✅ | Dependency scanning |
| Insufficient Logging | ✅ | Comprehensive logging |

### CWE Coverage

- CWE-89 (SQL Injection): ✅ Parameterized queries
- CWE-79 (XSS): ✅ Output encoding
- CWE-352 (CSRF): ✅ CSRF tokens
- CWE-434 (File Upload): ✅ File type validation
- CWE-614 (Sensitive Cookie): ✅ HttpOnly, Secure, SameSite

## Incident Response

### Incident Classification

| Severity | Response Time | Actions |
|----------|---------------|---------|
| Critical | 15 minutes | Immediate escalation, incident commander |
| High | 1 hour | Investigation, containment |
| Medium | 4 hours | Assessment, remediation plan |
| Low | 24 hours | Documentation, follow-up |

### Response Procedure

1. **Detection** - Alert triggered
2. **Triage** - Assess severity
3. **Containment** - Limit damage
4. **Investigation** - Root cause analysis
5. **Remediation** - Fix vulnerability
6. **Recovery** - Restore normal operations
7. **Post-Mortem** - Document lessons learned

## Resources

- [OWASP Top 10](https://owasp.org/www-project-top-ten/)
- [CWE/SANS Top 25](https://cwe.mitre.org/top25/)
- [NIST Cybersecurity Framework](https://www.nist.gov/cyberframework/)
- [Security Headers](https://securityheaders.com/)

---

**Last Updated:** March 2026
**Maintained By:** SynQ Security Team
