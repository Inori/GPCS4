# GPCS4 Module System

## Overview

The module system is responsible for loading and linking modules. There are two types of modules. One is the ***native modules***, which are loaded from the PS4 firmware at load time by the emulator, and the other one is the ***virtual modules*** that we implement as a part of the emulator. The reason why we have the virtual modules rather than loading all modules from firmware is that some code in the firmware can contain syscalls, special instructions, or something that we can't directly run on the PC. The module system is capable of being configured to link different symbols from either the native modules or the virtual modules at compile time.

## Module System Components

To be done...

## Rules of Module Loading and Linking

One thing that may confuse newcomers is that the emulator does not treat virtual and native modules equally. The virtual modules are somehow the first-class citizens in the GPCS4, which are preferred by the module system, and all of them get loaded before the game executable is loaded. The native modules, on the other hand, are loaded on-demand after the loading of the game executable, so we use the term "**override**" to describe the module system uses an implementation from a native module instead of a virtual module.

### The Overridability Switches 

```
                                    +----------+
                                    |  Module  |    (ALLOW_MODULE_OVERRIDE)
                                    +----------+

                              +------+         +------+
                              | Lib0 |   ...   | Libn | (ALLOW_LIBRARY_OVERRIDE)
                              +--+---+         +------+
                                 |
                                 v
                            symb1 symb2   (ALLOW_FUNCTION_OVERRIDE)
                               symbn
```

As the ASCII diagram above shows that there are three levels as a PS4 module can contain one or more libraries, and a library holds multiple symbols. Accordingly, there are three levels of [overridability switches](https://github.com/Inori/GPCS4/blob/3fbbfe51cad7974eb780673e5ade5149c9e423c7/GPCS4/Emulator/RegisterModules.cpp#L13) that are configurable in the module registering code. 

+ Switches

  **ALLOW_MODULE_OVERRIDE** macro specifies a virtual module overridable.
  **ALLOW_LIBRARY_OVERRIDE** macro specifies a library overridable. It also sets the policy of symbol overriding inside the library, which we will talk about later. 

  **ALLOW_FUNCTION_OVERRIDE** macro specifies a function to be overridable.

  **DISALLOW_FUNCTION_OVERRIDE** macro specifies a function to be not overridable.

+ Symbol Overriding Policy of Library

   **Policy::AllowList**: Functions other than specified ones in this library are *NOT overrideable*.

   **Policy::DisallowList**: Functions other than specified ones in this library are *overridable*.
   
+ Examples

   ​	I present examples as Q&As base on [this](https://github.com/Inori/GPCS4/blob/3fbbfe51cad7974eb780673e5ade5149c9e423c7/GPCS4/Emulator/RegisterModules.cpp#99) module registering code.

   + **Q1: Is function  \_\_error(nid: 0xF41703CA43E6A352) of libkernel overrideable?**

       A1: Yes. Because Line:103 specifies that libkernel is overridable. Since the symbol overriding policy of libkernel is Policy::AllowList, and  \_\_error() function is set to overrideable in Line:104, It is overridable.
       
   + **Q2: Is function \_\_inet_ntop(nid:0xE296228683E0827F) of libkernel overridable?** 
   
       A2: No. Only \_\_error function in libkernel is overridable.
       
   + **Q3: Is function time(nid:0xC0B9459301BD51C4ULL) of libc overridable?** 
   
       A3: No. Although libc is set to overrideable, the time() function is in the "disallow list", therefore, it is not overridable.
       
   + **Q4: What happens to the libSceLibcInternal module as there is no library overridebility defnination?**
   
       A4: All of its libraries and all symbols inside those libraries are **overridable**.
   
### The Logic of Overriding

+ Is a function in a library overridable?

  ```
                              BEGIN
                      +-----------------+
                      |  Is the library |
                      |  containing     |  No
                      |  this function  +-------------------------+
                      |  overridable?   |                         |
                      |                 |                         |
                      +--------+--------+                         |
                               | Yes                              |
                   +-----------v------------+                     |
                   |                        |                     |
            +------+   Overriding Policy?   +-------+             |
            |      |                        |       |             |
            |      +------------------------+       |             |
       Policy::AllowList                      Policy::DisallowList|
            |                                       |             |
  +---------v---------+                  +----------v--------+    |
  |  Is this function |Yes           No  |  Is this function |    |
  |  in the list?     +---------v--------+  in the list?     |    |
  +--------+----------+         |        +----------+--------+    |
           |                    |                   |             |
        No |              +-----v-----+             | Yes         |
           |              |Overridable|             |             |
           |              +-----------+             |             |
           |            +----------------+          |             |
           +----------->+Not overridable +<---------+             |
                        +-------^--------+                        |
                                |                                 |
                                +---------------------------------+
  
  ```

  

+ Is a library of a native module overridable?

```
          BEGIN
+----------------------+
| Is the native module |   No
| containing this      +---------------------------------------+
| library loadable?    |                                       |
+-----------+----------+          +------------------+         |
            |                  +->+  Overridable     |         |
            | Yes              |  +------------------+         |
+-----------v-------------+    |                               |
| Is there any libray     |No  |                               |
| overridability settings +----+                               |
| in the module?          |    ^                               |
+-----------+-------------+    |                               |
            |                  |  +-----------------+          |
            | Yes              |  | Not Overridable +<---------+
+-----------v-------------+    |  +-----------------+          ^
|Is this module in the    | Yes|                               |
|overridable list         +----+                               |
|                         |                                    |
+-----------+-------------+                                    |
            | No                                               |
            |                                                  |
            +--------------------------------------------------+
```

+ Is a native module loadable?     

```
                                   BEGIN       
                            +----------------+
                            | Virtual module |  No  +----------+
                            | with same name +------> Loadable |
                            | exists?        |      +----^-----+
                            +--------+-------+           |
                                     | Yes               |
                                     |                   |
                            +--------v--------+          |
                            |  Is that module |   Yes    |
                            |  overridable?   +----------+
                            |                 |
                            +--------+--------+
                                     | No
                               +-----v------+
                               |Not Loadable|
                               +------------+
```





