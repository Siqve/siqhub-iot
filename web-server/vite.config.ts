import { defineConfig } from 'vite'
import { svelte } from '@sveltejs/vite-plugin-svelte'

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [svelte()],
    build: {
        rollupOptions: {
            output: {
                entryFileNames: '[name].js',
                assetFileNames: (assetInfo) => {
                    if (assetInfo.name == 'index.css')
                        return 'index.css';
                    return 'asset/' + assetInfo.name;
                },
            }
        }
    }
})
