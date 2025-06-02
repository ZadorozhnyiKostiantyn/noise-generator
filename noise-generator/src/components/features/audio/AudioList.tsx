import { useAudioList } from '@/hooks/useAudioList'
import Audio from './Audio'
import AudioLoader from './AudioLoader'

export function AudioList() {
  const {
    data,
    activeAudioId,
    isLoading,
    isError,
    play,
    deleteAudio,
    playState,
  } = useAudioList()

  return (
    <AudioLoader isLoading={isLoading} isError={isError}>
      <div className="max-w-md mx-auto flex flex-col gap-2 min-w-[250px]">
        {data?.map((file) => (
          <Audio
            key={file.id}
            isActive={file.id === activeAudioId}
            isLoading={file.id === activeAudioId && playState.isPending}
            file={file}
            onClick={play}
            onDelete={deleteAudio}
          />
        ))}
      </div>
    </AudioLoader>
  )
}
