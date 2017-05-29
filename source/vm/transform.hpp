#pragma once

#include <string>
#include <type_traits>

#include <boost/locale.hpp>

#include <v8.h>

namespace braid::vm {
  template<typename CharT = char, typename Traits>
  inline v8::Local<v8::String> transform(const std::basic_string<CharT, Traits>& string) {
    return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), string.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
  }

  inline v8::Local<v8::String> transform(const std::basic_string<uint16_t, std::char_traits<uint16_t>>& string) {
    return v8::String::NewFromTwoByte(v8::Isolate::GetCurrent(), string.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
  }

  inline v8::Local<v8::String> transform(const std::basic_string<wchar_t, std::char_traits<wchar_t>>& string) {
    const std::string utf = boost::locale::conv::utf_to_utf<char>(string);

    return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), utf.c_str());
  }

  template<typename T>
  inline v8::Local<v8::Value> transform(const T value); // cannot use, must use an explicit specialization

  template<>
  inline v8::Local<v8::Value> transform(const bool boolean) {
    return v8::Boolean::New(v8::Isolate::GetCurrent(), boolean);
  }

  template<>
  inline v8::Local<v8::Value> transform(const int number) {
    return v8::Number::New(v8::Isolate::GetCurrent(), number);
  }
}