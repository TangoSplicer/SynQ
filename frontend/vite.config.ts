import { fileURLToPath } from "node:url";
import path from "node:path";
import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";

const projectRoot = path.dirname(fileURLToPath(import.meta.url));

export default defineConfig({
  plugins: [react()],
  root: path.resolve(projectRoot, "client"),
  resolve: {
    alias: {
      "@": path.resolve(projectRoot, "client/src"),
    },
  },
  build: {
    outDir: path.resolve(projectRoot, "dist"),
    emptyOutDir: true,
    rollupOptions: {
      output: {
        manualChunks(id) {
          if (!id.includes("node_modules")) return undefined;
          if (id.includes("@radix-ui")) return "radix-ui";
          if (id.includes("lucide-react")) return "icons";
          if (id.includes("react-dom") || id.includes("react/") || id.includes("wouter")) return "react-routing";
          return "vendor";
        },
      },
    },
  },
});
