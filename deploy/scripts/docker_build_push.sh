#!/bin/bash
# ───────────────────────────────────────────────────────────
# Docker Build + Push Script for SynQ Apps
# Builds a Docker image from scaffold and pushes to a registry
# ───────────────────────────────────────────────────────────

APP_NAME="synq-app"
VERSION="v1.0.0"
IMAGE_NAME="yourdockeruser/$APP_NAME:$VERSION"
DOCKERFILE_PATH="../Dockerfile"

echo "[1/4] Building Docker image: $IMAGE_NAME"
docker build -f $DOCKERFILE_PATH -t $IMAGE_NAME ..

echo "[2/4] Verifying Docker image..."
docker images | grep $APP_NAME

echo "[3/4] Logging into DockerHub (or private registry)"
docker login

echo "[4/4] Pushing image to registry..."
docker push $IMAGE_NAME

echo "[✓] Docker image pushed: $IMAGE_NAME"