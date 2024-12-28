#include <unicode/uchar.h>
#include <unicode/brkiter.h>
#include <memory>
#include <unordered_map>
#include <map>

#include <iostream>
#include <unicode/uscript.h>

#include <unicode/uchar.h>
#include <unicode/uscript.h>
#include <unicode/uloc.h>
#include <unordered_map>
#include <unordered_set>
#include <unicode/utypes.h>

#include <algorithm>
#include <random>
#include <chrono>

UChar32 get_apostrophe_for_lang(const std::string& lang) {
    static const std::unordered_map<std::string, UChar32> apostrophes_per_lang = {
        // Languages with U+2019 (Typographic Apostrophe)
        {"en", 0x2019},  // English
        {"fr", 0x2019},  // French
        {"de", 0x2019},  // German
        {"it", 0x2019},  // Italian
        {"pt", 0x2019},  // Portuguese
        {"nl", 0x2019},  // Dutch
        {"sv", 0x2019},  // Swedish
        {"no", 0x2019},  // Norwegian
        {"da", 0x2019},  // Danish
        {"ro", 0x2019},  // Romanian
        {"fi", 0x2019},  // Finnish
        {"ca", 0x2019},  // Catalan
        {"hu", 0x2019},  // Hungarian
        {"pl", 0x2019},  // Polish
        {"cs", 0x2019},  // Czech
        {"sk", 0x2019},  // Slovak
        {"sl", 0x2019},  // Slovenian
        {"lt", 0x2019},  // Lithuanian
        {"lv", 0x2019},  // Latvian
        {"et", 0x2019},  // Estonian
        {"sq", 0x2019},  // Albanian
        {"bs", 0x2019},  // Bosnian
        {"hr", 0x2019},  // Croatian
        {"sr", 0x2019},  // Serbian
        {"bg", 0x2019},  // Bulgarian

        // Special Characters for Specific Languages
        {"haw", 0x02BB}, // Hawaiian (ʻokina)
        {"hy", 0x055A},  // Armenian apostrophe (U+055A)
        {"ka", 0x2019},  // Georgian (generally uses U+2019 in modern typography)
        {"kk", 0x02BC},  // Kazakh (glottal stop apostrophe)
        {"mn", 0x0027},  // Mongolian uses straight apostrophe
        {"my", 0x0027},  // Burmese (no strict apostrophe, U+0027 can be used informally)
        {"tl", 0x2019},  // Filipino/Tagalog (Typographic apostrophe)
        {"am", 0x0027},  // Amharic (informally uses straight apostrophe)

        // Languages with U+0027 (Straight Apostrophe)
        {"es", 0x0027},  // Spanish (uses straight apostrophe)
        {"zh", 0x0027},  // Chinese (rarely uses apostrophes, U+0027 when needed)
        {"ja", 0x0027},  // Japanese (rarely uses apostrophes, U+0027 when needed)
        {"ru", 0x0027},  // Russian (rarely uses apostrophes, U+0027 when needed)
        {"ko", 0x0027},  // Korean (U+0027, though rarely used)
        {"ar", 0x0027},  // Arabic (informally U+0027, though apostrophes aren't common)
        {"tr", 0x0027},  // Turkish (straight apostrophe)
        {"hi", 0x0027},  // Hindi (informal use of U+0027)
        {"vi", 0x0027},  // Vietnamese (straight apostrophe used in informal writing)
        {"id", 0x0027},  // Indonesian (U+0027 is the default)
        {"th", 0x0027},  // Thai (informal use of U+0027)
        {"uk", 0x0027},  // Ukrainian (rarely used, U+0027 in informal contexts)
        {"fa", 0x0027},  // Persian (rarely uses apostrophes, U+0027 when needed)
        {"sw", 0x0027},  // Swahili (informal use of U+0027)
        {"bn", 0x0027},  // Bengali (straight apostrophe used informally)
        {"is", 0x0027},  // Icelandic (U+0027 is used for contractions)
        {"ms", 0x0027},  // Malay (informal use of U+0027)
        {"mk", 0x0027},  // Macedonian (rarely uses apostrophes, U+0027 when needed)
        {"ta", 0x0027},  // Tamil (informal use of U+0027)
        {"te", 0x0027},  // Telugu (informal use of U+0027)
        {"ur", 0x0027},  // Urdu (informally uses straight apostrophe)
        {"mr", 0x0027},  // Marathi (U+0027 is often used in informal contexts)
        {"gu", 0x0027},  // Gujarati (informally uses U+0027)
        {"ml", 0x0027},  // Malayalam (informal use of U+0027)
        {"kn", 0x0027},  // Kannada (informal use of U+0027)
        {"so", 0x0027},  // Somali (U+0027 is commonly used for contractions)
        {"pa", 0x0027},  // Punjabi (informal use of U+0027)
    };

    try {
        return apostrophes_per_lang.at(lang);
    } catch (...) {
        return 0x0027;
    }
}

inline bool is_apostrophe(UChar32 c) {
    static const std::unordered_set<UChar32> apostrophe_symbols = {
        0x0027,  // Apostrophe (Straight single quote) - ' (U+0027)
        0x2019,  // Right single quotation mark (Typographic apostrophe) - ’ (U+2019)
        0x02BC,  // Modifier letter apostrophe (Glottal stop) - ʼ (U+02BC)
        0x275C,  // Heavy single comma quotation mark ornament - ❜ (U+275C)
        0xFF07,  // Fullwidth apostrophe - ＇ (U+FF07)
        0x2018,  // Left single quotation mark (used as an opening apostrophe) - ‘ (U+2018)
        0x02BB,  // Modifier letter turned comma (ʻokina, used in Hawaiian) - ʻ (U+02BB)
        0xA78C  // Latin small letter saltillo (used in some native Latin American languages) - ꞌ (U+A78C)
    };

    return apostrophe_symbols.contains(c);
}

std::unordered_set<uint32_t> get_desired_script_codes(const char* language) {
    UErrorCode status = U_ZERO_ERROR;
    UScriptCode script_buff[10] = {USCRIPT_INVALID_CODE};
    int script_count = uscript_getCode(language, script_buff, ULOC_KEYWORDS_CAPACITY, &status);

    std::unordered_set<uint32_t> desired_script_codes;

    if (U_FAILURE(status) || script_count < 1) {
        return desired_script_codes;
    }

    for (int i = 0; i < script_count; ++i) {
        desired_script_codes.insert(script_buff[i]);
    }

    desired_script_codes.insert(static_cast<uint32_t>(USCRIPT_INHERITED));

    return desired_script_codes;
}

inline std::string unicode_to_str(const icu::UnicodeString& unicode_str) {
    std::string utf8Str;
    unicode_str.toUTF8String(utf8Str);
    return utf8Str;
}

inline bool is_char_script_desired(UChar c, const std::unordered_set<uint32_t>& desired_script_codes) {
    UErrorCode status = U_ZERO_ERROR;
    UScriptCode script_code = uscript_getScript(c, &status);

    if (U_FAILURE(status)) {
        return true;
    }

    return desired_script_codes.contains(script_code);
}

enum class LID : int32_t {unknown = 0, en = 1, ja = 2, ca = 3};

LID get_lang_id_from_lang(const char* lang) {
    static const std::unordered_map<std::string, LID> ids = {
        {"en", LID::en},
        {"ja", LID::ja},
        {"ca", LID::ca}
    };

    try {
        return ids.at(lang);
    } catch (...) {
        return LID::unknown;
    }
}

inline bool is_alpha_from_lang(UChar32 ch, LID lang_id) {
    if (lang_id == LID::en) {
        return (ch >= 0x41 && ch <= 0x5A) || (ch >= 0x61 && ch <= 0x7A);
    }
    return u_isalpha(ch);
}

inline bool is_non_spacing_mark_required(const std::string& language) {
    static const std::unordered_set<std::string> non_spacing_mark_langs = {"ar", "fa", "mr", "hi", "ta", "te", "kn", "ml", "bn", "gu", "pa"};
    return non_spacing_mark_langs.contains(language);
}

namespace special_chars {
    static const std::unordered_set<UChar32> japanese = {0x30FC};
    static const std::unordered_set<UChar32> catalan = {0xB7};
}

bool is_word_char_valid(UChar32 c, LID lang_id, const std::unordered_set<uint32_t>& desired_script_codes, bool non_spacing_mark_required) {
    // if (is_alpha_from_lang(c, lang_id) && is_char_script_desired(c, desired_script_codes)) return true;
    if (is_char_script_desired(c, desired_script_codes)) return true;

    auto char_type = u_charType(c);

    if (char_type == U_LOWERCASE_LETTER || char_type == U_UPPERCASE_LETTER || char_type == U_OTHER_LETTER) {
        return true;
    }

    if (non_spacing_mark_required && char_type == U_NON_SPACING_MARK) return true;

    // language specific
    if (lang_id == LID::ja) {
        if (special_chars::japanese.contains(c)) return true;
    }

    return false;
}


bool is_phrase_language_valid(const std::string& str, const char* language) {
    auto text = icu::UnicodeString::fromUTF8(icu::StringPiece(str.data()));

    UErrorCode status = U_ZERO_ERROR;
    auto locale = icu::Locale(language);
    std::unique_ptr<icu::BreakIterator> bi(icu::BreakIterator::createWordInstance(locale, status));
    if (U_FAILURE(status)) {
        throw std::runtime_error("failed_to_process");
    }

    bi->setText(text);

    auto desired_script_codes = get_desired_script_codes(language);

    auto lang_id = get_lang_id_from_lang(language);

    auto non_spacing_mark_required = is_non_spacing_mark_required(language);

    int32_t start = bi->first();
    for (int32_t end = bi->next(); end != icu::BreakIterator::DONE; start = end, end = bi->next()) {
        if (!u_isalpha(text[start])) continue;

        icu::UnicodeString word;
        text.extractBetween(start, end, word);
        word = word.toLower(locale);

        for (int32_t i = 0; i < word.length(); ++i) {
            UChar32 c = word.char32At(i);

            if (u_isdigit(c)) return false;

            if (is_apostrophe(c)) {
                word.setCharAt(i, get_apostrophe_for_lang(language));
            } else {
                bool is_alpha_from_lang_b = is_alpha_from_lang(c, lang_id);
                bool is_char_script_desired_b = is_char_script_desired(c, desired_script_codes);
                if (!is_word_char_valid(c, lang_id, desired_script_codes, non_spacing_mark_required)) {
                    return false;
                }
            }
        }
    }

    return true;
}