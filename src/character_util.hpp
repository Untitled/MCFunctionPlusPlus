#pragma once

namespace mpp {

bool is_lower_case_letter(char c) {return c >= 'a' && c <= 'z';};
bool is_capital_letter(char c) {return c >= 'A' && c <= 'Z';};
bool is_number_character(char c) {return c >= '0' && c <= '9';};
bool is_letter(char c) {return is_lower_case_letter(c) || is_capital_letter(c);}
bool is_blank(char c) {return c == ' ' || c == '\t' || c == '\r' || c == '\n';}

} //  namespace mpp