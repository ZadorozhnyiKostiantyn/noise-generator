import { Card, CardHeader, CardContent } from '@/components/ui/card'
import useRecording from '@/hooks/useRecording'
import RecorderHeader from './RecorderHeader'
import RecorderButtons from './RecorderButtons'
import RecorderError from './RecorderError'

const Recorder = () => {
  const { isRecording, isToggle, toggleError, toggleRecordingHandler } =
    useRecording()

  const handleToggle = async () => {
    const date = new Date()
    const options: Intl.DateTimeFormatOptions = {
      weekday: 'long',
      hour: '2-digit',
      minute: '2-digit',
      hour12: false,
    }
    const formattedDate = date.toLocaleString('en-US', options)
    const fileName = `${formattedDate}, ${date.getTime()}`
    await toggleRecordingHandler(fileName)
  }

  return (
    <Card className="gap-0">
      <CardHeader className="gap-4">
        <div className="text-lg font-semibold text-center">
          <RecorderHeader isRecording={isRecording} isStarting={isToggle} />
        </div>
      </CardHeader>
      <CardContent className="space-y-2">
        <div className="flex justify-center">
          <RecorderButtons
            isRecording={isRecording}
            isToggle={isToggle}
            onToggle={handleToggle}
          />
        </div>
        <RecorderError startError={toggleError} stopError={toggleError} />
      </CardContent>
    </Card>
  )
}

export default Recorder
