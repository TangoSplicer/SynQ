"""Minimal FastAPI recovery surface for the SynQ backend prototype.

Only the root and health endpoints are implemented here. Historical router
modules, database wiring, GraphQL, WebSocket, ML, and collaboration features
remain unavailable until they are separately restored and tested.
"""

from os import getenv

from fastapi import FastAPI, status
from fastapi.middleware.cors import CORSMiddleware


APP_NAME = "SynQ Backend Recovery"
APP_VERSION = "0.1.0-recovery"
API_PREFIX = "/api/v1"

app = FastAPI(
    title=APP_NAME,
    version=APP_VERSION,
    description="Minimal health surface for the experimental SynQ backend recovery.",
)

allowed_origins = [origin.strip() for origin in getenv("CORS_ORIGINS", "*").split(",") if origin.strip()]
app.add_middleware(
    CORSMiddleware,
    allow_origins=allowed_origins,
    allow_credentials=allowed_origins != ["*"],
    allow_methods=["GET"],
    allow_headers=["Content-Type"],
)


@app.get("/health", status_code=status.HTTP_200_OK)
async def health_check() -> dict[str, str]:
    """Return the bounded recovery-service health state."""
    return {
        "status": "ok",
        "service": APP_NAME,
        "version": APP_VERSION,
        "scope": "health-only recovery surface",
    }


@app.get("/", status_code=status.HTTP_200_OK)
async def root() -> dict[str, str]:
    """Describe the currently implemented backend boundary."""
    return {
        "message": "SynQ backend recovery service",
        "version": APP_VERSION,
        "health": "/health",
        "docs": "/docs",
        "scope": "Only root and health endpoints are currently implemented.",
    }


if __name__ == "__main__":
    import uvicorn

    uvicorn.run(
        "app.main:app",
        host=getenv("API_HOST", "0.0.0.0"),
        port=int(getenv("API_PORT", "8000")),
        reload=getenv("APP_RELOAD", "false").lower() == "true",
    )
