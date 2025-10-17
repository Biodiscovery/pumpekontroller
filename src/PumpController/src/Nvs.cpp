#include "hardware/flash.h"
#include "hardware/sync.h"
#include "pico/stdlib.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "PumpController/Nvs.hpp"

// ---- Config ----
#define SECTOR_SIZE FLASH_SECTOR_SIZE // 4096
#define PAGE_SIZE FLASH_PAGE_SIZE     // 256
#define PAGES_PER_SECTOR (SECTOR_SIZE / PAGE_SIZE)
#define FLASH_CFG_OFFSET (PICO_FLASH_SIZE_BYTES - SECTOR_SIZE) // last 4KB
#define RECORD_MAGIC 0x44415445u                               // 'D''A''T''E'

namespace pump_control::nvs {

// ---- Record format (placed at start of each page) ----
typedef struct {
  uint32_t magic;    // RECORD_MAGIC
  uint32_t seq;      // monotonically increasing
  uint16_t len;      // bytes incl. '\0'
  uint16_t reserved; // keep header 4B-aligned
  char date[DATE_MAX_LEN];
} __attribute__((packed)) date_record_t;

static inline const uint8_t *sector_base(void) {
  return (const uint8_t *)(XIP_BASE + FLASH_CFG_OFFSET);
}

// Return pointer to the last valid record in the sector (or NULL)
static const date_record_t *find_last_record(void) {
  const uint8_t *base = sector_base();
  const date_record_t *last = NULL;

  for (int i = 0; i < PAGES_PER_SECTOR; ++i) {
    const date_record_t *rec = (const date_record_t *)(base + i * PAGE_SIZE);
    if (rec->magic != RECORD_MAGIC)
      break; // first empty page
    if (rec->len == 0 || rec->len > DATE_MAX_LEN)
      break;
    last = rec; // keep advancing
  }
  return last;
}

// Return offset (within the sector) of first free page, or SECTOR_SIZE if full
static uint32_t find_free_page_offset(void) {
  const uint8_t *base = sector_base();
  for (uint32_t i = 0; i < PAGES_PER_SECTOR; ++i) {
    const date_record_t *rec = (const date_record_t *)(base + i * PAGE_SIZE);
    if (rec->magic != RECORD_MAGIC)
      return i * PAGE_SIZE;
  }
  return SECTOR_SIZE;
}

static void erase_sector(void) {
  uint32_t ints = save_and_disable_interrupts();
  flash_range_erase(FLASH_CFG_OFFSET, SECTOR_SIZE);
  restore_interrupts(ints);
}

bool load_date(char *out) {
  const date_record_t *rec = find_last_record();
  if (!rec) {
    out[0] = '\0';
    return false;
  }
  strncpy(out, rec->date, DATE_MAX_LEN);
  out[DATE_MAX_LEN - 1] = '\0';
  return true;
}

void save_date(const char *date) {
  if (!date)
    return;
  uint16_t len = (uint16_t)strnlen(date, DATE_MAX_LEN - 1) + 1;

  const date_record_t *last = find_last_record();
  uint32_t seq = last ? (last->seq + 1) : 1;

  // Prepare one whole page to program
  uint8_t page[PAGE_SIZE];
  memset(page, 0xFF, sizeof(page));
  date_record_t rec = {
      .magic = RECORD_MAGIC, .seq = seq, .len = len, .reserved = 0};
  memset(rec.date, 0, DATE_MAX_LEN);
  memcpy(rec.date, date, len);
  memcpy(page, &rec, sizeof(rec)); // record at start of page

  uint32_t off = find_free_page_offset();
  if (off >= SECTOR_SIZE) { // sector full -> erase & start over
    erase_sector();
    off = 0;
  }

  // Program exactly one aligned 256-byte page
  uint32_t ints = save_and_disable_interrupts();
  flash_range_program(FLASH_CFG_OFFSET + off, page, PAGE_SIZE);
  restore_interrupts(ints);
}

} // namespace pump_control::nvs
