import React from "react";
import ReactDOM from "react-dom/client";
import { createMemoryRouter, RouterProvider } from "react-router-dom";

import Browser from "@/pages/browser";
import BrowserRoot from "@/pages/browser_root";
import CreateZone from "@/pages/create_zone";
import ErrorPage from "@/pages/error_page";
import Root from "@/pages/root";
import "@/styles/globals.css";

const router = createMemoryRouter([
  {
    path: "/",
    element: <Root />,
    errorElement: <ErrorPage />,
    children: [
      {
        path: "/",
        element: <BrowserRoot />,
        children: [
          {
            path: "/",
            element: <Browser />,
          },
          {
            path: "/create",
            element: <CreateZone />,
          },
        ],
      },
    ],
  },
]);

ReactDOM.createRoot(document.getElementById("root")!).render(
  <React.StrictMode>
    <RouterProvider router={router} />
  </React.StrictMode>,
);
