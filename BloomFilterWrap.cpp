/* Copyright (c) 2015 Brian R. Bondy. Distributed under the MPL2 license.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "BloomFilterWrap.h"


namespace BloomFilterWrap {

using namespace v8;

Persistent<Function> BloomFilterWrap::constructor;

BloomFilterWrap::BloomFilterWrap(unsigned int nElements, double nFPRate, unsigned int nTweak,unsigned char nFlagsIn)
  : CBloomFilter(nElements, nFPRate, nTweak, nFlagsIn) {
}

BloomFilterWrap::~BloomFilterWrap() {
}

void BloomFilterWrap::Init(Local<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "Bloom"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "insert", BloomFilterWrap::Insert);
  NODE_SET_PROTOTYPE_METHOD(tpl, "contains", BloomFilterWrap::Contains);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "Bloom"),
               tpl->GetFunction());
}

void BloomFilterWrap::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new BloomFilter(...)`
    unsigned int nElements = args[0]->IsUndefined() ? 0 : args[0]->Uint32Value();
    unsigned int nFPRate = args[0]->IsUndefined() ? 0 : args[1]->Uint32Value();
    unsigned int nTweak = args[0]->IsUndefined() ? 0 : args[2]->Uint32Value();
    unsigned int nFlagsIn = args[0]->IsUndefined() ? 0 : args[3]->Uint32Value();

    BloomFilterWrap* obj = new BloomFilterWrap(nElements,nFPRate,nTweak,nFlagsIn);
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function `BloomFilter(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Context> context = isolate->GetCurrentContext();
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Object> result =
        cons->NewInstance(context, argc, argv).ToLocalChecked();
    args.GetReturnValue().Set(result);
  }
}

void BloomFilterWrap::Insert(const FunctionCallbackInfo<Value>& args) {// Uint32array[8]
  Isolate* isolate = args.GetIsolate();
  Handle<Value> v = args[0];
  v8::Handle<v8::Array> array = v8::Handle<v8::Array>::Cast(v);

  uint256 hash = *(uint256*)&array;

  BloomFilterWrap* obj = ObjectWrap::Unwrap<BloomFilterWrap>(args.Holder());
  obj->insert(hash);
}

void BloomFilterWrap::Contains(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Handle<Value> v = args[0];
  v8::Handle<v8::Array> array = v8::Handle<v8::Array>::Cast(v);
  uint256 hash = *(uint256*)&array;

  BloomFilterWrap* obj = ObjectWrap::Unwrap<BloomFilterWrap>(args.Holder());
  bool exists = obj->contains(hash);

  args.GetReturnValue().Set(Boolean::New(isolate, exists));
}


}  // namespace BloomFilterWrap
