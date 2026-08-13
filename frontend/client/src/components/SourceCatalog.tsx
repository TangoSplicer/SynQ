import { useEffect, useState } from "react";
import { AlertTriangle, Database, RefreshCw } from "lucide-react";
import { Button } from "@/components/ui/button";
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card";
import { catalogEndpoint, fetchExampleCatalog, type ExampleCatalog } from "@/lib/example-catalog";

type CatalogState =
  | { kind: "loading" }
  | { kind: "ready"; catalog: ExampleCatalog }
  | { kind: "unconfigured" }
  | { kind: "error"; message: string };

export default function SourceCatalog() {
  const [state, setState] = useState<CatalogState>(() => catalogEndpoint() ? { kind: "loading" } : { kind: "unconfigured" });
  const endpoint = catalogEndpoint();

  const loadCatalog = () => {
    if (endpoint === null) {
      setState({ kind: "unconfigured" });
      return;
    }
    const controller = new AbortController();
    setState({ kind: "loading" });
    fetchExampleCatalog(undefined, controller.signal)
      .then((catalog) => setState({ kind: "ready", catalog }))
      .catch((error: unknown) => {
        if (error instanceof DOMException && error.name === "AbortError") return;
        setState({ kind: "error", message: error instanceof Error ? error.message : "The catalog request failed." });
      });
    return () => controller.abort();
  };

  useEffect(() => loadCatalog(), [endpoint]);

  return (
    <section className="mb-12 rounded-xl border border-slate-200 bg-white p-6 dark:border-slate-700 dark:bg-slate-900">
      <div className="flex flex-col justify-between gap-4 sm:flex-row sm:items-start">
        <div>
          <div className="flex items-center gap-2 text-blue-700 dark:text-blue-300"><Database className="h-5 w-5" aria-hidden="true" /><span className="text-sm font-semibold uppercase tracking-[0.16em]">Connected catalog</span></div>
          <h2 className="mt-2 text-2xl font-bold text-slate-900 dark:text-white">Source-only example metadata</h2>
          <p className="mt-2 max-w-2xl text-sm leading-6 text-slate-600 dark:text-slate-300">This panel displays only the backend’s verified metadata contract. It does not load source code, compile programs, or execute examples.</p>
        </div>
        {state.kind === "ready" || state.kind === "error" ? <Button variant="outline" size="sm" className="gap-2" onClick={() => loadCatalog()}><RefreshCw className="h-3.5 w-3.5" />Retry</Button> : null}
      </div>

      {state.kind === "loading" ? <p className="mt-6 text-sm text-slate-500" aria-live="polite">Loading configured source-only catalog…</p> : null}
      {state.kind === "unconfigured" ? <p className="mt-6 text-sm text-slate-500">No catalog service URL is configured for this build. Set <code>VITE_SYNQ_API_BASE_URL</code> to enable this optional local integration.</p> : null}
      {state.kind === "error" ? <div className="mt-6 flex gap-3 rounded-lg border border-amber-300 bg-amber-50 p-4 text-sm text-amber-950 dark:border-amber-800 dark:bg-amber-950/30 dark:text-amber-100" role="alert"><AlertTriangle className="mt-0.5 h-4 w-4 shrink-0" aria-hidden="true" />{state.message}</div> : null}
      {state.kind === "ready" ? (
        <div className="mt-6">
          <p className="mb-4 text-sm text-slate-500">{state.catalog.count} records returned · {state.catalog.verification}</p>
          <div className="grid gap-3 sm:grid-cols-2">
            {state.catalog.examples.map((example) => (
              <Card key={example.id} className="border-slate-200 shadow-none dark:border-slate-700">
                <CardHeader className="pb-2"><CardDescription>{example.domain}</CardDescription><CardTitle className="text-base">{example.title}</CardTitle></CardHeader>
                <CardContent><p className="text-sm leading-6 text-slate-600 dark:text-slate-300">{example.description}</p></CardContent>
              </Card>
            ))}
          </div>
        </div>
      ) : null}
    </section>
  );
}
