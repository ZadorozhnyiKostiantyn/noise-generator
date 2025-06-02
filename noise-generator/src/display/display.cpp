#include <SSD1306Wire.h>

SSD1306Wire display(0x3c, SDA, SCL);

void setupDisplay() {
  display.init(); // Initialize the OLED display
  display.clear(); // Clear the display buffer
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setColor(WHITE);
  display.drawString(0, 0, "Hello!");
  display.display(); // Actually display all of the above
}

void drawMessage(String message) {
  display.clear();
  int16_t x = 0;
  int16_t y = 0;
  int lineHeight = 30;  // залежить від розміру шрифту

  String line = "";
  for (int i = 0; i < message.length(); i++) {
    char c = message[i];
    line += c;
    
    // Якщо рядок став занадто довгим або зустріли перенос
    if (display.getStringWidth(line) > 127 || c == '\n') {
      display.drawString(x, y, line);
      y += lineHeight;
      line = "";
    }
  }

  // Виводимо залишок (останній рядок)
  if (line.length() > 0 && y < 64) {
    display.drawString(x, y, line);
  }

  display.display();
}