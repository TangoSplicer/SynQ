/**
 * ML Suggestion Panel Component
 *
 * Displays ML-driven suggestions for:
 * - Circuit optimization
 * - Resource estimation
 * - Pattern recognition
 */

import React, { useState, useEffect, useCallback } from 'react';
import { Button } from '@/components/ui/button';
import { Card } from '@/components/ui/card';
import { AlertCircle, Zap, TrendingDown, Lightbulb, Loader2 } from 'lucide-react';

interface Suggestion {
  id: string;
  type: 'optimization' | 'resource' | 'pattern';
  title: string;
  description: string;
  confidence: number;
  details: Record<string, any>;
  actionable: boolean;
  impact?: string;
}

interface MLSuggestionPanelProps {
  circuitContent: string;
  onApplySuggestion?: (suggestion: Suggestion) => void;
  apiUrl: string;
  token: string;
}

const MLSuggestionPanel: React.FC<MLSuggestionPanelProps> = ({
  circuitContent,
  onApplySuggestion,
  apiUrl,
  token
}) => {
  const [suggestions, setSuggestions] = useState<Suggestion[]>([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [selectedSuggestion, setSelectedSuggestion] = useState<Suggestion | null>(null);
  const [debounceTimer, setDebounceTimer] = useState<NodeJS.Timeout | null>(null);

  // Fetch ML predictions
  const fetchPredictions = useCallback(async (content: string) => {
    if (!content || content.length < 10) {
      setSuggestions([]);
      return;
    }

    setLoading(true);
    setError(null);

    try {
      // Parse circuit data (simplified)
      const circuitData = {
        gate_count: (content.match(/gate/gi) || []).length,
        qubits: Math.max(5, (content.match(/q\d+/gi) || []).length),
        classical_bits: Math.max(2, (content.match(/c\d+/gi) || []).length)
      };

      // Fetch optimization prediction
      const optimizationRes = await fetch(
        `${apiUrl}/ml/predict/circuit-optimization`,
        {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
            'Authorization': `Bearer ${token}`
          },
          body: JSON.stringify(circuitData)
        }
      );

      // Fetch resource estimation
      const resourceRes = await fetch(
        `${apiUrl}/ml/predict/resource-estimation`,
        {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
            'Authorization': `Bearer ${token}`
          },
          body: JSON.stringify(circuitData)
        }
      );

      // Fetch pattern recognition
      const patternRes = await fetch(
        `${apiUrl}/ml/predict/pattern-recognition`,
        {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
            'Authorization': `Bearer ${token}`
          },
          body: JSON.stringify(circuitData)
        }
      );

      const results: Suggestion[] = [];

      if (optimizationRes.ok) {
        const data = await optimizationRes.json();
        results.push({
          id: 'opt-1',
          type: 'optimization',
          title: 'Circuit Optimization',
          description: `Reduce gates by ${data.gate_reduction_percent}% with ${data.gate_reduction_percent > 20 ? 'high' : 'medium'} confidence`,
          confidence: data.confidence,
          details: data,
          actionable: true,
          impact: `${data.estimated_speedup}x faster execution`
        });
      }

      if (resourceRes.ok) {
        const data = await resourceRes.json();
        results.push({
          id: 'res-1',
          type: 'resource',
          title: 'Resource Estimation',
          description: `Estimated ${data.estimated_qubits} qubits, ${data.estimated_execution_time_ms}ms execution`,
          confidence: data.confidence,
          details: data,
          actionable: false
        });
      }

      if (patternRes.ok) {
        const data = await patternRes.json();
        if (data.patterns_identified && data.patterns_identified.length > 0) {
          results.push({
            id: 'pat-1',
            type: 'pattern',
            title: 'Pattern Recognition',
            description: `Found ${data.patterns_identified.length} optimization patterns`,
            confidence: data.confidence,
            details: data,
            actionable: true
          });
        }
      }

      setSuggestions(results);
    } catch (err) {
      setError(err instanceof Error ? err.message : 'Failed to fetch predictions');
    } finally {
      setLoading(false);
    }
  }, [apiUrl, token]);

  // Debounce circuit content changes
  useEffect(() => {
    if (debounceTimer) {
      clearTimeout(debounceTimer);
    }

    const timer = setTimeout(() => {
      fetchPredictions(circuitContent);
    }, 500);

    setDebounceTimer(timer);

    return () => clearTimeout(timer);
  }, [circuitContent, fetchPredictions]);

  const handleApplySuggestion = (suggestion: Suggestion) => {
    onApplySuggestion?.(suggestion);
    setSelectedSuggestion(suggestion);
  };

  const getIcon = (type: string) => {
    switch (type) {
      case 'optimization':
        return <TrendingDown className="w-4 h-4 text-blue-500" />;
      case 'resource':
        return <Zap className="w-4 h-4 text-yellow-500" />;
      case 'pattern':
        return <Lightbulb className="w-4 h-4 text-purple-500" />;
      default:
        return <AlertCircle className="w-4 h-4 text-gray-500" />;
    }
  };

  const getConfidenceColor = (confidence: number) => {
    if (confidence >= 0.9) return 'bg-green-100 text-green-800';
    if (confidence >= 0.75) return 'bg-blue-100 text-blue-800';
    if (confidence >= 0.6) return 'bg-yellow-100 text-yellow-800';
    return 'bg-orange-100 text-orange-800';
  };

  return (
    <div className="flex flex-col h-full gap-4 p-4 bg-background border-l border-border overflow-y-auto">
      <div className="flex items-center justify-between">
        <h2 className="text-lg font-semibold">ML Suggestions</h2>
        {loading && <Loader2 className="w-4 h-4 animate-spin text-muted-foreground" />}
      </div>

      {error && (
        <Card className="p-3 bg-red-50 border-red-200">
          <p className="text-sm text-red-800">{error}</p>
        </Card>
      )}

      {suggestions.length === 0 && !loading && (
        <Card className="p-4 text-center text-muted-foreground">
          <p className="text-sm">No suggestions available</p>
          <p className="text-xs mt-2">Add more gates to your circuit to get suggestions</p>
        </Card>
      )}

      <div className="space-y-3">
        {suggestions.map(suggestion => (
          <Card
            key={suggestion.id}
            className={`p-4 cursor-pointer transition-all ${
              selectedSuggestion?.id === suggestion.id
                ? 'ring-2 ring-blue-500 bg-blue-50'
                : 'hover:bg-muted'
            }`}
            onClick={() => setSelectedSuggestion(suggestion)}
          >
            <div className="flex items-start gap-3">
              <div className="mt-1">{getIcon(suggestion.type)}</div>

              <div className="flex-1 min-w-0">
                <div className="flex items-center gap-2 mb-1">
                  <h3 className="font-semibold text-sm">{suggestion.title}</h3>
                  <span
                    className={`px-2 py-1 rounded text-xs font-medium ${getConfidenceColor(
                      suggestion.confidence
                    )}`}
                  >
                    {Math.round(suggestion.confidence * 100)}%
                  </span>
                </div>

                <p className="text-sm text-muted-foreground mb-2">{suggestion.description}</p>

                {suggestion.impact && (
                  <p className="text-xs text-green-600 font-medium mb-3">
                    💡 Impact: {suggestion.impact}
                  </p>
                )}

                {suggestion.actionable && (
                  <Button
                    size="sm"
                    variant="default"
                    onClick={(e) => {
                      e.stopPropagation();
                      handleApplySuggestion(suggestion);
                    }}
                    className="w-full"
                  >
                    Apply Suggestion
                  </Button>
                )}
              </div>
            </div>
          </Card>
        ))}
      </div>

      {selectedSuggestion && (
        <Card className="p-4 bg-muted">
          <h4 className="font-semibold text-sm mb-3">Details</h4>
          <div className="space-y-2 text-xs">
            {Object.entries(selectedSuggestion.details).map(([key, value]) => (
              <div key={key} className="flex justify-between">
                <span className="text-muted-foreground capitalize">
                  {key.replace(/_/g, ' ')}:
                </span>
                <span className="font-medium">
                  {typeof value === 'object' ? JSON.stringify(value) : String(value)}
                </span>
              </div>
            ))}
          </div>
        </Card>
      )}
    </div>
  );
};

export default MLSuggestionPanel;
