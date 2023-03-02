#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

// Structure for the RIFF chunk header
typedef struct {
  uint32_t id;
  uint32_t size;
  uint32_t type;
} RIFF_header;

// Structure for the 'LIST' chunk
typedef struct {
  uint32_t id;
  uint32_t size;
  uint32_t type;
} LIST_chunk;

// Structure for the 'INFO' chunk
typedef struct {
  uint32_t id;
  uint32_t size;
  char data[0];
} INFO_chunk;

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s file.wav\n", argv[0]);
    return 1;
 // Open the file for reading and writing
  int fd = open(filename, O_RDWR);
  if (fd < 0) {
    perror("open");
    return 1;
  }

  // Read the RIFF chunk header
  RIFF_header header;
  ssize_t n = read(fd, &header, sizeof(header));
  if (n < (ssize_t)sizeof(header)) {
    perror("read");
    return 1;
  }

  // Check the file type
  if (header.id != 0x46464952 || header.type != 0x45564157) {  // "RIFF" and "WAVE"
    fprintf(stderr, "Error: not a .wav file\n");
    return 1;
  }

  // Read the 'LIST' chunk
  LIST_chunk list_chunk;
  n = read(fd, &list_chunk, sizeof(list_chunk));
  if (n < (ssize_t)sizeof(list_chunk)) {
    perror("read");
    return 1;
  }

  // Check the chunk type
  if (list_chunk.id != 0x5453494C || list_chunk.type != 0x4F464E49) {  // "LIST" and "INFO"
    fprintf(stderr, "Error: 'LIST' chunk not found\n");

 // Check the chunk type
  if (list_chunk.id != 0x5453494C || list_chunk.type != 0x4F464E49) {  // "LIST" and "INFO"
    fprintf(stderr, "Error: 'LIST' chunk not found\n");
    return 1;
  }

  // Read the 'INFO' chunk
  INFO_chunk info_chunk;
  n = read(fd, &info_chunk, sizeof(info_chunk));
  if (n < (ssize_t)sizeof(info_chunk)) {
    perror("read");
    return 1;
  }

  // Check the chunk type
  if (info_chunk.id != 0x4F464E49) {  // "INFO"
    fprintf(stderr, "Error: 'INFO' chunk not found\n");
    return 1;
  }

  // Find the 'ICOM' subchunk
  while (info_chunk.id != 0x4D4F4349) {  // "ICOM"
    // Seek to the next subchunk
    off_t pos = lseek(fd, info_chunk.size, SEEK_CUR);
    if (pos < 0) {
      perror("lseek");
      return 1;
    }

    // Read the next subchunk header
    n = read(fd, &info_chunk, sizeof(info_chunk));
    if (n < (ssize_



  