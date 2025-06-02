import { Switch } from '@/components/ui/switch'
import {
  Card,
  CardHeader,
  CardTitle,
  CardDescription,
} from '@/components/ui/card'
import { useUltrasonicControl } from '@/hooks/useUltrasonicControl'
import { Spinner } from '@/components/ui/spinner'

const UltrasonicControl = () => {
  const { isUltrasonic, toggle, isLoading, isError } = useUltrasonicControl()

  return (
    <Card className="p-4">
      <CardHeader className="p-0 flex flex-row items-center justify-between">
        <div className="flex flex-col gap-2">
          <CardTitle>Ultrasonic</CardTitle>
          <CardDescription>
            Control the ultrasonic signal. Turn it on or off as needed.
          </CardDescription>
          {isError && (
            <p className="text-sm text-red-500">Failed to toggle ultrasonic.</p>
          )}
        </div>
        <div className="flex items-center gap-2">
          {isLoading && (
            <Spinner size={'small'} className="animate-spin text-gray-500" />
          )}
          <Switch
            checked={isUltrasonic}
            onCheckedChange={toggle}
            disabled={isLoading}
          />
        </div>
      </CardHeader>
    </Card>
  )
}

export default UltrasonicControl
