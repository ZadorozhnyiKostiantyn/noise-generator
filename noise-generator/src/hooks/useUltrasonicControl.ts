import { useMutation } from '@tanstack/react-query'
import { fetcher } from '@/lib/utils'
import { useNoiseGeneratorStore } from '@/stores/useNoiseGeneratorStore'

const toggleUltrasonic = async (isActive: boolean) => {
  return fetcher<void>('/toggle-ultrasonic', {
    method: 'POST',
    body: new URLSearchParams({
      isActive: isActive.toString(),
    }),
  })
}

export const useUltrasonicControl = () => {
  const { isUltrasonic, setIsUltrasonic } = useNoiseGeneratorStore()

  const {
    mutateAsync: toggle,
    isPending,
    isError,
  } = useMutation({
    mutationFn: toggleUltrasonic,
  })
  const handleToggle = async () => {
    await toggle(!isUltrasonic)
    setIsUltrasonic(!isUltrasonic)
  }

  return {
    isUltrasonic,
    toggle: handleToggle,
    isLoading: isPending,
    isError: isError,
  }
}
