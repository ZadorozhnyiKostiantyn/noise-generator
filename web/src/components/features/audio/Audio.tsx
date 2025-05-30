import { Button } from '@/components/ui/button'
import { Card, CardTitle } from '@/components/ui/card'
import { Spinner } from '@/components/ui/spinner'
import {
  ContextMenu,
  ContextMenuTrigger,
  ContextMenuContent,
  ContextMenuItem,
} from '@/components/ui/context-menu' // Підключіть ContextMenu із shadcn
import type { AudioFile } from '@/hooks/useAudioList'
import { cn } from '@/lib/utils'
import { PlayIcon, PauseIcon, Trash2Icon } from 'lucide-react'

interface AudioProps {
  file: AudioFile
  onClick?: (id: AudioFile['id']) => void
  onDelete?: (id: AudioFile['id']) => void
  isActive: boolean
  isLoading: boolean
}

const Audio = ({
  file,
  onClick,
  onDelete,
  isActive = false,
  isLoading = false,
}: AudioProps) => {
  return (
    <ContextMenu>
      <ContextMenuTrigger>
        <Card
          key={file.id}
          className={cn(
            'bg-white cursor-pointer flex-row justify-between items-center gap-4 px-4 py-2 shadow-md rounded-lg transition-all duration-300 hover:shadow-lg hover:bg-blue-50',
            {
              'bg-blue-100 ring-2 ring-blue-500': isActive,
            },
          )}
          onClick={() => onClick?.(file.id)}
        >
          <CardTitle className="text-base font-semibold">{file.name}</CardTitle>
          <Button size="icon" variant="outline" className="rounded-full">
            {isLoading ? (
              <Spinner size={'small'} className="text-blue-500" />
            ) : isActive ? (
              <PauseIcon className="h-5 w-5 text-blue-500" />
            ) : (
              <PlayIcon className="h-5 w-5" />
            )}
          </Button>
        </Card>
      </ContextMenuTrigger>
      <ContextMenuContent>
        <ContextMenuItem
          onClick={() => onDelete?.(file.id)}
          className="flex items-center gap-2"
        >
          <Trash2Icon className="h-4 w-4 text-red-500" />
          Delete
        </ContextMenuItem>
      </ContextMenuContent>
    </ContextMenu>
  )
}

export default Audio
