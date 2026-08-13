/**
 * Recovery-status design: emphasize verified evidence, visible boundaries, and
 * small next steps. Avoid feature promises that the recovery profile cannot run.
 */
import { lazy, Suspense } from "react";
import { AlertTriangle, ArrowRight, Braces, CheckCircle2, Code2, ExternalLink, FlaskConical, Server } from "lucide-react";
import { Button } from "@/components/ui/button";
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card";

const InteractiveExamples = lazy(() => import("@/components/InteractiveExamples"));

const repositoryUrl = "https://github.com/TangoSplicer/SynQ";

const verifiedAreas = [
  {
    icon: Braces,
    title: "Compiler recovery profile",
    detail: "The core static library builds in the default recovery profile. Two focused smoke executables pass locally and in the compiler-core workflow.",
  },
  {
    icon: Code2,
    title: "Frontend validation",
    detail: "The frontend installs from its frozen lockfile, type-checks, runs 30 local unit tests, and produces a production build. The CI workflow repeats those checks.",
  },
  {
    icon: Server,
    title: "Backend recovery surface",
    detail: "The backend currently provides root, health, and source-only example catalog endpoints. It does not compile or execute SynQ examples.",
  },
];

const parserStatements = [
  "let <identifier> = <value>",
  "print <text>",
  "delay <non-negative milliseconds>",
  "quantum <kernel>",
  "ai <prompt>",
];

export default function Home() {
  return (
    <div className="min-h-screen bg-slate-50 text-slate-950 dark:bg-slate-950 dark:text-slate-50">
      <nav className="sticky top-0 z-50 border-b border-slate-200 bg-white/90 backdrop-blur dark:border-slate-800 dark:bg-slate-950/90">
        <div className="container flex items-center justify-between py-4">
          <a href="#status" className="flex items-center gap-2 font-semibold tracking-tight">
            <Braces className="h-6 w-6 text-blue-600" aria-hidden="true" />
            <span>SynQ recovery</span>
          </a>
          <div className="flex items-center gap-3">
            <a className="hidden text-sm text-slate-600 hover:text-blue-700 sm:block dark:text-slate-300" href="#boundaries">Boundaries</a>
            <a href={repositoryUrl} target="_blank" rel="noreferrer">
              <Button size="sm" variant="outline" className="gap-2">Repository <ExternalLink className="h-3.5 w-3.5" /></Button>
            </a>
          </div>
        </div>
      </nav>

      <main>
        <section className="container py-20 md:py-28">
          <div className="max-w-4xl">
            <div className="mb-6 inline-flex items-center gap-2 rounded-full border border-amber-300 bg-amber-50 px-3 py-1 text-sm font-medium text-amber-950 dark:border-amber-700 dark:bg-amber-950/40 dark:text-amber-100">
              <AlertTriangle className="h-4 w-4" aria-hidden="true" /> Experimental recovery profile
            </div>
            <h1 className="max-w-3xl text-5xl font-bold leading-tight tracking-tight md:text-6xl">
              A small, tested foundation for rebuilding SynQ.
            </h1>
            <p className="mt-6 max-w-3xl text-lg leading-8 text-slate-600 dark:text-slate-300">
              SynQ is an experimental hybrid quantum-classical-AI language prototype. This repository now distinguishes code that builds and tests from historical plans and incomplete subsystems.
            </p>
            <div className="mt-8 flex flex-wrap gap-3">
              <a href="#status"><Button className="gap-2">View verified status <ArrowRight className="h-4 w-4" /></Button></a>
              <a href={`${repositoryUrl}/blob/main/docs/PROJECT_STATUS.md`} target="_blank" rel="noreferrer"><Button variant="outline">Read project status</Button></a>
            </div>
          </div>
        </section>

        <section id="status" className="border-y border-slate-200 bg-white py-16 dark:border-slate-800 dark:bg-slate-900">
          <div className="container">
            <div className="mb-10 max-w-2xl">
              <p className="text-sm font-semibold uppercase tracking-[0.18em] text-blue-700 dark:text-blue-300">Verified locally and in CI where noted</p>
              <h2 className="mt-3 text-3xl font-bold">Current recovery status</h2>
            </div>
            <div className="grid gap-5 md:grid-cols-3">
              {verifiedAreas.map(({ icon: Icon, title, detail }) => (
                <Card key={title} className="border-slate-200 shadow-none dark:border-slate-700">
                  <CardHeader>
                    <Icon className="h-6 w-6 text-blue-600" aria-hidden="true" />
                    <CardTitle className="pt-4">{title}</CardTitle>
                  </CardHeader>
                  <CardContent><CardDescription className="text-sm leading-6">{detail}</CardDescription></CardContent>
                </Card>
              ))}
            </div>
          </div>
        </section>

        <section className="container grid gap-10 py-20 lg:grid-cols-[1.2fr_0.8fr]">
          <div>
            <p className="text-sm font-semibold uppercase tracking-[0.18em] text-blue-700 dark:text-blue-300">Parser boundary</p>
            <h2 className="mt-3 text-3xl font-bold">What the current recovery parser accepts</h2>
            <p className="mt-4 max-w-2xl leading-7 text-slate-600 dark:text-slate-300">
              The parser is deliberately line-oriented and preserves declaration values as source text. It is not a full language grammar, type checker, or evaluator.
            </p>
            <div className="mt-7 grid gap-3 sm:grid-cols-2">
              {parserStatements.map((statement) => (
                <div key={statement} className="flex items-center gap-3 rounded-lg border border-slate-200 bg-white p-4 font-mono text-sm dark:border-slate-700 dark:bg-slate-900">
                  <CheckCircle2 className="h-4 w-4 shrink-0 text-emerald-600" aria-hidden="true" />
                  {statement}
                </div>
              ))}
            </div>
          </div>
          <Card className="self-start border-blue-200 bg-blue-50 shadow-none dark:border-blue-900 dark:bg-blue-950/30">
            <CardHeader><FlaskConical className="h-6 w-6 text-blue-700" aria-hidden="true" /><CardTitle className="pt-4">How to interpret the examples</CardTitle></CardHeader>
            <CardContent><CardDescription className="text-sm leading-6 text-slate-700 dark:text-slate-200">
              Source examples and interactive editor output are demonstrations only. A green build or smoke test does not establish real quantum hardware execution, AI synthesis, cloud provider integration, or production readiness.
            </CardDescription></CardContent>
          </Card>
        </section>

        <section id="boundaries" className="bg-slate-950 py-20 text-slate-50">
          <div className="container grid gap-10 lg:grid-cols-[0.9fr_1.1fr]">
            <div>
              <p className="text-sm font-semibold uppercase tracking-[0.18em] text-blue-300">Not currently claimed</p>
              <h2 className="mt-3 text-3xl font-bold">Useful boundaries are part of the product.</h2>
            </div>
            <ul className="grid gap-3 text-slate-300 sm:grid-cols-2">
              {[
                "A complete SynQ language or command-line tool",
                "Optional compiler targets, bindings, or REPL",
                "Historical database, GraphQL, WebSocket, ML, or collaboration services",
                "Docker Compose deployment or external-provider execution",
              ].map((boundary) => <li key={boundary} className="rounded-lg border border-slate-700 p-4">{boundary}</li>)}
            </ul>
          </div>
        </section>

        <Suspense fallback={<section className="container py-20" aria-busy="true" />}>
          <InteractiveExamples />
        </Suspense>

        <section className="container py-20">
          <div className="rounded-2xl border border-slate-200 bg-white p-8 dark:border-slate-700 dark:bg-slate-900 md:p-12">
            <p className="text-sm font-semibold uppercase tracking-[0.18em] text-blue-700 dark:text-blue-300">Next recovery increments</p>
            <h2 className="mt-3 text-3xl font-bold">Expand only what can be demonstrated.</h2>
            <p className="mt-4 max-w-3xl leading-7 text-slate-600 dark:text-slate-300">The active path is to add one parser feature, route contract, or tested integration at a time—then record the command and result before making a broader claim.</p>
          </div>
        </section>
      </main>
    </div>
  );
}
