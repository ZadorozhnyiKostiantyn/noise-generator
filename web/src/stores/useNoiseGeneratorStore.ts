import { type Nullable } from './../types/global.d'
import type { AudioFile } from '@/hooks/useAudioList'
import type { BooleanFn, VoidFn } from '@/types/global'
import { create } from 'zustand'

export interface ApiNoiseGeneratorState {
  activeAudioId: Nullable<AudioFile['id']>
  audioFiles:  AudioFile[]
  isRecording: boolean
  isUltrasonic: boolean
}

export interface NoiseGeneratorState extends Omit<ApiNoiseGeneratorState, 'audioFiles'> {
  audioFiles:  Nullable<AudioFile[]>
  setAudioFiles: VoidFn<AudioFile[]>
  setActiveAudioId: VoidFn<Nullable<AudioFile['id']>>
  setIsRecording: BooleanFn
  setIsUltrasonic: BooleanFn
}

export const useNoiseGeneratorStore = create<NoiseGeneratorState>((set) => ({
  activeAudioId: null,
  audioFiles: [],
  isRecording: false,
  isUltrasonic: false,
  setActiveAudioId: (id: Nullable<AudioFile['id']>) => set({ activeAudioId: id }),
  setAudioFiles: (files: AudioFile[]) => set({ audioFiles: files }),
  setIsRecording: (val: boolean) => set({ isRecording: val }),
  setIsUltrasonic: (val: boolean) => set({ isUltrasonic: val }),
}))
