import { ErrorMessage } from "@/components/ui/errorMessage"


interface RecorderErrorProps {
  startError: boolean
  stopError: boolean
}

const RecorderError = ({ startError, stopError }: RecorderErrorProps) => {
  if (!startError && !stopError) return null

  return (
    <div className="space-y-2">
      {startError && (
        <ErrorMessage description="Failed to start recording. Please try again." />
      )}
      {stopError && (
        <ErrorMessage description="Failed to stop recording. Please try again." />
      )}
    </div>
  )
}

export default RecorderError
