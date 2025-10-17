#pragma once

#define DATE_MAX_LEN 32 // max ISO8601 string including '\0'

namespace pump_control::nvs {

bool load_date(char *out);
void save_date(const char *date);

} // namespace pump_control::nvs
