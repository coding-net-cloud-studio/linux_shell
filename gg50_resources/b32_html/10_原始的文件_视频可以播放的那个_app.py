# -*- coding: utf-8 -*-
import os
from flask import Flask
from flask import render_template
app = Flask(__name__)

# 参考内容
# PYTHON FLASK将读取的图片返回给WEB前端
# https://www.freesion.com/article/6156980304/
# 在Flask编程中显示路径汇总
# https://blog.csdn.net/sinat_41667855/article/details/119328917

# 定义系统路径的变量
BASE_DIR = os.path.dirname(__file__)
# 定义静态文件的路径
static_dir = os.path.join(BASE_DIR, 'static')
# 定义模板文件的路径
templates_dir = os.path.join(BASE_DIR, 'templates')


def return_img_stream(img_local_path):
    """
    工具函数:
    获取本地图片流
    :param img_local_path:文件单张图片的本地绝对路径
    :return: 图片流
    """
    import base64
    img_stream = ''
    with open(img_local_path, 'rb') as img_f:
        img_stream = img_f.read()
        img_stream = base64.b64encode(img_stream).decode()
    return img_stream

@app.route('/')
@app.route('/<name>')
def hello(name=None):
    print("====放置静态图片的目录如下=========================")
    print(static_dir)
    print("====放置静态图片的目录如上=========================")
    img_path = static_dir + '/' + '22_liao_go_web_qq.png'
    img_stream = return_img_stream(img_path)
    return render_template('index.html', name=name,img_stream=img_stream)


if __name__ == '__main__':
    app.run(host='0.0.0.0')