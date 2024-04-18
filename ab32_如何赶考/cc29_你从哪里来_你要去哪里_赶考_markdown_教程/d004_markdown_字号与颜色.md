<font size="4">Markdown 字号/颜色</font>

如果你想指定字号/颜色，可以尝试用html里的 `<font> </font>` 标签来实现。

正常情况下，字号3应最接近默认大小。

---

<font color="#FF7F00">示例文本：</font>

```html
<font size="3">三号文字</font>

<font size="4">四号文字</font>

<font size="5" color="#FF0000">五号红字</font>

<font color="#FF0000">红色文字</font>

<font color="#FFA500">橙色文字</font>

<font color="#FFFF00">黄色文字</font>

<font color="#00FF00">绿色文字</font>

<font color="#00FFFF">青色文字</font>

<font color="#0000FF">蓝色文字</font>

<font color="#A020F0">紫色文字</font>

<font color="#FFC0CB">粉色文字</font>

<font color="#A52A2A">棕色文字</font>
```

<font color="#7093DB">渲染效果：</font>

<font size="3">三号文字</font>

<font size="4">四号文字</font>

<font size="5" color="#FF0000">五号红字</font>

<font color="#FF0000">红色文字</font>

<font color="#FFA500">橙色文字</font>

<font color="#FFFF00">黄色文字</font>

<font color="#00FF00">绿色文字</font>

<font color="#00FFFF">青色文字</font>

<font color="#0000FF">蓝色文字</font>

<font color="#A020F0">紫色文字</font>

<font color="#FFC0CB">粉色文字</font>

<font color="#A52A2A">棕色文字</font>

---

对于解析 `font` 标签中 `size` 的值，Typora只取出了整数部分，而ChaTi/轻记可精确到小数。

简记工具提供了快捷键“彩色文字标签”，但这并非最佳方案。下边贴几个常用的标签吧。

1.橙色文字：
[](005i)
```
<font color="#FF7F00"></font>
```
[]( 005i)
渲染示例：<font color="#FF7F00">橙色文字</font>

[点我复制](yun://art.copy/?i=005i)

2.蓝色文字：
[](006i)
```
<font color="#7093DB"></font>
```
[]( 006i)
渲染示例：<font color="#7093DB">蓝色文字</font>

[点我复制](yun://art.copy/?i=006i)

3.四号字：
[](008i)
```
<font size="4"></font>
```
[]( 008i)
渲染效果：

<font size="4">四号字</font>

[点我复制](yun://art.copy/?i=008i)
