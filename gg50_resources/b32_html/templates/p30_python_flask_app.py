#!/usr/bin/env python2
# -*- coding: utf-8 -*-

# https://www.cnblogs.com/gooutlook/p/13914827.html

import os
import time
from flask import Flask, render_template, Response,request,redirect,url_for,jsonify,send_file, send_from_directory,json, jsonify,make_response

pathnow=os.getcwd()

HTML_PATH=pathnow

app = Flask(
    __name__,
    template_folder='.',  # 表示在当前目录 (myproject/A/) 寻找模板文件
    static_folder='',     # 空 表示为当前目录 (myproject/A/) 开通虚拟资源入口
    static_url_path='',
)

# app = Flask(__name__)


@app.route('/')
def index():
    return render_template('index.html')




if __name__ == '__main__':
    app.run(host='0.0.0.0')