const S3_ENDPOINT = "https://minio.zonesconvolution.com";

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

const getImageUrl = (containerId: string, resourceId: string) => {
  return `${S3_ENDPOINT}/images-processed/${getS3ResourcePath({
    containerId: containerId,
    resourceId: resourceId,
    extension: "jpeg",
  })}`;
};

const getIrUrl = (
  zoneId: string,
  irId: string,
  position: "centre" | "left" | "right",
) => {
  return `${S3_ENDPOINT}/impulse-responses-processed/${getIrResourcePath(zoneId, irId, position)}`;
};

export { getIrUrl, getImageUrl };
