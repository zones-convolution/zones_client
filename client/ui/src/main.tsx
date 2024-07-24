import React from "react";
import ReactDOM from "react-dom/client";
import { ErrorBoundary } from "react-error-boundary";

import { Button } from "@/components/ui/button";

import Root from "@/pages/root";
import "@/styles/globals.css";

ReactDOM.createRoot(document.getElementById("root")!).render(
  <React.StrictMode>
    <ErrorBoundary
      fallbackRender={({ error, resetErrorBoundary }) => {
        return (
          <div
            id="error-page"
            className="flex flex-col gap-8 justify-center items-center h-screen"
          >
            <h1 className="text-4xl font-bold">Oops!</h1>
            <p>Sorry, an unexpected error has occurred.</p>
            <p className="text-destructive">
              <i>{error.message}</i>
            </p>
            <Button onClick={resetErrorBoundary}>Try again</Button>
          </div>
        );
      }}
      onReset={(details) => {}}
    >
      <Root />
    </ErrorBoundary>
  </React.StrictMode>,
);
