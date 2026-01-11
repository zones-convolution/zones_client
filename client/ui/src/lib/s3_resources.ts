import { Config } from "@/lib/config";

export interface IS3ResourcePath {
  containerId: string;
  resourceId: string;
  extension: string;
}

const getS3ResourcePath = ({
  containerId,
  resourceId,
  extension,
}: IS3ResourcePath): string => `${containerId}/${resourceId}.${extension}`;

const getIrResourcePath = (
  zoneId: string,
  irId: string,
  position: "centre" | "left" | "right",
) => {
  return getS3ResourcePath({
    containerId: zoneId,
    resourceId: `${irId}/${position}`,
    extension: "wav",
  });
};

const getImageUrl = (zoneId: string, imageId: string) => {
  return `${Config.S3_HOST}/images-processed/${getS3ResourcePath({
    containerId: zoneId,
    resourceId: imageId,
    extension: "jpeg",
  })}`;
};
const getIrUrl = (
  zoneId: string,
  irId: string,
  position: "centre" | "left" | "right",
) => {
  return `${Config.S3_HOST}/impulse-responses-processed/${getIrResourcePath(zoneId, irId, position)}`;
};

const getProfileImageUrl = (userId: string) => {
  return `${Config.S3_HOST}/profile-pictures-processed/${userId}.jpeg`;
};

export { getIrUrl, getImageUrl, getProfileImageUrl };
