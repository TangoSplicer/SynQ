import { useEffect, useRef, useState } from 'react';
import { Button } from '@/components/ui/button';
import { Play, Copy, Check } from 'lucide-react';

interface CodeEditorProps {
  defaultCode: string;
  language?: string;
  title?: string;
}

export default function CodeEditor({ defaultCode, language = 'python', title }: CodeEditorProps) {
  const editorRef = useRef<HTMLDivElement>(null);
  const [code, setCode] = useState(defaultCode);
  const [output, setOutput] = useState('');
  const [isRunning, setIsRunning] = useState(false);
  const [copied, setCopied] = useState(false);

  useEffect(() => {
    // Simple code execution simulation (for demo purposes)
    // In production, this would call a backend API
  }, []);

  const handleRun = async () => {
    setIsRunning(true);
    setOutput('Executing code...\n');
    
    // Simulate code execution
    setTimeout(() => {
      setOutput(
        `// SynQ Code Execution Output\n` +
        `// Language: ${language}\n` +
        `// Status: Success\n\n` +
        `// Output:\n` +
        `Ground state energy: -1.137 Ha\n` +
        `Circuit depth: 12 gates\n` +
        `Execution time: 2.34ms\n\n` +
        `// Note: For live execution, connect to SynQ backend API`
      );
      setIsRunning(false);
    }, 1500);
  };

  const handleCopy = () => {
    navigator.clipboard.writeText(code);
    setCopied(true);
    setTimeout(() => setCopied(false), 2000);
  };

  return (
    <div className="bg-slate-900 dark:bg-slate-950 rounded-lg overflow-hidden border border-slate-700">
      {title && (
        <div className="bg-slate-800 px-6 py-3 border-b border-slate-700">
          <h3 className="text-sm font-semibold text-slate-100">{title}</h3>
        </div>
      )}
      
      <div className="grid grid-cols-2 gap-0">
        {/* Code Editor */}
        <div className="border-r border-slate-700">
          <div className="flex items-center justify-between px-4 py-2 bg-slate-800 border-b border-slate-700">
            <span className="text-xs font-medium text-slate-400">Editor</span>
            <Button
              size="sm"
              variant="ghost"
              onClick={handleCopy}
              className="h-6 px-2 text-xs"
            >
              {copied ? (
                <>
                  <Check className="w-3 h-3 mr-1" />
                  Copied
                </>
              ) : (
                <>
                  <Copy className="w-3 h-3 mr-1" />
                  Copy
                </>
              )}
            </Button>
          </div>
          
          <textarea
            value={code}
            onChange={(e) => setCode(e.target.value)}
            className="w-full h-64 p-4 bg-slate-900 text-slate-100 font-mono text-sm resize-none focus:outline-none"
            spellCheck="false"
          />
        </div>

        {/* Output */}
        <div>
          <div className="flex items-center justify-between px-4 py-2 bg-slate-800 border-b border-slate-700">
            <span className="text-xs font-medium text-slate-400">Output</span>
            <Button
              size="sm"
              onClick={handleRun}
              disabled={isRunning}
              className="h-6 px-2 text-xs gap-1"
            >
              <Play className="w-3 h-3" />
              {isRunning ? 'Running...' : 'Run'}
            </Button>
          </div>
          
          <pre className="w-full h-64 p-4 bg-slate-900 text-slate-300 font-mono text-xs overflow-auto whitespace-pre-wrap break-words">
            {output || '// Output will appear here'}
          </pre>
        </div>
      </div>

      <div className="bg-slate-800 px-4 py-2 border-t border-slate-700 text-xs text-slate-400">
        <p>💡 Tip: Edit the code and click "Run" to see the output. For production use, connect to a real SynQ backend.</p>
      </div>
    </div>
  );
}
