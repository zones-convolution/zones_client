import { BehaviorSubject, Subject } from "rxjs";
import { z } from "zod";

import { addNativeEventListener, juce } from "@/lib/juce";

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

export interface IPlayerIPC {
  update: (set: (state: PlayerState) => PlayerState) => Promise<void>;
  state: BehaviorSubject<PlayerState>;
}

export class PlayerIPC implements IPlayerIPC {
  public readonly state: BehaviorSubject<PlayerState> =
    new BehaviorSubject<PlayerState>({
      playing: false,
      looping: false,
      resource: "snare",
    });

  constructor() {
    this.getPlayerState().then(this.handleReceivePlayerState);
    addNativeEventListener(onPlayerUpdateNative, this.handleReceivePlayerState);
  }

  private handleReceivePlayerState = (data: any) => {
    this.state.next(PlayerState.parse(JSON.parse(data)));
  };

  private getPlayerState = async () => {
    return this.handleReceivePlayerState(await getPlayerStateNative());
  };

  update = async (set: (state: PlayerState) => PlayerState): Promise<void> => {
    await playerUpdateNative(JSON.stringify(set(this.state.value)));
  };
}
