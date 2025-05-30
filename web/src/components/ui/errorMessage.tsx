import { AlertCircle } from 'lucide-react'
import { Alert, AlertDescription, AlertTitle } from './alert'

export type ErrorMessageProps = {
  title?: string
  description: string
}

export const ErrorMessage = ({
  title = 'Error',
  description,
}: ErrorMessageProps) => (
  <Alert variant="destructive">
    <AlertCircle className="h-4 w-4" />
    <AlertTitle>{title}</AlertTitle>
    <AlertDescription>{description}</AlertDescription>
  </Alert>
)
