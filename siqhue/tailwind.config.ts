import type {Config} from "tailwindcss";

const colors = {
    c_gray: {
        800: "#171312",
        700: "#242424",
        500: "#3D3D3D",
        300: "#525252",
        100: "#666666",
    },
    c_purple: {
        700: "#18122B",
        500: "#393053",
        300: "#443C68",
        100: "#635985",
    },
    c_violet: {
        500: "#A8A0E4",
        300: "#BEB8EB",
        100: "#E2DFF6"
    },
    c_pink: {
        700: "#F2404F",
        500: "#F45B69",
        300: "#F78D95",
        100: "#FAB3B9",
    },
    c_blue: {
        900: "#050610",
        800: "#0F112F",
        700: "#181C4E",
        500: "#262C7D",
        400: "#353DAC",
        300: "#636ACF",
        200: "#8288D9",
        100: "#A2A6E2",
    }
}

const config: Config = {
    content: [
        "./src/pages/**/*.{js,ts,jsx,tsx,mdx}",
        "./src/components/**/*.{js,ts,jsx,tsx,mdx}",
        "./src/containers/**/*.{js,ts,jsx,tsx,mdx}",
        "./src/managers/**/*.{js,ts,jsx,tsx,mdx}",
        "./src/app/**/*.{js,ts,jsx,tsx,mdx}",
    ],
    theme: {
        extend: {
            backgroundImage: {
                "gradient-radial": "radial-gradient(var(--tw-gradient-stops))",
                "gradient-conic":
                    "conic-gradient(from 180deg at 50% 50%, var(--tw-gradient-stops))",
            },
            colors: {
                ...colors,
                background: {
                    main: colors.c_gray["700"],
                },
                text: {
                    main: colors.c_violet["300"],
                    "header-main": colors.c_violet["100"],
                    complimentary: colors.c_pink["500"],
                },
            },
        },
    },
    plugins: [],
};
export default config;
