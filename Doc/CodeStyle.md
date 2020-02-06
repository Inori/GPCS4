# Code Style for GPCS4

***Keep in mind that the code you write is not only read by youself.***


Basically, the code style is based on Sony's style.  

For example:
1. Use lower camel case for `variable` and `function` name.  
   like: `uint8_t shaderModifier;`, `void generateVsFetchShader();`

2. Use upper camel case for `class`, `enum` and `struct` name.  
   like: `class ConstantUpdateEngine;`, `enum PsslType;`, `struct VsStageRegisters;`

3. Use `m_` prefix for class member variable,   
   like: `void* m_cmdptr`  

   Use `g_` prefix for global variable,  
   like: `SceModule g_moduleLibc`  

4. Use stdint types  
   like `uint8_t`, `int64_t`, `uint32_t`

5. Start left brace `{` in new line, not at the end of current line.  
   like:
   ```
   if (shaderResourceOffset != 0)
   {
       doSomething();
   }
   ```

<br><br>

And based on the above Sony's style, we add the following extra rules, to make our code more clean and easy to maintain.

## Rules must be followed:
1. One function, one `return`.

2. Use `do {} while(false)` pattern in functions that are complex and/or going to be complex.  
   e.g. functions which have more than one `if` statement.  

   The advantage of this pattern is that, it can help us to find the most important part of one function more easily, this is called *Guard Clauses*  
   Help us not to forget resource freeing.  
   And this will force the code to grow vertically, not to expend horizontally, thus easy to understand and modify.  

    For examp;e:
    ```
    bool ModuleLoader::loadModule(std::string const &fileName, MemoryMappedModule **modOut)
    {
        bool retVal = false;
        MemoryMappedModule mod = {};
        do
        {
            if (!modOut)
            {
                break;
            }

            retVal = loadModuleFromFile(fileName, &mod);
            if (!retVal)
            {
                LOG_ERR("load module failed %s", fileName.cstr());
                break;
            }

            retVal = loadDependencies();
            if (!retVal)
            {
                LOG_ERR("load dependencies failed");
                break;
            }

            *modOut = &(m_modSystem.getMemoryMappedModules()[0]);
            retVal  = true;
        } while (false);

        if (!retVal)
        {
            releaseModule(mod);
        }

        return retVal;
    }
    ```

3. Do not write code nested more than 3 layers (`do{} while(false);` not included). Write nested statement as few as possible.  

   You can use `break` or `continue` to cancel nested code, or write a new function if necessary.
   
   For example:

   Recommended:
   ```
   if (!isFileExist(fileName))
   {
      break;
   }
   file = createFile(fileName);
   ```
   Discouraged:
   ```
   if (!isFileExist(fileName))
   {
      file = createFile(fileName);
   }
   ```

   Recommended:
   ```
    for (int i = 0; i != count; ++i)
    {
        if (!item[i].valid)
        {
            continue;
        }

        if (item[i].size < requireSize)
        {
            continue;
        }

        processItem(&item[i]);
    }
   ```
   Discouraged:
   ```
    for (int i = 0; i != count; ++i)
    {
        if (item[i].valid)
        {
            if (item[i].size >= requireSize)
            {
                processItem(&item[i]);
            }
        }
    }
   ```

   Recommended:
   ```
    void processOneItem(Item* item);

    for (int i = 0; i != count; ++i)
    {
        processOneItem(&item[i]);
    }
   ```
   Discouraged:
   ```
    for (int i = 0; i != count; ++i)
    {
        if (item[i].valid)
        {
            if (item[i].size >= requireSize)
            {
                processItem(&item[i]);
            }
        }
    }
   ```
4. Do not write a function longer than your sceen's height. Typically 50-80 lines at most.  
   Some table type functions not included, like a big `switch` statement.

5. Add brace to `if`, `while` statement even if it has only one line of code followed.

6. Do not use magic numbers directly, give it a meaningful name.

7. Local variables should be initialized when defined.

8. Do not use `goto`, use `do {} while(false)` mentioned above instead.

9. Do not use C++ exception.


## Rules recommended:

1. Design first in your mind, then coding.  
   It's better to make a high level design first in your mind and then coding,  
   like which class responsable for which job.  
   Think about code reuse.  
   Do not just write what comes your mind directly and hastily.  
   It will make your code looks messy and hard to maintian.  
   If you are not sure how to design, we can discuss first.

2. Functions should return simple type, like `void`, `bool`, `uint32_t`.

3. Try to name code tokens more friendly to non-english country people.  
   e.g. use `textureAlign`, not `tA`

4. Use macros as few as possible.

5. Make a variable's scope as small as possible.  
   If you can use a local variable, don't make it a class member.  
   If you can use a class member, don't make it global. And so on.

6. Include only required. Use forward declaration to resolve include dependencies.



PS1: Third party library code is not limited by the above rules.  
PS2: Forget about some old Hungarian notation style code, I'll fix that once I have time.