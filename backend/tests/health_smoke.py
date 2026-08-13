"""Executable smoke test for the documented minimal backend recovery surface."""

import asyncio

import httpx

from app.main import API_PREFIX, APP_NAME, APP_VERSION, app


def require(condition: bool, message: str) -> None:
    if not condition:
        raise AssertionError(message)


async def main() -> None:
    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://synq.local") as client:
        root_response = await client.get("/")
        require(root_response.status_code == 200, "root endpoint returns HTTP 200")
        root = root_response.json()
        require(root["message"] == "SynQ backend recovery service", "root reports recovery service")
        require(root["version"] == APP_VERSION, "root reports expected version")
        require(root["examples"] == f"{API_PREFIX}/examples", "root links to example catalog")

        health_response = await client.get("/health")
        require(health_response.status_code == 200, "health endpoint returns HTTP 200")
        health = health_response.json()
        require(health == {
            "status": "ok",
            "service": APP_NAME,
            "version": APP_VERSION,
            "scope": "health and source-only example catalog recovery surface",
        }, "health response matches documented recovery contract")

        catalog_response = await client.get(f"{API_PREFIX}/examples")
        require(catalog_response.status_code == 200, "example catalog returns HTTP 200")
        catalog = catalog_response.json()
        require(catalog["count"] == 4, "example catalog returns all bounded entries")
        require(catalog["verification"] == "source-only-unverified", "catalog declares verification boundary")
        require(all(entry["verification"] == "source-only-unverified" for entry in catalog["examples"]),
                "each catalog entry declares verification boundary")

        quantum_response = await client.get(f"{API_PREFIX}/examples", params={"domain": "quantum"})
        require(quantum_response.status_code == 200, "domain-filtered catalog returns HTTP 200")
        require(quantum_response.json()["examples"] == [
            {
                "id": "quantum-vqe",
                "domain": "quantum",
                "title": "VQE for H2 Molecule",
                "description": "Illustrative source for a variational quantum eigensolver workflow.",
                "verification": "source-only-unverified",
            }
        ], "domain filter returns the documented quantum entry")

        detail_response = await client.get(f"{API_PREFIX}/examples/quantum-vqe")
        require(detail_response.status_code == 200, "example detail returns HTTP 200")
        require(detail_response.json()["example"]["id"] == "quantum-vqe", "detail returns requested example")

        invalid_domain = await client.get(f"{API_PREFIX}/examples", params={"domain": "missing"})
        require(invalid_domain.status_code == 400, "catalog rejects unknown domain")
        missing_example = await client.get(f"{API_PREFIX}/examples/missing")
        require(missing_example.status_code == 404, "catalog returns HTTP 404 for unknown example")

    require(API_PREFIX == "/api/v1", "reserved API prefix remains stable")
    print("SynQ backend health smoke test passed")


asyncio.run(main())
