import { Button } from '@/components/ui/button'
import { MicIcon, StopCircle, Loader2 } from 'lucide-react'

interface RecorderButtonsProps {
  isRecording: boolean
  isToggle: boolean
  onToggle: () => Promise<void>
}

const RecorderButtons = ({
  isRecording,
  isToggle,
  onToggle,
}: RecorderButtonsProps) => {
  if (!isRecording) {
    return (
      <Button
        size="lg"
        variant="destructive"
        onClick={onToggle}
        disabled={isToggle}
        className="flex items-center gap-2"
      >
        {isToggle ? (
          <>
            <Loader2 className="h-5 w-5 animate-spin" />
            Starting...
          </>
        ) : (
          <>
            <MicIcon className="h-6 w-6" />
            Start
          </>
        )}
      </Button>
    )
  }
  return (
    <Button
      size="lg"
      variant="destructive"
      onClick={onToggle}
      disabled={isToggle}
      className="flex items-center gap-2"
    >
      {isToggle ? (
        <>
          <Loader2 className="h-5 w-5 animate-spin" />
          Stopping...
        </>
      ) : (
        <>
          <StopCircle className="h-6 w-6" />
          Stop
        </>
      )}
    </Button>
  )
}

export default RecorderButtons
