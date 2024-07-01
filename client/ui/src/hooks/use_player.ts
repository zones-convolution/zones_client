import { useEffect, useRef, useState } from "react";

import { PlayerIPC, PlayerResource, PlayerState } from "@/ipc/player_ipc";

interface IUsePlayer {
  playerState: PlayerState;
  togglePlaying: () => Promise<void>;
  toggleLooping: () => Promise<void>;
  selectResource: (resource: PlayerResource) => Promise<void>;
}

const usePlayer = (): IUsePlayer => {
  const { current: playerIPC } = useRef(new PlayerIPC());

  const [playerState, setPlayerState] = useState<PlayerState>(
    playerIPC.state.value,
  );

  const togglePlaying = async () => {
    await playerIPC.update((state) => ({ ...state, playing: !state.playing }));
  };

  const toggleLooping = async () => {
    setPlayerState({ ...playerState, looping: !playerState.looping });
    await playerIPC.update((state) => ({ ...state, looping: !state.looping }));
  };

  const selectResource = async (resource: PlayerResource) => {
    await playerIPC.update((state) => ({ ...state, resource: resource }));
  };

  useEffect(() => {
    let sub = playerIPC.state.subscribe({ next: (s) => setPlayerState(s) });
    return () => sub.unsubscribe();
  }, []);

  return {
    playerState: playerState,
    togglePlaying: togglePlaying,
    toggleLooping: toggleLooping,
    selectResource: selectResource,
  };
};

export { usePlayer };
