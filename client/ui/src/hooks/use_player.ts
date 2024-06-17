import { useEffect, useState } from "react";
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
type PlayerState = z.infer<typeof PlayerState>;

interface IUsePlayer {
  playerState: PlayerState;
  togglePlaying: () => Promise<void>;
  toggleLooping: () => Promise<void>;
  selectResource: (resource: PlayerResource) => Promise<void>;
}

const playerUpdateNative = juce.getNativeFunction("player_update_native");
const getPlayerStateNative = juce.getNativeFunction("get_player_state_native");
const onPlayerUpdateNative = "on_player_update_native";

const usePlayer = (): IUsePlayer => {
  const [playerState, setPlayerState] = useState<PlayerState>({
    playing: false,
    looping: false,
    resource: "snare",
  });

  const playerUpdate = async (playerState: PlayerState) => {
    await playerUpdateNative(JSON.stringify(playerState));
  };

  const togglePlaying = async () => {
    await playerUpdate({
      ...playerState,
      playing: !playerState.playing,
    });
  };

  const toggleLooping = async () => {
    setPlayerState({ ...playerState, looping: !playerState.looping });
    await playerUpdate({
      ...playerState,
      looping: !playerState.looping,
    });
  };

  const selectResource = async (resource: PlayerResource) => {
    await playerUpdate({ ...playerState, resource: resource });
  };

  const getPlayerState = async () => {
    return await getPlayerStateNative();
  };

  const handleReceivePlayerState = (data: any) => {
    try {
      setPlayerState(PlayerState.parse(JSON.parse(data)));
    } catch (err) {
      console.error("Failed to parse PlayerState!", err);
    }
  };

  useEffect(() => {
    getPlayerState().then(handleReceivePlayerState);

    let registrationId = addNativeEventListener(
      onPlayerUpdateNative,
      handleReceivePlayerState,
    );

    return () => {
      removeNativeEventListener(registrationId);
    };
  }, []);

  useEffect(() => {
    fetch(juce.getBackendResourceAddress("userzones.json"))
      .then((res) => {
        res.json().then((data) => {
          console.log(data);
        });
      })
      .catch((err) => {
        console.error(err);
      });
  }, []);

  return {
    playerState: playerState,
    togglePlaying: togglePlaying,
    toggleLooping: toggleLooping,
    selectResource: selectResource,
  };
};

export { usePlayer };
