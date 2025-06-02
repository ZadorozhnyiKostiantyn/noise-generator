import { fetcher } from '@/lib/utils'
import { useNoiseGeneratorStore } from '@/stores/useNoiseGeneratorStore'
import { useMutation } from '@tanstack/react-query'

export type AudioFile = {
  id: number
  name: string
}

const togglePlaybackAudio = async (id: number) => {
  return fetcher<void>('/toggle-playback', {
    method: 'POST',
    body: new URLSearchParams({
      id: id.toString(),
    }),
  })
}

export const useAudioList = () => {
  const { audioFiles, setAudioFiles, activeAudioId, setActiveAudioId } =
    useNoiseGeneratorStore()

  const playMutation = useMutation({
    mutationFn: togglePlaybackAudio,
  })

  const deleteMutation = useMutation({
    mutationFn: async (id: number) => {
      return fetcher<void>(`/delete-audio/${id}`, {
        method: 'DELETE',
      })
    },
    onSuccess: (_, id) => {
      if(!audioFiles) return;
      setAudioFiles(audioFiles.filter((file) => file.id !== id))
    },
  })

  const handleStop = async (id: AudioFile['id']) => {
    setActiveAudioId(null)
    await playMutation.mutateAsync(id)
  }

  const handlePlay = async (id: AudioFile['id']) => {
    if (id === activeAudioId) return handleStop(id)
    setActiveAudioId(id)
    await playMutation.mutateAsync(id)
  }

  const handleDelete = async (id: AudioFile['id']) => {
    await deleteMutation.mutateAsync(id)
  }

  return {
    data: audioFiles ?? [],
    isLoading: !audioFiles,
    isError: false,
    activeAudioId,
    play: handlePlay,
    stop: handleStop,
    deleteAudio: handleDelete,
    playState: playMutation,
    deleteState: deleteMutation,
  }
}
