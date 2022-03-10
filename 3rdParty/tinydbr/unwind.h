/*
Copyright 2021 Google LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

https ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef UNWIND_H
#define UNWIND_H


class TinyDBR;
class ModuleInfo;

class UnwindData {
public:
  virtual ~UnwindData() = default;
};

class UnwindGenerator {
public:
  UnwindGenerator(TinyDBR& tinyinst) : tinydbr_(tinyinst) {}
  virtual ~UnwindGenerator() = default;
  
  virtual void Init() {}

  virtual void OnModuleInstrumented(ModuleInfo* module) { }
  virtual void OnModuleUninstrumented(ModuleInfo* module) { }

  virtual size_t MaybeRedirectExecution(ModuleInfo* module, size_t IP, void* context) {
    return IP;
  }

  virtual void OnBasicBlockStart(ModuleInfo* module,
                         size_t original_address,
                         size_t translated_address)
  { }

  virtual void OnInstruction(ModuleInfo* module,
                     size_t original_address,
                     size_t translated_address)
  { }

  virtual void OnBasicBlockEnd(ModuleInfo* module,
                       size_t original_address,
                       size_t translated_address)
  { }
  
  virtual void OnModuleLoaded(void *module, char *module_name) { }

  virtual void OnReturnAddress(ModuleInfo *module, size_t original_address, size_t translated_address) { }
  
  virtual bool HandleBreakpoint(ModuleInfo* module, void* address, void* context)
  {
	  return false;
  }

  virtual bool Is64BitOnly() { return false; }

protected:
	TinyDBR& tinydbr_;
};

#endif // UNWIND_H
