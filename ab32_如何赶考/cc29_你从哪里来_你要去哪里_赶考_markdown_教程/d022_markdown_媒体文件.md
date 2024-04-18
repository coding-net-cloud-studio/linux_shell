<font size="4">Markdown 媒体文件</font>

<font color="#7093DB">播放视频代码：</font>

```html
<video id="video" controls="" preload="none" poster="封面图链接"><source id="mp4" src="视频地址" type="video/mp4"></video>
```

<font color="#7093DB">播放音频代码：</font>

```html
<audio id="audio" controls="" preload="none"><source id="mp3" src="音频地址"></audio>
```

由于 ChaTi/轻记 使用TextView（文本框）进行渲染，难以插入视频、音频这种媒体文件，所以请考虑用跳到链接的方式单独播放。

示例：

[MP3音频-追光者](https://xiaobeiit.gitee.io/src/user/zhuiguangzhe.mp3)

当然，可能会直接调用下载器来下载该文件。即便能在WebView（浏览器组件）内直接播放音频，安卓原生界面也着实“一言难尽”。

等哪天我实在闲的慌的话，可能会研究下怎么支持播放音频，主要是目前还没啥这方面的需求。
