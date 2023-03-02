#ifdef __APPLE__

void sound(int frequency) {
  asm("movb $0x61, %dx;"
       "inb %dx, %al;"
       "or $3, %al;"
       "movb %al, %ah;"
       "movb $0x61, %dx;"
       "outb %ah, %dx;"
       "movb $0xB6, %al;"
       "movb $0x43, %dx;"
       "outb %al, %dx;"
       "movb $0x34, %al;"
       "movb $0x42, %dx;"
       "outb %al, %dx;"
       "movb %bl, %al;"
       "movb $0x42, %dx;"
       "outb %al, %dx;"
       "movb %bh, %al;"
       "movb $0x42, %dx;"
       "outb %al, %dx"
      :
      : "b" (frequency & 0xFF), "b" (frequency >> 8));
}

#else
// original sound function goes here
#endif