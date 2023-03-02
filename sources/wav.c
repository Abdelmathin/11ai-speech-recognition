/*

$ gcc -o play play.c
$ ./play audio.wav

*/

#include <sys/ioctl.h>
#include <sys/sysctl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define AUDIO_PORT 0x61

int main(int argc, char** argv) {
  // Check command line arguments
  if (argc != 2) {
    write(2, "Usage: ", 8);
    write(2, argv[0], strlen(argv[0]));
    write(2, " <filename>\n", 12);
    return 1;
  }

  // Enable access to audio port
  int mib[4] = {CTL_KERN, KERN_IOKIT, KERN_IOKIT_IOPORT, AUDIO_PORT};
  if (sysctl(mib, 4, NULL, NULL, NULL, 0) == -1) {
    perror("Error enabling access to audio port");
    return 1;
  }

  // Open WAV file
  int fd = open(argv[1], 0);
  if (fd == -1) {
    perror("Error opening file");
    return 1;
  }

  // Read WAV header
  char header[44];
  if (read(fd, header, 44) != 44) {
    perror("Error reading file");
    close(fd);
    return 1;
  }

  // Check if WAV file is PCM format
  if (header[34] != 1) {
    write(2, "Error: Unsupported WAV format\n", 31);
    close(fd);
    return 1;
  }

  // Enable audio hardware
  outb(inb(AUDIO_PORT) | 3, AUDIO_PORT);

  // Set audio hardware parameters
  int sampleRate = *(uint32_t*)(header + 24);
  outb(0xB6, 0x43);
  outb(0x34, 0x42);
  outb(sampleRate & 0xFF, 0x42);
  outb(sampleRate >> 8, 0x42);

  // Read audio data from file and write to audio hardware
  int16_t sample;
  while (read(fd, &sample, 2) == 2) {
    outb(sample & 0xFF, 0x42);
    outb(sample >> 8, 0x42);
  }

 // Close file and disable audio hardware
  close(fd);
  outb(inb(AUDIO_PORT) & ~3, AUDIO_PORT);

  return 0;
}


