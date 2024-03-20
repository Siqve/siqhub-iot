import { cookies } from "next/headers";


export const getCookie = (cookie: string) => {
    const cookieStore = cookies();
    return cookieStore.get(cookie);
};

export const hasCookie = (cookie: string) => {
    const cookieStore = cookies();
    return cookieStore.has(cookie)
};

export const setCookie = (cookie: string, data: string) => {
    const cookieStore = cookies();
    cookieStore.set(cookie, data);
};

export const deleteCookie = (cookie: string) => {
    const cookieStore = cookies();
    cookieStore.delete(cookie);
};
