import { Device } from "@/managers/DeviceManager";
import clsx from "clsx";

export const DeviceIcon = ({
    device,
    onClick,
}: {
    device: Device;
    onClick: () => void;
}) => {
    return (
        <div className="flex w-fit flex-col items-center gap-1">
            <div
                className={clsx(
                    "h-28 w-28 cursor-pointer rounded-full ",
                    device.gradientClass,
                )}
                onClick={onClick}
            />
            <p className={clsx("cursor-pointer", device.textClass)} onClick={onClick}>
                {device.name}
            </p>
        </div>
    );
};
