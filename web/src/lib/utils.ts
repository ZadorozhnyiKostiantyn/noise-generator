import {  clsx } from 'clsx'
import { twMerge } from 'tailwind-merge'
import type {ClassValue} from 'clsx';

export function cn(...inputs: Array<ClassValue>) {
  return twMerge(clsx(inputs))
}

export function createFetchInstance(baseURL: string, defaultOptions: RequestInit = {}) {
  return async <T>(endpoint: string, options: RequestInit = {}): Promise<T> => {
    const url = `${baseURL}${endpoint}`;
    const mergedOptions = { ...defaultOptions, ...options };
    const response = await fetch(url, mergedOptions);

    if (!response.ok) {
      throw new Error(`Fetch error: ${response.status} ${response.statusText}`);
    }

    const contentType = response.headers.get("Content-Type");
    if (contentType && contentType.includes("application/json")) {
      return response.json() as T;
    } else {
      return response.text() as unknown as T; // Якщо це текст, повертаємо його як T
    }
  };
}

export const BASE_URL = "http://192.168.4.1"
export const fetcher = createFetchInstance(BASE_URL);
