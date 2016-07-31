#!/usr/bin/python -tt
# -*- coding: utf-8 -*-
"""
filename: lab517.py
author:   zhuming9011@gmail.com
datetime: 2016-07-32
copyright:
"""
import requests
import http.cookiejar
from bs4 import BeautifulSoup

URL = "http://54.169.206.26/"

HEADERS = {
    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8",
    "User-Agent": ("Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) "
                   "Chrome/48.0.2564.116 Safari/537.36"),
    "Content-Type": "application/x-www-form-urlencoded",
    "Accept-Encoding": "gzip, deflate"
}

if not URL.endswith('/'):
    URL += '/'


def session():
    s = requests.Session()
    s.cookies = http.cookiejar.LWPCookieJar("cookies")
    try:
        s.cookies.load("cookies")
    except:
        pass
    return s


def is_login():
    r = session().get(URL + "u.php", headers=HEADERS)
    r.encoding = "utf-8"
    if int(r.status_code) == 200:
        soup = BeautifulSoup(r.text, "html.parser")
        return soup.find("div", class_="userInfo_home") is not None
    return False


def login(user, passwd):
    data = {
        "jumpurl": URL + "index.php",
        "step": 2,
        "pwuser": user,
        "pwpwd": passwd,
        "cktime": 31536000,
        "question": 0,
        "customquest": "",
        "head_login": "",
        "hideid": 0,
        "lqt": 0,
        "submit": None,
        "answer": None,
    }

    s = session()
    r = s.post(URL + "login.php", headers=HEADERS, data=data)
    r.encoding = "utf-8"
    s.cookies.save()
    return int(r.status_code) == 200 and '山东大学517实验室交流论坛 - Powered by phpwind' in r.text[:100]    


class Subject(object):
    """代表论坛中发表的一个帖子"""
    def __init__(self, module, title, author, time, link):
        super(Subject, self).__init__()
        self._module = module
        self._title = title
        self._author = author
        self._time = time
        self._link = URL + link

    def module(self):
        """帖子所在的模块名"""
        return self._module

    def title(self):
        """帖子标题"""
        return self._title

    def author(self):
        """帖子作者"""
        return self._author

    def link(self):
        """帖子链接"""
        return self._link

    def time(self):
        """帖子发表时间"""
        return self._time

    def __str__(self):
        return '(' + ', '.join([self._time, self._title, self._author, self._module]) + ')'

    def __repr__(self):
        return self.__str__()


class Module(object):
    """代表一个实验室板块, 如例会区、灌水区等"""
    def __init__(self, name, href, date):
        super(Module, self).__init__()
        self._name = name
        self._url = URL + href
        self._date = date

    def name(self):
        return self._name

    def url(self):
        return self._url

    def latest_post_date(self):
        return self._date

    def latest_posts(self, count):
        """返回该板块最新发表的帖子, 最大个数count"""
        result = []
        r = session().get(self._url, headers=HEADERS)
        r.encoding = 'utf-8'
        if int(r.status_code) == 200:
            soup = BeautifulSoup(r.text, "html.parser")
            for tr in soup.find_all("tr", class_="tr3"):
                try:
                    td_sub = tr.find("td", class_="subject")  # subject
                    td_aut = tr.find("td", class_="author")  # author
                    time = str(td_aut.find("p").string)
                    link = str(td_sub.find("a")['href'])
                    title = str(td_sub.find("a").string)
                    author = str(td_aut.find("a").string)
                    subject = Subject(self._name, title, author, time, link)
                    result.append(subject)
                except:
                    pass
        result.sort(key=Subject.time, reverse=True)
        return result[:count]

    def __str__(self):
        return "<Module: %s, %s, %s>" % (self.name(), self.latest_post_date(), self.url())


class Lab517(object):
    def __init__(self, url=URL):
        super(Lab517, self).__init__()
        self.url = url if url.endswith('/') else url + '/'

    def modules(self):
        """返回论坛所有的模块"""
        r = session().get(self.url, headers=HEADERS)
        r.encoding = "utf-8"
        if int(r.status_code) == 200:
            soup = BeautifulSoup(r.text, "html.parser")
            names = soup.select("h2.forumT > a")
            dates = soup.select("p.fNum")
            dates = [t.text.split()[0] for t in dates]
            assert len(dates) == len(names)
            return [Module(name.string, name['href'], date) for name, date in zip(names, dates)]
        else:
            print("failed to get modules")
            return []
