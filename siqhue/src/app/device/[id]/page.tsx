import { getDevice } from "@/managers/DeviceManager";
import { redirect } from "next/navigation";
import { ArrowLeft } from "@phosphor-icons/react/dist/ssr";
import resolveConfig from "tailwindcss/resolveConfig";
import tailwindConfig from "../../../../tailwind.config";
import { DeviceHeader } from "@components/device/DeviceHeader";

const DevicePage = async ({ params }: { params: { id: string } }) => {
    const device = getDevice(params.id);
    if (!device) redirect("/");

    return (
        <div>
            <DeviceHeader name={device.name}/>
        </div>
    );
};

export default DevicePage;
