import { useEffect, useState } from "react";

import {
  defaultPlayerState,
  PlayerResource,
  PlayerState,
  playerUpdateListener,
  updatePlayerState,
} from "@/ipc/player_ipc";

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
    return playerUpdateListener((s) => setPlayerState(s));
  }, []);

  return {
    playerState: playerState,
    togglePlaying: togglePlaying,
    toggleLooping: toggleLooping,
    selectResource: selectResource,
  };
};

export { usePlayer };
