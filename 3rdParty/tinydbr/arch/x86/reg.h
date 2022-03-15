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

#ifndef ARCH_X86_REG_H
#define ARCH_X86_REG_H

#define ARCH_SP RSP
#define ARCH_PC RIP
#define ARCH_RETURN_VALUE_REGISTER RAX
#define ARCH_PC RIP
#define ORIG_ADDR_REG RAX

enum Register {
  RAX,
  RCX,
  RDX,
  RBX,
  RSP,
  RBP,
  RSI,
  RDI,
  R8,
  R9,
  R10,
  R11,
  R12,
  R13,
  R14,
  R15,
  RIP
};

#endif  // ARCH_X86_REG_H
