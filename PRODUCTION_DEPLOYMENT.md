# SynQ Production Deployment Guide

## Pre-Deployment Checklist

### Code Quality
- [ ] All tests passing (85%+ coverage)
- [ ] No critical security issues
- [ ] Code reviewed and approved
- [ ] Documentation updated
- [ ] Performance benchmarks met
- [ ] All CI/CD pipelines green

### Infrastructure
- [ ] Kubernetes cluster provisioned
- [ ] Database backups configured
- [ ] Load balancers configured
- [ ] CDN configured
- [ ] Monitoring and logging set up
- [ ] Alerting configured

### Security
- [ ] SSL/TLS certificates installed
- [ ] Secrets configured in production
- [ ] Firewall rules configured
- [ ] DDoS protection enabled
- [ ] WAF rules configured
- [ ] Incident response plan documented

### Operations
- [ ] Runbooks created
- [ ] On-call schedule established
- [ ] Escalation procedures defined
- [ ] Rollback procedures tested
- [ ] Backup and recovery tested
- [ ] Disaster recovery plan documented

## Deployment Architecture

```
┌─────────────────────────────────────────────────────┐
│                    CDN (CloudFlare)                 │
└──────────────────────┬──────────────────────────────┘
                       │
┌──────────────────────┴──────────────────────────────┐
│              Load Balancer (HTTPS)                  │
└──────────────────────┬──────────────────────────────┘
                       │
        ┌──────────────┼──────────────┐
        │              │              │
   ┌────▼────┐   ┌────▼────┐   ┌────▼────┐
   │Frontend  │   │ Backend  │   │Compiler │
   │Pod (x2)  │   │Pod (x3)  │   │Pod (x2) │
   └────┬─────┘   └────┬─────┘   └────┬────┘
        │              │              │
        └──────────────┼──────────────┘
                       │
        ┌──────────────┴──────────────┐
        │                             │
   ┌────▼────────┐          ┌────────▼────┐
   │ PostgreSQL  │          │   Redis     │
   │ (Primary)   │          │  (Cache)    │
   └────┬────────┘          └─────────────┘
        │
   ┌────▼────────┐
   │ PostgreSQL  │
   │ (Replica)   │
   └─────────────┘
```

## Kubernetes Deployment

### 1. Create Namespaces

```bash
kubectl create namespace synq-prod
kubectl create namespace synq-monitoring
```

### 2. Configure Secrets

```bash
# Database credentials
kubectl create secret generic db-credentials \
  --from-literal=username=synq_user \
  --from-literal=password=<strong-password> \
  -n synq-prod

# API keys
kubectl create secret generic api-keys \
  --from-literal=jwt_secret=<jwt-secret> \
  --from-literal=api_key=<api-key> \
  -n synq-prod

# Docker registry
kubectl create secret docker-registry regcred \
  --docker-server=docker.io \
  --docker-username=<username> \
  --docker-password=<password> \
  -n synq-prod
```

### 3. Deploy Services

```bash
# Apply manifests
kubectl apply -f k8s/namespace.yaml
kubectl apply -f k8s/configmap.yaml
kubectl apply -f k8s/secrets.yaml
kubectl apply -f k8s/backend-deployment.yaml
kubectl apply -f k8s/frontend-deployment.yaml
kubectl apply -f k8s/compiler-deployment.yaml
kubectl apply -f k8s/database-statefulset.yaml
kubectl apply -f k8s/redis-deployment.yaml
kubectl apply -f k8s/ingress.yaml
kubectl apply -f k8s/service.yaml

# Verify deployment
kubectl get pods -n synq-prod
kubectl get services -n synq-prod
kubectl get ingress -n synq-prod
```

### 4. Configure Auto-Scaling

```yaml
apiVersion: autoscaling/v2
kind: HorizontalPodAutoscaler
metadata:
  name: backend-hpa
  namespace: synq-prod
spec:
  scaleTargetRef:
    apiVersion: apps/v1
    kind: Deployment
    name: backend
  minReplicas: 3
  maxReplicas: 10
  metrics:
  - type: Resource
    resource:
      name: cpu
      target:
        type: Utilization
        averageUtilization: 70
  - type: Resource
    resource:
      name: memory
      target:
        type: Utilization
        averageUtilization: 80
```

## Database Setup

### 1. PostgreSQL Configuration

```sql
-- Create database
CREATE DATABASE synq_prod;

-- Create user
CREATE USER synq_user WITH PASSWORD '<strong-password>';

-- Grant privileges
GRANT ALL PRIVILEGES ON DATABASE synq_prod TO synq_user;

-- Create extensions
\c synq_prod
CREATE EXTENSION IF NOT EXISTS "uuid-ossp";
CREATE EXTENSION IF NOT EXISTS "pg_trgm";

-- Run migrations
python manage.py migrate
```

### 2. Backup Strategy

```bash
# Daily backups at 2 AM UTC
0 2 * * * pg_dump synq_prod | gzip > /backups/synq_$(date +\%Y\%m\%d).sql.gz

# Upload to S3
0 3 * * * aws s3 cp /backups/synq_*.sql.gz s3://synq-backups/

# Retention: 30 days
find /backups -name "synq_*.sql.gz" -mtime +30 -delete
```

### 3. Replication Setup

```bash
# On primary
pg_basebackup -h primary.synq.internal -D /var/lib/postgresql/data -U replication -v -P -W

# On replica
recovery.conf:
standby_mode = 'on'
primary_conninfo = 'host=primary.synq.internal user=replication password=<password>'
```

## Monitoring & Logging

### 1. Prometheus Configuration

```yaml
global:
  scrape_interval: 15s
  evaluation_interval: 15s

scrape_configs:
  - job_name: 'kubernetes-pods'
    kubernetes_sd_configs:
      - role: pod
    relabel_configs:
      - source_labels: [__meta_kubernetes_pod_annotation_prometheus_io_scrape]
        action: keep
        regex: true
```

### 2. Grafana Dashboards

```bash
# Import dashboards
kubectl apply -f k8s/grafana-dashboards.yaml

# Access Grafana
kubectl port-forward -n synq-monitoring svc/grafana 3000:80
# Visit http://localhost:3000
```

### 3. ELK Stack (Elasticsearch, Logstash, Kibana)

```yaml
apiVersion: v1
kind: ConfigMap
metadata:
  name: logstash-config
  namespace: synq-monitoring
data:
  logstash.conf: |
    input {
      kubernetes {
        namespace => "synq-prod"
      }
    }
    filter {
      mutate {
        add_field => { "[@metadata][index_name]" => "synq-logs-%{+YYYY.MM.dd}" }
      }
    }
    output {
      elasticsearch {
        hosts => ["elasticsearch:9200"]
        index => "%{[@metadata][index_name]}"
      }
    }
```

## SSL/TLS Configuration

### 1. Let's Encrypt Certificate

```bash
# Install cert-manager
kubectl apply -f https://github.com/cert-manager/cert-manager/releases/download/v1.12.0/cert-manager.yaml

# Create ClusterIssuer
kubectl apply -f - <<EOF
apiVersion: cert-manager.io/v1
kind: ClusterIssuer
metadata:
  name: letsencrypt-prod
spec:
  acme:
    server: https://acme-v02.api.letsencrypt.org/directory
    email: admin@synq.dev
    privateKeySecretRef:
      name: letsencrypt-prod
    solvers:
    - http01:
        ingress:
          class: nginx
EOF
```

### 2. Ingress with TLS

```yaml
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: synq-ingress
  namespace: synq-prod
  annotations:
    cert-manager.io/cluster-issuer: "letsencrypt-prod"
spec:
  ingressClassName: nginx
  tls:
  - hosts:
    - synq.dev
    - api.synq.dev
    secretName: synq-tls
  rules:
  - host: synq.dev
    http:
      paths:
      - path: /
        pathType: Prefix
        backend:
          service:
            name: frontend
            port:
              number: 80
  - host: api.synq.dev
    http:
      paths:
      - path: /
        pathType: Prefix
        backend:
          service:
            name: backend
            port:
              number: 8000
```

## Deployment Steps

### 1. Pre-Deployment

```bash
# Run all checks
./scripts/security_check.py
./scripts/benchmark.py
npm run test -- --coverage
pytest --cov=backend

# Tag release
git tag -a v1.0.0 -m "Production release v1.0.0"
git push origin v1.0.0
```

### 2. Build and Push Images

```bash
# Build images
docker build -t synq-backend:1.0.0 backend/
docker build -t synq-frontend:1.0.0 frontend/
docker build -t synq-compiler:1.0.0 compiler/

# Push to registry
docker push docker.io/synq/backend:1.0.0
docker push docker.io/synq/frontend:1.0.0
docker push docker.io/synq/compiler:1.0.0
```

### 3. Deploy to Kubernetes

```bash
# Update image tags in manifests
sed -i 's|image: synq-backend:latest|image: synq-backend:1.0.0|g' k8s/*.yaml

# Apply manifests
kubectl apply -f k8s/ -n synq-prod

# Wait for rollout
kubectl rollout status deployment/backend -n synq-prod
kubectl rollout status deployment/frontend -n synq-prod
```

### 4. Smoke Testing

```bash
# Check service health
curl https://api.synq.dev/health
curl https://synq.dev/

# Run integration tests
./scripts/integration_tests.sh

# Monitor logs
kubectl logs -f deployment/backend -n synq-prod
```

## Rollback Procedure

### If Issues Detected

```bash
# Immediate rollback
kubectl rollout undo deployment/backend -n synq-prod
kubectl rollout undo deployment/frontend -n synq-prod

# Verify rollback
kubectl rollout status deployment/backend -n synq-prod

# Check logs
kubectl logs deployment/backend -n synq-prod
```

## Post-Deployment

### 1. Verify Services

```bash
# Check all pods running
kubectl get pods -n synq-prod

# Check services
kubectl get svc -n synq-prod

# Check ingress
kubectl get ingress -n synq-prod

# Test endpoints
curl https://api.synq.dev/health
curl https://synq.dev/
```

### 2. Enable Monitoring

```bash
# Verify Prometheus scraping
kubectl logs -f prometheus-0 -n synq-monitoring

# Check Grafana dashboards
kubectl port-forward -n synq-monitoring svc/grafana 3000:80
```

### 3. Document Deployment

```bash
# Create deployment record
cat > deployments/v1.0.0.md <<EOF
# Deployment v1.0.0

## Date: $(date)
## Deployed by: $(whoami)

### Changes
- Feature 1
- Feature 2
- Bug fix 1

### Performance
- Response time: 450ms (p95)
- Throughput: 1000 req/sec
- Error rate: 0.05%

### Issues
- None

### Rollback: Not required
EOF
```

## Troubleshooting

### Pod Not Starting

```bash
# Check pod status
kubectl describe pod <pod-name> -n synq-prod

# Check logs
kubectl logs <pod-name> -n synq-prod

# Check events
kubectl get events -n synq-prod --sort-by='.lastTimestamp'
```

### High CPU/Memory

```bash
# Check resource usage
kubectl top pods -n synq-prod

# Check HPA status
kubectl get hpa -n synq-prod

# Scale manually if needed
kubectl scale deployment backend --replicas=5 -n synq-prod
```

### Database Connection Issues

```bash
# Test database connection
kubectl run -it --rm debug --image=postgres:15 --restart=Never -- \
  psql -h postgres.synq-prod.svc.cluster.local -U synq_user -d synq_prod

# Check database logs
kubectl logs -f statefulset/postgres -n synq-prod
```

## Success Metrics

- ✅ All services running and healthy
- ✅ Response time < 500ms (p95)
- ✅ Throughput > 1000 req/sec
- ✅ Error rate < 0.1%
- ✅ Uptime > 99.9%
- ✅ All monitoring alerts configured
- ✅ Backup and recovery tested

---

**Last Updated:** March 2026
**Maintained By:** SynQ Operations Team
