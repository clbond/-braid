#pragma once

#include <string>
#include <type_traits>

#include <boost/locale.hpp>

#include <v8.h>

namespace braid::vm {
  inline v8::Local<v8::String> transform(v8::Isolate* isolate, const std::basic_string<char, std::char_traits<char>>& string) {
    v8::HandleScope scope(isolate);

    return v8::Local<v8::String>::New(isolate, v8::String::NewFromUtf8(isolate, string.c_str()));
  }

  inline v8::Local<v8::String> transform(v8::Isolate* isolate, const std::basic_string<uint16_t, std::char_traits<uint16_t>>& string) {
    v8::HandleScope scope(isolate);

    return v8::Local<v8::String>::New(isolate, v8::String::NewFromTwoByte(isolate, string.c_str(), v8::NewStringType::kNormal).ToLocalChecked());
  }

  inline v8::Local<v8::String> transform(v8::Isolate* isolate, const std::basic_string<wchar_t, std::char_traits<wchar_t>>& string) {
    v8::HandleScope scope(isolate);

    const std::string utf = boost::locale::conv::utf_to_utf<char>(string);

    return v8::Local<v8::String>::New(isolate, v8::String::NewFromUtf8(isolate, utf.c_str()));
  }

  template<typename T>
  inline v8::Local<v8::Value> transform(v8::Isolate* isolate, const T value); // cannot use, must use an explicit specialization

  template<>
  inline v8::Local<v8::Value> transform(v8::Isolate* isolate, const bool boolean) {
    v8::HandleScope scope(isolate);

    return v8::Boolean::New(isolate, boolean);
  }

  template<>
  inline v8::Local<v8::Value> transform(v8::Isolate* isolate, const int number) {
    v8::HandleScope scope(isolate);

    return v8::Number::New(isolate, number);
  }
}