import { afterEach, describe, expect, it, vi } from "vitest";
import { catalogEndpoint, fetchExampleCatalog } from "@/lib/example-catalog";

const validCatalog = {
  examples: [{
    id: "quantum-vqe",
    domain: "quantum",
    title: "VQE for H2 Molecule",
    description: "Illustrative source for a variational quantum eigensolver workflow.",
    verification: "source-only-unverified",
  }],
  count: 1,
  verification: "source-only-unverified",
  scope: "Metadata only; these examples are not compiled or executed by this service.",
};

describe("source-only catalog client", () => {
  afterEach(() => vi.restoreAllMocks());

  it("constructs a stable catalog endpoint from an explicit base URL", () => {
    expect(catalogEndpoint("http://localhost:8000/")).toBe("http://localhost:8000/api/v1/examples");
  });

  it("retrieves the bounded catalog contract without claiming execution", async () => {
    const fetchMock = vi.spyOn(globalThis, "fetch").mockResolvedValue(new Response(JSON.stringify(validCatalog), { status: 200 }));
    await expect(fetchExampleCatalog("http://localhost:8000")).resolves.toEqual(validCatalog);
    expect(fetchMock).toHaveBeenCalledWith("http://localhost:8000/api/v1/examples", { signal: undefined });
  });

  it("rejects an unconfigured or malformed catalog response", async () => {
    await expect(fetchExampleCatalog(undefined)).rejects.toThrow("not configured");
    vi.spyOn(globalThis, "fetch").mockResolvedValue(new Response(JSON.stringify({ examples: [] }), { status: 200 }));
    await expect(fetchExampleCatalog("http://localhost:8000")).rejects.toThrow("unexpected response shape");
  });
});
