import { ArrowLeft } from "@phosphor-icons/react/dist/ssr";
import resolveConfig from "tailwindcss/resolveConfig";
import tailwindConfig from "../../../tailwind.config";
import Link from "next/link";
import { Config } from "tailwindcss";

export const DeviceHeader = ({ name }: { name: string }) => {
    const fullConfig = resolveConfig(tailwindConfig);
    type configWithCustomColor = typeof fullConfig & {
        theme: { colors: { text: { main: string } } };
    };

    return (
        <div className="flex items-center gap-4 border-b pt-3">
            <Link href="/">
                <ArrowLeft
                    color={
                        (fullConfig as configWithCustomColor).theme.colors.text
                            .main
                    }
                    size="54"
                />
            </Link>
            <h2 className="text-5xl">{name}</h2>
        </div>
    );
};
