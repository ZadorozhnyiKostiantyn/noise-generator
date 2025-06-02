import { useMutation } from '@tanstack/react-query';
import { fetcher } from '@/lib/utils';
import { useNoiseGeneratorStore } from '@/stores/useNoiseGeneratorStore';

const toggleRecoding = async (filename: string) => {
  if (!filename || typeof filename !== 'string') {
    throw new Error('Filename must be a valid string');
  }

  return fetcher<void>('/toggle-recording', {
    method: 'POST',
    body: new URLSearchParams({
      filename: filename,
    }),
  });
};

const useRecording = () => {
  const { isRecording, setIsRecording } = useNoiseGeneratorStore()
  const toggleRecordingMutation = useMutation({
    mutationFn: toggleRecoding,
  });

  const toggleRecordingHandler = async (filename: string) => {
    try {
      await toggleRecordingMutation.mutateAsync(filename);
      setIsRecording(true);
    } catch (error) {
      console.error('Error starting recording:', error);
    }
  };

  return {
    isRecording: isRecording,
    toggleRecordingHandler,
    isToggle: toggleRecordingMutation.isPending,
    toggleError: toggleRecordingMutation.isError,
  };
};

export default useRecording;