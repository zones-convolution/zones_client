const createAbortError = () => {
  if (typeof DOMException !== "undefined") {
    return new DOMException("The operation was aborted.", "AbortError");
  }

  const error = new Error("The operation was aborted.");
  error.name = "AbortError";
  return error;
};

export const withAbort = <T>(promise: PromiseLike<T> | T, signal?: AbortSignal) => {
  const pendingPromise = Promise.resolve(promise);

  if (!signal) return pendingPromise;
  if (signal.aborted) return Promise.reject(createAbortError());

  return new Promise<T>((resolve, reject) => {
    const handleAbort = () => {
      reject(createAbortError());
    };

    signal.addEventListener("abort", handleAbort, { once: true });

    pendingPromise.then(
      (value) => {
        signal.removeEventListener("abort", handleAbort);
        resolve(value);
      },
      (error) => {
        signal.removeEventListener("abort", handleAbort);
        reject(error);
      },
    );
  });
};

export const isAbortError = (error: unknown) => {
  return error instanceof Error && error.name === "AbortError";
};
