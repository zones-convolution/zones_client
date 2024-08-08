import axios from "axios";

import { Config } from "@/lib/config";

const instance = axios.create({
  baseURL: Config.API_BASE_URL,
  timeout: 40000,
  headers: {
    Accept: "application/json, text/plain, /",
    "Content-Type": "application/json; charset=utf-8",
  },
});

export const fetcher = async (url: string) => {
  return instance.get(url).then((res) => {
    if (!res.data) {
      throw Error(res.data.message);
    }

    return res.data;
  });
};

export default instance;
