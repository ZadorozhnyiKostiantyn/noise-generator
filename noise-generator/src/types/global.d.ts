export type Nullable<T> = T | null;

export type VoidFn<T> = (value: T) => void;

export type BooleanFn = VoidFn<boolean>;