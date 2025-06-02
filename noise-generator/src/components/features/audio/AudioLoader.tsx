import { ErrorMessage } from '@/components/ui/errorMessage'
import { Skeleton } from '@/components/ui/skeleton'
import type { ReactNode } from 'react'

interface AudioLoaderProps {
  isLoading?: boolean
  isError?: boolean
  children: ReactNode
}

const AudioLoader = ({ isLoading, isError, children }: AudioLoaderProps) => {
  if (isError) {
    return (
      <ErrorMessage description="Failed to load audio files. Please try again later." />
    )
  }

  if (isLoading) {
    return (
      <div className="max-w-md mx-auto space-y-4 min-w-[250px]">
        {[...Array(5)].map((_, index) => (
          <Skeleton key={index} className="h-12 w-full rounded-lg" />
        ))}
      </div>
    )
  }

  return <>{children}</>
}

export default AudioLoader
