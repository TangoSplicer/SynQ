/**
 * Collaboration Demo Page
 * Showcases real-time collaborative editing with WebSocket integration
 */

import React, { useState, useEffect } from 'react';
import { CollaborationProvider } from '@/contexts/CollaborationContext';
import { CollaborativeEditor } from '@/components/CollaborativeEditor';
import { Button } from '@/components/ui/button';
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from '@/components/ui/card';
import { ArrowLeft, Users, Zap, MessageSquare } from 'lucide-react';

export default function CollaborationDemo() {
  const [isJoined, setIsJoined] = useState(false);
  const [sessionId, setSessionId] = useState<string>('');
  const [userId, setUserId] = useState<string>('');
  const [userName, setUserName] = useState<string>('');
  const [content, setContent] = useState(`// Welcome to SynQ Collaborative Editing!
// This is a real-time collaborative editor

from qml import VQE, H2_Hamiltonian

circuit Ansatz {
  qreg q[2]
  h(q[0])
  rz(q[0], theta)
  cx(q[0], q[1])
}

solver = VQE(
  ansatz=Ansatz,
  hamiltonian=H2_Hamiltonian,
  optimizer="COBYLA"
)

result = solver.run()
print(f"Ground state energy: {result.eigenvalue}")
`);

  // Generate random session ID and user ID
  useEffect(() => {
    const randomSessionId = `session-${Math.random().toString(36).substr(2, 9)}`;
    const randomUserId = `user-${Math.random().toString(36).substr(2, 9)}`;
    setSessionId(randomSessionId);
    setUserId(randomUserId);
  }, []);

  const handleJoinSession = () => {
    if (!userName.trim()) {
      alert('Please enter your name');
      return;
    }
    setIsJoined(true);
  };

  const handleLeaveSession = () => {
    setIsJoined(false);
    setUserName('');
  };

  if (!isJoined) {
    return (
      <div className="min-h-screen bg-gradient-to-br from-slate-50 to-slate-100 dark:from-slate-950 dark:to-slate-900">
        {/* Header */}
        <div className="border-b border-slate-200 dark:border-slate-800 bg-white dark:bg-slate-900">
          <div className="container py-4">
            <a href="/" className="inline-flex items-center gap-2 text-slate-600 dark:text-slate-400 hover:text-slate-900 dark:hover:text-white transition-colors mb-4">
              <ArrowLeft className="w-4 h-4" />
              Back to Home
            </a>
          </div>
        </div>

        {/* Main content */}
        <div className="container py-20">
          <div className="max-w-2xl mx-auto">
            {/* Title */}
            <div className="text-center mb-12">
              <h1 className="text-4xl md:text-5xl font-bold text-slate-900 dark:text-white mb-4">
                Real-Time Collaborative Editing
              </h1>
              <p className="text-lg text-slate-600 dark:text-slate-400">
                Experience live multi-user editing with presence awareness
              </p>
            </div>

            {/* Features grid */}
            <div className="grid md:grid-cols-3 gap-6 mb-12">
              <Card>
                <CardHeader>
                  <CardTitle className="flex items-center gap-2 text-base">
                    <Zap className="w-4 h-4 text-yellow-500" />
                    Real-Time Sync
                  </CardTitle>
                </CardHeader>
                <CardContent className="text-sm text-slate-600 dark:text-slate-400">
                  All changes sync instantly across connected users
                </CardContent>
              </Card>

              <Card>
                <CardHeader>
                  <CardTitle className="flex items-center gap-2 text-base">
                    <Users className="w-4 h-4 text-blue-500" />
                    Presence Awareness
                  </CardTitle>
                </CardHeader>
                <CardContent className="text-sm text-slate-600 dark:text-slate-400">
                  See remote user cursors and selections in real-time
                </CardContent>
              </Card>

              <Card>
                <CardHeader>
                  <CardTitle className="flex items-center gap-2 text-base">
                    <MessageSquare className="w-4 h-4 text-green-500" />
                    Comments
                  </CardTitle>
                </CardHeader>
                <CardContent className="text-sm text-slate-600 dark:text-slate-400">
                  Add threaded comments to specific lines
                </CardContent>
              </Card>
            </div>

            {/* Join form */}
            <Card>
              <CardHeader>
                <CardTitle>Join a Collaboration Session</CardTitle>
                <CardDescription>
                  Enter your name to join the demo session
                </CardDescription>
              </CardHeader>
              <CardContent className="space-y-4">
                <div>
                  <label className="block text-sm font-medium text-slate-900 dark:text-white mb-2">
                    Your Name
                  </label>
                  <input
                    type="text"
                    value={userName}
                    onChange={(e) => setUserName(e.target.value)}
                    onKeyPress={(e) => e.key === 'Enter' && handleJoinSession()}
                    placeholder="Enter your name..."
                    className="w-full px-4 py-2 border border-slate-300 dark:border-slate-600 rounded-lg bg-white dark:bg-slate-800 text-slate-900 dark:text-white placeholder-slate-400 dark:placeholder-slate-500 focus:outline-none focus:ring-2 focus:ring-blue-500"
                  />
                </div>
                <div className="text-sm text-slate-600 dark:text-slate-400">
                  <p className="mb-2"><strong>Session ID:</strong> {sessionId}</p>
                  <p><strong>User ID:</strong> {userId}</p>
                </div>
                <Button
                  onClick={handleJoinSession}
                  disabled={!userName.trim()}
                  size="lg"
                  className="w-full"
                >
                  Join Session
                </Button>
              </CardContent>
            </Card>
          </div>
        </div>
      </div>
    );
  }

  return (
    <CollaborationProvider wsUrl="ws://localhost:8000/api/v1/collaboration/ws">
      <div className="min-h-screen bg-slate-950 flex flex-col">
        {/* Header */}
        <div className="border-b border-slate-800 bg-slate-900 px-6 py-4">
          <div className="flex items-center justify-between">
            <div>
              <h1 className="text-2xl font-bold text-white">Collaborative Editor</h1>
              <p className="text-sm text-slate-400">Session: {sessionId}</p>
            </div>
            <Button
              onClick={handleLeaveSession}
              variant="outline"
              className="text-white border-slate-600 hover:bg-slate-800"
            >
              Leave Session
            </Button>
          </div>
        </div>

        {/* Editor */}
        <div className="flex-1 overflow-hidden p-6">
          <CollaborativeEditor
            initialContent={content}
            onContentChange={setContent}
            placeholder="Start typing to collaborate..."
          />
        </div>
      </div>
    </CollaborationProvider>
  );
}
