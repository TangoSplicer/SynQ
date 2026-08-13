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
  },
});
