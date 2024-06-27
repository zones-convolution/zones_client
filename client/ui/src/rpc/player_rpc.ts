import { z } from "zod";

import {
  addNativeEventListener,
  juce,
  removeNativeEventListener,
} from "@/lib/juce";

const PlayerResource = z.enum(["snare", "numbers"]);
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

export interface IPlayerRPC {
  update: (playerState: PlayerState) => Promise<void>;
  state: PlayerState;
}
