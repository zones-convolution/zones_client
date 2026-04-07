import { useEffect, useState } from "react";

import {
  defaultPlayerState,
  PlayerResource,
  PlayerState,
  playerUpdateListener,
  updatePlayerState,
  getPlayerState,
} from "@/ipc/player_ipc";
import { isAbortError } from "@/lib/abortable";

interface IUsePlayer {
  playerState: PlayerState;
  togglePlaying: () => Promise<void>;
  toggleLooping: () => Promise<void>;
  selectResource: (resource: PlayerResource) => Promise<void>;
}

const usePlayer = (): IUsePlayer => {
  const [playerState, setPlayerState] =
    useState<PlayerState>(defaultPlayerState);

  const togglePlaying = async () => {
    await updatePlayerState({ ...playerState, playing: !playerState.playing });
  };

  const toggleLooping = async () => {
    let newState: PlayerState = {
      ...playerState,
      looping: !playerState.looping,
    };

    setPlayerState(newState);
    await updatePlayerState(newState);
  };

  const selectResource = async (resource: PlayerResource) => {
    await updatePlayerState({ ...playerState, resource: resource });
  };

  useEffect(() => {
    const controller = new AbortController();
    const unsubscribe = playerUpdateListener(setPlayerState);

    getPlayerState({ signal: controller.signal })
      .then(setPlayerState)
      .catch((error) => {
        if (!isAbortError(error)) console.error(error);
      });

    return () => {
      controller.abort();
      unsubscribe();
    };
  }, []);

  return {
    playerState: playerState,
    togglePlaying: togglePlaying,
    toggleLooping: toggleLooping,
    selectResource: selectResource,
  };
};

export { usePlayer };
