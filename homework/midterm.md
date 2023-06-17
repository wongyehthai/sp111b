> # 内容主要來源：
> [**手把手教你构建 C 语言编译器0-4**](https://lotabout.me/2015/write-a-C-interpreter-1/)
> [**Chatgpt**](https://chat.openai.com)
> [**程式碼來源**](https://disq.us/url?url=https%3A%2F%2Fgithub.com%2Flotabout%2Fwrite-a-C-interpreter%3AZVUO1v-fpg4Id2pxKHnbjUN-7_I&cuid=2272845)
# **構建c語言編譯器**

1. 解释器与编译器仅在代码生成阶段有区别，而其它方面如词法分析、语法分析是一样的。
1. 解释器需要我们实现自己的虚拟机与指令集，而这部分能帮助我们了解计算机的工作原理。

## **4個主要運用函數**
1. `next()` 用于词法分析，获取下一个标记，它将自动忽略空白字符。
1. `program()` 语法分析的入口，分析整个 C 语言程序。
1. `expression(level)` 用于解析一个表达式。
1. `eval()` 虚拟机的入口，用于解释目标代码。

## **xc-tutor.c的gpt解析**

> 這個程式是一個簡單的編譯器，用於解釋和執行簡單的程式碼。以下是程式的主要結構和功能：
> 
> 1. `void next()` 函式：這個函式用於獲取下一個字符，並將其存儲在全域變數 `token` 中。
> 
> 2. `void expression(int level)` 函式：這個函式用於處理表達式，但在目前的程式碼中並沒有實際執行任何操作，因為 `// do nothing` 註解。
> 
> 3. `void program()` 函式：這個函式是主要的程式解析函式。它在獲取下一個 token 後進入一個迴圈，直到 token 為 0（即結束）。在迴圈中，它將目前的 token 印出來並繼續取得下一個 token。
> 
> 4. `int eval()` 函式：這個函式用於執行程式碼，但在目前的程式碼中並沒有實際執行任何操作，因為 `// do nothing yet` 註解。
> 
> 5. `int main(int argc, char **argv)` 函式：這是程式的進入點。它首先對命令行參數進行處理，然後初始化變數。接下來，它打開指定的檔案並讀取檔案內容到 `src` 字串中。然後調用 `program()` 函式進行程式解析和輸出 token。最後，調用 `eval()` 函式執行程式碼。
> 
> 總結來說，這個程式碼是一個簡單的編譯器框架，它可以解析程式碼並列印出每個 token。然而，目前的程式碼並沒有實際執行任何計算或操作。這只是一個簡單的示例程式碼，用於構建一個編譯器的初始框架。在這個框架的基礎上，你可以進一步擴展和實現編譯器的各個部分，例如語法分析、代碼生成和執行等。

## **进程的内存**
1. 代码段（text）用于存放代码（指令）。
1. 数据段（data）用于存放初始化了的数据，如int i = 10;，就需要存放到数据段中。
1. 未初始化数据段（bss）用于存放未初始化的数据，如 int i[1000];，因为不关心其中的真正数值，所以单独存放可以节省空间，减少程序的体积。
1. 栈（stack）用于处理函数调用相关的数据，如调用帧（calling frame）或是函数的局部变量等。
1. 堆（heap）用于为程序动态分配内存。

這次只打算動代码段、数据段以及栈。

## **寄存器總類：**
1. `PC `程序计数器，它存放的是一个内存地址，该地址中存放着 下一条 要执行的计算机指令。
1. `SP `指针寄存器，永远指向当前的栈顶。注意的是由于栈是位于高地址并向低地址增长的，所以入栈时 SP 的值减小。
1. `BP` 基址指针。也是用于指向栈的某些位置，在调用函数时会使用到它。
1. `AX `通用寄存器，我们的虚拟机中，它用于存放一条指令执行后的结果。

## **chatgpt1314-1330**
> 這段程式碼負責為虛擬機器分配記憶體。它使用 `malloc()` 函式來分配三個不同區域的記憶體：`text`、`data` 和 `stack`。
> 
> 1. `text`：這個區域用於存儲程式的指令碼。`text` 是一個指向字節（byte）的指標，指向分配的記憶體區域的起始位置。
> 
> 2. `data`：這個區域用於存儲程式的數據。`data` 也是一個指向字節的指標，指向分配的記憶體區域的起始位置。
> 
> 3. `stack`：這個區域用於虛擬機器的堆疊。同樣地，`stack` 是一個指向字節的指標，指向分配的記憶體區域的起始位置。
> 
> 在這段程式碼中，首先使用 `malloc(poolsize)` 分別為 `text`、`data` 和 `stack` 分配了 `poolsize` 大小的記憶體。如果記憶體分配失敗，程式會印出錯誤訊息並返回 `-1`。
> 
> 接著，使用 `memset()` 函式將分配的記憶體區域初始化為零。`memset()` 函式將指定的內存區域填充為給定的值，這裡使用零來初始化這些區域。
> 
> 總結來說，這段程式碼分配了虛擬機器所需的記憶體區域，並將其初始化為零。這些區域分別用於存儲指令碼、數據和堆疊，以支援虛擬機器的運行。

## 指令集

 CPU 能识别的命令的集合，也可以说是 CPU 能理解的语言。
```
 // instructions
enum { LEA ,IMM ,JMP ,CALL,JZ  ,JNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PUSH,
       OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,
       OPEN,READ,CLOS,PRTF,MALC,MSET,MCMP,EXIT };
```
> 指令的順序：帶有參數的指令在前，沒有參數的指令在后。

`MOV`是所有指令中最基础的一个，它用于将数据放进寄存器或内存地址，有点类似于 C 语言中的赋值语句。x86 的 `MOV` 指令有两个参数，分别是源地址和目标地址：`MOV dest, source` （Intel 风格），表示将 `source` 的内容放在 `dest` 中，它们可以是一个数、寄存器或是一个内存地址。

因爲原作者的虛擬機只有一個寄存器，要做到一個指令做參數類型的識別有點難度，於是將`MOV`分成了五個指令：
1. `IMM <num>` 将 `<num>` 放入寄存器 `ax` 中。
1. `LC` 将对应地址中的字符载入 `ax` 中，要求 `ax` 中存放地址。
1. `LI` 将对应地址中的整数载入 `ax` 中，要求 `ax` 中存放地址。
1. `SC` 将 `ax` 中的数据作为字符存放入地址中，要求栈顶存放地址。
1. `SI` 将 `ax` 中的数据作为整数存放入地址中，要求栈顶存放地址。 

## **chatgpt1230-1234**
> 這段程式碼是 `eval()` 函式的一部分。該函式包含了一個無窮迴圈，其中根據變數 `op` 的值來執行不同的指令操作。
> 
> 在這段程式碼中，根據 `op` 的值，進行不同的指令處理：
> 
> - 如果 `op` 等於 `IMM`，則將下一個指令的值（`*pc++`）載入到寄存器 `ax` 中。
> - 如果 `op` 等於 `LC`，則將 `ax` 中的位址所指向的字符值載入到 `ax` 中。
> - 如果 `op` 等於 `LI`，則將 `ax` 中的位址所指向的整數值載入到 `ax` 中。
> - 如果 `op` 等於 `SC`，則將 `ax` 的值存儲到 `*sp` 指向的位址中，同時將 `ax` 的值存儲到堆疊（`*sp++`）中。
> - 如果 `op` 等於 `SI`，則將 `ax` 的值存儲到 `*sp` 指向的位址中。
> 
> 這些指令根據虛擬機器的設計，用於操作寄存器 `ax`、記憶體中的位址以及堆疊。請注意，這只是 `eval()` 函式的一部分，還有其他指令的處理邏輯未顯示出來。
> 
> 程式碼的其餘部分沒有給出，可能包含了處理其他指令、更新變數值、跳轉到其他程式區塊等邏輯。最後，函式返回整數值 0。
> 
> 請注意，根據程式碼的結構，這段程式碼中的無窮迴圈會導致函式無法正常結束，這可能是程式碼的一個錯誤。在實際應用中，你需要根據具體需求和邏輯來確定何時結束迴圈。

`PUSH`将 ax 的值放入栈中

`else if (op == PUSH) {*--sp = ax;}                                     // push the value of ax onto the stack`


`JMP <addr>` 是跳转指令，无条件地将当前的 `PC` 寄存器设置为指定的 `<addr>`

`else if (op == JMP)  {pc = (int *)*pc;}                                // jump to the address`
> `pc` 寄存器指向的是 下一条 指令。所以此时它存放的是 `JMP` 指令的参数，即 `<addr>` 的值。

`JZ/JNZ`，为了实现 `if` 语句，需要条件判断相关的指令。这里只实现两个最简单的条件判断，即结果（`ax`）为零或不为零情况下的跳转。

```
else if (op == JZ)   {pc = ax ? pc + 1 : (int *)*pc;}                   // jump if ax is zero
else if (op == JNZ)  {pc = ax ? (int *)*pc : pc + 1;}                   // jump if ax is not zero
```

## 子函数调用
引入的命令有 `CALL`, `ENT`, `ADJ` 及 `LEV`。
`CALL` 的作用是跳转到地址为 `<addr>` 的子函数
`RET` 则用于从子函数中返回
不能使用`JMP`指令的原因:
从子函数中返回时，程序需要回到跳转之前的地方继续运行，这就需要事先将这个位置信息存储起来。反过来，子函数要返回时，就需要获取并恢复这个信息。因此实际中我们将 `PC` 保存在栈中。如下：
```
else if (op == CALL) {*--sp = (int)(pc+1); pc = (int *)*pc;}           // call subroutine
//else if (op == RET)  {pc = (int *)*sp++;}                              // return from subroutine;
```
**编译器参数是顺序入栈的**
取自[維基百科](https://en.wikipedia.org/wiki/X86_calling_conventions)
```
int callee(int, int, int);

int caller(void)
{
	int i, ret;

	ret = callee(1, 2, 3);
	ret += 5;
	return ret;
}
```
会生成如下的 x86 汇编代码：
```
caller:
	; make new call frame
	push    ebp
	mov     ebp, esp
        sub     1, esp       ; save stack for variable: i
	; push call arguments
	push    3
	push    2
	push    1
	; call subroutine 'callee'
	call    callee
	; remove arguments from frame
	add     esp, 12
	; use subroutine result
	add     eax, 5
	; restore old call frame
        mov     esp, ebp
	pop     ebp
	; return
	ret
```
上面这段代码在原作者的虚拟机里会有几个问题：
1. `push ebp`，但 `PUSH` 指令并无法指定寄存器。
1. `mov ebp, esp`， `MOV` 指令同样功能不足。
1. `add esp, 12`，也是一样的问题（尽管还没定义）。
***所以上面的指令，連函數調用都無法實現。***
`ENT <size>` 指的是 `enter`，用于实现 ‘make new call frame’ 的功能，即保存当前的栈指针，同时在栈上保留一定的空间，用以存放局部变量。对应的汇编代码为：
```
; make new call frame
push    ebp
mov     ebp, esp
       sub     1, esp       ; save stack for variable: i
```
实现如下：
`else if (op == ENT)  {*--sp = (int)bp; bp = sp; sp = sp - *pc++;}      // make new stack frame`

`ADJ <size>` 用于实现 ‘remove arguments from frame’。在将调用子函数时压入栈中的数据清除，本质上是因为原作者 `ADD` 指令功能有限。对应的汇编代码为：
```
; remove arguments from frame
add     esp, 12
```
实现如下：
`else if (op == ADJ)  {sp = sp + *pc++;}                                // add esp, <size>`

`LEV` 本质上这个指令并不是必需的，只是我们的指令集中并没有 `POP` 指令。并且三条指令写来比较麻烦且浪费空间，所以用一个指令代替。对应的汇编指令为：
```
; restore old call frame
       mov     esp, ebp
pop     ebp
; return
ret
```
具体的实现如下：
`else if (op == LEV)  {sp = bp; bp = (int *)*sp++; pc = (int *)*sp++;}  // restore call frame and PC`
`LEV` 已经把 `RET` 的功能包含了，所以不再需要 `RET` 指令。

`LEA`
解决调用帧的问题，但还有一个问题是如何在子函数中获得传入的参数。这里首先要了解的是当参数调用时，栈中的调用帧是什么样的。依旧用上面的例子（只是现在用“顺序”调用参数）：
```
sub_function(arg1, arg2, arg3);

|    ....       | high address
+---------------+
| arg: 1        |    new_bp + 4
+---------------+
| arg: 2        |    new_bp + 3
+---------------+
| arg: 3        |    new_bp + 2
+---------------+
|return address |    new_bp + 1
+---------------+
| old BP        | <- new BP
+---------------+
| local var 1   |    new_bp - 1
+---------------+
| local var 2   |    new_bp - 2
+---------------+
|    ....       |  low address
```
为了获取第一个参数，需要得到 `new_bp + 4`，但就如上面的说，原作者的 `ADD` 指令无法操作除 ax 外的寄存器，所以提供了一个新的指令：`LEA <offset>`

实现如下：
`else if (op == LEA)  {ax = (int)(bp + *pc++);}                         // load address for arguments.`
> 以上为实现函数调用需要的指令

## 运算符指令
### **chatgpt1246-1261**
> 這段程式碼是 `eval()` 函式的一部分，處理了一些二元運算指令。根據變數 `op` 的值，它執行了不同的運算並將結果存儲到寄存器 `ax` 中。
> 
> 以下是每個運算指令的功能：
> 
> - 如果 `op` 等於 `OR`，則將 `*sp++` 和 `ax` 的值進行位元或運算，並將結果存儲到 `ax` 中。
> - 如果 `op` 等於 `XOR`，則將 `*sp++` 和 `ax` 的值進行位元異或運算，並將結果存儲到 `ax` 中。
> - 如果 `op` 等於 `AND`，則將 `*sp++` 和 `ax` 的值進行位元與運算，並將結果存儲到 `ax` 中。
> - 如果 `op` 等於 `EQ`，則比較 `*sp++` 和 `ax` 的值是否相等，並將比較結果（0 或 1）存儲到 `ax` 中。
> - 如果 `op` 等於 `NE`，則比較 `*sp++` 和 `ax` 的值是否不相等，並將比較結果（0 或 1）存儲到 `ax` 中。
> - 如果 `op` 等於 `LT`，則比較 `*sp++` 和 `ax` 的值是否小於，並將比較結果（0 或 1）存儲到 `ax` 中。
> - 如果 `op` 等於 `LE`，則比較 `*sp++` 和 `ax` 的值是否小於等於，並將比較結果（0 或 1）存儲到 `ax` 中。
> - 如果 `op` 等於 `GT`，則比較 `*sp++` 和 `ax` 的值是否大於，並將比較結果（0 或 1）存儲到 `ax` 中。
> - 如果 `op` 等於 `GE`，則比較 `*sp++` 和 `ax` 的值是否大於等於，並將比較結果（0 或 1）存儲到 `ax` 中。
> - 如果 `op` 等於 `SHL`，則將 `*sp++` 的值左位移 `ax` 次，並將結果存儲到 `ax` 中。
> - 如果 `op` 等於 `SHR`，則將 `*sp++` 的值右位移 `ax` 次，並將結果存儲到 `ax` 中。
> - 如果 `op` 等於`ADD`：則將 `*sp++` 和 `ax` 的值相加，並將結果存儲到 `ax` 中。
> - 如果 `op` 等於 `SUB`，則將 `*sp++` 減去 `ax` 的值，並將結果存儲到 `ax` 中。
> - 如果 `op` 等於 `MUL`，則將 `*sp++` 和 `ax` 的值相乘，並將結果存儲到 `ax` 中。
> - 如果 `op` 等於 `DIV`，則將 `*sp++` 除以 `ax` 的值，並將結果存儲到 `ax` 中。
> - 如果 `op` 等於 `MOD`，則將 `*sp++` 對 `ax` 的值取餘數，並將結果存儲到 `ax` 中。
> 
> 這些指令根據虛擬機器的設計，執行不同的二元運算並將結果存儲到寄存器 `ax` 中。程式碼的其餘部分可能包含了處理其他指令、更新變數值、跳轉到其他程式區塊等邏輯。
> 
> 請注意，根據程式碼的結構，此部分程式碼只是 `eval()` 函式的一部分，可能還有其他指令的處理邏輯未顯示出來。

## 内置函数
编译器中需要用到的函数有：`exit`, `open`, `close`, `read`, `printf`, `malloc`, `memset` 及 `memcmp`
### **chatgpt1263-1273**
> 這段程式碼處理了虛擬機器中的各種指令，每個指令都有對應的操作。其中包括：
> 
> - `EXIT` 指令：印出 `*sp` 的值，表示程式結束，並將 `*sp` 作為結果返回。
> - `OPEN` 指令：呼叫 `open()` 函式，打開指定的檔案，並將結果存儲在 `ax` 中。
> - `CLOS` 指令：呼叫 `close()` 函式，關閉指定的檔案，並將結果存儲在 `ax` 中。
> - `READ` 指令：呼叫 `read()` 函式，從檔案中讀取指定數量的位元組，並將結果存儲在 `ax` 中。
> - `PRTF` 指令：根據 `pc` 和堆疊上的參數呼叫 `printf()` 函式，並將結果存儲在 `ax` 中。
> - `MALC` 指令：呼叫 `malloc()` 函式，動態分配指定大小的記憶體，並將結果存儲在 `ax` 中。
> - `MSET` 指令：呼叫 `memset()` 函式，將指定的值設定到指定的記憶體區域中，並將結果存儲在 `ax` 中。
> - `MCMP` 指令：呼叫 `memcmp()` 函式，比較兩個記憶體區域的內容，並將結果存儲在 `ax` 中。
> 
> 如果指令 `op` 不是以上列出的任何一個指令，則會印出錯誤訊息，並返回 -1 表示出現錯誤。
> 
> 這段程式碼確保了虛擬機器能夠執行不同的指令，並處理各種情況。

測試：计算 `10+20`，在 `main` 函数中加入下列代码：
int main(int argc, char *argv[])
```
{
    ax = 0;
    ...

    i = 0;
    text[i++] = IMM;
    text[i++] = 10;
    text[i++] = PUSH;
    text[i++] = IMM;
    text[i++] = 20;
    text[i++] = ADD;
    text[i++] = PUSH;
    text[i++] = EXIT;
    pc = text;

    ...
    program();
}
```
编译程序 `gcc xc-tutor.c`，运行程序：`./a.out hello.c`。输出：`exit(30)`

代码里有一些指针的强制转换，默认是 32 位的，因此在 64 位机器下，会出现 `segmentation fault`，解决方法（二选一）：

编译时加上 `-m32` 参数：`gcc -m32 xc-tutor.c`
在代码的开头，增加 `#define int long long`，`long long` 是 64 位的，不会出现强制转换后的问题。

## 词法分析器
用于对源码字符串做预处理，以减少语法分析器的复杂程度。

词法分析器以源码字符串为输入，输出为标记流（token stream），即一连串的标记，每个标记通常包括： `(token, token value)` 即标记本身和标记的值。例如，源码中若包含一个数字 `'998'` ，词法分析器将输出 `(Number, 998)`，即（数字，998）。再例如：
```
2 + 3 * (4 - 5)
=>
(Number, 2) Add (Number, 3) Multiply Left-Bracket (Number, 4) Subtract (Number, 5) Right-Bracket
```
## 词法分析器与编译器
> 要是深入词法分析器，你就会发现，它的本质上也是编译器。我们的编译器是以标记流为输入，输出汇编代码，而词法分析器则是以源码字符串为输入，输出标记流。
```
                   +-------+                      +--------+
-- source code --> | lexer | --> token stream --> | parser | --> assembly
                   +-------+                      +--------+
```
**在这个前提下，可以这样认为：直接从源代码编译成汇编代码是很困难的，因为输入的字符串比较难处理。所以先编写一个较为简单的编译器（词法分析器）来将字符串转换成标记流，而标记流对于语法分析器而言就容易处理得多了。**
## 词法分析器的实现
不会一次性地将所有源码全部转换成标记流的原因：
1. 字符串转换成标记流有时是有状态的，即与代码的上下文是有关系的。
1. 保存所有的标记流没有意义且浪费空间。

实际的处理方法是提供一个函数`next()`，每次调用该函数则返回下一个标记。

## 支持的标记
```
//全局
// tokens and classes (operators last and in precedence order)
enum {
  Num = 128, Fun, Sys, Glo, Loc, Id,
  Char, Else, Enum, If, Int, Return, Sizeof, While,
  Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec, Brak
};
```
支持的标记符：
例如，将 `=` 解析为 `Assign`；将 `==` 解析为 `Eq`；将 `!=` 解析为 `Ne` 等等
标记是有顺序的，跟它们在 C 语言中的优先级有关，如 `*(Mul)` 的优先级就要高于 `+(Add)` 
自己就构成了标记的字符：方括号 `]` 或波浪号 `~`等等
不另外处理它们的原因是：
1. 它们是单字符的，即并不是多个字符共同构成标记（如 == 需要两个字符）；
1. 它们不涉及优先级关系。

## 词法分析器的框架
即 next() 函数的主体：
```
void next() {
    char *last_pos;
    int hash;

    while (token = *src) {
        ++src;
        // parse token here
    }
    return;
}
```
词法分析器碰到一个不认识的字符该處理方式：
1. 指出错误发生的位置，并退出整个程序
1. 指出错误发生的位置，跳过当前错误并继续编译

> `while` 循环的作用就是跳过这些無法识别的字符，同时還能处理空白字符。C 语言中空格是用来作为分隔用的，并不作为语法的一部分。因此在实现中将它作为“不识别”的字符，这个 `while` 循环可以用来跳过它。

## 换行符
换行符和空格类似，但有一点不同，每次遇到换行符，需要将当前的行号加一：
```
// parse token here
...

if (token == '\n') {
    ++line;
}
...
```
## 宏定义

C 语言的宏定义以字符 `#` 开头，如 #` include <stdio.h>`。原作者的编译器并不支持宏定义，所以直接跳过它们。
```
else if (token == '#') {
    // skip macro, because we will not support it
    while (*src != 0 && *src != '\n') {
        src++;
    }
}
```
## 标识符与符号表

> 标识符（identifier）可以理解为变量名。对于语法分析而言，我们并不关心一个变量具体叫什么名字，而只关心这个变量名代表的唯一标识。例如 `int a`; 定义了变量 `a`，而之后的语句` a = 10`，需要知道这两个 `a` 指向的是同一个变量。
> 基于这个理由，词法分析器会把扫描到的标识符全都保存到一张表中，遇到新的标识符就去查这张表，如果标识符已经存在，就返回它的唯一标识。
```
struct identifier {
    int token;
    int hash;
    char * name;
    int class;
    int type;
    int value;
    int Bclass;
    int Btype;
    int Bvalue;
}
```
1. `token`：该标识符返回的标记，理论上所有的变量返回的标记都应该是 `Id`，但实际上由于我们还将在符号表中加入关键字如 `if`, `while` 等，它们都有对应的标记。
1. `hash`：顾名思义，就是这个标识符的哈希值，用于标识符的快速比较。
1. `name`：存放标识符本身的字符串。
1. `class`：该标识符的类别，如数字，全局变量或局部变量等。
1. `type`：标识符的类型，即如果它是个变量，变量是 `int` 型、`char` 型还是指针型。
1. `value`：存放这个标识符的值，如标识符是函数，刚存放函数的地址。
1. `BXXXX`：C 语言中标识符可以是全局的也可以是局部的，当局部标识符的名字与全局标识符相同时，用作保存全局标识符的信息。
目標為能自舉，定义的语法不支持 `struct`，故而使用下列方式。
```
Symbol table:
----+-----+----+----+----+-----+-----+-----+------+------+----
 .. |token|hash|name|type|class|value|btype|bclass|bvalue| ..
----+-----+----+----+----+-----+-----+-----+------+------+----
    |<---       one single identifier                --->|
```
即用一个整型数组来保存相关的ID信息。每个ID占用数组中的9个空间，分析标识符的相关代码如下：
```
int token_val;                // value of current token (mainly for number)
int *current_id,              // current parsed ID
    *symbols;                 // symbol table

// fields of identifier
enum {Token, Hash, Name, Type, Class, Value, BType, BClass, BValue, IdSize};


void next() {
        ...

        else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || (token == '_')) {

            // parse identifier
            last_pos = src - 1;
            hash = token;

            while ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_')) {
                hash = hash * 147 + *src;
                src++;
            }

            // look for existing identifier, linear search
            current_id = symbols;
            while (current_id[Token]) {
                if (current_id[Hash] == hash && !memcmp((char *)current_id[Name], last_pos, src - last_pos)) {
                    //found one, return
                    token = current_id[Token];
                    return;
                }
                current_id = current_id + IdSize;
            }


            // store new ID
            current_id[Name] = (int)last_pos;
            current_id[Hash] = hash;
            token = current_id[Token] = Id;
            return;
        }
        ...
}
```
### **chatgpt96-123**
> 在這段程式碼中，我們看到了一些與識別子（identifier）相關的變數和函式。
> 
> - `token_val` 是目前識別子的值（主要用於數字）。
> - `current_id` 是目前解析的識別子的指針。
> - `symbols` 是符號表（symbol table）的指針。
> 
> 在符號表中，識別子的相關資訊以欄位形式存儲。這些欄位包括：
> 
> - `Token`：識別子的類型。
> - `Hash`：識別子的雜湊值。
> - `Name`：識別子的名稱。
> - `Type`：識別子的型別。
> - `Class`：識別子的類別。
> - `Value`：識別子的值。
> - `BType`：識別子的基本型別。
> - `BClass`：識別子的基本類別。
> - `BValue`：識別子的基本值。
> - `IdSize`：識別子結構的大小。
> 
> 在 `next()` 函式中，當遇到字母（包括大小寫字母）或底線時，程式會開始解析識別子。它會記錄最後一個字符的位置 `last_pos`，計算識別子的雜湊值 `hash`，並繼續讀取識別子的字符直到不再是字母、數字或底線為止。接著，它會在符號表中進行線性搜索，查找是否已經存在相同的識別子。如果找到了相同的識別子，則將其相應的類型 `Token` 設為目前的 `token` 值。如果找不到相同的識別子，則將新的識別子信息存儲到符號表中，並將 `token` 設置為 `Id`（表示識別子）。
> 
> 這段程式碼處理了識別子的解析和符號表的管理，確保了在編譯過程中能夠正確識別和處理識別子。
> 查找已有标识符的方法是线性查找 `symbols` 表

## 数字

> 数字中较为复杂的一点是需要支持十进制、十六进制及八进制。逻辑也较为直接，可能唯一不好理解的是获取十六进制的值相关的代码。
> `token_val = token_val * 16 + (token & 15) + (token >= 'A' ? 9 : 0);`
### **chatgpt125-145**
> 在這段程式碼中，當遇到數字時，程式會開始解析數字。
> 
> 如果數字以非零數字開頭（範圍從 '1' 到 '9'），那麼它被解析為十進制數字。程式會將該數字的字符形式轉換為對應的數值，並持續讀取後續的數字字符，進行位數上的累加，直到遇到非數字字符為止。最終，該數字的數值將存儲在變數 `token_val` 中。
> 
> 如果數字以 '0' 開頭，那麼它可能是十六進制數字或八進制數字。如果接下來的字符是 'x' 或 'X'，則該數字被解析為十六進制數字。程式會從下一個字符開始讀取，並持續讀取符合十六進制數字範圍的字符（0-9、a-f、A-F），將它們轉換為對應的數值進行位數上的累加，直到遇到非十六進制字符為止。最終，該數字的數值將存儲在變數 `token_val` 中。
> 
> 如果接下來的字符不是 'x' 或 'X'，則該數字被解析為八進制數字。程式會持續讀取符合八進制數字範圍的字符（0-7），將它們轉換為對應的數值進行位數上的累加，直到遇到非八進制字符為止。最終，該數字的數值將存儲在變數 `token_val` 中。
> 
> 解析完成後，程式將 `token` 設置為 `Num`（表示數字）。
> 
> 這段程式碼處理了數字的解析，將數字字符轉換為相應的數值表示，並將其存儲在變數 `token_val` 中，以供後續編譯過程使用。
## 字符串
### **chatgpt164-191**
> 在這段程式碼中，當遇到雙引號（"）或單引號（'）時，程式將開始解析字符串文字。
> 
> 如果遇到雙引號，則程式會從下一個字符開始讀取，並持續讀取非零字符或雙引號字符（表示結束字符串）之間的字符。在讀取過程中，程式會處理跳脫字符。當程式遇到反斜線（\）字符時，它會解析下一個字符並進行處理。如果下一個字符是 'n'，則將其轉換為換行符（'\n'）。如果遇到雙引號字符，則表示字符串解析完成。
> 
> 如果遇到單引號，則程式將讀取下一個字符並將其作為單個字符的數值表示。在讀取過程中，程式不會處理跳脫字符。
> 
> 解析完成後，如果是雙引號，則 token 設置為 Num，並將字符串文字的起始位置存儲在變數 token_val 中。如果是單引號，則 token 仍然是單引號本身。
> 
> 這段程式碼處理了字符串文字的解析，將字符串中的字符轉換為相應的數值表示，並根據解析的類型設置 token 和 token_val 的值，以供後續編譯過程使用。

## 注释
### **chatgpt152-161**
> 在這段程式碼中，當遇到斜杠（/）時，程式會進行特殊處理。
> 
> 如果下一個字符也是斜杠（/），則表示遇到了註解。程式會忽略掉整行的註解內容，直到遇到換行符（'\n'）為止。換句話說，程式會跳過斜杠後面的所有字符，直到達到該行的結尾。
> 
> 如果下一個字符不是斜杠，則表示遇到了除法運算符。此時，程式將將 `token` 設置為 `Div`，表示除法操作，並返回。
> 
> 這段程式碼處理了註解和除法運算符的情況，確保在解析源代碼時能正確處理這些情況，以便後續的編譯過程能正確理解和處理這些符號。

## 其它
### **chatgpt193-299**
> 這段程式碼是在 `next()` 函式中處理各種運算符的情況。根據不同的符號，程式會解析並設置相應的 `token` 值。
> 
> 以下是各個情況的處理：
> 
> - 當 `token` 為等號（`=`）時，程式會檢查下一個字符是否也是等號，若是則將 `token` 設置為 `Eq` 表示相等運算符，否則設置為 `Assign` 表示賦值運算符。
> - 當 `token` 為加號（`+`）時，程式會檢查下一個字符是否也是加號，若是則將 `token` 設置為 `Inc` 表示遞增運算符，否則設置為 `Add` 表示加法運算符。
> - 當 `token` 為減號（`-`）時，程式會檢查下一個字符是否也是減號，若是則將 `token` 設置為 `Dec` 表示遞減運算符，否則設置為 `Sub` 表示減法運算符。
> - 當 `token` 為驚嘆號（`!`）時，程式會檢查下一個字符是否也是等號，若是則將 `token` 設置為 `Ne` 表示不等於運算符。
> - 當 `token` 為小於號（`<`）時，程式會檢查下一個字符是否也是等號或小於號，若是則將 `token` 設置為 `Le` 表示小於等於運算符或 `Shl` 表示位元左移運算符，否則設置為 `Lt` 表示小於運算符。
> - 當 `token` 為大於號（`>`）時，程式會檢查下一個字符是否也是等號或大於號，若是則將 `token` 設置為 `Ge` 表示大於等於運算符或 `Shr` 表示位元右移運算符，否則設置為 `Gt` 表示大於運算符。
> - 當 `token` 為垂直線（`|`）時，程式會檢查下一個字符是否也是垂直線，若是則將 `token` 設置為 `Lor` 表示邏輯或運算符，否則，將 `token` 設置為 `Or`，表示按位或運算符。
> 
> - 當 `token` 為和號（`&`）時，程式會檢查下一個字符是否也是和號，若是則將 `token` 設置為 `Lan`，表示邏輯與運算符，否則設置為 `And`，表示按位與運算符。
> 
> - 當 `token` 為插入符號（`^`）時，將 `token` 設置為 `Xor`，表示按位異或運算符。
> 
> - 當 `token` 為百分比符號（`%`）時，將 `token` 設置為 `Mod`，表示求餘運算符。
> 
> - 當 `token` 為星號（`*`）時，將 `token` 設置為 `Mul`，表示乘法運算符。
> 
> - 當 `token` 為左方括號（`[`）時，將 `token` 設置為 `Brak`，表示陣列下標運算符。
> 
> - 當 `token` 為問號（`?`）時，將 `token` 設置為 `Cond`，表示條件運算符。
> 
> - 當 `token` 為波浪號（`~`）、分號（`;`）、左大括號（`{`）、右大括號（`}`）、左小括號（`(`）、右小括號（`)`）、右方括號（`]`）、逗號（`,`）或冒號（`:`）時，直接將該字符作為 `token` 返回。
> 
> 在每個分支結束時，都使用 `return` 關鍵字終止函式的執行。

## 关键字与内置函数
> 虽然上面写完了词法分析器，但还有一个问题需要考虑，那就是“关键字”，例如 `if`, `while`, `return` 等。它们不能被作为普通的标识符，因为有特殊的含义。
> 
> 一般有两种处理方法：
> 1. 词法分析器中直接解析这些关键字。
> 1. 在语法分析前将关键字提前加入符号表。
> 
> 这里采用第二种方法

### **chatgpt1334-1354**
> 在這段程式碼中，首先定義了變數和函式的類型，包括 CHAR、INT 和 PTR。idmain 變數用於保存 main 函式的資訊。
> 
> 在 main 函式中，將一些關鍵字和庫函式添加到符號表中。src 字符串包含了一些關鍵字和庫函式的名稱。透過迴圈遍歷這些名稱，將它們作為符號添加到符號表中。關鍵字和庫函式都有相應的類型和值。
> 
> 接著調用 next 函式來獲取下一個標記，並為其設置相應的 Token 值，以識別它是哪種類型的符號。迴圈遍歷關鍵字和庫函式名稱時，每次調用 next 函式後，將 current_id[Token] 設置為適當的類型。
> 
> 最後，調用 next 函式來處理 void 類型，並將 current_id 分配給 idmain 變數，以保存 main 函式的資訊。
> 
> 然後繼續執行 program 函式，該函式可能用於解析和執行程式的主體部分。
> 
> 請注意，程式碼中未顯示 next 函式的完整實現，因此無法確定它的細節邏輯。

## chatgpt递归下降
> 遞歸下降（Recursive Descent）是一種常見的語法分析方法，用於解析上下文無關文法（Context-Free Grammar）的語言。在遞歸下降解析器中，每個非終端符號都對應到一個對應的函式，這些函式負責解析相應的非終端符號。
> 
> 在你提供的程式碼中，`program` 函式可能是遞歸下降解析器的入口點，用於解析程式的主體部分。以下是一個簡化的示例，展示了遞歸下降解析器的一些可能實現細節：
> 
> ```cpp
> // Helper function to match the current token and advance to the next token
> void match(int expectedToken) {
>     if (token == expectedToken) {
>         next(); // Advance to the next token
>     } else {
>         // Handle error: Unexpected token
>         // ...
>     }
> }
> 
> // Non-terminal functions corresponding to grammar rules
> void program() {
>     // Parse program
>     // ...
>     statement(); // Call statement function
>     // ...
> }
> 
> void statement() {
>     if (token == IF) {
>         // Parse if statement
>         // ...
>         match(IF); // Match 'if' keyword
>         // ...
>     } else if (token == WHILE) {
>         // Parse while statement
>         // ...
>         match(WHILE); // Match 'while' keyword
>         // ...
>     } else {
>         // Handle error: Invalid statement
>         // ...
>     }
>     // ...
> }
> 
> // Other non-terminal functions for parsing different grammar rules
> 
> // Entry point of the parser
> void parse() {
>     next();     // Get the first token
>     program();  // Start parsing the program
>     // ...
> }
> ```
> 
> 在上述示例中，每個非終端符號（如 `program`、`statement`）對應到一個對應的函式。這些函式負責解析相應的非終端符號，並調用其他相應的函式來處理該規則的子規則。
> 
> 在每個函式中，我們使用 `match` 函式來匹配當前的標記（token）是否符合預期，並前進到下一個標記。如果不符合預期，則可能需要處理錯誤情況。
> 
> 請注意，實際的遞歸下降解析器可能會更複雜，涉及更多的規則和細節。此處僅提供了一個簡化的示例，以展示遞歸下降解析器的基本思想和結構。

## 终结符与非终结符chatgpt
> ```
> <expr> ::= <expr> + <term>
>          | <expr> - <term>
>          | <term>
> 
> <term> ::= <term> * <factor>
>          | <term> / <factor>
>          | <factor>
> 
> <factor> ::= ( <expr> )
>            | Num
> ```
> 在上下文無關文法（CFG）中，文法規則的符號可以分為兩類：終結符（Terminals）和非終結符（Non-terminals）。
> 
> 1. 終結符（Terminals）：終結符是語法中的基本符號，代表語法中的實際字元或單詞。終結符可以是字母、數字、符號或關鍵字。在語法分析過程中，終結符是由輸入的源代碼生成的符號。在代碼中，終結符以字面形式出現。
> 
>    在你提供的程式碼中，例如 `char`、`else`、`enum`、`if`、`int`、`return`、`sizeof`、`while` 等關鍵字都可以被視為終結符。
> 
> 2. 非終結符（Non-terminals）：非終結符是語法中的符號，代表一個抽象的語法結構。非終結符可以由一個或多個終結符和/或其他非終結符組成。在語法分析過程中，非終結符被進一步展開為終結符的序列。在代碼中，非終結符以名稱形式出現。
> 
>    在你提供的程式碼中，例如 `program`、`statement` 就是非終結符，它們代表程式的主體部分和語句部分。
> 
> 綜上所述，終結符和非終結符在上下文無關文法中具有不同的角色，終結符代表語法中的具體單詞或字元，而非終結符代表語法的抽象結構。這種區分有助於建立文法規則，解析語法並生成相應的語法樹或抽象語法樹。

## 四则运算的递归下降
对 `3 * (4 + 2)` 进行语法分析。我们假设词法分析器已经正确地将其中的数字识别成了标记 `Num`。

递归下降是从起始的非终结符开始（顶），本例中是 `<expr>`，实际中可以自己指定，不指定的话一般认为是第一个出现的非终结符。
```
1. <expr> => <expr>
2.           => <term>        * <factor>
3.              => <factor>     |
4.                 => Num (3)   |
5.                              => ( <expr> )
6.                                   => <expr>           + <term>
7.                                      => <term>          |
8.                                         => <factor>     |
9.                                            => Num (4)   |
10.                                                        => <factor>
11.                                                           => Num (2)
```
可以看到，整个解析的过程是在不断对非终结符进行替换（向下），直到遇见了终结符（底）。而我们可以从解析的过程中看出，一些非终结符如`<expr>`被递归地使用了。
    
## 左递归chatgpt
> 左遞歸（Left Recursion）是一種在文法中出現的情況，其中一個非終結符的產生式以自身作為開頭，或者通過一系列產生式的遞迴關係最終以自身作為開頭。這種情況下，該文法存在左遞歸。
> 
> 左遞歸可以導致語法分析器進入無窮遞迴，無法正確地解析語法。因此，在設計文法時，我們通常希望避免左遞歸，或者在存在左遞歸的情況下進行修改。
> 
> 以下是一個具有左遞歸的簡單例子：
> 
> ```
> expr -> expr + term
>       | term
> ```
> 
> 在這個例子中，`expr` 非終結符的產生式以 `expr` 開頭，形成了左遞歸。當我們嘗試對此文法進行語法分析時，解析器可能陷入無窮遞迴，無法完成分析。
> 
> 解決左遞歸的方法之一是進行左遞歸消除。這可以通過將左遞歸產生式重新編寫為等價但不具有左遞歸的形式來實現。對於上述例子，可以進行左遞歸消除如下：
> 
> ```
> expr -> term expr'
> expr' -> + term expr'
>        | ε
> ```
> 
> 在這個修改後的文法中，左遞歸已被消除。`expr` 的產生式不再以 `expr` 開頭，而是以 `term` 開頭，並在後面跟著 `expr'`。`expr'` 則包含了以 `+` 開頭的遞迴部分和一個空產生式，表示終止條件。
> 
> 通過消除左遞歸，我們可以改善文法的可解析性和解析器的效能。在設計文法時，避免左遞歸或進行左遞歸消除是一個重要的技巧。
    
## 四则运算的实现
本节中专注语法分析器部分的实现，具体实现很容易，直接贴上代码，就是上述的消除左递归后的文法直接转换而来的：
```
    int expr();

int factor() {
    int value = 0;
    if (token == '(') {
        match('(');
        value = expr();
        match(')');
    } else {
        value = token_val;
        match(Num);
    }
    return value;
}

int term_tail(int lvalue) {
    if (token == '*') {
        match('*');
        int value = lvalue * factor();
        return term_tail(value);
    } else if (token == '/') {
        match('/');
        int value = lvalue / factor();
        return term_tail(value);
    } else {
        return lvalue;
    }
}

int term() {
    int lvalue = factor();
    return term_tail(lvalue);
}

int expr_tail(int lvalue) {
    if (token == '+') {
        match('+');
        int value = lvalue + term();
        return expr_tail(value);
    } else if (token == '-') {
        match('-');
        int value = lvalue - term();
        return expr_tail(value);
    } else {
        return lvalue;
    }
}

int expr() {
    int lvalue = term();
    return expr_tail(lvalue);
}
```
### chatgpt解釋
> 這段程式碼實現了一個簡單的遞歸下降解析器，用於解析表達式（expression）。它包含了因子（factor）、項（term）和表達式（expr）的解析函數。
> 
> 首先，讓我們來了解一下每個函數的作用：
> 
> - `factor()` 函數解析因子，它檢查當前的 `token`，如果是開括號 `(`，則遞迴調用 `expr()` 解析表達式，然後匹配閉括號 `)`。如果 `token` 不是開括號，則將 `token_val`（表示數字的值）賦值給 `value`，並匹配 `Num`（數字）。
> - `term_tail(int lvalue)` 函數解析項的尾部，接收 `lvalue`（左運算元）作為參數。它檢查當前的 `token`，如果是乘號 `*`，則匹配乘號並將 `lvalue` 與下一個因子的結果相乘，然後遞迴調用 `term_tail()` 繼續處理後面的項。如果是除號 `/`，則類似地處理。如果既不是乘號也不是除號，則直接返回 `lvalue`。
> - `term()` 函數解析項，它首先解析一個因子作為左運算元，然後調用 `term_tail()` 解析可能存在的乘法或除法操作。
> - `expr_tail(int lvalue)` 函數解析表達式的尾部，接收 `lvalue`（左運算元）作為參數。它檢查當前的 `token`，如果是加號 `+`，則匹配加號並將 `lvalue` 與下一個項的結果相加，然後遞迴調用 `expr_tail()` 繼續處理後面的表達式。如果是減號 `-`，則類似地處理。如果既不是加號也不是減號，則直接返回 `lvalue`。
> - `expr()` 函數解析表達式，它首先解析一個項作為左運算元，然後調用 `expr_tail()` 解析可能存在的加法或減法操作。
> 
> 這種遞歸下降的解析方法可以根據運算符的優先級和結合性進行解析，確保表達式的正確計算順序。在這個例子中，因子的優先級最高，然後是項，最後是表達式。這意味著表達式將按照正確的優先級進行計算，例如，`2 + 3 * 4` 將被解析為 `2 + (3 * 4)`。
> 
> 這段程式碼的主要思想是使用遞歸下降方法根據文法的規則進行解析，每個函數對應於文法的一個非終結符號。它通過調用相應的解析函數來處理不同的表達式結構，並利用遞迴的方式實現優先級的處理。
> 
> 在主函數 `main()` 中，我們可以看到程式碼建立了一個表示關鍵字和庫函數的符號表，並初始化了 `src` 變數以包含一些關鍵字和庫函數的字串。然後，它使用 `next()` 函數和 `current_id` 陣列將關鍵字和庫函數添加到符號表中。
> 
> 最後，它調用 `expr()` 函數來解析整個表達式。
> 
> 這段程式碼只是一個簡單的示例，並不包含所有可能的表達式結構和錯誤處理。實際上，解析器的實現可能更複雜，需要處理更多的文法規則和錯誤情況。這只是一個基礎的框架，可以擴展和修改以適應更多的需求。
    
可以看到，有了BNF方法后，采用递归向下的方法来实现编译器是很直观的。
### 詞法分析器 
```
##include <stdio.h>
##include <stdlib.h>

enum {Num};
int token;
int token_val;
char *line = NULL;
char *src = NULL;

void next() {
    // skip white space
    while (*src == ' ' || *src == '\t') {
        src ++;
    }

    token = *src++;

    if (token >= '0' && token <= '9' ) {
        token_val = token - '0';
        token = Num;

        while (*src >= '0' && *src <= '9') {
            token_val = token_val*10 + *src - '0';
            src ++;
        }
        return;
    }
}

void match(int tk) {
    if (token != tk) {
        printf("expected token: %d(%c), got: %d(%c)\n", tk, tk, token, token);
        exit(-1);
    }
    next();
}
```
### chatgpt
> 這段程式碼是一個簡單的解析器，用於解析包含數字的表達式。它包含一些基本的功能，例如讀取下一個令牌（token）、匹配令牌等。
> 
> 以下是程式碼中的主要部分的說明：
> 
> - `next()` 函數：該函數用於讀取下一個令牌。它首先跳過空白字符，然後讀取一個字符並將其存儲在 `token` 中。如果該字符是數字字符，則開始解析數字並將其存儲在 `token_val` 中。
> 
> - `match()` 函數：該函數用於驗證當前的令牌是否與給定的令牌匹配。如果不匹配，它會輸出一條錯誤消息，指示預期的令牌和實際獲得的令牌，然後終止程式。
> 
> 在這個程式碼中，只有一種終結符號 `Num`，表示數字令牌。當讀取到數字時，使用 `token` 變數來存儲令牌類型，使用 `token_val` 變數來存儲數字的值。
> 
> 這只是解析器的一部分，它僅提供了基本的令牌讀取和匹配功能。要完整解析表達式，可能需要更多的程式碼來處理不同類型的運算符、優先級規則等。
> 
> 請注意，這段程式碼可能不完整，缺少一些必要的部分來構建完整的解析器。要構建一個完整的解析器，需要進一步開發並添加相關功能。
    
最后是main函数：
```
int main(int argc, char *argv[])
{
    size_t linecap = 0;
    ssize_t linelen;
    while ((linelen = getline(&line, &linecap, stdin)) > 0) {
        src = line;
        next();
        printf("%d\n", expr());
    }
    return 0;
}
```
### chatgpt
> 這是 `main` 函數的程式碼，它用於從標準輸入讀取多行表達式，並對每個表達式進行解析和評估。
> 
> 程式碼的主要部分如下：
> 
> ```c
> int main(int argc, char *argv[])
> {
>     size_t linecap = 0;
>     ssize_t linelen;
>     while ((linelen = getline(&line, &linecap, stdin)) > 0) {
>         src = line;
>         next();
>         printf("%d\n", expr());
>     }
>     return 0;
> }
> ```
> 
> 在 `main` 函數中，它使用 `getline` 函數從標準輸入逐行讀取表達式。讀取的每一行都存儲在 `line` 變數中。接下來，它設置 `src` 變數為 `line`，以便在解析器中使用。然後調用 `next()` 函數來獲取第一個令牌。
> 
> 接下來，它使用 `expr()` 函數來解析和評估表達式，並將結果以整數形式打印出來。
> 
> 循環繼續讀取下一行表達式，直到到達文件結尾或發生錯誤。最後，返回 0 表示程式執行成功結束。
> 
> 請注意，這段程式碼假設表達式以一行一行的形式從標準輸入讀取。並且在進行表達式求值時，假設 `expr()` 函數已經被實現並能夠正確解析和評估表達式。
> 