import React from "react";
import ReactDOM from "react-dom/client";
import { createMemoryRouter, RouterProvider } from "react-router-dom";

import Browser from "@/browser";
import CreateZone from "@/create_zone";
import Editor from "@/editor";
import ErrorPage from "@/error_page";
import Preferences from "@/preferences";
import Root from "@/root";
import "@/styles/globals.css";

const router = createMemoryRouter([
  {
    path: "/",
    element: <Root />,
    errorElement: <ErrorPage />,
    children: [
      {
        path: "/",
        element: <Editor />,
      },
      {
        path: "/browser",
        element: <Browser />,
      },
      {
        path: "/preferences",
        element: <Preferences />,
      },
      {
        path: "/create",
        element: <CreateZone />,
      },
    ],
  },
]);
ReactDOM.createRoot(document.getElementById("root")!).render(
  <React.StrictMode>
    <RouterProvider router={router} />
  </React.StrictMode>,
);
