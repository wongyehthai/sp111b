> # 内容主要來源：
> [**手把手教你构建 C 语言编译器5-9**](https://lotabout.me/2015/write-a-C-interpreter-1/)<br>
> [**Chatgpt**](https://chat.openai.com)<br>
> [**程式碼來源**](https://disq.us/url?url=https%3A%2F%2Fgithub.com%2Flotabout%2Fwrite-a-C-interpreter%3AZVUO1v-fpg4Id2pxKHnbjUN-7_I&cuid=2272845)

# 变量定义
## EBNF 表示 chatgpt
> EBNF（擴展巴科斯范式）是一種擴展的上下文無關文法表示法，用於描述語言的語法結構。它使用一組規則來定義語法的產生式和終結符。以下是一個簡單的示例 EBNF 表示：
> 
> ```
> <expr> ::= <term> { ('+' | '-') <term> }
> <term> ::= <factor> { ('*' | '/') <factor> }
> <factor> ::= '(' <expr> ')' | <number>
> <number> ::= <digit> { <digit> }
> <digit> ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
> ```
> 
> 在這個例子中，我們定義了一個簡單的算術表達式語法，其中 `<expr>` 表示一個表達式，由 `<term>` 和可選的一系列加法或減法運算符和 `<term>` 構成。同樣地，`<term>` 由 `<factor>` 和可選的一系列乘法或除法運算符和 `<factor>` 構成。`<factor>` 可以是括號中的 `<expr>` 或者是 `<number>`。`<number>` 是一個或多個 `<digit>` 的序列，而 `<digit>` 則是數字 0 到 9 中的任意一個。
> 
> EBNF 通常使用尖括號 `< >` 表示非終結符，使用單引號或雙引號表示終結符，並使用竖线 `|` 表示選擇（或）操作。大括號 `{ }` 表示零次或多次重複，方括號 `[ ]` 表示可選的元素。
> 
> EBNF 是一種直觀且易於閱讀和理解的文法表示法，它在描述語言的語法結構方面非常有用。
    
## 解析变量的定义
### chatgpt1205-1209
> 根據提供的程式碼片段，`program()` 函式是用於解析整個程式的入口點。以下是該函式的解釋：
> 
> ```c
> void program() {
>     // 取得下一個 token
>     next();
>     while (token > 0) {
>         global_declaration();
>     }
> }
> ```
> 
> 這個函式的主要目的是使用 `next()` 函式取得下一個 token，並且在仍然有 token 的情況下進入一個迴圈。在迴圈內部，調用 `global_declaration()` 函式來解析全域宣告。
> 
> 該程式碼片段並未提供 `global_declaration()` 函式的內容，因此無法對其進行進一步解釋。如果需要更多的協助，請提供 `global_declaration()` 函式的實現細節。

## global_declaration()
### chatgpt1129-1202
> 根據提供的程式碼片段，`global_declaration()` 函式用於解析全域宣告。以下是該函式的 EBNF 表示：
> 
> ```
> global_declaration ::= enum_decl | variable_decl | function_decl
> 
> enum_decl ::= 'enum' [id] '{' id ['=' 'num'] {',' id ['=' 'num']} '}'
> 
> variable_decl ::= type {'*'} id {',' {'*'} id} ';'
> 
> function_decl ::= type {'*'} id '(' parameter_decl ')' '{' body_decl '}'
> ```
> 
> 根據上述表示法，`global_declaration()` 函式進行以下操作：
> 
> 1. 初始化 `basetype` 為 `INT`。
> 2. 若目前的 `token` 為 `Enum`，則進行 `enum` 宣告的解析。
>    - 如果 `token` 不是 `{`，則跳過 `id` 部分。
>    - 如果 `token` 為 `{`，則解析分配部分。
>    - 解析完成後，匹配分號 `;`。
> 3. 解析型別資訊。
>    - 若 `token` 為 `Int`，則匹配 `Int`。
>    - 若 `token` 為 `Char`，則匹配 `Char`，並將 `basetype` 設為 `CHAR`。
> 4. 解析逗號分隔的變數宣告。
>    - 設定 `type` 為 `basetype`。
>    - 解析指標型別，注意可能存在 `int ****x;` 的情況。
>    - 若 `token` 不是 `Id`，則顯示無效的宣告並終止程式。
>    - 若 `current_id[Class]` 不為零，則表示該識別符已存在，顯示重複的宣告並終止程式。
>    - 匹配 `Id`。
>    - 若 `token` 為 `(`，則表示是函式宣告，設定 `current_id[Class]` 為 `Fun`，並記錄函式的記憶體位址。
>    - 否則，表示是變數宣告，設定 `current_id[Class]` 為 `Glo`，並分配記憶體位址。
>    - 若 `token` 為逗號 `,`，則匹配逗號。
> 5. 取得下一個 token。
> 
> 根據程式碼，該程式碼片段並未提供 `enum_declaration()`、`function_declaration()`、`match()` 和其他可能的子函式的內容。如果需要進一步的協助，請提供這些子函式的實現細節。

## enum_declaration()
### chatgpt970-988
> 根據提供的程式碼片段，`enum_declaration()` 函式用於解析 `enum` 的宣告。以下是該函式的 EBNF 表示：
> 
> ```
> enum_declaration ::= 'enum' [id] '{' id ['=' 'num'] {',' id ['=' 'num']} '}'
> ```
> 
> 根據上述表示法，`enum_declaration()` 函式進行以下操作：
> 
> 1. 初始化計數變數 `i` 為 0。
> 2. 在 `token` 不是 `}` 的情況下循環執行以下步驟：
>    - 若 `token` 不是 `Id`，則顯示無效的列舉識別符並終止程式。
>    - 取得下一個 token。
>    - 若 `token` 為 `Assign`，則表示有初始化值，執行以下步驟：
>      - 取得下一個 token。
>      - 若 `token` 不是 `Num`，則顯示無效的列舉初始化值並終止程式。
>      - 將 `i` 設為 `token_val`。
>      - 取得下一個 token。
>    - 設定 `current_id` 的屬性：
>      - `Class` 為 `Num`。
>      - `Type` 為 `INT`。
>      - `Value` 為 `i` 的值，並將 `i` 遞增。
>    - 若 `token` 為逗號 `,`，則取得下一個 token。
> 
> 根據程式碼，該程式碼片段並未提供 `next()` 和其他可能的子函式的內容。如果需要進一步的協助，請提供這些子函式的實現細節。

## `match` 函数
### chatgpt
> `match()` 函式用於比對目前的 `token` 是否符合預期的 `tk`。若相符，則呼叫 `next()` 函式取得下一個 `token`，否則顯示錯誤訊息並終止程式。
> 
> 根據提供的程式碼片段，`match()` 函式的 EBNF 表示為：
> 
> ```
> match(tk) ::= if (token == tk) { next(); } else { display error message and exit; }
> ```
> 
> 請注意，EBNF 表示法中的 `tk` 是 `token` 的符號表示，並非字面值。
> 
> 這個函式非常簡單，只需比對目前的 `token` 與預期的 `tk` 是否相等，並根據結果執行相應的動作。
    
# 函数定义
## 解析函数的定义
```
...
if (token == '(') {
    current_id[Class] = Fun;
    current_id[Value] = (int)(text + 1); // the memory address of function
    function_declaration();
} else {
...
```
即在这断代码之前，已经为当前的标识符（identifier）设置了正确的类型，上面这断代码为当前的标识符设置了正确的类别（Fun），以及该函数在代码段（text segment）中的位置。接下来开始解析函数定义相关的内容：`parameter_decl` 及 `body_decl`。
    
### 函数参数与汇编代码gpt
> 以下是示範如何在程式中處理函式參數以及相應的汇编代码：
> 
> ```c
> void function_declaration() {
>     // function_declaration ::= type {'*'} id '(' parameter_decl ')' '{' body_decl '}'
> 
>     match('(');
>     parameter_declaration();
>     match(')');
>     match('{');
>     body_declaration();
>     match('}');
> }
> 
> void parameter_declaration() {
>     // parameter_declaration ::= type {'*'} id {',' type {'*'} id}
> 
>     int type;
>     int param_count = 0;
> 
>     while (token != ')') {
>         type = basetype;
>         // parse pointer type
>         while (token == Mul) {
>             match(Mul);
>             type = type + PTR;
>         }
> 
>         if (token != Id) {
>             printf("%d: bad parameter declaration\n", line);
>             exit(-1);
>         }
> 
>         // assign memory address for the parameter
>         current_id[BClass] = current_id[Class];  // parameter is treated as local variable
>         current_id[BType] = current_id[Type];
>         current_id[BValue] = current_id[Value];
>         current_id[Class] = Loc;  // update the class of the identifier to Local
>         current_id[Type] = type;
>         current_id[Value] = param_count++;  // assign a position for the parameter
> 
>         match(Id);
> 
>         if (token == ',') {
>             match(',');
>         }
>     }
> }
> ```
> 
> 上述程式碼片段中的 `function_declaration()` 函式處理函式宣告，包括參數宣告和函式主體的處理。在 `parameter_declaration()` 函式中，我們處理函式的參數宣告，包括參數的型別和名稱。在這個範例中，我們將參數視為本地變數，並為每個參數指定一個記憶體位置。
> 
> 至於汇编代码，它將根據特定的架構和編譯器而有所不同。汇编代码的生成通常涉及將高階程式碼轉換為該架構的組合語言指令。因此，我無法直接提供與你的程式碼相對應的完整汇编代码。不過，我可以提供一些示範性的汇编指令，用於顯示如何在汇编層面處理函式參數。
> 
> ```assembly
> ; 函式宣告
> function:
>     push ebp           ; 儲存舊的堆疊底指標
>     mov ebp, esp       ; 設定新的堆疊底指標
>     sub esp, <size>    ; 為本地變數開啟堆疊空間
> 
>     ; 參數存取
>     mov eax, [ebp+8]   ; 存取第一個參數
>     mov ebx, [ebp+12]  ; 存取第二個參數
>     ...
> 
>     ; 函式主體
>     ...
> 
>     ; 函式結束
>     mov esp, ebp       ; 還原堆疊指標
>     pop ebp            ; 還原舊的堆疊底指標
>     ret                ; 返回呼叫點
> ```
> 
> 這只是一個簡單的示範，真實的汇编代码會根據不同的編譯器和架構有所不同。在這個示例中，我們使用 `push` 指令將舊的堆疊底指標儲存起來，然後使用 `mov` 指令將新的堆疊底指標設定為當前的堆疊指標。接著，使用 `sub` 指令為本地變數開啟堆疊空間。
> 
> 在參數存取的部分，我們使用 `mov` 指令將參數值從相對於堆疊底指標的偏移位置複製到暫存器中，以便在函式內部使用。
> 
> 函式主體部分省略了實際的程式碼，你可以在這個部分實現函式的邏輯。
> 
> 最後，在函式結束時，我們使用 `mov` 指令將堆疊指標還原回舊的堆疊底指標位置，然後使用 `pop` 指令將舊的堆疊底指標還原回暫存器中，最後使用 `ret` 指令返回到函式的呼叫點。
> 
> 請注意，上述汇编代码僅為示範，實際生成的汇编代码可能會因編譯器和架構而有所不同。建議使用特定的編譯器和架構工具鏈來生成汇编代碼，以確保正確性和最佳性能。
    
## 函数定义的解析gpt
    
> 函式定義的解析可以根據語言的文法進行，以下是一個簡單的示例，展示了如何解析函式定義的部分：
> 
> ```c
> void function_declaration() {
>     match('(');
>     parameter_declaration();
>     match(')');
>     match('{');
>     body_declaration();
>     match('}');
> }
> 
> void parameter_declaration() {
>     // 解析參數列表，例如：int a, int b, char* c
>     while (token != ')') {
>         int type = basetype;
>         int pointer_count = 0;
> 
>         // 解析指標類型，例如：int*，可能存在多級指標
>         while (token == Mul) {
>             match(Mul);
>             pointer_count++;
>         }
> 
>         // 解析參數名稱
>         if (token != Id) {
>             printf("%d: expected parameter name\n", line);
>             exit(-1);
>         }
>         match(Id);
> 
>         // 將參數添加到符號表
>         current_id[Class] = Loc;  // 區域變數
>         current_id[Type] = type + pointer_count;
>         current_id[Value] = ebp + local_variables_count * sizeof(int);
> 
>         // 更新區域變數計數
>         local_variables_count++;
> 
>         // 解析下一個參數
>         if (token == ',') {
>             match(',');
>         }
>     }
> }
> 
> void body_declaration() {
>     // 解析函式主體內的語句和宣告
>     while (token != '}') {
>         if (token == Int || token == Char) {
>             // 宣告區域變數
>             variable_declaration();
>         } else {
>             // 解析語句
>             statement();
>         }
>     }
> }
> 
> void variable_declaration() {
>     // 解析變數宣告，例如：int a, b, c;
>     int type = basetype;
>     int pointer_count = 0;
> 
>     // 解析指標類型
>     while (token == Mul) {
>         match(Mul);
>         pointer_count++;
>     }
> 
>     // 解析變數名稱
>     if (token != Id) {
>         printf("%d: expected variable name\n", line);
>         exit(-1);
>     }
>     match(Id);
> 
>     // 將變數添加到符號表
>     current_id[Class] = Loc;  // 區域變數
>     current_id[Type] = type + pointer_count;
>     current_id[Value] = ebp + local_variables_count * sizeof(int);
> 
>     // 更新區域變數計數
>     local_variables_count++;
> 
>     // 解析下一個變數宣告
>     if (token == ',') {
>         match(',');
>         variable_declaration();
>     }
> 
>     match(';');
> }
> 
> void statement() {
>     // 解析語句
>     // ...
> }
> ```
> 
> 上述示例中的函式 `function_declaration` 負責解析函式的定義，其中調用了其他函式 `parameter_declaration`、`body_declaration` 和 `variable_declaration` 來解析參數、函式主體和變數宣告部分。你可以根据需要扩展 `statement` 函数来解析更多类型的语句，例如控制流语句（if、while、for）、表达式语句、返回语句等。以下是一个示例，展示了如何扩展 `statement` 函数来解析 if 语句和返回语句：
> 
> ```c
> void statement() {
>     // 解析语句
>     if (token == If) {
>         // 解析 if 语句
>         match(If);
>         match('(');
>         int condition = expr();
>         match(')');
> 
>         // 生成条件跳转指令
>         int label1 = new_label();
>         int label2 = new_label();
>         emit("CMP", condition, 0);
>         emit("JE", label1);
>         statement();
>         emit("JMP", label2);
>         emit_label(label1);
>         match(Else);
>         statement();
>         emit_label(label2);
>     } else if (token == Return) {
>         // 解析返回语句
>         match(Return);
>         if (token != ';') {
>             int value = expr();
>             emit("MOV", eax, value);
>         }
>         emit("JMP", exit_label);
>         match(';');
>     } else {
>         // 其他类型的语句，例如表达式语句
>         expr();
>         match(';');
>     }
> }
> ```
> 
> 在这个示例中，我们通过判断当前的 `token` 类型来决定解析哪种类型的语句。对于 if 语句，我们首先解析条件表达式，然后生成条件跳转指令，跳转到相应的代码块。对于返回语句，我们解析返回值表达式（如果有的话），然后生成相应的指令。对于其他类型的语句，我们简单地解析表达式并匹配分号。
> 
> 需要注意的是，以上示例仅为演示如何扩展 `statement` 函数，实际的语法和指令生成可能需要根据具体的编程语言和目标架构进行调整和扩展。

### chatgpt1106-1125
> 这段代码是用于解析函数声明的函数 `function_declaration()`。让我们逐步分析这个函数的实现：
> 
> 1. 匹配左括号 `'('`，表示函数参数的开始。
> 2. 调用 `function_parameter()` 函数，解析函数参数。
> 3. 匹配右括号 `')'`，表示函数参数的结束。
> 4. 匹配左花括号 `'{'`，表示函数体的开始。
> 5. 调用 `function_body()` 函数，解析函数体。
> 6. 注释掉 `match('}')` 的代码行 `// ①`。这意味着在函数声明中不会匹配右花括号 `'}'`，这个代码段可能存在错误或者不完整。
> 7. 解开局部变量声明。使用循环遍历符号表中的所有符号，如果符号的类别为局部变量（`current_id[Class] == Loc`），则将其类别、类型和值恢复为备份变量中保存的值（`current_id[Class] = current_id[BClass]`，`current_id[Type] = current_id[BType]`，`current_id[Value] = current_id[BValue]`）。
> 8. 更新 `current_id` 指针，将其指向下一个符号的位置，以继续循环遍历符号表。
> 
> 需要注意的是，代码中的注释 `// ①` 表示在这个位置不会匹配右花括号 `'}'`，这可能会导致匹配不完整或出现错误。你可能需要根据实际需要取消注释或根据语言规范进行调整。
> 
> 此外，以上代码片段仅展示了函数声明的解析部分，并未包含完整的语法和错误处理。实际的实现可能需要根据具体的语言和编译器进行调整和完善。
## 解析参数
### chatgpt1001-1041
> 这段代码是用于解析函数参数的函数 `function_parameter()`。让我们逐步分析这个函数的实现：
> 
> 1. 初始化变量 `type` 和 `params`，其中 `params` 用于记录参数个数。
> 
> 2. 进入循环，直到遇到右括号 `')'`，解析函数参数的声明。
> 
>    a. 首先解析参数的类型 `type`。如果当前的 `token` 是 `Int`，则表示参数类型为 `INT`，如果是 `Char`，则表示参数类型为 `CHAR`。在解析指针类型时，循环匹配 `Mul`，每次循环将 `type` 加上 `PTR`。
> 
>    b. 解析参数的名称。如果当前的 `token` 不是 `Id`，则表示参数声明有误；如果当前的参数已经在局部变量中存在（`current_id[Class] == Loc`），则表示重复的参数声明。
> 
>    c. 在存储参数之前，我们需要保存当前的局部变量的信息。为此，我们将当前的类别、类型和值存储在备份变量中（`current_id[BClass]`、`current_id[BType]`、`current_id[BValue]`），并将当前的类别设为局部变量（`current_id[Class] = Loc`），类型设为参数类型（`current_id[Type] = type`），值设为当前参数的索引（`current_id[Value] = params++`）。
> 
>    d. 如果当前的 `token` 是逗号 `','`，则表示还有更多的参数，继续匹配。
> 
> 3. 循环结束后，`index_of_bp` 的值为参数个数加 1，用于定位 `bp` 指针在栈上的索引。
> 
> 需要注意的是，以上代码片段仅展示了如何解析函数参数，并没有包含完整的语法和错误处理。实际的实现可能需要根据具体的语言和编译器进行调整和完善。

## 函数体的解析
### chatgpt1044-1103
> 这段代码是用于解析函数体的函数 `function_body()`。让我们逐步分析这个函数的实现：
> 
> 1. 声明一个变量 `pos_local`，用于记录局部变量在栈上的位置。
> 2. 将 `pos_local` 初始化为 `index_of_bp`，即参数的个数加一，表示局部变量相对于基址指针的偏移量。
> 3. 进入循环 `while (token == Int || token == Char)`，用于解析局部变量的声明。
>    - 解析局部变量的基本类型，将其存储在变量 `basetype` 中。
>    - 匹配基本类型的关键字，即 `Int` 或 `Char`。
>    - 进入循环 `while (token != ';')`，用于解析同一行上的多个局部变量声明。
>      - 解析局部变量的类型，包括指针类型，将其存储在变量 `type` 中。
>      - 解析局部变量的标识符，即变量名。
>      - 存储局部变量的信息到符号表中：类别设为局部变量（`current_id[Class] = Loc`），类型设为 `type`，值设为当前局部变量在栈上的位置（`++pos_local`）。
>      - 如果遇到逗号 `,`，表示同一行上还有其他局部变量声明，继续循环。
>    - 匹配分号 `;`，表示局部变量声明结束。
> 4. 保存局部变量在栈上的空间大小。
>    - 将指令 `ENT` 存储到代码段中，表示进入一个新的函数作用域。
>    - 将局部变量占用的空间大小（`pos_local - index_of_bp`）存储到代码段中，用于之后调整栈指针。
> 5. 进入循环 `while (token != '}')`，用于解析函数体中的语句。
>    - 调用 `statement()` 函数，解析一条语句。
> 6. 发出代码，用于离开子函数。
>    - 将指令 `LEV` 存储到代码段中，表示离开当前函数作用域。
> 
> 需要注意的是，这段代码片段中使用了注释 `// ①` 和 `// ②` 标记的位置，分别表示代码的两个重要部分。在 `// ①` 处，解析局部变量的声明；在 `// ②` 处，保存局部变量的栈空间大小以及函数体中的语句解析。这些位置的代码应该根据语言规范进行调整和完善。
> 
> 此外，以上代码片段仅展示了函数体的解析部分，并未包含完整的语法和错误处理。实际的实现可能需要根据具体的语言和编译器进行调整和完善。

# 语句
> C 语言区分“语句”（statement）和“表达式”（expression）两个概念。简单地说，可以认为语句就是表达式加上末尾的分号。
> 
> 在我们的编译器中共识别 6 种语句：
> 
> 1. `if (...) <statement> [else <statement>]`
> 1. `while (...) <statement>`
> 1. `{ <statement> }`
> 1. `return xxx;`
> 1. `<empty statement>`;
> 1. `expression;` (expression end with semicolon)
> 它们的语法分析都相对容易，重要的是去理解如何将这些语句编译成汇编代码，下面我们逐一解释。

## IF 语句
IF 语句的作用是跳转，跟据条件表达式决定跳转的位置。我们看看下面的伪代码：

```
if (...) <statement> [else <statement>]

  if (<cond>)                   <cond>
                                JZ a
    <true_statement>   ===>     <true_statement>
  else:                         JMP b
a:                           a:
    <false_statement>           <false_statement>
b:                           b:
```
对应的汇编代码流程为：

1. 执行条件表达式 `<cond>`。
1. 如果条件失败，则跳转到 `a` 的位置，执行 `else` 语句。这里 `else` 语句是可以省略的，此时 `a` 和 `b` 都指向 IF 语句后方的代码。
1. 因为汇编代码是顺序排列的，所以如果执行了 `true_statement`，为了防止因为顺序排列而执行了 `false_statement`，所以需要无条件跳转 `JMP b`。
        
### chatgpt876-910
> 这段代码是用于解析 `if` 语句的部分。让我们逐步分析这个代码块的实现：
> 
> 1. 匹配关键字 `If`。
> 2. 匹配左括号 `(`。
> 3. 调用 `expression(Assign)` 解析条件表达式。
> 4. 匹配右括号 `)`。
> 5. 发出条件判断指令 `JZ`（跳转到零），并将跳转目标地址存储在变量 `b` 所指向的位置。
> 6. 解析 `if` 语句的主体部分，即调用 `statement()` 解析语句。
> 7. 检查当前的 `token` 是否为关键字 `Else`，表示存在 `else` 分支。
>    - 如果存在 `else` 分支，则继续解析 `else` 分支的语句。
>    - 如果不存在 `else` 分支，则跳过该部分。
> 8. 发出无条件跳转指令 `JMP`，将跳转目标地址存储在变量 `b` 所指向的位置。这个跳转目标地址在第 6 步中记录为 `text + 3`，即跳过 `else` 分支的代码。
> 9. 更新之前发出的条件判断指令 `JZ` 的跳转目标地址为当前的 `text + 1`。这里使用 `text + 1` 是为了跳过整个 `if` 语句的执行流程，使程序继续执行 `if` 语句之后的代码。
> 
> 需要注意的是，这段代码片段中并未展示完整的错误处理和控制流程的生成。此外，变量 `b` 在这里被使用为一个临时变量，用于存储跳转目标地址。具体的实现可能需要根据语言规范和编译器的要求进行调整。
> 
> 以上代码块展示了如何解析和生成 `if` 语句的代码。在实际的编译器实现中，还需要考虑其他因素，例如 `if` 语句的嵌套、条件表达式的类型检查、生成对应的汇编指令等。
        
## While 语句
While 语句比 If 语句简单，它对应的汇编代码如下：

```
a:                     a:
   while (<cond>)        <cond>
                         JZ b
    <statement>          <statement>
                         JMP a
b:                     b:
```
### chatgpt911-935
> 这段代码是用于解析 `else if` 语句的部分。让我们逐步分析这个代码块的实现：
> 
> 1. 匹配关键字 `While`。
> 2. 将变量 `a` 设置为当前的 `text + 1`。这里的 `a` 用于记录 `while` 循环开始的位置。
> 3. 匹配左括号 `(`。
> 4. 调用 `expression(Assign)` 解析条件表达式。
> 5. 匹配右括号 `)`。
> 6. 发出条件判断指令 `JZ`（跳转到零），并将跳转目标地址存储在变量 `b` 所指向的位置。
> 7. 解析 `while` 循环的主体部分，即调用 `statement()` 解析语句。
> 8. 发出无条件跳转指令 `JMP`，将跳转目标地址设置为变量 `a` 所记录的位置，即回到 `while` 循环的开始。
> 9. 更新之前发出的条件判断指令 `JZ` 的跳转目标地址为当前的 `text + 1`。这里使用 `text + 1` 是为了跳过整个 `while` 循环的执行流程，使程序继续执行 `while` 循环之后的代码。
> 
> 需要注意的是，这段代码片段中并未展示完整的错误处理和控制流程的生成。此外，变量 `a` 和 `b` 在这里被使用为临时变量，用于存储跳转目标地址。具体的实现可能需要根据语言规范和编译器的要求进行调整。
> 
> 以上代码块展示了如何解析和生成 `else if` 语句的代码。在实际的编译器实现中，还需要考虑其他因素，例如 `else if` 语句的嵌套、条件表达式的类型检查、生成对应的汇编指令等。
## Return 语句
### chatgpt946-958
> 这段代码用于解析 `else if` 语句中的 `return` 语句。让我们逐步分析这个代码块的实现：
> 
> 1. 匹配关键字 `Return`。
> 2. 如果当前的 `token` 不是分号 `;`，则调用 `expression(Assign)` 解析返回表达式。
> 3. 匹配分号 `;`，表示返回语句结束。
> 4. 发出函数返回的汇编指令 `LEV`，用于退出当前函数。
> 
> 这段代码块处理了 `return` 语句的解析和生成对应的汇编指令。如果存在返回值表达式，将调用 `expression(Assign)` 解析并生成相应的指令。最后，发出函数返回的汇编指令 `LEV`，表示函数执行结束并返回。
> 
> 需要注意的是，这里并未展示完整的错误处理和控制流程的生成。具体的实现可能需要根据语言规范和编译器的要求进行调整。
		
## 其它语句
```
else if (token == '{') {
    // { <statement> ... }
    match('{');

    while (token != '}') {
        statement();
    }

    match('}');
}
else if (token == ';') {
    // empty statement
    match(';');
}
else {
    // a = b; or function_call();
    expression(Assign);
    match(';');
}
```
### chatgpt
> 这段代码处理了 `else if` 语句中的多种情况，包括代码块、空语句以及赋值语句或函数调用语句。让我们逐步分析这个代码块的实现：
> 
> 1. 如果当前的 `token` 是左花括号 `{`，表示开始一个代码块。匹配左花括号，并使用循环解析内部的语句，直到遇到右花括号 `}`。
> 2. 如果当前的 `token` 是分号 `;`，表示为空语句。匹配分号即可。
> 3. 否则，当前的 `token` 可能是赋值语句或函数调用语句。调用 `expression(Assign)` 解析表达式，然后匹配分号 `;` 结束语句的解析。
> 
> 这段代码块处理了 `else if` 语句中的不同情况。它允许解析代码块、空语句以及赋值语句或函数调用语句，并相应地执行相应的操作。
> 
> 需要注意的是，这里并未展示完整的错误处理和控制流程的生成。具体的实现可能需要根据语言规范和编译器的要求进行调整。

# 表达式
## 运算符的优先级gpt
> 运算符的优先级是指在表达式中各个运算符的执行顺序。具有较高优先级的运算符将在具有较低优先级的运算符之前执行。如果表达式中存在多个运算符，可以使用优先级来确定它们的计算顺序，以确保表达式的求值结果符合预期。
> 
> 以下是常见运算符的优先级从高到低的一般顺序（同一行表示相同优先级）：
> 
> 1. 一元运算符:
>    - 前缀递增/递减 (++/-- 前缀)
>    - 正号 (+) 和负号 (-)
>    - 逻辑非 (!) 和按位非 (~)
>    - 强制类型转换 (例如，(int), (float))
> 
> 2. 乘法和除法:
>    - 乘法 (*)、除法 (/) 和取模/取余 (%)
> 
> 3. 加法和减法:
>    - 加法 (+) 和减法 (-)
> 
> 4. 移位运算符:
>    - 左移位 (<<) 和右移位 (>>)
> 
> 5. 按位与运算符:
>    - 按位与 (&)
> 
> 6. 按位异或运算符:
>    - 按位异或 (^)
> 
> 7. 按位或运算符:
>    - 按位或 (|)
> 
> 8. 关系运算符:
>    - 大于 (>)、小于 (<)、大于等于 (>=) 和小于等于 (<=)
> 
> 9. 相等性运算符:
>    - 相等 (==) 和不等 (!=)
> 
> 10. 逻辑与运算符:
>     - 逻辑与 (&&)
> 
> 11. 逻辑或运算符:
>     - 逻辑或 (||)
> 
> 12. 赋值运算符:
>     - 简单赋值 (=) 和复合赋值运算符 (例如，+=, -=, *=, /=)
> 
> 运算符的优先级可以使用括号来改变默认的计算顺序。括号内的表达式将优先计算，然后再考虑其他运算符的优先级。
> 
> 需要注意的是，不同的编程语言可能会略有不同的运算符优先级规则。因此，在具体的编程语言中，应该参考相应的语言规范或文档来了解确切的运算符优先级。
> 
> 希望以上信息能对你有所帮助！如果你有任何其他问题，请随时提问。

## 一元运算符
### 常量
首先是数字，用 `IMM` 指令将它加载到 `AX` 中即可：

```
if (token == Num) {
    match(Num);

    // emit code
    *++text = IMM;
    *++text = token_val;
    expr_type = INT;
}
```
		
>GPT
> 如果 `token` 是数字（`Num`），则匹配成功，可以继续解析表达式。
> 
> 代码解析如下：
> 
> - 匹配成功，继续向前获取下一个标记（token）。
> - 生成代码指令 `IMM`，表示立即数（即常量），指令的操作数是 `token_val`，即当前数字的值。
> - 更新 `expr_type` 为 `INT`，表示表达式的类型为整数类型。
> 
> 这段代码的作用是处理数字，并在代码中生成相应的指令来处理该数字。
> 
> 请注意，上述代码片段只是一个部分，还需要根据上下文的语法规则来完整解析表达式。
接着是字符串常量。它比较特殊的一点是 C 语言的字符串常量支持如下风格：

```
char *p;
p = "first line"
    "second line";
```
即跨行的字符串拼接，它相当于：

```
char *p;
p = "first linesecond line";
```
所以解析的时候要注意这一点：

```
else if (token == '"') {
    // emit code
    *++text = IMM;
    *++text = token_val;

    match('"');
    // store the rest strings
    while (token == '"') {
        match('"');
    }

    // append the end of string character '\0', all the data are default
    // to 0, so just move data one position forward.
    data = (char *)(((int)data + sizeof(int)) & (-sizeof(int)));
    expr_type = PTR;
}
```
>GPT
> 如果 `token` 是双引号（`"`），则匹配成功，可以继续解析表达式。
> 
> 代码解析如下：
> 
> - 匹配成功，继续向前获取下一个标记（token）。
> - 生成代码指令 `IMM`，表示立即数（即常量），指令的操作数是 `token_val`，即字符串的值。
> - 匹配双引号 `"`，表示字符串的开始。
> - 使用循环来匹配字符串中的字符，直到遇到结束的双引号 `"`。
> - 在存储字符串之后，将数据向前移动一个位置，以便附加字符串结束符 `\0`。
> - 更新 `expr_type` 为 `PTR`，表示表达式的类型为指针类型。
> 
> 这段代码的作用是处理字符串，并在代码中生成相应的指令来处理该字符串。
> 
> 请注意，上述代码片段只是一个部分，还需要根据上下文的语法规则来完整解析表达式。
		
## sizeof
### chatgpt365-392
> 如果 `token` 是 `Sizeof`，则匹配成功，可以继续解析表达式。
> 
> 根据代码注释中的提示，`sizeof` 实际上是一个一元运算符，可以处理 `sizeof(int)`、`sizeof(char)` 和 `sizeof(*...)`。
> 
> 以下是相应的代码解析：
> 
> ```c
> else if (token == Sizeof) {
>     // sizeof is actually an unary operator
>     // now only `sizeof(int)`, `sizeof(char)` and `sizeof(*...)` are
>     // supported.
>     match(Sizeof);
>     match('(');
>     expr_type = INT;
> 
>     if (token == Int) {
>         match(Int);
>     } else if (token == Char) {
>         match(Char);
>         expr_type = CHAR;
>     }
> 
>     while (token == Mul) {
>         match(Mul);
>         expr_type = expr_type + PTR;
>     }
> 
>     match(')');
> 
>     // emit code
>     *++text = IMM;
>     *++text = (expr_type == CHAR) ? sizeof(char) : sizeof(int);
> 
>     expr_type = INT;
> }
> ```
> 
> 代码解析如下：
> 
> - 匹配成功，继续向前获取下一个标记（token）。
> - 匹配左括号 `(`。
> - 设置 `expr_type` 为 `INT`，表示表达式的类型为整型。
> - 如果 `token` 是 `Int`，则匹配成功，继续向前获取下一个标记，并不做任何操作。
> - 如果 `token` 是 `Char`，则匹配成功，继续向前获取下一个标记，并将 `expr_type` 更新为 `CHAR`，表示表达式的类型为字符型。
> - 使用循环来匹配可能出现的 `*`，每次匹配成功，将 `expr_type` 更新为 `expr_type + PTR`，表示表达式的类型为指针类型。
> - 匹配右括号 `)`。
> - 生成代码指令 `IMM`，表示立即数（即常量），指令的操作数是根据 `expr_type` 的值来确定 `sizeof(char)` 或 `sizeof(int)`。
> - 更新 `expr_type` 为 `INT`，表示表达式的类型为整型。
> 
> 这段代码的作用是处理 `sizeof` 运算符，并在代码中生成相应的指令来计算类型的大小。
> 
> 请注意，上述代码片段只是一个部分，还需要根据上下文的语法规则来完整解析表达式。

## 变量与函数调用
### chatgpt393-468
> 如果 `token` 是 `Id`，则匹配成功，可以继续解析表达式。
> 
> 根据代码注释中的提示，当遇到 `Id` 时，可能有以下几种情况：
> 1. 函数调用
> 2. 枚举变量
> 3. 全局变量或局部变量
> 
> 以下是相应的代码解析：
> 
> ```c
> else if (token == Id) {
>     // there are several type when occurs to Id
>     // but this is unit, so it can only be
>     // 1. function call
>     // 2. Enum variable
>     // 3. global/local variable
>     match(Id);
> 
>     id = current_id;
> 
>     if (token == '(') {
>         // function call
>         match('(');
> 
>         // ①
>         // pass in arguments
>         tmp = 0; // number of arguments
>         while (token != ')') {
>             expression(Assign);
>             *++text = PUSH;
>             tmp++;
> 
>             if (token == ',') {
>                 match(',');
>             }
>         }
>         match(')');
> 
>         // ②
>         // emit code
>         if (id[Class] == Sys) {
>             // system functions
>             *++text = id[Value];
>         } else if (id[Class] == Fun) {
>             // function call
>             *++text = CALL;
>             *++text = id[Value];
>         } else {
>             printf("%d: bad function call\n", line);
>             exit(-1);
>         }
> 
>         // ③
>         // clean the stack for arguments
>         if (tmp > 0) {
>             *++text = ADJ;
>             *++text = tmp;
>         }
>         expr_type = id[Type];
>     } else if (id[Class] == Num) {
>         // ④
>         // enum variable
>         *++text = IMM;
>         *++text = id[Value];
>         expr_type = INT;
>     } else {
>         // ⑤
>         // variable
>         if (id[Class] == Loc) {
>             *++text = LEA;
>             *++text = index_of_bp - id[Value];
>         } else if (id[Class] == Glo) {
>             *++text = IMM;
>             *++text = id[Value];
>         } else {
>             printf("%d: undefined variable\n", line);
>             exit(-1);
>         }
> 
>         // ⑥
>         // emit code, default behaviour is to load the value of the
>         // address which is stored in `ax`
>         expr_type = id[Type];
>         *++text = (expr_type == Char) ? LC : LI;
>     }
> }
> ```
> 
> 代码解析如下：
> 
> - 匹配成功，继续向前获取下一个标记（token）。
> - 将当前标识符的信息存储到变量 `id` 中，以便后续使用。
> - 如果 `token` 是左括号 `(`，则表示这是一个函数调用。
>   - 匹配左括号 `(`。
>   - ①：传递参数。
>     - 使用循环解析参数表达式，并生成代码指令 `PUSH` 将参数压栈。
>     - 同时记录参数的数量 `tmp`。
>     - 如果参数之间有逗号 `,`，则匹配逗号并继续解析下一个参数。
> - 匹配右括号 `)`，表示函数调用的参数解析结束。
>   - ②：生成代码指令。
>     - 如果 `id` 的 `Class` 是 `Sys`，表示系统函数，则生成对应的系统函数指令。
>     - 如果 `id` 的 `Class` 是 `Fun`，表示函数调用，则生成 `CALL` 指令，并将函数的地址作为操作数。
>     - 否则，输出错误信息，表示函数调用错误，并退出程序。
> - ③：清理函数调用时压栈的参数。
>   - 如果参数数量 `tmp` 大于 0，则生成 `ADJ` 指令，并将参数数量作为操作数，用于调整栈指针。
> - 如果 `id` 的 `Class` 是 `Num`，表示枚举变量。
>   - ④：生成 `IMM` 指令，将枚举变量的值作为操作数。
>   - 表达式类型 `expr_type` 为 `INT`。
> - 否则，表示变量。
>   - ⑤：根据变量的 `Class` 生成相应的指令。
>     - 如果 `id` 的 `Class` 是 `Loc`，表示局部变量，则生成 `LEA` 指令，将局部变量的地址计算结果作为操作数。
>     - 如果 `id` 的 `Class` 是 `Glo`，表示全局变量，则生成 `IMM` 指令，将全局变量的地址作为操作数。
>     - 否则，输出错误信息，表示变量未定义，并退出程序。
>   - ⑥：生成代码指令，根据变量的类型加载相应的值。
>     - 如果表达式类型 `expr_type` 是 `Char`，则生成 `LC` 指令，从地址中加载一个字节。
>     - 否则，生成 `LI` 指令，从地址中加载一个字。
> 
> 以上是对代码的解析，根据不同情况生成相应的代码指令，并维护表达式类型 `expr_type` 的值。
		
## 强制转换
### chatgpt470-491
> 如果当前的 `token` 是左括号 `(`，则表示可能是类型转换或者普通的括号表达式。
> 
> - 先匹配左括号 `(`。
> - 如果接下来的 `token` 是 `Int` 或者 `Char`，则表示是类型转换。
>   - 临时变量 `tmp` 被赋值为 `Char` 或者 `INT`，取决于 `token` 的类型。
>   - 匹配 `token`。
>   - 如果之后有多个 `Mul` 符号，则将 `tmp` 的值增加 `PTR` 的数量。
>   - 匹配右括号 `)`。
>   - 调用 `expression(Inc)` 进行表达式解析，因为类型转换具有比自增运算符 `++` 更高的优先级。
>   - 表达式的类型 `expr_type` 被设置为 `tmp`。
> - 否则，表示是普通的括号表达式。
>   - 调用 `expression(Assign)` 进行表达式解析。
>   - 匹配右括号 `)`。

## 指针取值
### chatgpt492-505
> 如果当前的 `token` 是乘号 `Mul`，则表示可能是解引用操作。
> 
> - 先匹配乘号 `Mul`。
> - 调用 `expression(Inc)` 进行表达式解析，因为解引用操作具有与自增运算符 `++` 相同的优先级。
> - 如果表达式类型 `expr_type` 是指针类型（大于等于 `PTR`）：
>   - 将表达式类型 `expr_type` 减去 `PTR`，得到解引用后的类型。
> - 否则，表示解引用操作应用在非指针类型的表达式上，这是错误的。
>   - 输出错误信息并退出程序。
> - 根据解引用后的类型，生成相应的指令：
>   - 如果解引用后的类型是 `CHAR`，则生成 `LC` 指令，表示加载字符值。
>   - 如果解引用后的类型是 `INT`，则生成 `LI` 指令，表示加载整数值。
## 取址操作
### chatgpt506-518
> 如果当前的 `token` 是按位与运算符 `And`，则表示可能是获取地址操作。
> 
> - 先匹配按位与运算符 `And`。
> - 调用 `expression(Inc)` 进行表达式解析，因为获取地址操作具有与自增运算符 `++` 相同的优先级。
> - 如果生成的代码指令的前一个指令是 `LC`（加载字符值）或 `LI`（加载整数值）：
>   - 将代码指令指针 `text` 减一，相当于移除加载操作的指令，因为获取地址操作会使加载操作无效。
> - 否则，表示获取地址操作应用在无效的表达式上，这是错误的。
>   - 输出错误信息并退出程序。
> - 将表达式类型 `expr_type` 加上 `PTR`，表示获取地址后的类型是指针类型。

## 逻辑取反
### chatgpt519-531
> 如果当前的 `token` 是逻辑非运算符 `!`，则表示进行逻辑非操作。
> 
> - 先匹配逻辑非运算符 `!`。
> - 调用 `expression(Inc)` 进行表达式解析。
> - 发出代码指令，使用 `<expr> == 0` 进行逻辑判断。
>   - 将 `PUSH` 操作指令存入代码段，将表达式的值入栈。
>   - 将 `IMM` 操作指令存入代码段，将常量值 `0` 入栈。
>   - 将 `EQ` 操作指令存入代码段，比较栈顶两个值是否相等，并将结果入栈。
> - 将表达式类型 `expr_type` 设置为 `INT`，表示逻辑非操作的结果类型为整数类型。
## 按位取反
### chatgpt532-544
> 如果当前的 `token` 是位取反运算符 `~`，则表示进行位取反操作。
> 
> - 先匹配位取反运算符 `~`。
> - 调用 `expression(Inc)` 进行表达式解析。
> - 发出代码指令，使用 `<expr> XOR -1` 进行位取反操作。
>   - 将 `PUSH` 操作指令存入代码段，将表达式的值入栈。
>   - 将 `IMM` 操作指令存入代码段，将常量值 `-1` 入栈。
>   - 将 `XOR` 操作指令存入代码段，对栈顶的两个值进行按位异或操作，并将结果入栈。
> - 将表达式类型 `expr_type` 设置为 `INT`，表示位取反操作的结果类型为整数类型。

## 正负号
### chatgpt545-570
> 如果当前的 `token` 是加法运算符 `+`，则表示进行正数操作。
> 
> - 先匹配加法运算符 `+`。
> - 调用 `expression(Inc)` 进行表达式解析。
> 
> 由于正数操作不改变表达式的值，因此不需要发出额外的代码指令。
> 
> 最后将表达式类型 `expr_type` 设置为 `INT`，表示正数操作的结果类型为整数类型。
> 
> 如果当前的 `token` 是减法运算符 `-`，则表示进行负数操作。
> 
> - 先匹配减法运算符 `-`。
> - 判断下一个 `token` 是否为数字常量 `Num`。
>   - 如果是数字常量，表示负数操作的操作数是一个常量。
>     - 发出代码指令 `IMM`，将常量值取反并入栈。
>     - 匹配数字常量 `Num`。
>   - 如果不是数字常量，表示负数操作的操作数是一个表达式。
>     - 发出代码指令，先将常量值 `-1` 入栈，然后将表达式的值入栈。
>     - 发出代码指令 `MUL`，对栈顶的两个值进行乘法运算，并将结果入栈。
> 
> 最后将表达式类型 `expr_type` 设置为 `INT`，表示负数操作的结果类型为整数类型。
## 自增自减
### chatgpt571-590
> 如果当前的 `token` 是递增运算符 `++` 或递减运算符 `--`，则表示进行前缀递增或前缀递减操作。
> 
> - 先将当前的 `token` 保存到 `tmp` 变量中。
> - 匹配当前的 `token`。
> - 调用 `expression(Inc)` 进行表达式解析。
> 
> 下面是前缀递增和前缀递减操作的处理步骤：
> 
> 1. 检查指令序列中的最后一个指令。如果最后一个指令是 `LC`（加载字符）、`LI`（加载整数）之一，则表示操作的是一个左值（可修改的值）。否则，报告错误并退出。
> 
> 2. 在指令序列中插入指令以复制地址或值：
>    - 如果最后一个指令是 `LC`，则需要复制地址。插入指令 `PUSH` 将地址入栈，然后插入指令 `LC`。
>    - 如果最后一个指令是 `LI`，则需要复制值。插入指令 `PUSH` 将值入栈，然后插入指令 `LI`。
> 
> 3. 插入指令以将操作数的大小入栈：
>    - 如果表达式类型 `expr_type` 大于指针类型 `PTR`，表示操作数为整数类型，插入指令 `IMM` 将整数类型的大小（`sizeof(int)`）入栈。
>    - 如果表达式类型 `expr_type` 等于字符类型 `CHAR`，表示操作数为字符类型，插入指令 `IMM` 将字符类型的大小（`sizeof(char)`）入栈。
> 
> 4. 插入指令以执行递增或递减操作：
>    - 如果 `tmp` 等于 `Inc`，表示进行前缀递增操作，插入指令 `ADD` 将操作数与大小相加。
>    - 如果 `tmp` 等于 `Dec`，表示进行前缀递减操作，插入指令 `SUB` 将操作数与大小相减。
> 
> 5. 插入指令以存储结果：
>    - 如果表达式类型 `expr_type` 等于字符类型 `CHAR`，插入指令 `SC` 存储字符。
>    - 如果表达式类型 `expr_type` 等于整数类型 `INT` 或指针类型 `PTR`，插入指令 `SI` 存储整数。
> 
> 最后，将表达式类型 `expr_type` 设置为整数类型 `INT`，表示前缀递增或前缀递减操作的结果类型为整数类型。
## 二元运算符
### chatgpt
> 二元运算符具有不同的优先级和结合性。以下是常见的二元运算符及其优先级（从高到低）：
> 
> 1. 乘法（`*`）、除法（`/`）、取模（`%`）
> 2. 加法（`+`）、减法（`-`）
> 3. 左移（`<<`）、右移（`>>`）
> 4. 大于（`>`）、小于（`<`）、大于等于（`>=`）、小于等于（`<=`）
> 5. 相等（`==`）、不等（`!=`）
> 6. 按位与（`&`）
> 7. 按位异或（`^`）
> 8. 按位或（`|`）
> 9. 逻辑与（`&&`）
> 10. 逻辑或（`||`）
> 
> 当存在多个二元运算符时，具有更高优先级的运算符会先进行计算。例如，`3 + 4 * 2` 中，乘法运算符的优先级更高，先计算 `4 * 2`，然后再与 `3` 相加。
> 
> 需要注意的是，对于相同优先级的二元运算符，结合性决定了它们的计算顺序。例如，`2 - 3 - 4` 中，减法运算符具有左结合性，因此先计算 `2 - 3`，再与 `4` 相减。
> 
> 根据以上规则，可以在语法分析中正确处理二元运算符的优先级和结合性，生成相应的指令序列来执行运算操作。

之前定义过的各个标记，它们是以优先级从低到高排列的，即 `Assign` 的优先级最低，而 `Brak`（`[`） 的优先级最高。

```
enum {
  Num = 128, Fun, Sys, Glo, Loc, Id,
  Char, Else, Enum, If, Int, Return, Sizeof, While,
  Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec, Brak
};
```
所以，当我们调用 `expression(level)` 进行解析的时候，我们其实通过了参数 `level` 指定了当前的优先级。在前文的一元运算符处理中也用到了这一点。

所以，此时的二元运算符的解析的框架为：

```
while (token >= level) {
    // parse token for binary operator and postfix operator
}
```
## 赋值操作
赋值操作是优先级最低的运算符。考虑诸如 `a = (expession)` 的表达式，在解析 `=` 之前，我们已经为变量 `a` 生成了如下的汇编代码：

```
IMM <addr>
LC/LI
```
当解析完`=`右边的表达式后，相应的值会存放在 `ax` 中，此时，为了实际将这个值保存起来，我们需要类似下面的汇编代码：

```
IMM <addr>
PUSH
SC/SI
```
### chatgpt601-615
> 在这段代码中，首先将当前的表达式类型保存到临时变量 `tmp` 中，以便在赋值完成后恢复。然后检查当前的标记是否为赋值运算符（`Assign`）。
> 
> 如果标记是赋值运算符，表示出现了赋值表达式，即 `var = expr;` 的形式。接下来的步骤如下：
> 
> 1. 检查当前指令序列中的内容，判断是否是可赋值的左值（lvalue）。这里通过检查当前指令是否为 `LC`（取字符指令）或 `LI`（取整数指令）来判断。如果是可赋值的左值，将其转换为右值，即保存左值的指针。
> 
> 2. 调用 `expression(Assign)`，解析并生成右值表达式的指令序列。
> 
> 3. 恢复表达式类型为之前保存的值 `tmp`。
> 
> 4. 根据表达式类型的不同，生成赋值操作的指令。如果表达式类型为字符类型（`CHAR`），生成存储字符指令（`SC`），否则生成存储整数指令（`SI`）。
> 
> 这样，就完成了赋值表达式的处理，将右值赋给左值。
		
## 三目运算符
### chatgpt616-633
> 在这段代码中，首先检查当前的标记是否为条件运算符（`Cond`），即三元条件表达式的形式 `expr ? a : b`。
> 
> 如果是条件运算符，接下来的步骤如下：
> 
> 1. 生成一个条件判断的指令，使用 `JZ` 指令（跳转为零）来判断前面的表达式是否为假（即为零）。将该指令的地址存储在 `addr` 变量中，并在指令序列中占位。
> 
> 2. 调用 `expression(Assign)`，解析并生成条件为真时的表达式 `a` 的指令序列。
> 
> 3. 检查当前标记是否为冒号（`:`），表示条件运算符的分隔符。如果不是冒号，则打印错误信息并退出。
> 
> 4. 将前面占位的跳转指令的地址更新为当前指令序列的下一个位置（即跳过条件为真时的表达式）。
> 
> 5. 生成一个无条件跳转指令 `JMP`，将其地址存储在 `addr` 变量中，并在指令序列中占位。
> 
> 6. 调用 `expression(Cond)`，解析并生成条件为假时的表达式 `b` 的指令序列。
> 
> 7. 将前面占位的跳转指令的地址更新为当前指令序列的下一个位置（即跳过条件为假时的表达式）。
> 
> 通过上述步骤，就完成了条件运算符的处理，根据条件的真假选择执行相应的表达式。

## 逻辑运算符
这包括 `||` 和 `&&`。它们对应的汇编代码如下：

```
<expr1> || <expr2>     <expr1> && <expr2>

  ...<expr1>...          ...<expr1>...
  JNZ b                  JZ b
  ...<expr2>...          ...<expr2>...
b:                     b:
```
### chatgpt634-651
> 在这段代码中，首先检查当前的标记是否为逻辑或运算符（`Lor`）或逻辑与运算符（`Lan`）。
> 
> 如果是逻辑或运算符，接下来的步骤如下：
> 
> 1. 生成一个条件判断的指令，使用 `JNZ` 指令（跳转非零）来判断前面的表达式是否为真（即非零）。将该指令的地址存储在 `addr` 变量中，并在指令序列中占位。
> 
> 2. 调用 `expression(Lan)`，解析并生成逻辑或运算符右侧表达式的指令序列。
> 
> 3. 将前面占位的跳转指令的地址更新为当前指令序列的下一个位置（即跳过逻辑或运算符右侧表达式）。
> 
> 4. 将表达式类型 `expr_type` 设置为 `INT`，表示逻辑运算结果的类型为整数。
> 
> 如果是逻辑与运算符，接下来的步骤如下：
> 
> 1. 生成一个条件判断的指令，使用 `JZ` 指令（跳转为零）来判断前面的表达式是否为假（即为零）。将该指令的地址存储在 `addr` 变量中，并在指令序列中占位。
> 
> 2. 调用 `expression(Or)`，解析并生成逻辑与运算符右侧表达式的指令序列。
> 
> 3. 将前面占位的跳转指令的地址更新为当前指令序列的下一个位置（即跳过逻辑与运算符右侧表达式）。
> 
> 4. 将表达式类型 `expr_type` 设置为 `INT`，表示逻辑运算结果的类型为整数。
> 
> 通过上述步骤，就完成了逻辑或运算符和逻辑与运算符的处理，根据表达式的真假进行相应的逻辑运算。

## 数学运算符
它们包括 `|`, `^`, `&`, `==`, `!=` `<=`, `>=`, `<`, `>`, `<<`, `>>`, `+`, `-`, `*`, `/`, `%`。它们的实现都很类似，我们以异或 `^` 为例：

```
<expr1> ^ <expr2>

...<expr1>...          <- now the result is on ax
PUSH
...<expr2>...          <- now the value of <expr2> is on ax
XOR
```
### chatgpt660-667
> 在这段代码中，处理的是位异或运算符（`Xor`）。
> 
> 首先，匹配并消耗掉当前标记为位异或运算符。
> 
> 然后，生成一个 `PUSH` 指令，将栈顶的值保存起来。
> 
> 接下来，调用 `expression(And)`，解析并生成位异或运算符右侧表达式的指令序列。
> 
> 最后，生成一个 `XOR` 指令，将保存的值与右侧表达式的结果进行位异或运算。
> 
> 将表达式类型 `expr_type` 设置为 `INT`，表示位异或运算的结果类型为整数。
> 
> 通过以上步骤，完成了位异或运算符的处理，对两个操作数进行位异或运算并生成相应的指令序列。

其它的我们便不再详述。但这当中还有一个问题，就是指针的加减。在 C 语言中，指针加上数值等于将指针移位，且根据不同的类型移动的位移不同。如 `a + 1`，如果 `a` 是 `char *` 型，则移动一字节，而如果 `a` 是 `int *` 型，则移动 4 个字节（32位系统）。

另外，在作指针减法时，如果是两个指针相减（相同类型），则结果是两个指针间隔的元素个数。因此要有特殊的处理。
下面以加法为例，对应的汇编代码为：

```
<expr1> + <expr2>

normal         pointer

<expr1>        <expr1>
PUSH           PUSH
<expr2>        <expr2>     |
ADD            PUSH        | <expr2> * <unit>
               IMM <unit>  |
               MUL         |
               ADD
```
即当 `<expr1>` 是指针时，要根据它的类型放大 `<expr2>` 的值
### chatgpt740-755
> 在这段代码中，处理的是加法运算符（`Add`）。
> 
> 首先，匹配并消耗掉当前标记为加法运算符。
> 
> 然后，生成一个 `PUSH` 指令，将栈顶的值保存起来。
> 
> 接下来，调用 `expression(Mul)`，解析并生成加法运算符右侧表达式的指令序列。
> 
> 根据之前保存的表达式类型 `tmp`，判断当前表达式类型是否为指针类型且不是 `char *` 类型。
> 
> 如果是指针类型，生成额外的指令序列用于进行指针运算。首先，生成一个 `PUSH` 指令将整数值 `sizeof(int)` 入栈。然后，生成一个 `MUL` 指令，将栈顶的值与表达式结果相乘。
> 
> 最后，生成一个 `ADD` 指令，将保存的值与右侧表达式的结果进行加法运算。
> 
> 将表达式类型 `expr_type` 设置为之前保存的类型 `tmp`，表示加法运算的结果类型与操作数类型保持一致。
> 
> 通过以上步骤，完成了加法运算符的处理，对两个操作数进行加法运算并生成相应的指令序列。如果操作数中存在指针类型，则进行指针运算。
## 自增自减
这次是后缀形式的，即 `p++` 或 `p--`。与前缀形式不同的是，在执行自增自减后， `ax`上需要保留原来的值。所以我们首先执行类似前缀自增自减的操作，再将 `ax` 中的值执行减/增的操作。

```
// 前缀形式 生成汇编代码
*++text = PUSH;
*++text = IMM;
*++text = (expr_type > PTR) ? sizeof(int) : sizeof(char);
*++text = (tmp == Inc) ? ADD : SUB;
*++text = (expr_type == CHAR) ? SC : SI;

// 后缀形式 生成汇编代码
*++text = PUSH;
*++text = IMM;
*++text = (expr_type > PTR) ? sizeof(int) : sizeof(char);
*++text = (token == Inc) ? ADD : SUB;
*++text = (expr_type == CHAR) ? SC : SI;
*++text = PUSH;                                             //
*++text = IMM;                                              // 执行相反的增/减操作
*++text = (expr_type > PTR) ? sizeof(int) : sizeof(char);   //
*++text = (token == Inc) ? SUB : ADD;                       //
```
>GPT
> 这段代码处理的是前缀形式和后缀形式的自增和自减运算符。
> 
> 对于前缀形式，代码生成如下：
> 
> ```cpp
> *++text = PUSH;
> *++text = IMM;
> *++text = (expr_type > PTR) ? sizeof(int) : sizeof(char);
> *++text = (tmp == Inc) ? ADD : SUB;
> *++text = (expr_type == CHAR) ? SC : SI;
> ```
> 
> 首先，生成一个 `PUSH` 指令，将操作数的值入栈。
> 
> 接下来，生成一个 `IMM` 指令，将常数值入栈。这个常数值表示自增或自减的增量，根据操作符 `tmp`（前缀形式只能是自增）和操作数类型 `expr_type`（如果是 `char` 类型，就使用 `SC` 指令，否则使用 `SI` 指令）来确定增量的大小。
> 
> 然后，生成一个运算指令（`ADD` 或 `SUB`），将栈顶的值与增量相加或相减，得到结果。
> 
> 对于后缀形式，代码生成如下：
> 
> ```cpp
> *++text = PUSH;
> *++text = IMM;
> *++text = (expr_type > PTR) ? sizeof(int) : sizeof(char);
> *++text = (token == Inc) ? ADD : SUB;
> *++text = (expr_type == CHAR) ? SC : SI;
> *++text = PUSH;
> *++text = IMM;
> *++text = (expr_type > PTR) ? sizeof(int) : sizeof(char);
> *++text = (token == Inc) ? SUB : ADD;
> ```
> 
> 首先，生成一个 `PUSH` 指令，将操作数的值入栈。
> 
> 接下来，生成一个 `IMM` 指令，将常数值入栈，表示自增或自减的增量，根据操作符 `token`（后缀形式可以是自增或自减）和操作数类型 `expr_type` 来确定增量的大小。
> 
> 然后，生成一个运算指令（`ADD` 或 `SUB`），将栈顶的值与增量相加或相减，得到结果。
> 
> 接着，生成一个 `PUSH` 指令，将之前计算得到的结果再次入栈。
> 
> 最后，生成一个 `IMM` 指令，将常数值入栈，表示相反的增量（即负值），根据操作符 `token` 和操作数类型 `expr_type` 来确定增量的大小。
> 
> 通过以上步骤，完成了前缀形式和后缀形式的自增和自减运算符的处理，生成了相应的汇编代码。

## 数组取值操作
在学习 C 语言的时候你可能已经知道了，诸如 `a[10]` 的操作等价于 `*(a + 10)`。因此我们要做的就是生成类似的汇编代码：
### chatgpt835-856
> 这段代码处理数组访问操作 `var[xx]`。
> 
> 代码生成如下：
> 
> ```cpp
> else if (token == Brak) {
>     // array access var[xx]
>     match(Brak);
>     *++text = PUSH;
>     expression(Assign);
>     match(']');
> 
>     if (tmp > PTR) {
>         // pointer, `not char *`
>         *++text = PUSH;
>         *++text = IMM;
>         *++text = sizeof(int);
>         *++text = MUL;
>     }
>     else if (tmp < PTR) {
>         printf("%d: pointer type expected\n", line);
>         exit(-1);
>     }
>     expr_type = tmp - PTR;
>     *++text = ADD;
>     *++text = (expr_type == CHAR) ? LC : LI;
> }
> ```
> 
> 首先，匹配 `token` 为 `Brak`，表示数组访问的开始。
> 
> 然后，生成一个 `PUSH` 指令，将数组索引 `xx` 的值入栈。
> 
> 接下来，匹配右方括号 `]`，表示数组访问的结束。
> 
> 根据 `tmp` 的值判断操作数的类型。如果 `tmp` 大于 `PTR`，表示操作数是一个指针类型而不是 `char *`，则生成一系列指令来计算数组偏移量。首先，生成一个 `PUSH` 指令，将常数值 `sizeof(int)`（即整数类型的大小）入栈。然后，生成一个 `MUL` 指令，将数组索引与整数类型大小相乘，得到偏移量。
> 
> 如果 `tmp` 小于 `PTR`，则表示操作数类型错误，输出错误信息并退出程序。
> 
> 接下来，根据 `tmp` 减去 `PTR` 的结果确定新的表达式类型 `expr_type`，表示数组元素的类型。
> 
> 然后，生成一个 `ADD` 指令，将数组的基地址与偏移量相加，得到数组元素的地址。
> 
> 最后，根据 `expr_type` 的值生成相应的指令。如果 `expr_type` 是 `CHAR` 类型，则生成 `LC` 指令，表示加载字符型数组元素的值。如果 `expr_type` 是其他类型，则生成 `LI` 指令，表示加载整型数组元素的值。
> 
> 通过以上步骤，完成了数组访问操作的处理，生成了相应的汇编代码。

## 代码
除了上述对表达式的解析外，我们还需要初始化虚拟机的栈，我们可以正确调用 `main` 函数，且当 `main` 函数结束时退出进程。

```
int *tmp;
// setup stack
sp = (int *)((int)stack + poolsize);
*--sp = EXIT; // call exit if main returns
*--sp = PUSH; tmp = sp;
*--sp = argc;
*--sp = (int)argv;
*--sp = (int)tmp;
```
当然，最后要注意的一点是：所有的变量定义必须放在语句之前。

## 總結
作者每一章節都會有一個小結，而且當中混雜著他各種小心思，有興趣的人真的非常推薦去看，最後一章是作者的總結所以我就不多做了，歡迎直接去看。雖説是手把手教學，但我認爲還是應該要有一定的C語言基礎才來看這篇文章，因爲感覺作者用了各種語法來寫，看到一半就放棄了，後期製作都在流水的感覺QQ，都在用GPT解釋，但還是需要時間去理解，找時間會回來惡補。