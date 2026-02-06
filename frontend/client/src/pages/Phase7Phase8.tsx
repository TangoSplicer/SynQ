// Phase 7 & 8 Section Content
// This section should be inserted before the CTA section in Home.tsx

export const Phase7Phase8Section = `
      {/* Phase 7 Section */}
      <section className="container py-20">
        <div className="mb-12">
          <h3 className="text-2xl font-bold text-slate-900 dark:text-white mb-8 text-center">🔧 Phase 7: Backend WebSocket & ML Integration</h3>
          <p className="text-center text-slate-600 dark:text-slate-400 mb-12 max-w-3xl mx-auto">
            Production-grade backend infrastructure enabling real-time collaboration, ML-driven optimizations, and flexible GraphQL queries.
          </p>
          
          <div className="grid md:grid-cols-3 gap-6 mb-12">
            {/* WebSocket Server */}
            <Card className="border-2 border-cyan-500 dark:border-cyan-600 bg-cyan-50 dark:bg-cyan-900/20">
              <CardHeader>
                <CardTitle className="flex items-center gap-2">
                  <span className="text-2xl">🔌</span> WebSocket Server
                </CardTitle>
              </CardHeader>
              <CardContent className="space-y-3">
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-1">10,000+ Concurrent Connections</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Handles massive scale with connection pooling and load balancing</p>
                </div>
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-1">Message Queuing & Heartbeat</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Automatic reconnection and offline message buffering</p>
                </div>
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-1">Rate Limiting & Security</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Per-user rate limiting, injection detection, DoS protection</p>
                </div>
              </CardContent>
            </Card>

            {/* ML Training Pipeline */}
            <Card className="border-2 border-purple-500 dark:border-purple-600 bg-purple-50 dark:bg-purple-900/20">
              <CardHeader>
                <CardTitle className="flex items-center gap-2">
                  <span className="text-2xl">🤖</span> ML Training Pipeline
                </CardTitle>
              </CardHeader>
              <CardContent className="space-y-3">
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-1">Circuit Optimization</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">ML models predict gate reduction and circuit speedup</p>
                </div>
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-1">Resource Estimation</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Predict qubit requirements and execution time</p>
                </div>
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-1">Pattern Recognition</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Identify optimization opportunities in circuits</p>
                </div>
              </CardContent>
            </Card>

            {/* GraphQL API */}
            <Card className="border-2 border-pink-500 dark:border-pink-600 bg-pink-50 dark:bg-pink-900/20">
              <CardHeader>
                <CardTitle className="flex items-center gap-2">
                  <span className="text-2xl">📡</span> GraphQL API
                </CardTitle>
              </CardHeader>
              <CardContent className="space-y-3">
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-1">Type-Safe Queries</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">8+ types with 16+ queries and mutations</p>
                </div>
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-1">Flexible Data Fetching</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Request only the data you need</p>
                </div>
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-1">Schema Introspection</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Full API documentation and type information</p>
                </div>
              </CardContent>
            </Card>
          </div>
        </div>
      </section>

      {/* Phase 8 Section */}
      <section className="container py-20">
        <div className="mb-12">
          <h3 className="text-2xl font-bold text-slate-900 dark:text-white mb-8 text-center">⚡ Phase 8: Frontend Integration & Real-Time Features</h3>
          <p className="text-center text-slate-600 dark:text-slate-400 mb-12 max-w-3xl mx-auto">
            Complete frontend integration with real-time collaboration, ML suggestions, and interactive quantum circuit visualization.
          </p>
          
          <div className="grid md:grid-cols-2 gap-6 mb-12">
            {/* Real-Time Editor */}
            <Card className="border-2 border-green-500 dark:border-green-600 bg-green-50 dark:bg-green-900/20">
              <CardHeader>
                <CardTitle className="flex items-center gap-2">
                  <span className="text-2xl">✏️</span> Real-Time Editor
                </CardTitle>
              </CardHeader>
              <CardContent className="space-y-4">
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-2">WebSocket Synchronization</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Live edits sync across all collaborators with sub-100ms latency</p>
                </div>
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-2">Operational Transformation</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Conflict-free concurrent editing with automatic resolution</p>
                </div>
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-2">Presence Tracking</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">See remote cursors, selections, and user presence in real-time</p>
                </div>
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-2">Comment Threading</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Line-by-line comments with replies and resolution tracking</p>
                </div>
              </CardContent>
            </Card>

            {/* ML Suggestion Panel */}
            <Card className="border-2 border-orange-500 dark:border-orange-600 bg-orange-50 dark:bg-orange-900/20">
              <CardHeader>
                <CardTitle className="flex items-center gap-2">
                  <span className="text-2xl">💡</span> ML Suggestion Panel
                </CardTitle>
              </CardHeader>
              <CardContent className="space-y-4">
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-2">Circuit Optimization</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Real-time suggestions for gate reduction and speedup</p>
                </div>
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-2">Resource Estimates</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Predict qubit needs and execution time before submission</p>
                </div>
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-2">Pattern Recognition</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Identify repeated sequences and optimization opportunities</p>
                </div>
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-2">Confidence Scoring</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Visual confidence indicators for each suggestion</p>
                </div>
              </CardContent>
            </Card>

            {/* Circuit Visualizer */}
            <Card className="border-2 border-blue-500 dark:border-blue-600 bg-blue-50 dark:bg-blue-900/20">
              <CardHeader>
                <CardTitle className="flex items-center gap-2">
                  <span className="text-2xl">📊</span> Circuit Visualizer
                </CardTitle>
              </CardHeader>
              <CardContent className="space-y-4">
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-2">Interactive Canvas</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Render 1,000+ gate circuits with smooth zoom and pan</p>
                </div>
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-2">Gate Rendering</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Color-coded gates with parameter display and hover info</p>
                </div>
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-2">Circuit Metrics</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Real-time depth, gate count, and qubit utilization</p>
                </div>
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-2">Multi-Qubit Gates</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Visual connections for CNOT, CZ, and other multi-qubit ops</p>
                </div>
              </CardContent>
            </Card>

            {/* GraphQL Integration */}
            <Card className="border-2 border-red-500 dark:border-red-600 bg-red-50 dark:bg-red-900/20">
              <CardHeader>
                <CardTitle className="flex items-center gap-2">
                  <span className="text-2xl">🔗</span> GraphQL Integration
                </CardTitle>
              </CardHeader>
              <CardContent className="space-y-4">
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-2">Type-Safe Client</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Full TypeScript support with auto-generated types</p>
                </div>
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-2">React Hooks</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">useGraphQLQuery and useGraphQLMutation for seamless integration</p>
                </div>
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-2">Error Handling</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Automatic error handling and retry logic</p>
                </div>
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white mb-2">Data Caching</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Smart caching for optimal performance</p>
                </div>
              </CardContent>
            </Card>
          </div>
        </div>
      </section>
`;
