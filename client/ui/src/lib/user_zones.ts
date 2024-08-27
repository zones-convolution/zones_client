import { ZoneMetadata } from "@/hooks/zone_metadata";

const getUserZoneGradient = (zoneMetadata: ZoneMetadata): string => {
  let title = zoneMetadata.title ?? "";
  let description = zoneMetadata.description ?? "";

  const { irs } = zoneMetadata;

  const generateHash = (str: string): number => {
    let hash = 0;
    for (let i = 0; i < str.length; i++) {
      hash = (hash << 5) - hash + str.charCodeAt(i);
      hash |= 0;
    }
    return Math.abs(hash);
  };

  const titleHash = generateHash(title);
  const descriptionHash = generateHash(description);

  const titleColor1 = `#${((titleHash * 123456) % 0xffffff).toString(16).padStart(6, "0")}`;
  const titleColor2 = `#${((titleHash * 654321) % 0xffffff).toString(16).padStart(6, "0")}`;

  const descriptionColor1 = `#${((descriptionHash * 789012) % 0xffffff).toString(16).padStart(6, "0")}`;
  const descriptionColor2 = `#${((descriptionHash * 345678) % 0xffffff).toString(16).padStart(6, "0")}`;

  const irCount = irs.length;
  const brightnessFactor = Math.min(1, Math.max(0.5, irCount / 10));

  const adjustBrightness = (color: string, factor: number): string => {
    const r = Math.round(parseInt(color.substring(1, 3), 16) * factor);
    const g = Math.round(parseInt(color.substring(3, 5), 16) * factor);
    const b = Math.round(parseInt(color.substring(5, 7), 16) * factor);
    return `#${r.toString(16).padStart(2, "0")}${g.toString(16).padStart(2, "0")}${b.toString(16).padStart(2, "0")}`;
  };

  const brightenedTitleColor2 = adjustBrightness(titleColor2, brightnessFactor);
  const brightenedDescriptionColor2 = adjustBrightness(
    descriptionColor2,
    brightnessFactor,
  );

  return `linear-gradient(135deg, ${titleColor1}, ${brightenedTitleColor2}, ${descriptionColor1}, ${brightenedDescriptionColor2})`;
};

export { getUserZoneGradient };
