import type { ContextBridgeApi } from "../ts/preload.ts";

// 'api' needs to be declared global for typescript to know about it.
declare global {
  const api: ContextBridgeApi
}
