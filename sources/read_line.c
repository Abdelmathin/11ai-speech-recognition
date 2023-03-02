/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahabachi <ahabachi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/00/04 13:35:00 by ahabachi          #+#    #+#             */
/*   Updated: 2023/00/05 17:34:19 by ahabachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

char* read_line() {
  // Initialize variables
  char* line = NULL;
  size_t size = 0;

  // Save terminal attributes
  struct termios old_attr;
  tcgetattr(STDIN_FILENO, &old_attr);

  // Set terminal to raw mode
  struct termios raw_attr = old_attr;
  raw_attr.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_attr);

  // Read characters until newline or EOF
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
    // Append character to line
    line = realloc(line, size + 1);
    line[size++] = c;
  }

  // Append null terminator to line
  line = realloc(line, size + 1);
  line[size] = '\0';

  // Restore terminal attributes
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &old_attr);

  return line;
}

int main(int argc, char const *argv[])
{
  /* code */
  return 0;
}







