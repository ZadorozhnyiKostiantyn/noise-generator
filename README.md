# Noise Generator Project

## Опис

Цей репозиторій містить два основних компоненти:
- **noise-generator** — прошивка для мікроконтролера ESP32, яка генерує шум, керує аудіо входом/виходом, записує WAV-файли на SD-карту, та піднімає web-сервер для керування пристроєм.
- **web** — сучасний веб-додаток (React + Vite + TypeScript), який дозволяє керувати пристроєм, записувати, переглядати та завантажувати аудіо, а також керувати станом ультразвуку.

---

## Функціонал

### noise-generator (Embedded)
- Генерація шуму та робота з аудіо-сигналами (ADC/I2S/DAC).
- Запис та збереження WAV-файлів на SD-карту.
- Вбудований web-сервер для керування пристроєм через браузер.
- API для отримання стану пристрою та взаємодії з web-додатком.

### web (Веб-додаток)
- Перегляд списку аудіозаписів, запис нових, завантаження та видалення.
- Керування ультразвуковими функціями пристрою.
- Інтуїтивний UI на основі React, TanStack Router, TanStack Query, TailwindCSS, shadcn/ui.
- SPA-навігація, сучасний дизайн, підтримка мобільних пристроїв.

---

## Швидкий старт

### Веб-додаток

1. Перейдіть у директорію `web`:
   ```bash
   cd web
   ```
2. Встановіть залежності:
   ```bash
   npm install
   ```
3. Запустіть у режимі розробки:
   ```bash
   npm run start
   ```
   Додаток буде доступний на http://localhost:3000

4. Для білду production-версії:
   ```bash
   npm run build
   ```
5. Для перегляду білду:
   ```bash
   npm run serve
   ```

### Тестування, лінтинг, форматування
- Запуск тестів:
  ```bash
  npm run test
  ```
- Лінтинг:
  ```bash
  npm run lint
  ```
- Форматування:
  ```bash
  npm run format
  ```
- Автоматичний лінт+формат:
  ```bash
  npm run check
  ```

### Додавання UI-компонентів (shadcn/ui)
```bash
pnpx shadcn@latest add button
```

---

### Embedded (PlatformIO)

1. Відкрийте директорію `noise-generator` у PlatformIO/VSCode.
2. Підключіть плату ESP32.
3. Для компіляції та прошивки використовуйте стандартні команди PlatformIO:
   - **Build:**
     ```
     pio run
     ```
   - **Upload:**
     ```
     pio run --target upload
     ```
   - **Моніторинг порту:**
     ```
     pio device monitor
     ```

---

## Структура репозиторію

- `noise-generator/` — код прошивки, бібліотеки, основний код, налаштування, SD-карта, web-сервер.
- `web/` — сучасний веб-додаток для керування пристроєм.

---

## Технології
- **Embedded:** PlatformIO, C++, ESPAsyncWebServer, LittleFS, SD, I2S, ADC, DAC
- **Web:** React, Vite, TypeScript, TanStack Router, TanStack Query, TailwindCSS, shadcn/ui, Vitest, ESLint, Prettier

---

## Додатково
- Для розширення функціоналу дивіться документацію у відповідних підпапках.
- Для тестування embedded-коду використовуйте PlatformIO Test Runner (`test/README`).
- Для розробки web-додатку — дивіться `web/README.md`.

---

## Ліцензія

MIT
