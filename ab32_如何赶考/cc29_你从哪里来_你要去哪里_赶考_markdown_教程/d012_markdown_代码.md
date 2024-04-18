<font size="4">Markdown 代码</font>

并不一定真要放代码时才能用这个标签，这里举两个例子：

> 1.如果要重点突出某个字，可以用行内代码标签
> 
> 2.如果不想让Markdown渲染某段文字，可以用代码块标签进行包裹

Markdown中代码格式化可以通过三种方式：

> 行内式：通过一对 ` 包裹单行代码/文字实现；
> 
> 代码块：通过一对 ``` 包裹实现，可以指定代码的编程语言，如Python、JavaScript、Rust等，如果渲染器支持，将实现语法高亮；
> 
> 缩进式：通过缩进至少四个空格实现，部分支持语法高亮。

前两个很常用，缩进式写起来很恶心，不推荐。

ChaTi/轻记APP，代码块支持高亮显示，支持标注以下语言：

```
brainf*ck , c , clike , clojure , cpp , csharp , css , dart , git , go , groovy , java , javascript , json , kotlin , latex , makefile , markdown , markup , python , scala , sql , swift , yaml
```

注：请使用真实的语言名称（不是别名）。所以，应用 javascript 而不是 js ，应用 markup 而不是 xml 。另外，所有语言名称一律为小写字母。

---

<font color="#FF7F00">语法示例：</font>

```
1.行内式代码： `import os, sys`

2.代码块（无需缩进）：

\``` python
import os, sys

sys.path

print(os)
\```

3.缩进式：

    if (isAwesome){
      return true
    }
```

注：为避免标签渲染冲突，代码块标签前面我加了个反斜杠。

<font color="#7093DB">渲染效果：</font>

1.行内式代码： `import os, sys`

2.代码块（无需缩进）：

``` python
import os, sys

sys.path

print(os)
```

3.缩进式：

    if (isAwesome){
      return true
    }

