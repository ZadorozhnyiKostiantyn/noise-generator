interface RecorderHeaderProps {
  isRecording: boolean
  isStarting: boolean
}

const RecorderHeader = ({ isRecording, isStarting }: RecorderHeaderProps) => {
  if (isRecording) {
    return <span className="text-red-500">Recording...</span>
  }

  if (isStarting) {
    return <span className="text-blue-500">Starting Recording...</span>
  }

  return <span className="text-gray-700">Recorder Ready</span>
}

export default RecorderHeader
