// src/App.tsx
import { AudioList } from './components/features/audio/AudioList';
import Recorder from './components/features/recorder/Recorder';
import UltrasonicControl from './components/features/ultrasonic/UltrasonicControl';

function App() {
  return (
    <div className="min-h-screen flex flex-col items-center bg-gray-50 p-4">
      <div className="w-full max-w-md bg-white rounded-lg shadow-md p-4 space-y-4">
        <Recorder />
        <UltrasonicControl />
        <AudioList />
      </div>
    </div>
  );
}

export default App;
