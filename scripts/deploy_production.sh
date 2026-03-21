#!/bin/bash
# Production deployment script for SynQ platform

set -e

# Configuration
VERSION=${1:-latest}
NAMESPACE="synq-prod"
REGISTRY="docker.io/synq"
TIMEOUT=600

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Functions
log_info() {
    echo -e "${GREEN}✓${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}⚠${NC} $1"
}

log_error() {
    echo -e "${RED}✗${NC} $1"
}

# Pre-deployment checks
pre_deployment_checks() {
    log_info "Running pre-deployment checks..."
    
    # Check kubectl
    if ! command -v kubectl &> /dev/null; then
        log_error "kubectl not found"
        exit 1
    fi
    
    # Check cluster connection
    if ! kubectl cluster-info &> /dev/null; then
        log_error "Cannot connect to Kubernetes cluster"
        exit 1
    fi
    
    # Check namespace exists
    if ! kubectl get namespace $NAMESPACE &> /dev/null; then
        log_warn "Namespace $NAMESPACE does not exist, creating..."
        kubectl create namespace $NAMESPACE
    fi
    
    log_info "Pre-deployment checks passed"
}

# Build and push images
build_images() {
    log_info "Building Docker images..."
    
    docker build -t $REGISTRY/backend:$VERSION backend/
    docker build -t $REGISTRY/frontend:$VERSION frontend/
    docker build -t $REGISTRY/compiler:$VERSION compiler/
    
    log_info "Pushing images to registry..."
    docker push $REGISTRY/backend:$VERSION
    docker push $REGISTRY/frontend:$VERSION
    docker push $REGISTRY/compiler:$VERSION
    
    log_info "Images built and pushed successfully"
}

# Update manifests
update_manifests() {
    log_info "Updating Kubernetes manifests..."
    
    # Update image tags
    sed -i "s|image: $REGISTRY/backend:.*|image: $REGISTRY/backend:$VERSION|g" k8s/*.yaml
    sed -i "s|image: $REGISTRY/frontend:.*|image: $REGISTRY/frontend:$VERSION|g" k8s/*.yaml
    sed -i "s|image: $REGISTRY/compiler:.*|image: $REGISTRY/compiler:$VERSION|g" k8s/*.yaml
    
    log_info "Manifests updated"
}

# Deploy to Kubernetes
deploy_kubernetes() {
    log_info "Deploying to Kubernetes..."
    
    kubectl apply -f k8s/ -n $NAMESPACE
    
    log_info "Waiting for rollout..."
    kubectl rollout status deployment/backend -n $NAMESPACE --timeout=${TIMEOUT}s
    kubectl rollout status deployment/frontend -n $NAMESPACE --timeout=${TIMEOUT}s
    
    log_info "Deployment successful"
}

# Smoke tests
smoke_tests() {
    log_info "Running smoke tests..."
    
    # Get ingress IP
    INGRESS_IP=$(kubectl get ingress -n $NAMESPACE -o jsonpath='{.items[0].status.loadBalancer.ingress[0].ip}')
    
    if [ -z "$INGRESS_IP" ]; then
        log_warn "Ingress IP not available yet"
        return
    fi
    
    # Test backend health
    if curl -f http://$INGRESS_IP/health &> /dev/null; then
        log_info "Backend health check passed"
    else
        log_error "Backend health check failed"
        return 1
    fi
    
    # Test frontend
    if curl -f http://$INGRESS_IP/ &> /dev/null; then
        log_info "Frontend health check passed"
    else
        log_error "Frontend health check failed"
        return 1
    fi
    
    log_info "Smoke tests passed"
}

# Verify deployment
verify_deployment() {
    log_info "Verifying deployment..."
    
    # Check pod status
    RUNNING=$(kubectl get pods -n $NAMESPACE --field-selector=status.phase=Running --no-headers | wc -l)
    TOTAL=$(kubectl get pods -n $NAMESPACE --no-headers | wc -l)
    
    log_info "Pods running: $RUNNING/$TOTAL"
    
    if [ $RUNNING -ne $TOTAL ]; then
        log_warn "Not all pods are running"
        kubectl get pods -n $NAMESPACE
    fi
    
    # Check services
    kubectl get services -n $NAMESPACE
    
    # Check ingress
    kubectl get ingress -n $NAMESPACE
}

# Rollback if needed
rollback() {
    log_warn "Rolling back deployment..."
    
    kubectl rollout undo deployment/backend -n $NAMESPACE
    kubectl rollout undo deployment/frontend -n $NAMESPACE
    
    kubectl rollout status deployment/backend -n $NAMESPACE --timeout=${TIMEOUT}s
    kubectl rollout status deployment/frontend -n $NAMESPACE --timeout=${TIMEOUT}s
    
    log_info "Rollback complete"
}

# Main deployment flow
main() {
    log_info "Starting SynQ production deployment (v$VERSION)"
    
    # Run checks
    pre_deployment_checks
    
    # Build and deploy
    build_images
    update_manifests
    deploy_kubernetes
    
    # Verify
    verify_deployment
    smoke_tests
    
    log_info "Deployment complete!"
    log_info "Access the application at: https://synq.dev"
}

# Error handling
trap 'log_error "Deployment failed"; rollback; exit 1' ERR

# Run main
main
