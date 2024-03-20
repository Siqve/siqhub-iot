import { cookies } from "next/headers";

const cookieStore = cookies();

export const getCookie = (cookie: string) => {
    return cookieStore.get(cookie);
};

export const hasCookie = (cookie: string) => {
    return cookieStore.has(cookie)
};

export const setCookie = (cookie: string, data: string) => {
    // const cookieStore = cookies();
    cookieStore.set(cookie, data);
};

export const deleteCookie = (cookie: string) => {
    // const cookieStore = cookies();
    cookieStore.delete(cookie);
};
