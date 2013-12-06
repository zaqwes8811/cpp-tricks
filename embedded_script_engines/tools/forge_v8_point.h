// Autogenerated by - D:/home/lugansky-igor/in-the-vicinity-cc/embedded_script_engines/tools/first_level_wrapper.py
#ifndef FORGEV8UNKNOWNS_H_
#define FORGEV8UNKNOWNS_H_

// Other
#include <v8.h>

#include "point.h"

//StateLess
class ForgeV8unknowns {
 public:
  ForgeV8unknowns() { }

  // TODO: It need be impl. manual
  v8::Handle<v8::Object> Forge_NI(
      Point* point, 
      v8::Isolate* isolate,
      v8::Persistent<v8::ObjectTemplate>* blueprint);

  // TODO: It need be impl. manual
  v8::Handle<v8::ObjectTemplate> MakeBlueprint_NI(v8::Isolate* isolate);

  static void ForgeV8unknowns::v8_getter_lengthAllParams___(
    v8::Local<v8::String> name,
    const v8::PropertyCallbackInfo<v8::Value>& info);

  static void ForgeV8unknowns::v8_getter_counterDebug___(
    v8::Local<v8::String> name,
    const v8::PropertyCallbackInfo<v8::Value>& info);

  static void ForgeV8unknowns::v8_getter_typeControl_(
    v8::Local<v8::String> name,
    const v8::PropertyCallbackInfo<v8::Value>& info);

  static void ForgeV8unknowns::v8_getter_failsReseted_(
    v8::Local<v8::String> name,
    const v8::PropertyCallbackInfo<v8::Value>& info);

  static void ForgeV8unknowns::v8_getter_currentQueryIndex_(
    v8::Local<v8::String> name,
    const v8::PropertyCallbackInfo<v8::Value>& info);

};
#endif