import * as flatbuffers from "flatbuffers";
import { useEffect, useState } from "react";

import { PlayerResource, PlayerState } from "@/lib/generated/zones";
import {
  addNativeEventListener,
  juce,
  removeNativeEventListener,
} from "@/lib/juce";

interface IPlayerState {
  looping: boolean;
  playing: boolean;
  resource: PlayerResource;
}

interface IUsePlayer {
  playerState: IPlayerState;
  togglePlaying: () => Promise<void>;
  toggleLooping: () => Promise<void>;
  selectResource: (resource: PlayerResource) => Promise<void>;
}

const playerUpdateNative = juce.getNativeFunction("player_update_native");
const getPlayerStateNative = juce.getNativeFunction("get_player_state");
const onPlayerUpdateNative = "on_player_update_native";

const usePlayer = (): IUsePlayer => {
  const [playerState, setPlayerState] = useState<IPlayerState>({
    playing: false,
    looping: false,
    resource: PlayerResource.Snare,
  });

  const playerUpdate = async (playerState: IPlayerState) => {
    try {
      let builder = new flatbuffers.Builder(1);
      let state = PlayerState.createPlayerState(
        builder,
        playerState.playing,
        playerState.looping,
        playerState.resource,
      );
      builder.finish(state);
      let buf = builder.asUint8Array();
      await playerUpdateNative({ data: buf });
    } catch (err) {
      console.error("OHH NOOOO flatbuffer :CCC @!!");
    }
  };

  const togglePlaying = async () => {
    await playerUpdate({
      ...playerState,
      playing: !playerState.playing,
    });
  };

  const toggleLooping = async () => {
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

  useEffect(() => {
    getPlayerState().then((s) => {
      console.log("UPDATED");
      console.log(s);
    });

    let registrationId = addNativeEventListener(
      onPlayerUpdateNative,
      (data: any) => {
        let buf = new flatbuffers.ByteBuffer(data);
        let playerStateObj = PlayerState.getRootAsPlayerState(buf).unpack();

        setPlayerState({
          playing: playerStateObj.playing,
          looping: playerStateObj.looping,
          resource: playerStateObj.resource,
        });
      },
    );

    return () => {
      removeNativeEventListener(registrationId);
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
