#!/usr/bin/env python3

# 导入reloading模块,该模块用于实现自动重载功能
# https://github.com/julvo/reloading
from reloading import reloading

# 导入 Flask 框架以创建 Web 应用
from flask import Flask, render_template

# 导入 markdown 库用于将 Markdown 文本转换为 HTML
import markdown

# 导入 TocExtension 以支持在 Markdown 中生成目录
from markdown.extensions.toc import TocExtension

# 创建 Flask 应用实例
app = Flask(__name__)


# 定义一个函数,用于将 Markdown 文本转换为带有目录的 HTML
@reloading
def markdown_to_html(markdown_text):
    # 使用 Markdown 的 TocExtension 扩展来添加目录,标记为 '##' 的标题
    html = markdown.markdown(markdown_text, extensions=[TocExtension(marker="##")])
    return html


# 定义路由处理函数,当访问根路径 '/' 时执行
@reloading
@app.route("/")
def index():
    # 定义 Markdown 内容
    markdown_content = "# Hello, Markdown 1060!"

    ## This is a section header."
    # 调用函数将 Markdown 转换为 HTML
    html_content = markdown_to_html(markdown_content)
    # 使用 render_template 函数渲染模板,并将 HTML 内容传递给模板中的 'content' 变量
    return render_template("index.html", content=html_content)


# 当脚本作为主程序运行时,启动 Flask 应用的开发服务器
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5055, debug=True)
