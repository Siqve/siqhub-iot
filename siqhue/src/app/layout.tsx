import type { Metadata } from "next";
import { Roboto } from "next/font/google";
import "../styles/reset.css";
import "../styles/globals.css";
import { ReactNode } from "react";

const roboto = Roboto({
    weight: "400",
    subsets: ["latin"],
});

export const metadata: Metadata = {
    title: "SiqHue",
    description: "An LED Strip controller",
};

const RootLayout = ({
    children,
}: Readonly<{
    children: ReactNode;
}>) => {
    return (
        <html lang="en">
            <body className={roboto.className}>
                <main>{children}</main>
            </body>
        </html>
    );
};

export default RootLayout;
