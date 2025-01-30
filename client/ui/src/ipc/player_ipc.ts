import { z } from "zod";

import {
  addNativeEventListener,
  juce,
  removeNativeEventListener,
} from "@/lib/juce";

const PlayerResource = z.enum(["snare", "voice"]);
export type PlayerResource = z.infer<typeof PlayerResource>;

const PlayerState = z.object({
  looping: z.boolean(),
  playing: z.boolean(),
  resource: PlayerResource,
});

export type PlayerState = z.infer<typeof PlayerState>;

const playerUpdateNative = juce.getNativeFunction("player_update_native");
const getPlayerStateNative = juce.getNativeFunction("get_player_state_native");
const onPlayerUpdateNative = "on_player_update_native";

export const defaultPlayerState: PlayerState = {
  playing: false,
  looping: false,
  resource: "snare",
};

const handleReceivePlayerState = (data: any) => {
  try {
    return PlayerState.parse(JSON.parse(data));
  } catch (err) {
    console.error("Failed to parse PlayerIPC!", err);
  }

  return defaultPlayerState;
};

export const getPlayerState = async () => {
  return handleReceivePlayerState(await getPlayerStateNative());
};

export const updatePlayerState = async (state: PlayerState): Promise<void> => {
  await playerUpdateNative(JSON.stringify(state));
};

export const playerUpdateListener = (
  onUpdate: (state: PlayerState) => void,
) => {
  const listener = addNativeEventListener(onPlayerUpdateNative, (data: any) =>
    onUpdate(handleReceivePlayerState(data)),
  );

  return () => {
    removeNativeEventListener(listener);
  };
};
