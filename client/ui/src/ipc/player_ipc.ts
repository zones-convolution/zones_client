import { Subject } from "rxjs";
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
  update: (playerState: PlayerState) => Promise<void>;
  state: Subject<PlayerState>;
}

class PlayerIPC implements IPlayerIPC {
  public readonly state: Subject<PlayerState> = new Subject<PlayerState>();

  constructor() {
    this.getPlayerState().then((playerState) => this.state.next(playerState));
    addNativeEventListener(onPlayerUpdateNative, this.handleReceivePlayerState);
  }

  private handleReceivePlayerState = (data: any): PlayerState => {
    return PlayerState.parse(JSON.parse(data));
  };

  private getPlayerState = async () => {
    return this.handleReceivePlayerState(await getPlayerStateNative());
  };

  update = async (playerState: PlayerState): Promise<void> => {
    await playerUpdateNative(JSON.stringify(playerState));
  };
}
