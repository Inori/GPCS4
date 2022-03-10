# TinyDBR

![UE4 Demo](https://github.com/Inori/TinyDBR/blob/master/ScreenShot/demo.jpg)

## What is TinyDBR?

TinyDBR is meant for tiny dynamic binary rewriter fox x86 instruction set.

This is a port to the [TinyInst](https://github.com/googleprojectzero/TinyInst) by Google Project Zero team to fit my own needs.

The original TinyInst works as a debuuger and the target process runs seperately as a debuggee.

While TinyDBR runs inter the target process and translate instructions right there.

## How TinyDBR works?

Currently, TinyDBR only support Windows and X64.

Both TinyInst and TinyDBR will protect the target's code to non-executable property, then an attempt to execute the target code will raise an execute exception.

But compared to TinyInst, which catch the exception and translate instructions in debug event loop of the debugger process, TinyDBR registers a VEH handler at the target process, and does all tranlation steps within the VEH handler.

Other parts are almost the same as the original TinyInst.

## Memory Monitor
TinyDBR ships with a memory access monitor which can monitor memory read/write with some limitations(see below).

Support all SSE and AVX/AVX512 instructions including gather and scatter.

Users can inherite the `MemoryCallback` class to get notified when memory access happens. 

See [here](https://github.com/Inori/TinyDBR/blob/master/Translator/main.cpp) for an example.

### Limitations
There are some limitations due to my own usage. 

Basically, what I want is just to monitor heap access, code and stack access is not useful for me, 
 
besides, monitor all of that is too expensive.

1. Code memory is not supported. (e.g. call [mem])
2. Stack memory is not supported (e.g. mov rax, [rsp - 8])
3. FS and GS segment access is not supported. (e.g. mov rax, gs:[58])
4. Conditional read and write are not implemented accurately. (e.g. cmpxchg, vpgatherqd)

    I removed the condition, which means the callback will always be called no matter the memory referenced is really read/written or not.
    This reduced the complexity of the implementation. 

    But even with this limitation, it also ensures that, before the memory read
    we have chance to feed the memory the target may read and, after the memory write, we always get the correct content of the target memory.

## TODO List:
1. ~~Refactory the public interface for easy usage.~~ Done.
2. Remove remote memory backup as we now have only one process.
3. ~~Support rewrite shellcode without modules.~~ Done.
4. Support rewrite multiple modules.
5. Support other platform.


