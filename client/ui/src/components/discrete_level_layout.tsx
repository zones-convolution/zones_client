import { FC, Fragment } from "react";

import { Separator } from "@/components/ui/separator";

const getLinearValue = (dbValue: number) => {
  return Math.pow(10, dbValue / 20);
};

const getSkewedValue = (value: number) => {
  return Math.log10(1 + 9 * value);
};

const LevelLabel: FC<{ level: number }> = ({ level }) => {
  const value = getSkewedValue(getLinearValue(level));
  return (
    <div
      className="absolute font-thin text-sm w-full text-center"
      style={{
        top: `${(1 - value) * 100}%`,
        transform: "translateY(-50%)",
      }}
    >
      {level}
    </div>
  );
};

const InvisibleLabel: FC<{ level: number }> = ({ level }) => (
  <div className="invisible font-thin text-sm">{level}</div>
);

const LevelSeparator: FC<{ level: number }> = ({ level }) => {
  const value = getSkewedValue(getLinearValue(level));

  return (
    <div
      className="absolute w-full"
      style={{
        top: `${(1 - value) * 100}%`,
        transform: "translateY(-50%)",
      }}
    >
      <Separator className="shrink" />
    </div>
  );
};

const LevelLabels: FC<{ levels: number[] }> = ({ levels }) => {
  return (
    <div className="relative">
      {levels.map((level, index) => (
        <Fragment key={index}>
          <LevelLabel key={`label-${index}`} level={level} />
          <InvisibleLabel key={`invisible-${index}`} level={level} />
        </Fragment>
      ))}
    </div>
  );
};

export const DiscreteLevelLayout: FC<{
  levels: number[];
  showLabels: boolean;
}> = ({ levels, showLabels }) => {
  return (
    <div className="w-full h-full flex flex-row gap-1">
      {showLabels && <LevelLabels levels={levels} />}
      <div className="relative flex-grow">
        {levels.map((level, index) => (
          <LevelSeparator key={`separator-${index}`} level={level} />
        ))}
      </div>
    </div>
  );
};
