"""Minimal FastAPI recovery surface for the SynQ backend prototype.

Only the root and health endpoints are implemented here. Historical router
modules, database wiring, GraphQL, WebSocket, ML, and collaboration features
remain unavailable until they are separately restored and tested.
"""

from os import getenv

from fastapi import FastAPI, HTTPException, Query, status
from fastapi.middleware.cors import CORSMiddleware

from app.example_catalog import available_domains, find_example, list_examples


APP_NAME = "SynQ Backend Recovery"
APP_VERSION = "0.1.0-recovery"
API_PREFIX = "/api/v1"

app = FastAPI(
    title=APP_NAME,
    version=APP_VERSION,
    description="Bounded health and source-only example catalog for the experimental SynQ backend recovery.",
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
        "scope": "health and source-only example catalog recovery surface",
    }


@app.get("/", status_code=status.HTTP_200_OK)
async def root() -> dict[str, str]:
    """Describe the currently implemented backend boundary."""
    return {
        "message": "SynQ backend recovery service",
        "version": APP_VERSION,
        "health": "/health",
        "docs": "/docs",
        "examples": f"{API_PREFIX}/examples",
        "scope": "Only root, health, and source-only example catalog endpoints are currently implemented.",
    }


@app.get(f"{API_PREFIX}/examples", status_code=status.HTTP_200_OK)
async def examples(domain: str | None = Query(default=None, pattern=r"^[a-z-]+$")) -> dict[str, object]:
    """List source-only example metadata, optionally filtered by a known domain."""
    if domain is not None and domain not in available_domains():
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail={"message": "Unknown example domain", "available_domains": sorted(available_domains())},
        )
    entries = list_examples(domain)
    return {
        "examples": entries,
        "count": len(entries),
        "verification": "source-only-unverified",
        "scope": "Metadata only; these examples are not compiled or executed by this service.",
    }


@app.get(f"{API_PREFIX}/examples/{{example_id}}", status_code=status.HTTP_200_OK)
async def example_detail(example_id: str) -> dict[str, object]:
    """Return one source-only example metadata record by its stable identifier."""
    entry = find_example(example_id)
    if entry is None:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Example not found")
    return {
        "example": entry,
        "scope": "Metadata only; this example is not compiled or executed by this service.",
    }


if __name__ == "__main__":
    import uvicorn

    uvicorn.run(
        "app.main:app",
        host=getenv("API_HOST", "0.0.0.0"),
        port=int(getenv("API_PORT", "8000")),
        reload=getenv("APP_RELOAD", "false").lower() == "true",
    )
