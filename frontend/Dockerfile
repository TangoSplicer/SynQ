# Multi-stage build for SynQ Frontend

# Stage 1: Builder
FROM node:22-alpine as builder

WORKDIR /build

# Copy package files
COPY package.json pnpm-lock.yaml ./

# Install dependencies
RUN npm install -g pnpm && \
    pnpm install --frozen-lockfile

# Copy source code
COPY client ./client

# Build application
RUN cd client && pnpm build

# Stage 2: Runtime
FROM nginx:alpine

# Copy nginx configuration
COPY nginx.conf /etc/nginx/nginx.conf

# Copy built application
COPY --from=builder /build/client/dist /usr/share/nginx/html

# Create non-root user
RUN addgroup -g 1000 nginx && \
    adduser -D -u 1000 -G nginx nginx && \
    chown -R nginx:nginx /usr/share/nginx/html

USER nginx

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
    CMD wget --quiet --tries=1 --spider http://localhost:80/ || exit 1

# Expose port
EXPOSE 80

# Run nginx
CMD ["nginx", "-g", "daemon off;"]
