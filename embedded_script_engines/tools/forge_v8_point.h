// Autogenerated by - D:/home/lugansky-igor/github/in-the-vicinity-cc/embedded_script_engines/tools/point_gen.py
#ifndef FORGEV8POINTS_H_
#define FORGEV8POINTS_H_

// Other
#include <v8.h>

#include "point.h"

//StateLess
class ForgeV8Points {
 public:
  ForgeV8Points() { }

  // TODO: It need be impl. manual
  v8::Handle<v8::Object> Forge_NI(
      Point* point, 
      v8::Isolate* isolate,
      v8::Persistent<v8::ObjectTemplate>* blueprint);

  // TODO: It need be impl. manual
  v8::Handle<v8::ObjectTemplate> MakeBlueprint_NI();

  static void ForgeV8Points::v8_getter_x_(
    v8::Local<v8::String> name,
    const v8::PropertyCallbackInfo<v8::Value>& info);

  static void ForgeV8Points::v8_getter_y_(
    v8::Local<v8::String> name,
    const v8::PropertyCallbackInfo<v8::Value>& info);

};
#endif