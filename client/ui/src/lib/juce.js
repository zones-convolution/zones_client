export * as juce from "juce-framework-frontend";

export const addNativeEventListener = (eventId, fn) =>
  window.__JUCE__.backend.addEventListener(eventId, fn);

export const removeNativeEventListener = (registrationId) =>
  window.__JUCE__.backend.removeEventListener(registrationId);
