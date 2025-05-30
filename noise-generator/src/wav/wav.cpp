#include "wav_utils.h"
#include "config/config.h"
#include <FS.h>
#include <SD.h>
#include <SPI.h>

bool ValidWavData(WavHeader_Struct *Wav)
{

    if (memcmp(Wav->RIFFSectionID, "RIFF", 4) != 0)
    {
        Serial.print("Invlaid data - Not RIFF format");
        return false;
    }
    if (memcmp(Wav->RiffFormat, "WAVE", 4) != 0)
    {
        Serial.print("Invlaid data - Not Wave file");
        return false;
    }
    if (memcmp(Wav->FormatSectionID, "fmt", 3) != 0)
    {
        Serial.print("Invlaid data - No format section found");
        return false;
    }
    if (memcmp(Wav->DataSectionID, "data", 4) != 0)
    {
        Serial.print("Invlaid data - data section not found");
        return false;
    }
    if (Wav->FormatID != 1)
    {
        Serial.print("Invlaid data - format Id must be 1");
        return false;
    }
    if (Wav->FormatSize != 16)
    {
        Serial.print("Invlaid data - format section size must be 16.");
        return false;
    }
    if ((Wav->NumChannels != 1) & (Wav->NumChannels != 2))
    {
        Serial.print("Invlaid data - only mono or stereo permitted.");
        return false;
    }
    if (Wav->SampleRate > 48000)
    {
        Serial.print("Invlaid data - Sample rate cannot be greater than 48000");
        return false;
    }
    if (Wav->BitsPerSample != 16)
    {
        Serial.print("Invlaid data - Only 16 bits per sample permitted.");
        return false;
    }
    return true;
}

void DumpWAVHeader(WavHeader_Struct *Wav)
{
    if (memcmp(Wav->RIFFSectionID, "RIFF", 4) != 0)
    {
        Serial.print("Not a RIFF format file - ");
        PrintData(Wav->RIFFSectionID, 4);
        return;
    }
    if (memcmp(Wav->RiffFormat, "WAVE", 4) != 0)
    {
        Serial.print("Not a WAVE file - ");
        PrintData(Wav->RiffFormat, 4);
        return;
    }
    if (memcmp(Wav->FormatSectionID, "fmt", 3) != 0)
    {
        Serial.print("fmt ID not present - ");
        PrintData(Wav->FormatSectionID, 3);
        return;
    }
    if (memcmp(Wav->DataSectionID, "data", 4) != 0)
    {
        Serial.print("data ID not present - ");
        PrintData(Wav->DataSectionID, 4);
        return;
    }
    // All looks good, dump the data
    Serial.print("Total size :");
    Serial.println(Wav->Size);
    Serial.print("Format section size :");
    Serial.println(Wav->FormatSize);
    Serial.print("Wave format :");
    Serial.println(Wav->FormatID);
    Serial.print("Channels :");
    Serial.println(Wav->NumChannels);
    Serial.print("Sample Rate :");
    Serial.println(Wav->SampleRate);
    Serial.print("Byte Rate :");
    Serial.println(Wav->ByteRate);
    Serial.print("Block Align :");
    Serial.println(Wav->BlockAlign);
    Serial.print("Bits Per Sample :");
    Serial.println(Wav->BitsPerSample);
    Serial.print("Data Size :");
    Serial.println(Wav->DataSize);
}

void PrintData(const char *Data, uint8_t NumBytes)
{
    for (uint8_t i = 0; i < NumBytes; i++)
        Serial.print(Data[i]);
    Serial.println();
}

void WriteWavHeader(File &file, uint32_t dataSize)
{
    uint16_t numChannels = 1;                 // mono
    uint32_t sampleRate = SAMPLE_RATE;        // 16000
    uint16_t bitsPerSample = BITS_PER_SAMPLE; // 16
    uint16_t audioFormat = 1;                 // PCM
    uint32_t byteRate = sampleRate * numChannels * bitsPerSample / 8;
    uint16_t blockAlign = numChannels * bitsPerSample / 8;

    // WAV header
    file.seek(0);
    file.write((const uint8_t *)"RIFF", 4);

    uint32_t chunkSize = 36 + dataSize;
    file.write((uint8_t *)&chunkSize, 4); // little endian
    file.write((const uint8_t *)"WAVE", 4);

    // fmt subchunk
    file.write((const uint8_t *)"fmt ", 4);
    uint32_t subChunk1Size = 16; // PCM
    file.write((uint8_t *)&subChunk1Size, 4);
    file.write((uint8_t *)&audioFormat, 2);
    file.write((uint8_t *)&numChannels, 2);
    file.write((uint8_t *)&sampleRate, 4);
    file.write((uint8_t *)&byteRate, 4);
    file.write((uint8_t *)&blockAlign, 2);
    file.write((uint8_t *)&bitsPerSample, 2);

    // data subchunk
    file.write((const uint8_t *)"data", 4);
    file.write((uint8_t *)&dataSize, 4);
}
