import React, { useEffect, type FC } from 'react'
import {
  useNoiseGeneratorStore,
  type ApiNoiseGeneratorState,
} from '@/stores/useNoiseGeneratorStore'
import { BASE_URL, fetcher } from '@/lib/utils'
import { useQuery } from '@tanstack/react-query'

export type NoiseGeneratorProviderProps = { children: React.ReactNode }

const fetchAudioFiles = async () => {
  return fetcher<ApiNoiseGeneratorState>('/state')
}

const NoiseGeneratorProvider: FC<NoiseGeneratorProviderProps> = ({
  children,
}) => {
  const { setIsRecording, setActiveAudioId, setAudioFiles, setIsUltrasonic } =
    useNoiseGeneratorStore()

  // Використовуємо useQuery для початкового фетчингу даних
  const { data, isSuccess } = useQuery({
    queryKey: ['noise-generator-state'],
    queryFn: fetchAudioFiles,
  })

  useEffect(() => {
    if (isSuccess) {
      setIsRecording(data.isRecording)
      setActiveAudioId(data.activeAudioId)
      setAudioFiles(data.audioFiles)
      setIsUltrasonic(data.isUltrasonic)
    }
  }, [isSuccess])

  // Підписка на SSE
  useEffect(() => {
    const eventSource = new EventSource(`${BASE_URL}/events`)

    eventSource.addEventListener('active-audio-id', (event) => {
      const id = Number(event.data)
      if (id === -1) {
        setActiveAudioId(null)
        return
      }
      setActiveAudioId(id)
    })

    eventSource.addEventListener('audio-files', (event) => {
      setAudioFiles(JSON.parse(event.data))
    })

    eventSource.addEventListener('is-recording', (event) => {
      console.log('event record', event);
      setIsRecording(event.data === "true")
    })

    eventSource.addEventListener('is-ultrasonic', (event) => {
      setIsUltrasonic(event.data === "true")
    })

    eventSource.onmessage = (event) => {
      console.log('event', event)

      try {
        const parsedData = JSON.parse(event.data)

        switch (event.type) {
          case 'is-recording':
            setIsRecording(parsedData === 'true')
            break
          case 'active-audio-id':
            const id = parseInt(parsedData)
            setActiveAudioId(id !== -1 ? id : null)
            break
          case 'is-ultrasonic':
            setIsUltrasonic(parsedData === 'true')
            break
          case 'audio-files':
            setAudioFiles(parsedData)
            break
          default:
            console.warn(`Unknown SSE event type: ${event.type}`)
        }
      } catch (error) {
        console.error('Failed to process SSE message:', error)
      }
    }

    eventSource.onerror = () => {
      console.error('SSE connection failed.')
      eventSource.close()
    }

    return () => {
      eventSource.close()
    }
  }, [setIsRecording, setActiveAudioId, setAudioFiles, setIsUltrasonic])

  return <>{children}</>
}

export default NoiseGeneratorProvider
