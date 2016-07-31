#!/usr/bin/python -tt
# -*- coding: utf-8 -*-
"""
filename: spider517.py
author:   zhuming9011@gmail.com
datetime: 2016-07-31
copyright:
"""
from flask import Flask
from flask import render_template

from lab517 import URL
from lab517 import is_login
from lab517 import login
from lab517 import Lab517
from lab517 import Module
from lab517 import Subject

app = Flask(__name__)
forum = Lab517()


@app.route('/')
def homepage():
    if is_login() or login("小爬虫", "littlespider"):
        modules = forum.modules()
        modules.sort(key=Module.latest_post_date, reverse=True)
        updates = []
        for m in modules:
            if len(updates) >= 20 and updates[19].time() > m.latest_post_date():
                break
            updates.extend(m.latest_posts(20))
            updates.sort(key=Subject.time, reverse=True) 
        return render_template('index.html', URL=URL, updates=updates)
    else:
        return render_template('error.html')


if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8000)
