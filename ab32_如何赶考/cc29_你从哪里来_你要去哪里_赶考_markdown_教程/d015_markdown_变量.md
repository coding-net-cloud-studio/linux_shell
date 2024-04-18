<font size="4">Markdown 变量</font>

Markdown 中支持变量定义和变量引用，且支持中文。

一处定义，处处使用，而且方便统一修改。

变量定义：[key]:value
变量引用：[key]、[text][key]

<font color="#FF7F00">语法示例：</font>

```
步骤1.定义链接

[小北IT官网]:https://xiaobeiit.cn

步骤2.引用链接：

(1) key引用方式：[小北IT官网]

(2) value引用方式：[自定义文本][小北IT官网]
```

<font color="#7093DB">渲染效果：</font>

步骤1.定义链接

[小北IT官网]:https://xiaobeiit.cn

步骤2.引用链接：

(1) key引用方式：[小北IT官网]

(2) value引用方式：[自定义文本][小北IT官网]

---

#### 进阶玩法

> yun://是 ChaTi/轻记 独创的云链链接协议，旨在弥补纯Markdown文章缺失的功能。
> 
> 已有云链功能：复制区域文本、复制指定文字、导出长图、回到文章顶部、移至文章底部。

<font color="#FF7F00">语法示例：</font>

```
步骤1.定义链接

[回到顶部]:yun://art.site/?i=top

步骤2.引用链接：

(1) key引用方式：[回到顶部]

(2) value引用方式：[自定义文本][回到顶部]
```

<font color="#7093DB">渲染效果：</font>

步骤1.定义链接

[回到顶部]:yun://art.site/?i=top

步骤2.引用链接：

(1) key引用方式：[回到顶部]

(2) value引用方式：[自定义文本][回到顶部]

---

#### 快捷复制

<font color="#FF7F00">回到顶部：</font>

[变量1-回到顶部](yun://art.copy/?t=[回到顶部]:yun://art.site/?i=top)

[变量1-调用标签](yun://art.copy/?t=[][回到顶部])

<font color="#FF7F00">移至底部：</font>

[变量2-移至底部](yun://art.copy/?t=[移至底部]:yun://art.site/?i=bottom)

[变量2-调用标签](yun://art.copy/?t=[][移至底部])

食用方法：

可将定义变量的文字扔到文章底部，在需使用该变量的位置，插入调用标签（第一个中括号里填写要显示的文字）。
