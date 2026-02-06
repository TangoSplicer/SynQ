/**
 * Quantum Circuit Visualizer Component
 *
 * Provides visual representation of quantum circuits with:
 * - Qubit lanes
 * - Gate placement
 * - Multi-qubit gate connections
 * - Measurement indicators
 * - Zoom and pan controls
 */

import React, { useState, useRef, useEffect } from 'react';
import { Button } from '@/components/ui/button';
import { Card } from '@/components/ui/card';
import { ZoomIn, ZoomOut, RotateCcw } from 'lucide-react';

interface Gate {
  id: string;
  type: string;
  qubits: number[];
  params?: Record<string, number>;
  position: number;
}

interface CircuitVisualizerProps {
  gates: Gate[];
  qubitCount: number;
  onGateClick?: (gate: Gate) => void;
  onGateAdd?: (position: number, qubit: number) => void;
  editable?: boolean;
}

const CircuitVisualizer: React.FC<CircuitVisualizerProps> = ({
  gates,
  qubitCount,
  onGateClick,
  onGateAdd,
  editable = false
}) => {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const [zoom, setZoom] = useState(1);
  const [panX, setPanX] = useState(0);
  const [panY, setPanY] = useState(0);
  const [hoveredGate, setHoveredGate] = useState<string | null>(null);

  const QUBIT_SPACING = 60;
  const GATE_WIDTH = 50;
  const GATE_HEIGHT = 40;
  const LANE_WIDTH = 100;
  const MARGIN = 40;

  // Draw circuit on canvas
  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;

    const ctx = canvas.getContext('2d');
    if (!ctx) return;

    // Clear canvas
    ctx.fillStyle = '#ffffff';
    ctx.fillRect(0, 0, canvas.width, canvas.height);

    // Save context state
    ctx.save();

    // Apply transformations
    ctx.translate(MARGIN + panX, MARGIN + panY);
    ctx.scale(zoom, zoom);

    // Draw qubit lanes
    ctx.strokeStyle = '#e5e7eb';
    ctx.lineWidth = 1;

    for (let i = 0; i < qubitCount; i++) {
      const y = i * QUBIT_SPACING;

      // Draw qubit line
      ctx.beginPath();
      ctx.moveTo(0, y);
      ctx.lineTo(LANE_WIDTH * 10, y);
      ctx.stroke();

      // Draw qubit label
      ctx.fillStyle = '#1f2937';
      ctx.font = '12px monospace';
      ctx.textAlign = 'right';
      ctx.fillText(`q${i}`, -10, y + 4);
    }

    // Draw gates
    gates.forEach(gate => {
      const minQubit = Math.min(...gate.qubits);
      const maxQubit = Math.max(...gate.qubits);
      const y = minQubit * QUBIT_SPACING;
      const height = (maxQubit - minQubit + 1) * QUBIT_SPACING;

      const x = gate.position * GATE_WIDTH;

      // Draw gate box
      ctx.fillStyle = getGateColor(gate.type);
      ctx.strokeStyle = hoveredGate === gate.id ? '#3b82f6' : '#6b7280';
      ctx.lineWidth = hoveredGate === gate.id ? 2 : 1;

      ctx.fillRect(x, y - GATE_HEIGHT / 2, GATE_WIDTH, height || GATE_HEIGHT);
      ctx.strokeRect(x, y - GATE_HEIGHT / 2, GATE_WIDTH, height || GATE_HEIGHT);

      // Draw gate label
      ctx.fillStyle = '#ffffff';
      ctx.font = 'bold 12px monospace';
      ctx.textAlign = 'center';
      ctx.textBaseline = 'middle';
      ctx.fillText(gate.type, x + GATE_WIDTH / 2, y);

      // Draw parameters if any
      if (gate.params && Object.keys(gate.params).length > 0) {
        ctx.fillStyle = '#9ca3af';
        ctx.font = '10px monospace';
        const paramStr = Object.entries(gate.params)
          .map(([k, v]) => `${k}=${v.toFixed(2)}`)
          .join(',');
        ctx.fillText(paramStr, x + GATE_WIDTH / 2, y + 15);
      }
    });

    // Draw measurements
    gates
      .filter(g => g.type === 'MEASURE')
      .forEach(gate => {
        const y = gate.qubits[0] * QUBIT_SPACING;
        const x = gate.position * GATE_WIDTH;

        // Draw measurement symbol
        ctx.strokeStyle = '#10b981';
        ctx.lineWidth = 2;
        ctx.beginPath();
        ctx.arc(x + GATE_WIDTH / 2, y, 15, 0, Math.PI);
        ctx.stroke();

        // Draw arrow
        ctx.beginPath();
        ctx.moveTo(x + GATE_WIDTH / 2, y - 15);
        ctx.lineTo(x + GATE_WIDTH / 2 + 8, y - 5);
        ctx.lineTo(x + GATE_WIDTH / 2 - 8, y - 5);
        ctx.closePath();
        ctx.fill();
      });

    ctx.restore();
  }, [gates, qubitCount, zoom, panX, panY, hoveredGate]);

  const handleZoomIn = () => setZoom(z => Math.min(z + 0.2, 3));
  const handleZoomOut = () => setZoom(z => Math.max(z - 0.2, 0.5));
  const handleReset = () => {
    setZoom(1);
    setPanX(0);
    setPanY(0);
  };

  const handleCanvasMouseMove = (e: React.MouseEvent<HTMLCanvasElement>) => {
    const canvas = canvasRef.current;
    if (!canvas) return;

    const rect = canvas.getBoundingClientRect();
    const x = (e.clientX - rect.left - MARGIN - panX) / zoom;
    const y = (e.clientY - rect.top - MARGIN - panY) / zoom;

    // Check if hovering over any gate
    let foundGate = null;
    for (const gate of gates) {
      const gateX = gate.position * GATE_WIDTH;
      const minQubit = Math.min(...gate.qubits);
      const maxQubit = Math.max(...gate.qubits);
      const gateY = minQubit * QUBIT_SPACING;
      const height = (maxQubit - minQubit + 1) * QUBIT_SPACING;

      if (
        x >= gateX &&
        x <= gateX + GATE_WIDTH &&
        y >= gateY - GATE_HEIGHT / 2 &&
        y <= gateY + height - GATE_HEIGHT / 2
      ) {
        foundGate = gate.id;
        break;
      }
    }

    setHoveredGate(foundGate);
  };

  const handleCanvasClick = (e: React.MouseEvent<HTMLCanvasElement>) => {
    if (hoveredGate) {
      const gate = gates.find(g => g.id === hoveredGate);
      if (gate) {
        onGateClick?.(gate);
      }
    }
  };

  const handleCanvasWheel = (e: React.WheelEvent<HTMLCanvasElement>) => {
    e.preventDefault();
    if (e.deltaY < 0) {
      handleZoomIn();
    } else {
      handleZoomOut();
    }
  };

  const getGateColor = (gateType: string): string => {
    const colors: Record<string, string> = {
      X: '#ef4444',
      Y: '#f97316',
      Z: '#eab308',
      H: '#3b82f6',
      S: '#8b5cf6',
      T: '#ec4899',
      RX: '#06b6d4',
      RY: '#10b981',
      RZ: '#f59e0b',
      CNOT: '#6366f1',
      CZ: '#a855f7',
      SWAP: '#14b8a6',
      MEASURE: '#10b981'
    };
    return colors[gateType] || '#9ca3af';
  };

  return (
    <div className="flex flex-col gap-4 h-full">
      {/* Toolbar */}
      <div className="flex items-center gap-2 p-4 border-b border-border">
        <Button
          variant="outline"
          size="sm"
          onClick={handleZoomIn}
          title="Zoom In"
        >
          <ZoomIn className="w-4 h-4" />
        </Button>
        <Button
          variant="outline"
          size="sm"
          onClick={handleZoomOut}
          title="Zoom Out"
        >
          <ZoomOut className="w-4 h-4" />
        </Button>
        <Button
          variant="outline"
          size="sm"
          onClick={handleReset}
          title="Reset View"
        >
          <RotateCcw className="w-4 h-4" />
        </Button>
        <span className="text-sm text-muted-foreground ml-auto">
          Zoom: {Math.round(zoom * 100)}%
        </span>
      </div>

      {/* Canvas */}
      <Card className="flex-1 p-2 overflow-hidden">
        <canvas
          ref={canvasRef}
          width={800}
          height={600}
          onMouseMove={handleCanvasMouseMove}
          onClick={handleCanvasClick}
          onWheel={handleCanvasWheel}
          className="w-full h-full bg-white cursor-crosshair"
        />
      </Card>

      {/* Info */}
      <div className="px-4 py-2 text-xs text-muted-foreground border-t border-border">
        <div className="flex justify-between">
          <span>{gates.length} gates</span>
          <span>{qubitCount} qubits</span>
          <span>Depth: {Math.max(...gates.map(g => g.position), 0) + 1}</span>
        </div>
      </div>
    </div>
  );
};

export default CircuitVisualizer;
