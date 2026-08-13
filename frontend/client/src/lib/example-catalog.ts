export type SourceOnlyExample = {
  id: string;
  domain: string;
  title: string;
  description: string;
  verification: "source-only-unverified";
};

export type ExampleCatalog = {
  examples: SourceOnlyExample[];
  count: number;
  verification: "source-only-unverified";
  scope: string;
};

const configuredBaseUrl = import.meta.env.VITE_SYNQ_API_BASE_URL?.replace(/\/$/, "");

export function catalogEndpoint(baseUrl = configuredBaseUrl): string | null {
  const normalizedBaseUrl = baseUrl?.replace(/\/+$/, "");
  return normalizedBaseUrl ? `${normalizedBaseUrl}/api/v1/examples` : null;
}

export async function fetchExampleCatalog(baseUrl = configuredBaseUrl, signal?: AbortSignal): Promise<ExampleCatalog> {
  const endpoint = catalogEndpoint(baseUrl);
  if (endpoint === null) {
    throw new Error("The source-only catalog service is not configured for this frontend build.");
  }

  const response = await fetch(endpoint, { signal });
  if (!response.ok) {
    throw new Error(`The source-only catalog request failed with HTTP ${response.status}.`);
  }

  const payload: unknown = await response.json();
  if (!isExampleCatalog(payload)) {
    throw new Error("The source-only catalog returned an unexpected response shape.");
  }
  return payload;
}

function isExampleCatalog(value: unknown): value is ExampleCatalog {
  if (typeof value !== "object" || value === null) return false;
  const payload = value as Partial<ExampleCatalog>;
  return payload.verification === "source-only-unverified" &&
    typeof payload.count === "number" &&
    Array.isArray(payload.examples) &&
    typeof payload.scope === "string" &&
    payload.examples.every((example) =>
      typeof example === "object" && example !== null &&
      typeof (example as SourceOnlyExample).id === "string" &&
      typeof (example as SourceOnlyExample).domain === "string" &&
      typeof (example as SourceOnlyExample).title === "string" &&
      typeof (example as SourceOnlyExample).description === "string" &&
      (example as SourceOnlyExample).verification === "source-only-unverified",
    );
}
